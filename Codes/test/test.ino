#include "EmonLib.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial
#define vCalibration 106.8
#define currCalibration 0.52
#define relay1 D1
#define relay2 D2
#define relay3 D3
#define relay4 D4
#define gled D5
#define rled D6
#define buzzer D7

// Credentials for Blynk and WiFi
char auth[] = "OogLgIO8h20Pzq3G4fvNw-JjyQVF2FQi";
char ssid[] = "Redmi Note 10 Pro";
char pass[] = "jss9wuznbcumeti";
BlynkTimer timer;

EnergyMonitor emon;
float voltage;
float current;
float power;
float kWh = 0;
unsigned long lastmillis = millis();

void setup() {
  Serial.begin(115200);
  emon.voltage(A0, vCalibration, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(A1, currCalibration); // Current: input pin, calibration.
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(gled, OUTPUT);
  pinMode(rled, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  while (Blynk.connect() == false) {
    // Wait until connected
  }
}

void myTimerEvent() {
  emon.calcVI(20, 2000);
  voltage = emon.Vrms;
  Serial.print("Vrms: ");
  Serial.print(voltage);
  Serial.print("V");
  Blynk.virtualWrite(V0, voltage);

  current = emon.Irms;
  Serial.print("\tIrms: ");
  Serial.print(current);
  Serial.print("A");
  Blynk.virtualWrite(V1, current);

  if (voltage >= 180 && voltage <= 220) {
    digitalWrite(rled, LOW);
    digitalWrite(gled, HIGH);
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
  } else {
    digitalWrite(gled, LOW);
    digitalWrite(rled, HIGH);
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    tone(buzzer, 500);
  }

  power = emon.apparentPower;
  Serial.print("\tPower: ");
  Serial.print(power);
  Serial.println("W");
  Blynk.virtualWrite(V2, power);

  kWh = kWh + emon.apparentPower * (millis() - lastmillis) / 3600000000.0;
  Serial.print("\tkWh: ");
  Serial.print(kWh, 4);
  Serial.println("kWh");
  Blynk.virtualWrite(V3, kWh);
  lastmillis = millis();
}

void loop() {
  Blynk.run();
  timer.run();
}
