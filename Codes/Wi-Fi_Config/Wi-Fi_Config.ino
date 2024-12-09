#include <WiFiManager.h>         // Include the WiFiManager library

void setup() {
  Serial.begin(115200);
  
  // Create an instance of WiFiManager
  WiFiManager wifiManager;
  
  // Uncomment the following line if you want to reset the saved settings
  // wifiManager.resetSettings();
  
  // Start the Wi-Fi manager, which will create an access point and serve a configuration portal
  wifiManager.autoConnect("ESP32-AP"); // "ESP32-AP" is the default AP name
  
  // Once connected, you can retrieve the SSID and password entered by the user
  Serial.println("Connected to Wi-Fi!");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("Password: ");
  Serial.println(WiFi.psk());
}

void loop() {
  // Your code here
}
