#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define LED_PIN 2

// WIFI
const char* ssid = "UNITEL NET CASA 2.4GHz_A886";
const char* password = "474frut4mba";
const char* GEMINI_API_KEY = "AIzaSyDOawXaJRDbWGrLk0LzEOqzv37fs2c4110";
const char* MAX_TOKENS = "1000";

// String res = "";
String userQuestion = "";

//====================================================
// Conectar WiFi
//====================================================
void connectWiFi() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}


//====================================================
// Perguntar à IA
//====================================================
String askGemini(String question) {

  HTTPClient https;

  String url =
      "https://generativelanguage.googleapis.com/v1beta/models/"
      "gemini-2.5-flash:generateContent?key=" +
      String(GEMINI_API_KEY);

  if (!https.begin(url)) {
    return "ERROR_CONNECTION";
  }

  https.addHeader("Content-Type", "application/json");

  // System role + pergunta
  String payload =
      "{"
      "\"systemInstruction\":{"
        "\"parts\":[{"
          "\"text\":\""
          "You are a smart assistant like Alexa. "
          "You can talk normally with users. "
          "If user wants to turn ON the LED, respond ONLY with LED_ON. "
          "If user wants to turn OFF the LED, respond ONLY with LED_OFF. "
          "For any other question, answer naturally. "
          "Never explain your commands."
          "\""
        "}]"
      "},"

      "\"contents\":[{"
        "\"parts\":[{"
          "\"text\":\"" + question + "\""
        "}]"
      "}],"

      "\"generationConfig\":{"
        "\"temperature\":0.2,"
        "\"maxOutputTokens\":" + String(MAX_TOKENS) +
      "}"

      "}";

  int httpCode = https.POST(payload);

  if (httpCode != HTTP_CODE_OK) {
    Serial.print("HTTP Error: ");
    Serial.println(httpCode);

    https.end();
    return "ERROR_REQUEST";
  }

  String response = https.getString();

  https.end();

  DynamicJsonDocument doc(8192);
  DeserializationError err = deserializeJson(doc, response);

  if (err) {
    return "ERROR_JSON";
  }

  String aiResponse = doc["candidates"][0]["content"]["parts"][0]["text"];

  aiResponse.trim();
  return aiResponse;
}


//====================================================
// Executar comandos de hardware
//====================================================
bool executeIfHardwareCommand(String response) {

  response.trim();

  if (response == "LED_ON") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("DEVICE ACTION: LED ligado");
    return true;
  }

  if (response == "LED_OFF") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("DEVICE ACTION: LED desligado");
    return true;
  }

  return false;
}


//====================================================
// Setup
//====================================================
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  connectWiFi();

  Serial.println();
  Serial.println("DUINO AI Pronto");
  Serial.println("Digite a sua pergunta:");
}


//====================================================
// Loop
//====================================================
void loop() {
  if (!Serial.available()) {
    return;
  }

  String userQuestion = Serial.readStringUntil('\n');

  userQuestion.trim();
  if (userQuestion.length() == 0) {
    return;
  }

  
  Serial.println("==============================================");
  Serial.print("UTILIZADOR: ");
  Serial.println(userQuestion);

  // Pergunta para a IA
  String aiResponse = askGemini(userQuestion);


  //---------------------------------------------------
  Serial.print("RESPOSTA BRUTA: ");
  Serial.println(aiResponse);
  //--------------------------------------------------

  // Se for comando, executa.
  bool wasCommand = executeIfHardwareCommand(aiResponse);

  // Se não for comando, conversa normalmente.
  if (!wasCommand) {
    Serial.print("DUINO AI: ");
    Serial.println(aiResponse);
  }

  Serial.println("==============================================");
  Serial.println("(DUINO) Alguma outra questão?:");
}

