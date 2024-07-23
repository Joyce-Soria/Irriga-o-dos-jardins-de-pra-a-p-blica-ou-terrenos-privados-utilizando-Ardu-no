
#include <ESP8266WiFi.h> // ou #include <WiFi.h> se estiver usando ESP32
#include <ESP8266HTTPClient.h> // ou #include <HTTPClient.h> se estiver usando ESP32

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* serverName = "http://your-server-address.com/update";

int sensorPin = A0;    // Pin connected to the sensor
int relayPin = D1;     // Pin connected to the relay
int threshold = 600;   // Threshold for soil moisture

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Turn off the relay initially
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.print("Soil Moisture: ");
  Serial.println(sensorValue);

  if (sensorValue < threshold) {
    digitalWrite(relayPin, LOW); // Turn on the relay
    sendDataToServer(sensorValue, "ON");
  } else {
    digitalWrite(relayPin, HIGH); // Turn off the relay
    sendDataToServer(sensorValue, "OFF");
  }
  
  delay(60000); // Wait for a minute before next reading
}

void sendDataToServer(int value, String status) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + "?value=" + String(value) + "&status=" + status;

    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
