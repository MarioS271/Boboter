// WEBUI_CONSTANTS_H
// Boboter
// (C) MarioS271 2025

#pragma once

constexpr const char* WIFI_SSID = "Home";
constexpr const char* WIFI_PASS = "MaR09_MiR19";

constexpr const char* HTML_PAGE = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Robot LED Controller</title>
</head>

<style>
    /* Minimalistisches, dunkles Design (Dark Mode) */
    :root {
        --bg-color: #1a1a1a;
        --text-color: #e0e0e0;
        --card-bg: #2a2a2a;
        --border-color: #444;
        --accent-color: #007bff;
        --selected-border: #4CAF50; /* Grün für Auswahl */
    }

    body {
        font-family: Arial, sans-serif;
        background-color: var(--bg-color);
        color: var(--text-color);
        margin: 0;
        padding: 20px;
        display: flex;
        justify-content: center;
        align-items: flex-start;
        min-height: 100vh;
    }

    .container {
        display: flex;
        gap: 40px;
        max-width: 900px;
        width: 100%;
    }

    /* Roboter-Grafik und LEDs */
    .robot-section {
        flex: 1;
        padding: 20px;
        background-color: var(--card-bg);
        border-radius: 12px;
        border: 1px solid var(--border-color);
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.3);
        display: flex;
        justify-content: center;
        align-items: center;
    }

    .robot-graphic {
        width: 300px;
        height: 300px;
        border-radius: 50%;
        background-color: #333;
        border: 5px solid var(--border-color);
        position: relative;
    }

    .led {
        width: 30px;
        height: 30px;
        border-radius: 50%;
        background-color: #555; /* Ausgeschalteter Zustand */
        border: 2px solid transparent;
        position: absolute;
        cursor: pointer;
        transition: border 0.2s, background-color 0.2s, box-shadow 0.2s;
        box-shadow: 0 0 5px rgba(0, 0, 0, 0.5);
    }

    /* Positionierung der 4 LEDs */
    #led-ul { top: 10%; left: 10%; } /* Upper Left */
    #led-ur { top: 10%; right: 10%; } /* Upper Right */
    #led-ll { bottom: 10%; left: 10%; } /* Lower Left */
    #led-lr { bottom: 10%; right: 10%; } /* Lower Right */

    .led.selected {
        border-color: var(--selected-border);
        box-shadow: 0 0 10px var(--selected-border), 0 0 20px rgba(76, 175, 80, 0.5);
    }

    /* Einstellungs-Box */
    .settings-section {
        flex: 1;
        padding: 20px;
        background-color: var(--card-bg);
        border-radius: 12px;
        border: 1px solid var(--border-color);
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.3);
    }

    .settings-section h2 {
        border-bottom: 2px solid var(--border-color);
        padding-bottom: 10px;
        margin-top: 0;
        color: var(--accent-color);
    }

    .setting-group {
        margin-bottom: 15px;
    }

    .setting-group label {
        display: block;
        margin-bottom: 5px;
        font-weight: bold;
    }

    input[type="color"],
    input[type="range"],
    select {
        width: 100%;
        padding: 8px;
        margin-top: 5px;
        border-radius: 4px;
        border: 1px solid var(--border-color);
        background-color: #444;
        color: var(--text-color);
        box-sizing: border-box; /* Wichtig für die Breite */
    }

    input[type="color"] {
        height: 40px;
        padding: 0;
        background: none;
    }

    /* Button-Styling (optional) */
    button {
        background-color: var(--accent-color);
        color: white;
        padding: 10px 15px;
        border: none;
        border-radius: 4px;
        cursor: pointer;
        transition: background-color 0.2s;
    }

    button:hover {
        background-color: #0056b3;
    }
</style>

<body>
    <div class="container">
        <div class="robot-section">
            <div class="robot-graphic">
                <div class="led selected" id="led-ul" data-id="ul"></div>
                <div class="led" id="led-ur" data-id="ur"></div>
                <div class="led" id="led-ll" data-id="ll"></div>
                <div class="led" id="led-lr" data-id="lr"></div>
            </div>
        </div>

        <div class="settings-section">
            <h2 id="settings-title">Einstellungen: LED Oben Links (UL)</h2>

            <div class="setting-group">
                <label for="color-picker">Farbe (RGB):</label>
                <input type="color" id="color-picker" value="#555555">
            </div>

            <div class="setting-group">
                <label for="brightness-slider">Helligkeit (0-255):</label>
                <input type="range" id="brightness-slider" min="0" max="255" value="128">
            </div>
            
            <div class="setting-group">
                <label for="pattern-select">Muster:</label>
                <select id="pattern-select">
                    <option value="solid">Dauerlicht</option>
                    <option value="blink">Blinken</option>
                    <option value="pulse">Pulsen</option>
                </select>
            </div>
            
            <button onclick="applySettings()">Einstellungen Übertragen</button>
            <p style="font-size: 0.8em; margin-top: 15px;">(Übertragungslogik muss noch implementiert werden)</p>
        </div>
    </div>
</body>

<script>
const leds = document.querySelectorAll('.led');
const settingsTitle = document.getElementById('settings-title');
let selectedLed = 'ul';

leds.forEach(led => {
    led.addEventListener('click', () => {
        leds.forEach(l => l.classList.remove('selected'));
        led.classList.add('selected');
        selectedLed = led.dataset.id;
        updateSettingsTitle(selectedLed);
    });
});

function updateSettingsTitle(id) {
    let title = "Einstellungen: ";
    switch(id) {
        case 'ul': title += "LED Oben Links (UL)"; break;
        case 'ur': title += "LED Oben Rechts (UR)"; break;
        case 'll': title += "LED Unten Links (LL)"; break;
        case 'lr': title += "LED Unten Rechts (LR)"; break;
    }
    settingsTitle.textContent = title;
}

// Transmission logic: send settings to ESP32
function applySettings() {
    const color = document.getElementById('color-picker').value;
    const brightness = document.getElementById('brightness-slider').value;
    const pattern = document.getElementById('pattern-select').value;

    // Convert color hex to RGB
    const r = parseInt(color.substring(1,3),16);
    const g = parseInt(color.substring(3,5),16);
    const b = parseInt(color.substring(5,7),16);

    // Build query string
    const url = `/variables?led=${selectedLed}&r=${r}&g=${g}&b=${b}&brightness=${brightness}&pattern=${pattern}`;

    // Send via GET
    fetch(url)
        .then(resp => resp.text())
        .then(data => console.log("ESP Response:", data))
        .catch(err => console.error(err));

    // Update LED visually
    const currentLedElement = document.getElementById(`led-${selectedLed}`);
    currentLedElement.style.backgroundColor = color;
}
</script>
</html>
)rawliteral";
