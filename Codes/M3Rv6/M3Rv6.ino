#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h>
#include "EmonLib.h"
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

EnergyMonitor emon;
float voltage;
float current;
float power;
float kWh = 0;
unsigned long lastmillis = 0;
unsigned long sendDataPrevMillis = 0;

#define RELAY_1_PIN 23
#define RELAY_2_PIN 22
#define RELAY_3_PIN 21
#define RELAY_4_PIN 19

int node1 = 0; // Variable for controlling Relay 1
int node2 = 0; // Variable for controlling Relay 2
int node3 = 0; // Variable for controlling Relay 3
int node4 = 0; // Variable for controlling Relay 4

bool appNode1;
bool appNode2;
bool appNode3;
bool appNode4;

// Insert Firebase project API Key
#define API_KEY "AIzaSyDi8dAWUfuQtmzZf9Q8YZzIJBi3bWBeEmQ"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "m3r-project-default-rtdb.asia-southeast1.firebasedatabase.app" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

// LCD Configuration
#define I2C_ADDR    0x27 // I2C Address for the LCD
#define LCD_COLS    16   // Number of columns in the LCD
#define LCD_ROWS    2    // Number of rows in the LCD
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

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
  // Setup relay pins as outputs
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);
  pinMode(RELAY_4_PIN, OUTPUT);

  // Set relay pins to initial state (HIGH)
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
  digitalWrite(RELAY_4_PIN, HIGH);
}

void readRelayStatus() {
  // Read the status of relay pins
  node1 = digitalRead(RELAY_1_PIN);
  node2 = digitalRead(RELAY_2_PIN);
  node3 = digitalRead(RELAY_3_PIN);
  node4 = digitalRead(RELAY_4_PIN);
}

void controlRelays() {
  // Control the relays based on node status
  digitalWrite(RELAY_1_PIN, (node1 == 0 || !appNode1) ? LOW : HIGH);
  digitalWrite(RELAY_2_PIN, (node2 == 0 || !appNode2) ? LOW : HIGH);
  digitalWrite(RELAY_3_PIN, (node3 == 0 || !appNode3) ? LOW : HIGH);
  digitalWrite(RELAY_4_PIN, (node4 == 0 || !appNode4) ? LOW : HIGH);
}

void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  setupWiFi();
  setupRelays();
  emon.voltage(35, 106.8, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(34, 0.52); // Current: input pin, calibration.

  // Initialize Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Sign up to Firebase
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase sign up successful");
    signupOK = true;
  } else {
    Serial.println("Firebase sign up failed");
    Serial.println("Reason: " + String(config.signer.signupError.message.c_str()));
  }

  // Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Read relay status
  readRelayStatus();

  // Read voltage, current, and power
  emon.calcVI(20, 2000);
  voltage = emon.Vrms;
  current = emon.Irms;
  power = emon.apparentPower;
  kWh = kWh + power * (millis() - lastmillis) / 3600000000.0; // Accumulate power usage
  lastmillis = millis();

  // Send data to Firebase
  sendDataToFirebase();

  // Control relays
  controlRelays();

  // Print data to LCD
  printLCD();
  
  delay(2000);
}

void printLCD() {
  // Clear the LCD screen
  lcd.clear();

  // Print voltage
  lcd.setCursor(0, 0);
  lcd.print("Voltage: ");
  lcd.print(voltage);
  lcd.print("V");

  // Print kWh
  lcd.setCursor(0, 1);
  lcd.print("kWh: ");
  lcd.print(kWh, 2);

  // Print relay status
  lcd.setCursor(8, 1);
  lcd.print("Relays: ");
  lcd.print(node1);
  lcd.print(node2);
  lcd.print(node3);
  lcd.print(node4);
}

void sendDataToFirebase() {
  // Send data to Firebase
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    if (Firebase.RTDB.setFloat(&fbdo, "M3R/Voltage",voltage)){
       Serial.print("Voltage: ");
       Serial.println(voltage);
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    if (Firebase.RTDB.setFloat(&fbdo, "M3R/Current", current)){
       Serial.print("Current: ");
       Serial.println(current);
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "M3R/Power", power)){
       Serial.print("Power: ");
       Serial.println(power);
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "M3R/KiloWattHour", kWh)){
       Serial.print("Kilo Watt Hour: ");
       Serial.println(kWh);
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    // Here we are reading the value from firebase.
    if (Firebase.RTDB.getBool(&fbdo, "M3R/node1")) {
      // Reading successful, convert the string value to float
      appNode1 = fbdo.boolData();
      
      // Print the retrieved voltage value
      Serial.print("Status of Node1 from APP: ");
      Serial.println(appNode1);
    }
    else {
      // Reading failed, print the error reason
      Serial.println("Failed to read Status of Node1 from APP");
      Serial.println("Reason: " + fbdo.errorReason());
    }

    
    if (Firebase.RTDB.getBool(&fbdo, "M3R/node2")) {
      // Reading successful, convert the string value to float
      appNode2 = fbdo.boolData();
      
      // Print the retrieved voltage value
      Serial.print("Status of Node2 from APP: ");
      Serial.println(appNode2);
    }
    else {
      // Reading failed, print the error reason
      Serial.println("Failed to read Status of Node2 from APP");
      Serial.println("Reason: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.getBool(&fbdo, "M3R/node3")) {
      // Reading successful, convert the string value to float
      appNode3 = fbdo.boolData();
      
      // Print the retrieved voltage value
      Serial.print("Status of Node3 from APP: ");
      Serial.println(appNode3);
    }
    else {
      // Reading failed, print the error reason
      Serial.println("Failed to read Status of Node3 from APP");
      Serial.println("Reason: " + fbdo.errorReason());
    }
    
    if (Firebase.RTDB.getBool(&fbdo, "M3R/node4")) {
      // Reading successful, convert the string value to float
      appNode4 = fbdo.boolData();
      
      // Print the retrieved voltage value
      Serial.print("Status of Node4 from APP: ");
      Serial.println(appNode4);
    }
    else {
      // Reading failed, print the error reason
      Serial.println("Failed to read Status of Node4 from APP");
      Serial.println("Reason: " + fbdo.errorReason());
    }
  }
}