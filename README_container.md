# mc-single-arm container

## Quick start

Pull the published image:

```bash
docker pull ghcr.io/jeffersonlab/mc-single-arm:latest
```

Or use a specific release tag:

```bash
docker pull ghcr.io/jeffersonlab/mc-single-arm:vX.Y.Z
```

Make directories on the host for inputs and results:

```bash
mkdir -p mc-data/infiles mc-data/outfiles mc-data/runout mc-data/worksim
```

Run the container:

```bash
docker run --rm -it \
  -p 8080:8080 \
  -v "$PWD/mc-data/infiles:/data/infiles" \
  -v "$PWD/mc-data/outfiles:/data/outfiles" \
  -v "$PWD/mc-data/runout:/data/runout" \
  -v "$PWD/mc-data/worksim:/data/worksim" \
  ghcr.io/jeffersonlab/mc-single-arm:latest
```

Open:

- `http://localhost:8080/` for saved inputs, editing, and run launch
- `http://localhost:8080/viewer` for the JSROOT browser

If `infiles` is empty the first time you start the container, the shipped example `.inp` files are copied into it automatically.

## What to know

- Save inputs by binding `infiles`.
- Save generated files by binding `outfiles`, `runout`, and `worksim`.

Browser flow:

1. Load a saved `.inp` from the list.
2. Edit fields if needed.
3. Save the `.inp`.
4. Click `Run + convert ROOT`.
5. You will be sent to a run page with the live log, plus downloads for `.inp` and `.root`.

## `--rm` and saved data

- If you use `--rm` and you do not bind host directories, the container is removed when it exits and nothing inside it is kept.
- If you do not use `--rm`, files can remain inside the stopped container and ROOT outputs can grow large.
- Even with `--rm`, bound host directories still keep your files.

Useful checks:

```bash
docker ps -a
docker exec <container_name> du -sh /data
docker exec <container_name> du -sh /data/worksim
docker rm -f <container_name>
```

## Notes

- The default image builds `src/mc_single_arm` and `util/root_tree/make_root_tree`.
- The default image does not include the legacy CERNLIB ntuple conversion workflow.
