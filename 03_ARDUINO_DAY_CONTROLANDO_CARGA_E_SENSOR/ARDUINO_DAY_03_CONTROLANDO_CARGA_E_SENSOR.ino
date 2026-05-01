#include "DHT.h"
#include <WiFi.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define LED_PIN 2
#define DHTPIN 4
#define DHTTYPE DHT11

#define TIME_BETWEEN_DHT11_READINGS 10000

// WIFI
const char* ssid = "UNITEL NET CASA 2.4GHz_A886";
const char* password = "474frut4mba";
const char* GEMINI_API_KEY = "AIzaSyDOawXaJRDbWGrLk0LzEOqzv37fs2c4110";
const char* MAX_TOKENS = "2000";

// String res = "";
unsigned long int timeDelay = 0;
float humidity = 0;
float temperature = 0;

String userQuestion = "";

DHT dht(DHTPIN, DHTTYPE);


//====================================================
// Conectar WiFi
//====================================================
void connectWiFi() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("CONECTANDO A WIFI...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("CONECTADO A WIFI!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}


//====================================================
// Perguntar à IA
//====================================================
String askGemini(String question) {

  readTemperature();

  HTTPClient https;

  String url =
    "https://generativelanguage.googleapis.com/v1beta/models/"
    "gemini-2.5-flash:generateContent?key="
    + String(GEMINI_API_KEY);

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

    "You control hardware and answer questions naturally. "

    "If the user wants to turn ON the LED, respond ONLY with LED_ON. "
    "If the user wants to turn OFF the LED, respond ONLY with LED_OFF. "

    "You also have access to real sensor data from the environment. "

    "If the user asks about temperature, humidity, weather conditions, "
    "or if it is hot/cold, use the sensor data provided in the prompt "
    "to answer naturally. "

    "Never invent sensor values."
    "\""
    "}]"
    "},"

    "\"contents\":[{"
    "\"parts\":[{"
    "\"text\":\""

    "Sensor Data:\\n"
    "Temperature: "
    + String(temperature) + " Celsius\\n"
                            "Humidity: "
    + String(humidity) + " Percent\\n\\n"

                         "User Question: "
    + question +

    "\""
    "}]"
    "}],"

    "\"generationConfig\":{"
    "\"temperature\":0.2,"
    "\"maxOutputTokens\":"
    + String(MAX_TOKENS) + "}"

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
// LEITURA DO HARDWARE
//====================================================
void readTemperature() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Falha ao ler dados do DHT11!"));
    return;
  }

  // Serial.println("======== LEITURA DO DHT11 ============");
  // Serial.print(F("Humidade: "));
  // Serial.println(humidity);
  // Serial.print(F("%  Temperatura: "));
  // Serial.print(temperature);
  // Serial.println(F("°C "));
  // Serial.println("======================================");
}

//====================================================
// Setup
//====================================================
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  connectWiFi();

  dht.begin();

  Serial.println();
  Serial.println("DUINO AI Pronto");
  Serial.println("Digite a sua pergunta:");
}


//====================================================
// Loop
//====================================================
void loop() {
  if (millis() - timeDelay > TIME_BETWEEN_DHT11_READINGS) {
    readTemperature();
    timeDelay = millis();
  }

  if (Serial.available()) {
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
}
