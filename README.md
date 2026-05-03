<h1 align="center" style="font-weight: bold;">ARDUINO DAY 2026 — Tecmicro 🤖</h1>

<p align="center">
  <img src="./04_ARDUINO_DAY_CONTROLANDO_CARGA_PELA_WEB/data/Arduino_DAYS2026_Logo.png" alt="Arduino Days 2026" height="80"/>
  &nbsp;&nbsp;&nbsp;
  <img src="./04_ARDUINO_DAY_CONTROLANDO_CARGA_PELA_WEB/data/Tecmicro-logo.png" alt="Tecmicro" height="80"/>
</p>


<p align="center">
  <a href="https://www.linkedin.com/in/erivelto-silva-39a61a275"><img src="https://img.shields.io/badge/Ask%20me-anything-1abc9c.svg"/></a>
  &ensp;&nbsp;
  <img src="https://img.shields.io/badge/License-MIT-blue.svg"/>
  &ensp;&nbsp;
  <img src="https://img.shields.io/badge/-Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white"/>
  &ensp;&nbsp;
  <img src="https://img.shields.io/badge/ESP32-IoT-blue?style=for-the-badge&logo=espressif"/>
  &ensp;&nbsp;
  <img src="https://img.shields.io/badge/AI-DeepSeek-blueviolet?style=for-the-badge"/>
</p>

<p align="center">
  <img src="./docs/event_poster.png" alt="Arduino Day 2026 — Tecmicro Event Poster" width="720"/>
</p>


`Menu:`
<ol>
  <li><a href="#about">About</a></li>
  <li><a href="#project-structure">Project Structure</a></li>
  <li><a href="#features">Features</a></li>
  <li><a href="#components">Hardware Components</a></li>
  <li><a href="#libraries">Libraries</a></li>
  <li><a href="#getting-started">Getting Started</a></li>
  <li><a href="#collaborators">Collaborators</a></li>
  <li><a href="#license">License</a></li>
  <li><a href="#keywords">Key Words</a></li>
  <li><a href="#references">References</a></li>
</ol>

---

<h2 id="about">📌 About</h2>

<p>
  <strong>
    A progressive series of ESP32 + AI projects built live at Arduino Days 2026, sponsored by Tecmicro.
    Starting from a basic AI chat over Serial Monitor and evolving into a fully self-hosted web dashboard
    where an AI assistant reads real sensors, controls hardware, and maintains conversation context —
    all running on a single ESP32.
  </strong>
</p>

