const state = {
  schema: [],
  savedFiles: [],
  values: {},
  selectedName: "",
  dirty: false,
};

const elements = {
  configSelect: document.getElementById("config-select"),
  loadConfig: document.getElementById("load-config"),
  configName: document.getElementById("config-name"),
  selectedName: document.getElementById("selected-name"),
  formSections: document.getElementById("form-sections"),
  statusText: document.getElementById("status-text"),
  importFile: document.getElementById("import-file"),
  dirtyBadge: document.getElementById("dirty-badge"),
  editorDetails: document.getElementById("editor-details"),
  runGroups: document.getElementById("run-groups"),
};

function groupedSchema() {
  const groups = new Map();
  for (const field of state.schema) {
    if (!groups.has(field.section)) {
      groups.set(field.section, []);
    }
    groups.get(field.section).push(field);
  }
  return [...groups.entries()];
}

function setStatus(message) {
  elements.statusText.textContent = message;
}

function setDirty(dirty) {
  state.dirty = dirty;
  elements.dirtyBadge.classList.toggle("hidden", !dirty);
}

function selectedDisplayName() {
  return elements.configName.value.trim() || state.selectedName || "No input loaded yet.";
}

function updateSelectedName() {
  elements.selectedName.textContent = selectedDisplayName();
}

function valueForField(field) {
  return state.values[field.key] ?? field.default;
}

function coerceFieldValue(field, rawValue) {
  if (field.kind === "int" || field.kind === "enum" || field.kind === "flag") {
    return Number.parseInt(rawValue, 10);
  }
  return Number.parseFloat(rawValue);
}

function renderForm() {
  elements.formSections.innerHTML = "";

  for (const [section, fields] of groupedSchema()) {
    const sectionEl = document.createElement("section");
    sectionEl.className = "form-section";

    const header = document.createElement("div");
    header.className = "form-section-header";
    header.innerHTML = `<h3>${section}</h3>`;
    sectionEl.appendChild(header);

    const grid = document.createElement("div");
    grid.className = "field-grid";

    for (const field of fields) {
      const wrapper = document.createElement("label");
      wrapper.className = "field-card";

      const title = document.createElement("span");
      title.className = "field-label";
      title.textContent = field.label;
      wrapper.appendChild(title);

      let input;
      if (field.kind === "enum" || field.kind === "flag") {
        input = document.createElement("select");
        const choices = field.kind === "flag"
          ? [
              { value: 0, label: "Disabled" },
              { value: 1, label: "Enabled" },
            ]
          : field.choices;

        for (const choice of choices) {
          const option = document.createElement("option");
          option.value = choice.value;
          option.textContent = choice.label;
          input.appendChild(option);
        }
        input.value = String(valueForField(field));
      } else {
        input = document.createElement("input");
        input.type = "number";
        input.step = field.step || (field.kind === "int" ? "1" : "any");
        input.value = valueForField(field);
      }

      input.name = field.key;
      input.addEventListener("input", () => {
        state.values[field.key] = coerceFieldValue(field, input.value);
        setDirty(true);
      });
      input.addEventListener("change", () => {
        state.values[field.key] = coerceFieldValue(field, input.value);
        setDirty(true);
      });
      wrapper.appendChild(input);

      const help = document.createElement("small");
      help.className = "field-help";
      help.textContent = field.help_text;
      wrapper.appendChild(help);

      grid.appendChild(wrapper);
    }

    sectionEl.appendChild(grid);
    elements.formSections.appendChild(sectionEl);
  }
}

async function fetchJson(url, options = {}) {
  const response = await fetch(url, options);
  if (!response.ok) {
    const text = await response.text();
    throw new Error(extractErrorMessage(text, response));
  }
  return response.json();
}

function extractErrorMessage(text, response) {
  const contentType = response.headers.get("Content-Type") || "";

  if (contentType.includes("application/json")) {
    try {
      const payload = JSON.parse(text);
      return payload.message || payload.error || `${response.status} ${response.statusText}`;
    } catch (_) {
      return `${response.status} ${response.statusText}`;
    }
  }

  if (contentType.includes("text/html")) {
    const messageMatch = text.match(/<p>Message:\s*([^<]+)<\/p>/i);
    if (messageMatch) {
      return messageMatch[1].replace(/\.$/, "");
    }
    const headingMatch = text.match(/<h1>([^<]+)<\/h1>/i);
    if (headingMatch) {
      return `${headingMatch[1]} (${response.status})`;
    }
  }

  const plain = text.replace(/<[^>]+>/g, " ").replace(/\s+/g, " ").trim();
  return plain || `${response.status} ${response.statusText}`;
}

function applyValues(values, name) {
  state.values = { ...values };
  state.selectedName = name || "";
  elements.configName.value = name || "";
  renderForm();
  updateSelectedName();
  setDirty(false);
}

