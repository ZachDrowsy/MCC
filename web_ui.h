#pragma once
#include <Arduino.h>

// UI only: existing network requests remain integration placeholders.
// Firmware must apply modes and validate settings independently of the browser.
// Mist AUTO: ON below humidityThresholdPercent; OFF at/above
// humidityOffThresholdPercent; retain the previous output between thresholds.
// Keep the maximum-run cutoff in firmware, even if the browser disconnects.
// Fan AUTO uses degrees F: ON above turnOnAboveF, OFF at/below turnOffBelowF.
// Heater AUTO uses degrees F: ON below turnOnBelowF, OFF at/above turnOffAboveF.


const char WEB_UI[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta name="theme-color" content="#1f6f4a">
  <title>Micro-Climate Controller</title>

  <style>
    * { box-sizing: border-box; }
    :root {
      --bg: #f4f7f5;
      --card: #ffffff;
      --text: #17211b;
      --muted: #6b756f;
      --border: #dfe6e1;
      --green: #2f6b4f;
      --green-soft: #e7f1eb;
      --red: #a33f3f;
      --shadow: 0 8px 24px rgba(25,45,34,.08);
    }

    body {
      margin: 0;
      font-family: Arial, Helvetica, sans-serif;
      background: var(--bg);
      color: var(--text);
    }

    .app {
      width: min(100%, 780px);
      margin: 0 auto;
      padding: 18px 14px 42px;
    }

    .topbar {
      display: flex;
      justify-content: space-between;
      align-items: flex-start;
      gap: 14px;
      margin-bottom: 16px;
    }

    h1 { margin: 0; font-size: 1.35rem; }
    .subtitle { color: var(--muted); margin-top: 5px; font-size: .9rem; }

    .status {
      white-space: nowrap;
      padding: 8px 10px;
      border-radius: 999px;
      border: 1px solid var(--border);
      background: var(--card);
      font-size: .8rem;
      font-weight: bold;
    }

    .sensor-grid {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 10px;
      margin-bottom: 14px;
    }

    .sensor, .card {
      background: var(--card);
      border: 1px solid var(--border);
      border-radius: 18px;
      box-shadow: var(--shadow);
    }

    .sensor { padding: 16px; }
    .sensor-label, .field-label { color: var(--muted); font-size: .8rem; }
    .sensor-value { font-size: 1.4rem; font-weight: bold; margin-top: 7px; }

    .card { padding: 16px; margin-bottom: 14px; }
    .card h2 { margin: 0 0 5px; font-size: 1.05rem; }
    .card-desc { color: var(--muted); font-size: .84rem; margin-bottom: 14px; }

    .mode-tabs {
      display: grid;
      grid-template-columns: repeat(4, 1fr);
      gap: 7px;
      margin-bottom: 14px;
    }

    .light-tabs { grid-template-columns: repeat(3, 1fr); }

    button {
      cursor: pointer;
      font-weight: bold;
      font-family: inherit;
    }

    .mode-tab, .day {
      min-height: 42px;
      border: 1px solid var(--border);
      border-radius: 11px;
      background: white;
      color: var(--text);
    }

    .mode-tab.active, .day.active {
      background: var(--green-soft);
      border-color: #aac8b8;
      color: var(--green);
    }

    .panel { display: none; }
    .panel.active { display: block; }

    .row {
      display: flex;
      gap: 10px;
      margin-top: 12px;
    }

    .field { width: 100%; }

    input[type="time"], input[type="number"] {
      width: 100%;
      min-height: 46px;
      margin-top: 7px;
      border: 1px solid var(--border);
      border-radius: 12px;
      padding: 0 12px;
      font-size: 1rem;
      background: white;
    }

    .days {
      display: grid;
      grid-template-columns: repeat(7, 1fr);
      gap: 6px;
      margin-top: 8px;
    }

    .summary {
      margin-top: 14px;
      padding: 12px;
      border-radius: 12px;
      background: var(--green-soft);
      color: #234d39;
      font-size: .9rem;
      line-height: 1.35;
    }

    .action {
      width: 100%;
      min-height: 48px;
      border: none;
      border-radius: 12px;
      background: var(--green);
      color: white;
      font-size: .96rem;
    }

    .action.on { background: var(--red); }

    .control-status {
      color: var(--muted);
      font-size: .86rem;
      margin: 0 0 10px;
    }

    .rule-box {
      padding: 13px;
      border: 1px solid var(--border);
      border-radius: 14px;
      background: #fbfcfb;
    }

    .rule-title { font-weight: bold; margin-bottom: 4px; }
    .rule-note { color: var(--muted); font-size: .82rem; line-height: 1.35; }

    .save {
      width: 100%;
      min-height: 52px;
      border: none;
      border-radius: 12px;
      background: var(--green);
      color: white;
      font-size: 1rem;
    }

    @media (max-width: 600px) {
      .sensor-grid { grid-template-columns: 1fr 1fr; }
      .sensor:first-child { grid-column: 1 / -1; }
      .row { flex-direction: column; }
      .mode-tabs { grid-template-columns: 1fr 1fr; }
      .light-tabs { grid-template-columns: repeat(3, 1fr); }
      .day { min-width: 0; padding: 0; }
    }
  </style>
</head>

<body>
<div class="app">

  <div class="topbar">
    <div>
      <h1>Micro-Climate Controller</h1>
      <div class="subtitle">Irrigation, mist, lighting, fan and heater controller</div>
    </div>
    <div class="status" id="connectionStatus">UI PREVIEW</div>
  </div>

  <div class="sensor-grid">
    <div class="sensor">
      <div class="sensor-label">Soil Moisture</div>
      <div class="sensor-value" id="moistureValue">N/A</div>
    </div>
    <div class="sensor">
      <div class="sensor-label">Temperature</div>
      <div class="sensor-value" id="temperatureValue">N/A</div>
    </div>
    <div class="sensor">
      <div class="sensor-label">Humidity</div>
      <div class="sensor-value" id="humidityValue">N/A</div>
    </div>
  </div>

  <!-- IRRIGATION -->
  <section class="card system-card" data-system="irrigation">
    <h2>Irrigation</h2>
    <div class="card-desc">Controls the irrigation solenoid independently from mist.</div>

    <div class="mode-tabs">
      <button class="mode-tab active" data-target="irrigation-off">Off</button>
      <button class="mode-tab" data-target="irrigation-manual">Manual</button>
      <button class="mode-tab" data-target="irrigation-schedule">Schedule</button>
      <button class="mode-tab" data-target="irrigation-auto">Automatic</button>
    </div>

    <div class="panel active" id="irrigation-off">
      <div class="rule-box">
        <div class="rule-title">Irrigation disabled</div>
        <div class="rule-note">No scheduled or moisture-triggered irrigation will run.</div>
      </div>
    </div>

    <div class="panel" id="irrigation-manual">
      <p class="control-status">Valve status: <strong id="irrigationStatus">Closed</strong></p>
      <button class="action" id="irrigationButton">Open Irrigation</button>
    </div>

    <div class="panel" id="irrigation-schedule">
      <div class="row">
        <label class="field">
          <span class="field-label">Start Time</span>
          <input type="time" id="irrigationStart" value="07:00">
        </label>
        <label class="field">
          <span class="field-label">Run Time (minutes)</span>
          <input type="number" id="irrigationDuration" min="1" max="120" value="10">
        </label>
      </div>

      <div style="margin-top:16px;">
        <div class="field-label">Days of Week</div>
        <div class="days" id="irrigationDays">
          <button class="day" data-day="Sun">S</button>
          <button class="day active" data-day="Mon">M</button>
          <button class="day" data-day="Tue">T</button>
          <button class="day active" data-day="Wed">W</button>
          <button class="day" data-day="Thu">T</button>
          <button class="day active" data-day="Fri">F</button>
          <button class="day" data-day="Sat">S</button>
        </div>
      </div>

      <div class="summary" id="irrigationSummary"></div>
    </div>

    <div class="panel" id="irrigation-auto">
      <div class="rule-box">
        <div class="rule-title">Soil Moisture Control</div>
        <div class="rule-note">When soil moisture falls below the threshold, irrigation can turn on automatically.</div>
      </div>
      <div class="row">
        <label class="field">
          <span class="field-label">Turn On Below (%)</span>
          <input type="number" id="moistureThreshold" min="0" max="100" value="30">
        </label>
        <label class="field">
          <span class="field-label">Maximum Run Time (minutes)</span>
          <input type="number" id="irrigationMaxRun" min="1" max="120" value="20">
        </label>
      </div>
    </div>
  </section>

  <!-- MIST -->
  <section class="card system-card" data-system="mist">
    <h2>Mist</h2>
    <div class="card-desc">Controls the mister solenoid independently from irrigation.</div>

    <div class="mode-tabs">
      <button class="mode-tab active" data-target="mist-off">Off</button>
      <button class="mode-tab" data-target="mist-manual">Manual</button>
      <button class="mode-tab" data-target="mist-schedule">Schedule</button>
      <button class="mode-tab" data-target="mist-auto">Automatic</button>
    </div>

    <div class="panel active" id="mist-off">
      <div class="rule-box">
        <div class="rule-title">Mist disabled</div>
        <div class="rule-note">No scheduled or humidity-triggered misting will run.</div>
      </div>
    </div>

    <div class="panel" id="mist-manual">
      <p class="control-status">Valve status: <strong id="mistStatus">Closed</strong></p>
      <button class="action" id="mistButton">Open Mist</button>
    </div>

    <div class="panel" id="mist-schedule">
      <div class="row">
        <label class="field">
          <span class="field-label">Start Time</span>
          <input type="time" id="mistStart" value="13:00">
        </label>
        <label class="field">
          <span class="field-label">Run Time (minutes)</span>
          <input type="number" id="mistDuration" min="1" max="120" value="5">
        </label>
      </div>

      <div style="margin-top:16px;">
        <div class="field-label">Days of Week</div>
        <div class="days" id="mistDays">
          <button class="day active" data-day="Sun">S</button>
          <button class="day" data-day="Mon">M</button>
          <button class="day active" data-day="Tue">T</button>
          <button class="day" data-day="Wed">W</button>
          <button class="day active" data-day="Thu">T</button>
          <button class="day" data-day="Fri">F</button>
          <button class="day active" data-day="Sat">S</button>
        </div>
      </div>

      <div class="summary" id="mistSummary"></div>
    </div>

    <div class="panel" id="mist-auto">
      <div class="rule-box">
        <div class="rule-title">Humidity Control</div>
        <div class="rule-note">Turn on below the lower humidity threshold and stop at or above the upper threshold. Between them, keep the previous state. The maximum run time still applies.</div>
      </div>
      <div class="row">
        <label class="field">
          <span class="field-label">Turn On Below (% RH)</span>
          <input type="number" id="humidityThreshold" min="0" max="100" value="45" required>
        </label>
        <label class="field">
          <span class="field-label">Turn Off At or Above (% RH)</span>
          <input type="number" id="humidityOffThreshold" min="0" max="100" value="55" required>
        </label>
      </div>
      <div class="row">
        <label class="field">
          <span class="field-label">Maximum Run Time (minutes)</span>
          <input type="number" id="mistMaxRun" min="1" max="120" value="10">
        </label>
      </div>
    </div>
  </section>

  <!-- LIGHT -->
  <section class="card system-card" data-system="light">
    <h2>Light</h2>
    <div class="card-desc">Independent relay control for the grow light.</div>

    <div class="mode-tabs light-tabs">
      <button class="mode-tab active" data-target="light-off">Off</button>
      <button class="mode-tab" data-target="light-manual">Manual</button>
      <button class="mode-tab" data-target="light-schedule">Schedule</button>
    </div>

    <div class="panel active" id="light-off">
      <div class="rule-box">
        <div class="rule-title">Light disabled</div>
        <div class="rule-note">The light relay will remain off.</div>
      </div>
    </div>

    <div class="panel" id="light-manual">
      <p class="control-status">Light status: <strong id="lightStatus">Off</strong></p>
      <button class="action" id="lightButton">Turn Light On</button>
    </div>

    <div class="panel" id="light-schedule">
      <div class="row">
        <label class="field">
          <span class="field-label">Turn On</span>
          <input type="time" id="lightStart" value="08:00">
        </label>
        <label class="field">
          <span class="field-label">Turn Off</span>
          <input type="time" id="lightEnd" value="18:00">
        </label>
      </div>

      <div style="margin-top:16px;">
        <div class="field-label">Days of Week</div>
        <div class="days" id="lightDays">
          <button class="day active" data-day="Sun">S</button>
          <button class="day active" data-day="Mon">M</button>
          <button class="day active" data-day="Tue">T</button>
          <button class="day active" data-day="Wed">W</button>
          <button class="day active" data-day="Thu">T</button>
          <button class="day active" data-day="Fri">F</button>
          <button class="day active" data-day="Sat">S</button>
        </div>
      </div>

      <div class="summary" id="lightSummary"></div>
    </div>
  </section>

  <!-- FAN -->
  <section class="card system-card" data-system="fan">
    <h2>Fan</h2>
    <div class="card-desc">Independent manual and automatic temperature control.</div>
    <div class="mode-tabs light-tabs">
      <button class="mode-tab active" data-target="fan-off">Off</button>
      <button class="mode-tab" data-target="fan-auto">Automatic</button>
      <button class="mode-tab" data-target="fan-manual">Manual</button>
    </div>
    <div class="panel active" id="fan-off">
      <div class="rule-box">
        <div class="rule-title">Fan disabled</div>
        <div class="rule-note">The fan relay will remain off.</div>
      </div>
    </div>
    <div class="panel" id="fan-auto">
      <div class="rule-box">
        <div class="rule-title">Temperature Control</div>
        <div class="rule-note">Turn on above the start temperature and off at or below the stop temperature. Between them, keep the previous state. Adjust these example values for your environment.</div>
      </div>
      <div class="row">
        <label class="field">
          <span class="field-label">Turn On Above (°F)</span>
          <input type="number" id="fanOnThreshold" step="0.1" value="80" required>
        </label>
        <label class="field">
          <span class="field-label">Turn Off At or Below (°F)</span>
          <input type="number" id="fanOffThreshold" step="0.1" value="76" required>
        </label>
      </div>
    </div>
    <div class="panel" id="fan-manual">
      <p class="control-status">Fan status: <strong id="fanStatus">Off</strong></p>
      <button class="action" id="fanButton">Turn Fan On</button>
    </div>
  </section>

  <!-- HEATER -->
  <section class="card system-card" data-system="heater">
    <h2>Heater</h2>
    <div class="card-desc">Independent manual and automatic temperature control.</div>
    <div class="mode-tabs light-tabs">
      <button class="mode-tab active" data-target="heater-off">Off</button>
      <button class="mode-tab" data-target="heater-auto">Automatic</button>
      <button class="mode-tab" data-target="heater-manual">Manual</button>
    </div>
    <div class="panel active" id="heater-off">
      <div class="rule-box">
        <div class="rule-title">Heater disabled</div>
        <div class="rule-note">The heater relay will remain off.</div>
      </div>
    </div>
    <div class="panel" id="heater-auto">
      <div class="rule-box">
        <div class="rule-title">Temperature Control</div>
        <div class="rule-note">Turn on below the start temperature and off at or above the stop temperature. Between them, keep the previous state. Adjust these example values for your environment.</div>
      </div>
      <div class="row">
        <label class="field">
          <span class="field-label">Turn On Below (°F)</span>
          <input type="number" id="heaterOnThreshold" step="0.1" value="65" required>
        </label>
        <label class="field">
          <span class="field-label">Turn Off At or Above (°F)</span>
          <input type="number" id="heaterOffThreshold" step="0.1" value="69" required>
        </label>
      </div>
    </div>
    <div class="panel" id="heater-manual">
      <p class="control-status">Heater status: <strong id="heaterStatus">Off</strong></p>
      <button class="action" id="heaterButton">Turn Heater On</button>
    </div>
  </section>

  <p class="rule-note" id="settingsFeedback" role="status">Controls preview settings only; device integration is pending.</p>
  <button class="save" id="saveButton">Save Settings</button>

</div>

<script>
  let irrigationOpen = false;
  let mistOpen = false;
  let lightOn = false;

  // Stores the selected operating mode for each independent system.
  const modes = {
    irrigation: "off",
    mist: "off",
    light: "off",
    fan: "off",
    heater: "off"
  };

  document.querySelectorAll(".system-card").forEach(function(card) {
    const system = card.dataset.system;

    card.querySelectorAll(".mode-tab").forEach(function(button) {
      button.addEventListener("click", function() {
        card.querySelectorAll(".mode-tab").forEach(function(other) {
          other.classList.remove("active");
        });

        card.querySelectorAll(".panel").forEach(function(panel) {
          panel.classList.remove("active");
        });

        button.classList.add("active");
        document.getElementById(button.dataset.target).classList.add("active");
        modes[system] = button.dataset.target.replace(system + "-", "");
        if ((system === "fan" || system === "heater") && modes[system] !== "manual") {
          manualOutputs[system] = false;
          renderManual(system);
        }
      });
    });
  });

  document.querySelectorAll(".day").forEach(function(button) {
    button.addEventListener("click", function() {
      button.classList.toggle("active");
      updateAllSummaries();
    });
  });

  ["irrigationStart", "irrigationDuration", "mistStart", "mistDuration", "lightStart", "lightEnd"].forEach(function(id) {
    document.getElementById(id).addEventListener("input", updateAllSummaries);
  });

  function formatTime(timeString) {
    if (!timeString) return "--";
    const parts = timeString.split(":");
    let hour = Number(parts[0]);
    const minute = Number(parts[1]);
    const suffix = hour >= 12 ? "PM" : "AM";
    hour = hour % 12;
    if (hour === 0) hour = 12;
    return hour + ":" + String(minute).padStart(2, "0") + " " + suffix;
  }

  function selectedDays(containerId) {
    const days = [];
    document.querySelectorAll("#" + containerId + " .day.active").forEach(function(button) {
      days.push(button.dataset.day);
    });
    return days.length ? days.join(", ") : "no selected days";
  }

  function updateAllSummaries() {
    const irrigationDuration = document.getElementById("irrigationDuration").value;
    document.getElementById("irrigationSummary").textContent =
      "Irrigation will run at " + formatTime(document.getElementById("irrigationStart").value) +
      " for " + irrigationDuration + " minute" + (irrigationDuration == 1 ? "" : "s") +
      " on " + selectedDays("irrigationDays") + ".";

    const mistDuration = document.getElementById("mistDuration").value;
    document.getElementById("mistSummary").textContent =
      "Mist will run at " + formatTime(document.getElementById("mistStart").value) +
      " for " + mistDuration + " minute" + (mistDuration == 1 ? "" : "s") +
      " on " + selectedDays("mistDays") + ".";

    document.getElementById("lightSummary").textContent =
      "Light will turn on at " + formatTime(document.getElementById("lightStart").value) +
      " and off at " + formatTime(document.getElementById("lightEnd").value) +
      " on " + selectedDays("lightDays") + ".";
  }

  document.getElementById("irrigationButton").addEventListener("click", function() {
    irrigationOpen = !irrigationOpen;
    document.getElementById("irrigationStatus").textContent = irrigationOpen ? "Open" : "Closed";
    this.textContent = irrigationOpen ? "Close Irrigation" : "Open Irrigation";
    this.classList.toggle("on", irrigationOpen);

    // Later ESP32 endpoint:
    // fetch(irrigationOpen ? "/api/irrigation/on" : "/api/irrigation/off", { method: "POST" });
  });

  document.getElementById("mistButton").addEventListener("click", function() {
    mistOpen = !mistOpen;
    document.getElementById("mistStatus").textContent = mistOpen ? "Open" : "Closed";
    this.textContent = mistOpen ? "Close Mist" : "Open Mist";
    this.classList.toggle("on", mistOpen);

    // Later ESP32 endpoint:
    // fetch(mistOpen ? "/api/mist/on" : "/api/mist/off", { method: "POST" });
  });

  document.getElementById("lightButton").addEventListener("click", function() {
    lightOn = !lightOn;
    document.getElementById("lightStatus").textContent = lightOn ? "On" : "Off";
    this.textContent = lightOn ? "Turn Light Off" : "Turn Light On";
    this.classList.toggle("on", lightOn);

    // Later ESP32 endpoint:
    // fetch(lightOn ? "/api/light/on" : "/api/light/off", { method: "POST" });
  });

  const manualOutputs = { fan: false, heater: false };
  function renderManual(system) {
    const name = system === "fan" ? "Fan" : "Heater";
    const on = manualOutputs[system];
    document.getElementById(system + "Status").textContent = on ? "On" : "Off";
    const button = document.getElementById(system + "Button");
    button.textContent = "Turn " + name + (on ? " Off" : " On");
    button.classList.toggle("on", on);
  }
  ["fan", "heater"].forEach(function(system) {
    document.getElementById(system + "Button").addEventListener("click", function() {
      if (modes[system] !== "manual") return;
      manualOutputs[system] = !manualOutputs[system];
      renderManual(system);
      // Future POST: /api/<system>/on or /api/<system>/off.
    });
  });

  function validateAutomaticSettings() {
    const pairs = [
      ["humidityThreshold", "humidityOffThreshold", "Mist stop humidity must be higher than its start humidity."],
      ["fanOffThreshold", "fanOnThreshold", "Fan start temperature must be higher than its stop temperature."],
      ["heaterOnThreshold", "heaterOffThreshold", "Heater stop temperature must be higher than its start temperature."]
    ];
    for (const [lowId, highId, message] of pairs) {
      const low = document.getElementById(lowId);
      const high = document.getElementById(highId);
      if (!low.checkValidity() || !high.checkValidity() ||
          !Number.isFinite(low.valueAsNumber) || !Number.isFinite(high.valueAsNumber) ||
          low.valueAsNumber >= high.valueAsNumber) return message;
    }
    const maxRun = document.getElementById("mistMaxRun");
    if (!maxRun.value || !maxRun.checkValidity()) return "Enter a mist maximum run time from 1 to 120 minutes.";
    return "";
  }

  function collectSettings() {
    return {
      irrigation: {
        mode: modes.irrigation,
        schedule: {
          start: document.getElementById("irrigationStart").value,
          durationMinutes: Number(document.getElementById("irrigationDuration").value),
          days: Array.from(document.querySelectorAll("#irrigationDays .day.active")).map(b => b.dataset.day)
        },
        automatic: {
          moistureThresholdPercent: Number(document.getElementById("moistureThreshold").value),
          maxRunMinutes: Number(document.getElementById("irrigationMaxRun").value)
        }
      },
      mist: {
        mode: modes.mist,
        schedule: {
          start: document.getElementById("mistStart").value,
          durationMinutes: Number(document.getElementById("mistDuration").value),
          days: Array.from(document.querySelectorAll("#mistDays .day.active")).map(b => b.dataset.day)
        },
        automatic: {
          humidityOffThresholdPercent: Number(document.getElementById("humidityOffThreshold").value),
          humidityThresholdPercent: Number(document.getElementById("humidityThreshold").value),
          maxRunMinutes: Number(document.getElementById("mistMaxRun").value)
        }
      },
      fan: {
        mode: modes.fan,
        automatic: {
          turnOnAboveF: Number(document.getElementById("fanOnThreshold").value),
          turnOffBelowF: Number(document.getElementById("fanOffThreshold").value)
        }
      },
      heater: {
        mode: modes.heater,
        automatic: {
          turnOnBelowF: Number(document.getElementById("heaterOnThreshold").value),
          turnOffAboveF: Number(document.getElementById("heaterOffThreshold").value)
        }
      },
      light: {
        mode: modes.light,
        schedule: {
          start: document.getElementById("lightStart").value,
          end: document.getElementById("lightEnd").value,
          days: Array.from(document.querySelectorAll("#lightDays .day.active")).map(b => b.dataset.day)
        }
      }
    };
  }

  document.getElementById("saveButton").addEventListener("click", function() {
    const error = validateAutomaticSettings();
    document.getElementById("settingsFeedback").textContent = error || "Settings prepared in this page; not sent to the controller.";
    if (error) return;
    const settings = collectSettings();
    console.log("Settings ready for ESP32:", settings);

    this.textContent = "Settings Prepared";
    setTimeout(() => { this.textContent = "Save Settings"; }, 1200);

    // Later ESP32 endpoint:
    // fetch("/api/settings", {
    //   method: "POST",
    //   headers: { "Content-Type": "application/json" },
    //   body: JSON.stringify(settings)
    // });
  });

  updateAllSummaries();
</script>

</body>
</html>
)rawliteral";
