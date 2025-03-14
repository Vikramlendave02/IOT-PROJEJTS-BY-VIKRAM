/*
 * Project: WiFiStatusIndicator using ESP8266
 * Description: Monitors Wi-Fi and internet connectivity, using LEDs to indicate status.
 * Green LED (D1) = Wi-Fi + Internet connected
 * Red LED (D2) = Wi-Fi connected, but no internet
 * Red LED (D2) = No Wi-Fi connection
 * Author: [https://www.linkedin.com/in/vikram-lendave-aa8231267/]
 */

#include <ESP8266WiFi.h>

const int D1 = 5;  // GPIO5
const int D2 = 4;  // GPIO4

#ifndef STASSID
#define STASSID "IOT"
#define STAPSK "IOT12345"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "8.8.8.8"; // Google's DNS server for pinging

void setup() {
  // Initialize LED pins as output
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Set Wi-Fi mode as STA (station mode)
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait until the Wi-Fi is connected
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
  // Check if Wi-Fi is connected
  if (WiFi.status() == WL_CONNECTED) {
    // Now, check if there is an internet connection
    long rtt = WiFi.ping(host); // Ping Google's DNS (8.8.8.8)

    if (rtt >= 0) {
      // If ping successful, internet is available
      Serial.println("Internet connected -->");
      digitalWrite(D1, HIGH);  // Turn on Green LED (Wi-Fi + Internet)
      digitalWrite(D2, LOW);   // Turn off Red LED
    } else {
      // If ping fails, there is no internet despite Wi-Fi connection
      Serial.println("No Internet -->");
      digitalWrite(D2, HIGH);  // Turn on Red LED (Wi-Fi, no internet)
      digitalWrite(D1, LOW);   // Turn off Green LED
    }
    delay(1000);  // Wait for 1 second
  } else {
    // If Wi-Fi is not connected
    Serial.println("Not Connected -->");
    digitalWrite(D2, HIGH);  // Turn on Red LED (No Wi-Fi)
    digitalWrite(D1, LOW);   // Turn off Green LED
    delay(1000);  // Wait for 1 second
  }
}