async function loadConfigList() {
  const payload = await fetchJson("/api/infiles");
  state.schema = payload.schema;
  state.savedFiles = payload.files;
  elements.configSelect.innerHTML = "";

  for (const name of payload.files) {
    const option = document.createElement("option");
    option.value = name;
    option.textContent = name;
    elements.configSelect.appendChild(option);
  }

  if (!state.schema.length) {
    throw new Error("Schema was not returned by the server.");
  }

  if (!Object.keys(state.values).length) {
    const defaults = Object.fromEntries(state.schema.map((field) => [field.key, field.default]));
    applyValues(defaults, "");
  }

  if (payload.files.length && !elements.configSelect.value) {
    elements.configSelect.value = payload.files[0];
  }
}

async function loadSelectedConfig() {
  const name = elements.configSelect.value;
  if (!name) {
    setStatus("Choose a file from the list first.");
    return;
  }
  const payload = await fetchJson(`/api/infiles/${encodeURIComponent(name)}`);
  applyValues(payload.values, payload.name);
  setStatus(`Loaded ${payload.name}.`);
}

function resetToDefaults() {
  const defaults = Object.fromEntries(state.schema.map((field) => [field.key, field.default]));
  applyValues(defaults, "new_config.inp");
  elements.editorDetails.open = true;
  setStatus("Loaded default values for a new input.");
}

async function importSelectedFile() {
  const file = elements.importFile.files[0];
  if (!file) {
    setStatus("Choose an .inp file to import.");
    return;
  }
  const content = await file.text();
  const payload = await fetchJson("/api/infiles/import", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ filename: file.name, content }),
  });
  await loadConfigList();
  elements.configSelect.value = payload.name;
  await loadSelectedConfig();
  setStatus(`Imported ${payload.name}.`);
}

function requestedConfigName() {
  const name = elements.configName.value.trim();
  if (!name) {
    throw new Error("Enter a config filename before saving or running.");
  }
  return name;
}

async function saveCurrentConfig({ confirmOverwrite = false } = {}) {
  const requestedName = requestedConfigName();
  const existing = state.savedFiles.includes(requestedName);
  const isOverwrite = existing && state.selectedName !== requestedName;

  if (confirmOverwrite && (isOverwrite || (existing && state.dirty))) {
    const accepted = window.confirm(`Overwrite the saved input ${requestedName}?`);
    if (!accepted) {
      throw new Error("Run canceled.");
    }
  }

  const payload = { values: state.values };
  const response = await fetchJson(`/api/infiles/${encodeURIComponent(requestedName)}`, {
    method: "PUT",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(payload),
  });

  await loadConfigList();
  elements.configSelect.value = response.name;
  state.selectedName = response.name;
  elements.configName.value = response.name;
  updateSelectedName();
  setDirty(false);
  setStatus(`Saved ${response.name}.`);
  return response.name;
}

function downloadCurrentConfig() {
  try {
    const requestedName = requestedConfigName();
    if (state.dirty) {
      setStatus("Save the input before downloading it.");
      return;
    }
    window.location.href = `/api/infiles/${encodeURIComponent(requestedName)}/download`;
  } catch (error) {
    setStatus(error.message);
  }
}

async function runCurrentConfig() {
  const name = requestedConfigName();
  const exists = state.savedFiles.includes(name);

  if (!exists || state.dirty) {
    const message = exists
      ? `Save and overwrite ${name} before running and converting to ROOT?`
      : `Save ${name} before running and converting to ROOT?`;
    if (!window.confirm(message)) {
      setStatus("Run canceled.");
      return;
    }
  }

  const savedName = await saveCurrentConfig({ confirmOverwrite: false });
  const payload = await fetchJson("/api/runs", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({
      name: savedName,
      convert_root: true,
      keep_bin: false,
    }),
  });

  const job = payload.job;
  const url = new URL("/run", window.location.origin);
  url.searchParams.set("job", job.id);
  url.searchParams.set("name", job.basename);
  window.location.href = url.toString();
}

function basenameForRunFile(filename) {
  const knownSuffixes = [".out", ".root", ".bin"];
  for (const suffix of knownSuffixes) {
    if (filename.endsWith(suffix)) {
      return filename.slice(0, -suffix.length);
    }
  }
  return "";
}

function groupRunFiles(outfiles, runout, worksim) {
  const groups = new Map();

  function ensureGroup(name) {
    if (!groups.has(name)) {
      groups.set(name, { name, out: null, log: null, root: null, bin: null, mtime: 0 });
    }
    return groups.get(name);
  }

  for (const file of outfiles.files) {
    const basename = basenameForRunFile(file.name);
    if (!basename) continue;
    const group = ensureGroup(basename);
    group.out = file;
    group.mtime = Math.max(group.mtime, file.mtime || 0);
  }

  for (const file of runout.files) {
    const basename = basenameForRunFile(file.name);
    if (!basename) continue;
    const group = ensureGroup(basename);
    group.log = file;
    group.mtime = Math.max(group.mtime, file.mtime || 0);
  }

  for (const file of worksim.files) {
    const basename = basenameForRunFile(file.name);
    if (!basename) continue;
    const group = ensureGroup(basename);
    if (file.name.endsWith(".root")) {
      group.root = file;
    } else if (file.name.endsWith(".bin")) {
      group.bin = file;
    }
    group.mtime = Math.max(group.mtime, file.mtime || 0);
  }

  return [...groups.values()].sort((a, b) => b.mtime - a.mtime || a.name.localeCompare(b.name));
}

