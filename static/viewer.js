const rootSelect = document.getElementById("root-file-select");
const viewerStatus = document.getElementById("viewer-status");
let painter = null;
let modulesPromise = null;

function currentFileFromQuery() {
  const params = new URLSearchParams(window.location.search);
  return params.get("file") || "";
}

function rootFileUrl(rootFileName) {
  return new URL(`/files/worksim/${encodeURIComponent(rootFileName)}`, window.location.origin).href;
}

function loadJsrootModules() {
  if (!modulesPromise) {
    modulesPromise = import("/static/jsroot/modules/gui.mjs");
  }
  return modulesPromise;
}

async function fetchJson(url) {
  const response = await fetch(url);
  if (!response.ok) {
    throw new Error(`${response.status} ${response.statusText}`);
  }
  return response.json();
}

async function loadRootFiles() {
  const payload = await fetchJson("/api/files?dir=worksim");
  const rootFiles = payload.files.filter((file) => file.name.endsWith(".root"));

  rootSelect.innerHTML = "";
  for (const file of rootFiles) {
    const option = document.createElement("option");
    option.value = file.name;
    option.textContent = file.name;
    rootSelect.appendChild(option);
  }

  if (!rootFiles.length) {
    viewerStatus.textContent = "No ROOT files found in worksim yet.";
    document.getElementById("jsroot-tree").innerHTML = "";
    document.getElementById("jsroot-display").innerHTML = "";
    return;
  }

  const requestedFile = currentFileFromQuery();
  const defaultFile = rootFiles.some((file) => file.name === requestedFile) ? requestedFile : rootFiles[rootFiles.length - 1].name;
  rootSelect.value = defaultFile;
  await openRootFile(defaultFile);
}

async function openRootFile(rootFileName) {
  if (!rootFileName) {
    return;
  }
  viewerStatus.textContent = `Opening ${rootFileName} in bundled JSROOT...`;
  const { HierarchyPainter } = await loadJsrootModules();

  if (painter) {
    painter.cleanup();
    painter = null;
  }

  document.getElementById("jsroot-tree").innerHTML = "";
  document.getElementById("jsroot-display").innerHTML = "";

  painter = new HierarchyPainter("mc-single-arm", "jsroot-tree");
  painter.setDisplay("simple", "jsroot-display");
  await painter.openRootFile(rootFileUrl(rootFileName));
  await painter.refreshHtml();

  const params = new URLSearchParams(window.location.search);
  params.set("file", rootFileName);
  window.history.replaceState({}, "", `${window.location.pathname}?${params.toString()}`);
  viewerStatus.textContent = `Opened ${rootFileName}. Select objects from the JSROOT browser pane.`;
}

document.getElementById("refresh-root-files").addEventListener("click", () => {
  loadRootFiles().catch((error) => {
    viewerStatus.textContent = error.message;
  });
});

rootSelect.addEventListener("change", () => {
  openRootFile(rootSelect.value).catch((error) => {
    viewerStatus.textContent = error.message;
  });
});

loadRootFiles().catch((error) => {
  viewerStatus.textContent = error.message;
});
