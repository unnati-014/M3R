#include <WiFiManager.h>         // Include the WiFiManager library
#include "EmonLib.h"

EnergyMonitor emon;
float voltage;
float current;
float power;
float kWh = 0;
unsigned long lastmillis = millis();

#define RELAY_1_PIN 23
#define RELAY_2_PIN 22
#define RELAY_3_PIN 21
#define RELAY_4_PIN 19

int node1 = 0; // Variable for controlling Relay 1
int node2 = 0; // Variable for controlling Relay 2
int node3 = 0; // Variable for controlling Relay 3
int node4 = 0; // Variable for controlling Relay 4

void setupWiFi() {
  WiFiManager wifiManager;
  wifiManager.autoConnect("ESP32-AP"); // "ESP32-AP" is the default AP name
  Serial.println("Connected to Wi-Fi!");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("Password: ");
  Serial.println(WiFi.psk());
}

void setupRelays() {
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);
  pinMode(RELAY_4_PIN, OUTPUT);

  pinMode(13, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);

  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
  digitalWrite(RELAY_4_PIN, HIGH);
}

void readRelayStatus() {
  node1 = digitalRead(13);
  node2 = digitalRead(12);
  node3 = digitalRead(14);
  node4 = digitalRead(27);
}

void controlRelays() {
  if (node1 == 0) {
    digitalWrite(RELAY_1_PIN, LOW);
  } else {
    digitalWrite(RELAY_1_PIN, HIGH);
  }

  if (node2 == 0) {
    digitalWrite(RELAY_2_PIN, LOW);
  } else {
    digitalWrite(RELAY_2_PIN, HIGH);
  }

  if (node3 == 0) {
    digitalWrite(RELAY_3_PIN, LOW);
  } else {
    digitalWrite(RELAY_3_PIN, HIGH);
  }

  if (node4 == 0) {
    digitalWrite(RELAY_4_PIN, LOW);
  } else {
    digitalWrite(RELAY_4_PIN, HIGH);
  }
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupRelays();
  emon.voltage(35, 106.8, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(34, 0.52); // Current: input pin, calibration.
}

void loop() {
  readRelayStatus();
  controlRelays();

  emon.calcVI(20, 2000); // Calculates voltage and current
  voltage = emon.Vrms, 2; // Get voltage RMS value
  current = emon.Irms, 4; // Get current RMS value
  power = emon.apparentPower, 4; // Get apparent power value
  kWh = kWh + power * (millis() - lastmillis) / 3600000000.0; // Accumulate power usage

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" A");

  Serial.print("Power: ");
  Serial.print(power);
  Serial.println(" W");

  Serial.print("kWh: ");
  Serial.print(kWh, 4);
  Serial.println(" kWh");

  lastmillis = millis(); // Update lastmillis for next iteration

  delay(2000); // Delay between readings
}
