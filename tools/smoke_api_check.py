#!/usr/bin/env python3

from __future__ import annotations

import json
import http.client
import sys
import time
import urllib.error
import urllib.parse
import urllib.request


def request(method: str, url: str, payload: dict | None = None) -> tuple[int, bytes]:
    body = None
    headers = {}
    if payload is not None:
        body = json.dumps(payload).encode("utf-8")
        headers["Content-Type"] = "application/json"
    req = urllib.request.Request(url, data=body, headers=headers, method=method)
    with urllib.request.urlopen(req, timeout=30) as response:
        return response.status, response.read()


def request_json(method: str, url: str, payload: dict | None = None) -> dict:
    _, body = request(method, url, payload)
    return json.loads(body.decode("utf-8"))


def wait_for_health(base_url: str) -> None:
    deadline = time.time() + 90
    while time.time() < deadline:
        try:
            payload = request_json("GET", f"{base_url}/api/healthz")
            if payload.get("status") == "ok":
                return
        except (urllib.error.URLError, ConnectionResetError, ConnectionRefusedError, http.client.RemoteDisconnected):
            time.sleep(1)
            continue
        time.sleep(1)
    raise RuntimeError("Timed out waiting for the container to become healthy.")


def main(argv: list[str]) -> int:
    if len(argv) != 2:
        raise SystemExit("usage: smoke_api_check.py <base_url>")

    base_url = argv[1].rstrip("/")
    wait_for_health(base_url)

    infiles_payload = request_json("GET", f"{base_url}/api/infiles")
    assert "ci_smoke.inp" in infiles_payload["files"], "ci_smoke.inp was not seeded into infiles"

    ci_payload = request_json("GET", f"{base_url}/api/infiles/ci_smoke.inp")
    assert ci_payload["values"]["n_trials"] == 250

    short_content = "\n".join(ci_payload["content"].splitlines()[:-1]) + "\n"
    imported = request_json(
        "POST",
        f"{base_url}/api/infiles/import",
        {"filename": "legacy_short.inp", "content": short_content},
    )
    assert imported["values"]["target_atomic_mass"] == 12.0

    request_json(
        "PUT",
        f"{base_url}/api/infiles/ci_smoke_roundtrip.inp",
        {"values": ci_payload["values"]},
    )
    roundtrip = request_json("GET", f"{base_url}/api/infiles/ci_smoke_roundtrip.inp")
    assert roundtrip["values"]["n_trials"] == 250

    run_payload = request_json(
        "POST",
        f"{base_url}/api/runs",
        {"name": "ci_smoke.inp", "convert_root": True, "keep_bin": False},
    )
    job_id = run_payload["job"]["id"]

    deadline = time.time() + 180
    while time.time() < deadline:
        job_payload = request_json("GET", f"{base_url}/api/runs/{job_id}")
        status = job_payload["job"]["status"]
        if status == "completed":
            break
        if status == "failed":
            _, log_body = request("GET", f"{base_url}/api/runs/{job_id}/log")
            log_text = log_body.decode("utf-8")
            raise RuntimeError(f"Container run failed:\n{log_text}")
        time.sleep(2)
    else:
        raise RuntimeError("Timed out waiting for smoke run to finish.")

    outfiles = request_json("GET", f"{base_url}/api/files?dir=outfiles")
    runout = request_json("GET", f"{base_url}/api/files?dir=runout")
    worksim = request_json("GET", f"{base_url}/api/files?dir=worksim")

    assert any(file["name"] == "ci_smoke.out" for file in outfiles["files"])
    assert any(file["name"] == "ci_smoke.out" for file in runout["files"])
    assert any(file["name"] == "ci_smoke.root" for file in worksim["files"])
    assert not any(file["name"] == "ci_smoke.bin" for file in worksim["files"])

    request("GET", f"{base_url}/viewer?file=ci_smoke.root")
    request("GET", f"{base_url}/run?name=ci_smoke")
    request("GET", f"{base_url}/static/jsroot/modules/gui.mjs")
    request("GET", f"{base_url}/files/worksim/ci_smoke.root")
    request("HEAD", f"{base_url}/files/worksim/ci_smoke.root")

    print("Smoke checks passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