> Get your DeepSeek API key at **[platform.deepseek.com](https://platform.deepseek.com)**

> 📄 Full presentation material: **[Arduino + IoT + GenAI (PDF)](./docs/Arduino_mais_IoT_mais_GenAI.pdf)**

---

<h2 id="project-structure">📁 Project Structure</h2>

```
ARDUINO_DAY_2026_Tecmicro/
│
├── 01_ARDUINO_DAY_ESP32_DeepSeek/
│   └── ARDUINO_DAY_01_ESP32_DeepSeek.ino
│       ESP32 connects to Wi-Fi and chats with DeepSeek AI via Serial Monitor.
│
├── 02_ARDUINO_DAY_CONTROLANDO_CARGA/
│   └── 02_ARDUINO_DAY_CONTROLANDO_CARGA.ino
│       AI chat + LED control. The AI responds LED_ON or LED_OFF to toggle hardware.
│
├── 03_ARDUINO_DAY_CONTROLANDO_CARGA_E_SENSOR/
│   └── 03_ARDUINO_DAY_CONTROLANDO_CARGA_E_SENSOR.ino
│       AI + LED + DHT11 sensor. Real temperature & humidity injected into every AI prompt.
│
└── 04_ARDUINO_DAY_CONTROLANDO_CARGA_PELA_WEB/
    ├── 04_ARDUINO_DAY_CONTROLANDO_CARGA_PELA_WEB.ino   ← ESP32 web server (SPIFFS + mDNS)
    └── data/                                            ← Files served from ESP32 filesystem
        ├── login.html / login.css
        ├── dashboard.html / dashboard.css / dashboard.js
        ├── Arduino_DAYS2026_Logo.png
        └── Tecmicro-logo.png
```

| # | Project | Interface | AI called from | Sensor |
|---|---------|-----------|----------------|--------|
| 01 | Basic AI Chat | Serial Monitor | ESP32 | — |
| 02 | AI + LED Control | Serial Monitor | ESP32 | — |
| 03 | AI + LED + Sensor | Serial Monitor | ESP32 | DHT11 |
| 04 | Full Web Dashboard | Browser | JavaScript | DHT11 |

---

<h2 id="features">✨ Features</h2>

- **DeepSeek AI integration** — natural language assistant running on or alongside the ESP32
- **LED command execution** — AI returns `LED_ON` / `LED_OFF` tokens that trigger real hardware
- **DHT11 sensor context** — live temperature & humidity injected into every AI system prompt
- **Self-hosted web dashboard** — ESP32 serves HTML/CSS/JS directly from SPIFFS; no cloud needed
- **Login system** — username/password gate before the dashboard (`admin` / `admin1234`)
- **Live sensor cards** — temperature, humidity, and LED state polled every 2 seconds
- **Animated LED card** — visual LED bulb that glows amber when the real LED is ON
- **Conversation memory** — rolling buffer of the last 10 messages for short-term AI context
- **mDNS support** — access the dashboard at `http://duinoai.local` on the local network
- **Arduino Days 2026 branding** — event and sponsor logos embedded in login and dashboard
- **Responsive dark UI** — Arduino teal palette, works on desktop and mobile

---

<h2 id="components">🔧 Hardware Components</h2>

| Component | Purpose |
|-----------|---------|
| ESP32 DevKit | Microcontroller, Wi-Fi, web server |
| DHT11 Sensor | Temperature and humidity readings |
| Onboard LED (GPIO 2) | Hardware output controlled by AI |
| USB Cable | Programming and power supply |
| Wi-Fi Router | Network access for API and dashboard |

**DHT11 Wiring:**

| DHT11 | ESP32 |
|-------|-------|
| VCC | 3.3V |
| GND | GND |
| DATA | GPIO 4 |

---

<h2 id="libraries">📦 Libraries</h2>

Install via **Arduino IDE → Sketch → Include Library → Manage Libraries**:

| Library | Projects |
|---------|---------|
| DHT sensor library *(Adafruit)* | 03, 04 |
| ArduinoJson | 01, 02, 03 |
| AsyncTCP | 04 |
| ESPAsyncWebServer | 04 |
| ESPmDNS *(built-in ESP32)* | 04 |
| SPIFFS *(built-in ESP32)* | 04 |
| WiFi *(built-in ESP32)* | All |
| HTTPClient *(built-in ESP32)* | 01, 02, 03 |

---

<h2 id="getting-started">🚀 Getting Started</h2>

**1. Clone the repository**
```bash
git clone https://github.com/eriveltosilva/ARDUINO_DAY_2026_Tecmicro.git
```

**2. Set your Wi-Fi credentials** — in every `.ino` file:
```cpp
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

**3. Set your DeepSeek API key**

Projects 01–03 (in the `.ino` file):
```cpp
const char* DEEPSEEK_API_KEY = "YOUR_DEEPSEEK_API_KEY";
```

Project 04 (in `data/dashboard.js`):
```js
const DEEPSEEK_API_KEY = "YOUR_DEEPSEEK_API_KEY";
```

**4. Install all required libraries** *(see Libraries section above)*

**5. Select the ESP32 board**
`Tools → Board → ESP32 Arduino → ESP32 Dev Module`

**6. Upload the sketch** — click the Upload (→) button in Arduino IDE

**7. Upload SPIFFS data** *(project 04 only)*
`Tools → ESP32 Sketch Data Upload`
> Requires the [ESP32 Sketch Data Uploader](https://github.com/me-no-dev/arduino-esp32fs-plugin) plugin.

**8. Open the dashboard** *(project 04)*

Check the Serial Monitor for the IP address, then open:
```
http://duinoai.local         ← mDNS (if supported by your router)
http://<ESP32_IP_ADDRESS>    ← direct IP fallback
```
Login: **admin / admin1234**

---

<h2 id="collaborators">👥 Collaborators</h2>

Special thank you to everyone who contributed to this project.

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/eriveltosilva">
        <img src="https://avatars.githubusercontent.com/u/125351173?s=400&u=d6b335b5e253bb45f3ed82ec322684ab203b5243&v=4" width="100px;" alt="Erivelto Silva"/><br>
        <sub><b>Erivelto Silva</b></sub>
      </a>
    </td>
  </tr>
</table>

---

<h2 id="license">📄 License</h2>

This project is licensed under the [MIT License](LICENSE) — Erivelto Silva.

---

<h2 id="keywords">🔑 Key Words</h2>

ESP32, Arduino, IoT, Artificial Intelligence, DeepSeek API, DHT11, Temperature, Humidity, LED Control, SPIFFS, ESPAsyncWebServer, Web Dashboard, AI Chat, Alexa, Conversation Memory, mDNS, Arduino Days 2026, Tecmicro, Angola, Maker, Open Source, Embedded Systems.


<h2 id="references"> References</h2>

<h3 id="cURL"> cURL to be imported on Postman</h3>

1. Arduino_DAY_01_TESTE_DeepSeek_API

```bash
curl --location 'https://api.deepseek.com/v1/chat/completions' \
--header 'Content-Type: application/json' \
--header 'Authorization: Bearer SEU_DEEPSEEK_API_KEY' \
--data '{
  "model": "deepseek-chat",
  "messages": [
    {
      "role": "user",
      "content": "HOW MUCH IS 2+9?"
    }
  ]
}'
```

2.Arduino_DAY_01_TESTE_DeepSeek_API
```bash
curl --location 'https://api.deepseek.com/v1/chat/completions' \
--header 'Content-Type: application/json' \
--header 'Authorization: Bearer SEU_DEEPSEEK_API_KEY' \
--data '{
  "model": "deepseek-chat",
  "messages": [
    {
      "role": "system",
      "content": "YOU ARE DUINO, A VIRTUAL ASSISTANT FOR ARDUINO DAY 2026. GIVE OBJECTIVE RESPONSES TO USERS."
    },
    {
      "role": "user",
      "content": "O ESP32 tem mais melhoria que o arduino?"
    }
  ],
  "temperature": 0.2,
  "max_tokens": 1000
}'
```


<h2 id="references"> References</h2>

- <a href="https://lastminuteengineers.com/esp32-pinout-reference/">ESP32 Pinout Reference</a>

- <a href="https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/">Installing the ESP32 Board in Arduino IDE (Windows, Mac OS X, Linux)</a>

- <a href="https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/">Install ESP32 Filesystem Uploader in Arduino IDE 1.19</a>

- <a href="https://randomnerdtutorials.com/arduino-ide-2-install-esp32-littlefs/">Arduino IDE 2: Install ESP32 LittleFS Uploader (Upload Files to the Filesystem)</a>

- <a href="https://github.com/earlephilhower/arduino-littlefs-upload/releases">LittleFS Uploader</a>

- <a href="https://platform.deepseek.com/usage">DeepSeek Platform</a>
