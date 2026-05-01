#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "UNITEL NET CASA 2.4GHz_A886";
const char* password = "474frut4mba";
const char* DeepSeek_Token = "sk-1e35f2add99443ee8f1c1c1318aa6e3d";
const int DeepSeek_Max_Tokens = 1000;
String res = "";


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();


  while (!Serial)
    ;


  // wait for WiFi connection
  WiFi.begin(ssid, password);
  Serial.print("CONECTANDO A WIFI:");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("CONECTADO");
  Serial.print("ENDEREÇO IP: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  Serial.println("");
  Serial.println("FAÇA A SUA PERGUNTA: ");
  while (!Serial.available())
    ;
  while (Serial.available()) {
    char add = Serial.read();
    res = res + add;
    delay(1);
  }
  int len = res.length();
  res = res.substring(0, (len - 1));
  res = "\"" + res + "\"";
  
  Serial.println("=================================================");
  Serial.print("UTILIZADOR: ");
  Serial.println(res);

  HTTPClient https;

  //Serial.print("[HTTPS] begin...\n");
  if (https.begin("https://api.deepseek.com/v1/chat/completions")) {  // HTTPS

    https.addHeader("Content-Type", "application/json");
    https.addHeader("Authorization", "Bearer " + (String)DeepSeek_Token);
    String payload = String("{\"model\":\"deepseek-chat\",\"messages\":[{\"role\":\"user\",\"content\":" + res + "}],\"max_tokens\":" + DeepSeek_Max_Tokens + "}");

    //Serial.print("[HTTPS] POST...\n");

    // start connection and send HTTP header
    int httpCode = https.POST(payload);

    // httpCode will be negative on error
    // file found at server

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload = https.getString();
      // Serial.println(payload);  // PAYLOAD PARA DEBUG

      DynamicJsonDocument doc(4096);


      deserializeJson(doc, payload);
      String Answer = doc["choices"][0]["message"]["content"];


      
      // For Filtering our Special Characters, WhiteSpaces and NewLine Characters
      Answer.trim();
      String filteredAnswer = "";
      for (size_t i = 0; i < Answer.length(); i++) {
        char c = Answer[i];
        if (isalnum(c) || isspace(c)) {
          filteredAnswer += c;
        } else {
          filteredAnswer += ' ';
        }
      }
      Answer = filteredAnswer;


      Serial.print("ASSISTENTE DE IA: ");
      Serial.println(Answer);
      Serial.println("=================================================");
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  res = "";
}