function readyLabel(value, yesText) {
  return value ? yesText : "waiting";
}

function buildRunLink(label, href, extraClass = "") {
  const link = document.createElement("a");
  link.href = href;
  link.textContent = label;
  link.className = extraClass ? `secondary-link ${extraClass}` : "secondary-link";
  link.target = "_blank";
  return link;
}

function renderRunGroups(groups) {
  elements.runGroups.innerHTML = "";

  if (!groups.length) {
    const empty = document.createElement("p");
    empty.className = "subtle";
    empty.textContent = "No run outputs found yet.";
    elements.runGroups.appendChild(empty);
    return;
  }

  for (const group of groups) {
    const card = document.createElement("section");
    card.className = "run-group-card";

    const header = document.createElement("div");
    header.className = "run-group-header";
    header.innerHTML = `
      <div>
        <h3>${group.name}</h3>
        <p class="subtle">runout: ${readyLabel(group.log, "ready")} | out: ${readyLabel(group.out, "ready")} | root: ${readyLabel(group.root, "ready")}</p>
      </div>
    `;

    const actions = document.createElement("div");
    actions.className = "run-group-actions";
    actions.appendChild(buildRunLink("Run page", `/run?name=${encodeURIComponent(group.name)}`));
    if (group.root) {
      actions.appendChild(buildRunLink("JSROOT", `/viewer?file=${encodeURIComponent(group.root.name)}`));
      actions.appendChild(buildRunLink("ROOT", group.root.url));
    }
    if (group.log) {
      actions.appendChild(buildRunLink("Log", group.log.url));
    }
    if (group.out) {
      actions.appendChild(buildRunLink("Out", group.out.url));
    }
    header.appendChild(actions);
    card.appendChild(header);

    const artifacts = document.createElement("div");
    artifacts.className = "run-group-artifacts";
    for (const [label, file] of [
      ["Input", `${group.name}.inp`],
      ["Run log", group.log ? group.log.name : null],
      ["Outfile", group.out ? group.out.name : null],
      ["ROOT", group.root ? group.root.name : null],
      ["BIN", group.bin ? group.bin.name : null],
    ]) {
      if (!file) continue;
      const pill = document.createElement("div");
      pill.className = "artifact-pill";
      pill.innerHTML = `<span class="artifact-pill-label">${label}</span><span>${file}</span>`;
      artifacts.appendChild(pill);
    }
    card.appendChild(artifacts);

    elements.runGroups.appendChild(card);
  }
}

async function refreshFileLists() {
  const [outfiles, runout, worksim] = await Promise.all([
    fetchJson("/api/files?dir=outfiles"),
    fetchJson("/api/files?dir=runout"),
    fetchJson("/api/files?dir=worksim"),
  ]);

  renderRunGroups(groupRunFiles(outfiles, runout, worksim));
}

function bindEvents() {
  document.getElementById("refresh-configs").addEventListener("click", () => {
    loadConfigList().then(() => setStatus("Refreshed saved inputs.")).catch((error) => setStatus(error.message));
  });
  document.getElementById("refresh-files").addEventListener("click", () => {
    refreshFileLists().then(() => setStatus("Refreshed outputs.")).catch((error) => setStatus(error.message));
  });
  elements.loadConfig.addEventListener("click", () => {
    loadSelectedConfig().catch((error) => setStatus(error.message));
  });
  document.getElementById("new-config").addEventListener("click", resetToDefaults);
  document.getElementById("save-config").addEventListener("click", () => {
    saveCurrentConfig({ confirmOverwrite: true }).catch((error) => setStatus(error.message));
  });
  document.getElementById("download-config").addEventListener("click", downloadCurrentConfig);
  document.getElementById("run-config").addEventListener("click", () => {
    runCurrentConfig().catch((error) => setStatus(error.message));
  });
  document.getElementById("import-submit").addEventListener("click", () => {
    importSelectedFile().catch((error) => setStatus(error.message));
  });
  elements.configName.addEventListener("input", () => {
    updateSelectedName();
    setDirty(true);
  });
}

async function init() {
  bindEvents();
  await loadConfigList();
  await refreshFileLists();
  if (elements.configSelect.value) {
    await loadSelectedConfig();
  } else {
    resetToDefaults();
  }
}

init().catch((error) => {
  setStatus(error.message);
});
