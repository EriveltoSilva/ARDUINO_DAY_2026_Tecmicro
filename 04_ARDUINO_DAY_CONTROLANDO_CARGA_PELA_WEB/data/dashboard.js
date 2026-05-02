const DEEPSEEK_API_KEY = "sk-1e35f2add99443ee8f1c1c1318aa6e3d";
const DEEPSEEK_URL = "https://api.deepseek.com/v1/chat/completions";

const SYSTEM_PROMPT =
  "You are a smart assistant like Alexa built into an ESP32 device. " +
  "You can talk normally with users. " +
  "If the user wants to turn ON the LED, respond ONLY with the exact text: LED_ON. " +
  "If the user wants to turn OFF the LED, respond ONLY with the exact text: LED_OFF. " +
  "For any other question, answer naturally and concisely. " +
  "Never explain what LED_ON or LED_OFF mean.";

// ---- DOM references ----
const temperatureSensor = document.getElementById("temperatureSensor");
const humiditySensor = document.getElementById("humiditySensor");
const ledIcon = document.getElementById("ledIcon");
const btnLed = document.getElementById("btnLed");
const chatMessages = document.getElementById("chatMessages");
const chatInput = document.getElementById("chatInput");
const btnSend = document.getElementById("btnSend");
const aiStatus = document.getElementById("aiStatus");

let currentLedState = false;

// ======================================================
// Data polling
// ======================================================
window.addEventListener("load", () => {
  receiveData();
  setInterval(receiveData, 2000);
});

function receiveData() {
  fetch("/dados")
    .then((resp) => resp.json())
    .then((data) => {
      temperatureSensor.textContent = data.temp.toFixed(1) + " °C";
      humiditySensor.textContent = data.humidity.toFixed(1) + " %";
      currentLedState = data.ledState;
      updateLedButton();
    })
    .catch((err) => console.error("Erro ao buscar dados:", err));
}

function updateLedButton() {
  if (currentLedState) {
    btnLed.textContent = "APAGAR";
    btnLed.className = "btn btn-danger";
    ledIcon.style.opacity = "1";
  } else {
    btnLed.textContent = "LIGAR";
    btnLed.className = "btn btn-primary";
    ledIcon.style.opacity = "0.35";
  }
}

// ======================================================
// LED manual control
// ======================================================
btnLed.addEventListener("click", () => {
  const url = currentLedState ? "/led/off" : "/led/on";
  fetch(url)
    .then((resp) => resp.json())
    .then(() => receiveData())
    .catch((err) => console.error("Erro ao controlar LED:", err));
});

// ======================================================
// Chat helpers
// ======================================================
function addMessage(text, role) {
  const div = document.createElement("div");
  div.className = "message " + role;
  div.textContent = text;
  chatMessages.appendChild(div);
  chatMessages.scrollTop = chatMessages.scrollHeight;
  return div;
}

function setAiStatus(text) {
  aiStatus.textContent = text;
}

function setBusy(busy) {
  btnSend.disabled = busy;
  chatInput.disabled = busy;
  setAiStatus(busy ? "a pensar..." : "pronto");
}

// ======================================================
// Send message to DeepSeek
// ======================================================
btnSend.addEventListener("click", sendMessage);

chatInput.addEventListener("keydown", (e) => {
  if (e.key === "Enter") sendMessage();
});

function sendMessage() {
  const text = chatInput.value.trim();
  if (!text) return;

  chatInput.value = "";
  addMessage(text, "user");

  const thinking = addMessage("A pensar...", "thinking");
  setBusy(true);

  fetch(DEEPSEEK_URL, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
      Authorization: "Bearer " + DEEPSEEK_API_KEY,
    },
    body: JSON.stringify({
      model: "deepseek-chat",
      messages: [
        { role: "system", content: SYSTEM_PROMPT },
        { role: "user", content: text },
      ],
      temperature: 0.2,
      max_tokens: 500,
    }),
  })
    .then((resp) => resp.json())
    .then((data) => {
      thinking.remove();
      setBusy(false);

      const reply = data.choices[0].message.content.trim();

      if (reply === "LED_ON") {
        fetch("/led/on")
          .then(() => receiveData())
          .catch((err) => console.error(err));
        addMessage("LED ligado!", "bot");
      } else if (reply === "LED_OFF") {
        fetch("/led/off")
          .then(() => receiveData())
          .catch((err) => console.error(err));
        addMessage("LED desligado!", "bot");
      } else {
        addMessage(reply, "bot");
      }
    })
    .catch((err) => {
      thinking.remove();
      setBusy(false);
      addMessage("Erro ao contactar a IA. Verifique a sua chave API.", "bot");
      console.error("DeepSeek error:", err);
    });
}
