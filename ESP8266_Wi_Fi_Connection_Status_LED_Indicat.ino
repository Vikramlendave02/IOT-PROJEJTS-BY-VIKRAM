#include <ESP8266WiFi.h>
/*int D1 = GLED;
int D2 = RLED;*/
#ifndef STASSID
#define STASSID "Vikram"
#define STAPSK "Vikram123"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "djxmmx.net";
const uint16_t port = 17;

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if(WiFi.status()==WL_CONNECTED)
  {
    Serial.println("Connected-->");
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    delay(100);
  }
  else
  {
    Serial.println("NOT Connected-->");
    digitalWrite(D2, HIGH);
    digitalWrite(D1, LOW);
    delay(100);
  }
  }
