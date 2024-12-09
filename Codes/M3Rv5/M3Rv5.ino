#include <WiFiManager.h>         // Include the WiFiManager library
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

#define RELAY_1_PIN 5 //23
#define RELAY_2_PIN 18 //22
#define RELAY_3_PIN 19 //21
#define RELAY_4_PIN  23 //19

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

//unsigned long sendDataPrevMillis = 0;
//int count = 0;
bool signupOK = false;

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
  if (node1 == 0 || appNode1 == 0) {
    digitalWrite(RELAY_1_PIN, LOW);
  } else {
    digitalWrite(RELAY_1_PIN, HIGH);
  }

  if (node2 == 0 || appNode2 == 0) {
    digitalWrite(RELAY_2_PIN, LOW);
  } else {
    digitalWrite(RELAY_2_PIN, HIGH);
  }

  if (node3 == 0 || appNode3 == 0) {
    digitalWrite(RELAY_3_PIN, LOW);
  } else {
    digitalWrite(RELAY_3_PIN, HIGH);
  }

  if (node4 == 0 || appNode4 == 0) {
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

  // Initialize Firebase
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  readRelayStatus();

  emon.calcVI(20,2000);
  // voltage = emon.Vrms, 2;
  current = emon.Irms, 4;
  power = emon.apparentPower, 4;
  kWh = kWh + power * (millis() - lastmillis) / 3600000000.0; // Accumulate power usage
  // For Test
  voltage = 220.6;
  // current = 4;
  // power = 4.6;
  // kWh = 123; // Accumulate power usage

  // Send data to Firebase
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0)) {
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
  Serial.println("______________________________");
  controlRelays();
  delay(2000);
}
