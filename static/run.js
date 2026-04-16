const params = new URLSearchParams(window.location.search);
const jobId = params.get("job") || "";
const basename = (params.get("name") || "").replace(/\.inp$/i, "");

const elements = {
  title: document.getElementById("run-title"),
  statusText: document.getElementById("run-status-text"),
  log: document.getElementById("run-log"),
  artifactStatus: document.getElementById("artifact-status"),
  links: {
    inp: document.getElementById("download-inp"),
    root: document.getElementById("download-root"),
    out: document.getElementById("download-out"),
    log: document.getElementById("download-log"),
    viewer: document.getElementById("open-viewer"),
  },
};

let pollTimer = null;
let lastLogText = "";

function setStatus(message) {
  elements.statusText.textContent = message;
}

function setLink(element, href, enabled) {
  element.href = href || "#";
  element.classList.toggle("disabled-link", !enabled);
  if (enabled) {
    element.target = "_blank";
  } else {
    element.removeAttribute("target");
  }
}

async function fetchJson(url) {
  const response = await fetch(url);
  if (!response.ok) {
    throw new Error(`${response.status} ${response.statusText}`);
  }
  return response.json();
}

async function fetchText(url) {
  const response = await fetch(url);
  if (!response.ok) {
    return "";
  }
  return response.text();
}

async function listDir(name) {
  const payload = await fetchJson(`/api/files?dir=${encodeURIComponent(name)}`);
  return payload.files.map((file) => file.name);
}

async function refreshArtifacts() {
  const [worksim, outfiles, runout] = await Promise.all([
    listDir("worksim"),
    listDir("outfiles"),
    listDir("runout"),
  ]);

  const rootName = `${basename}.root`;
  const outName = `${basename}.out`;
  const logName = `${basename}.out`;

  setLink(elements.links.inp, `/api/infiles/${encodeURIComponent(`${basename}.inp`)}/download`, true);
  setLink(elements.links.root, `/files/worksim/${encodeURIComponent(rootName)}`, worksim.includes(rootName));
  setLink(elements.links.out, `/files/outfiles/${encodeURIComponent(outName)}`, outfiles.includes(outName));
  setLink(elements.links.log, `/files/runout/${encodeURIComponent(logName)}`, runout.includes(logName));
  setLink(elements.links.viewer, `/viewer?file=${encodeURIComponent(rootName)}`, worksim.includes(rootName));

  elements.artifactStatus.innerHTML = "";
  for (const [label, exists] of [
    [rootName, worksim.includes(rootName)],
    [outName, outfiles.includes(outName)],
    [`runout/${logName}`, runout.includes(logName)],
  ]) {
    const item = document.createElement("li");
    item.textContent = `${label}: ${exists ? "ready" : "waiting"}`;
    elements.artifactStatus.appendChild(item);
  }
}

async function refreshLog() {
  const text = await fetchText(`/files/runout/${encodeURIComponent(`${basename}.out`)}`);
  if (text && text !== lastLogText) {
    const distanceFromBottom = elements.log.scrollHeight - elements.log.scrollTop - elements.log.clientHeight;
    const shouldFollow = distanceFromBottom < 48;

    elements.log.value = text;
    lastLogText = text;

    if (shouldFollow || !elements.log.scrollTop) {
      elements.log.scrollTop = elements.log.scrollHeight;
    }
  }
}

async function refreshRunState() {
  if (!basename) {
    setStatus("Missing run name in the URL.");
    return;
  }

  elements.title.textContent = `Run ${basename}`;
  await refreshArtifacts();
  await refreshLog();

  if (!jobId) {
    setStatus(`Viewing saved outputs for ${basename}.`);
    return;
  }

  try {
    const payload = await fetchJson(`/api/runs/${encodeURIComponent(jobId)}`);
    const job = payload.job;
    setStatus(`${job.basename}: ${job.status}`);

    if (job.status === "completed" || job.status === "failed") {
      clearInterval(pollTimer);
      pollTimer = null;
    }
  } catch (error) {
    setStatus(`Run metadata is no longer in memory. Showing saved files for ${basename}.`);
    clearInterval(pollTimer);
    pollTimer = null;
  }
}

document.getElementById("refresh-run").addEventListener("click", () => {
  refreshRunState().catch((error) => {
    setStatus(error.message);
  });
});

refreshRunState().catch((error) => {
  setStatus(error.message);
});

pollTimer = setInterval(() => {
  refreshRunState().catch((error) => {
    setStatus(error.message);
  });
}, 2000);
