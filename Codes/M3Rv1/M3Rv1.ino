#define RELAY_1_PIN 23
#define RELAY_2_PIN 22
#define RELAY_3_PIN 21
#define RELAY_4_PIN 19

// #define SWITCH_1_PIN 13
// #define SWITCH_2_PIN 12
// #define SWITCH_3_PIN 14
// #define SWITCH_4_PIN 27

int node1 = 0; // Variable for controlling Relay 1
int node2 = 0; // Variable for controlling Relay 2
int node3 = 0; // Variable for controlling Relay 3
int node4 = 0; // Variable for controlling Relay 4

void setup() {
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
  // Initialize Serial Monitor
  Serial.begin(115200);
}

void loop() {
  // Read the value of switch1 and update node1
  node1 = digitalRead(13);
  Serial.print("Value of node1: ");
  Serial.println(node1);
  // Read the value of switch2 and update node2
  node2 = digitalRead(12);
  Serial.print("Value of node2: ");
  Serial.println(node2);
  // Read the value of switch3 and update node3
  node3 = digitalRead(14);
  Serial.print("Value of node3: ");
  Serial.println(node3);
  // Read the value of switch4 and update node4
  node4 = digitalRead(27);
  Serial.print("Value of node4: ");
  Serial.println(node4);
  // Check the value of node1 and control Relay 1
  delay(1000);
  if (node1 == 0) {
    digitalWrite(RELAY_1_PIN, LOW); // Turn on Relay 1
    Serial.println("Relay 1 is ON");
  } else {
    digitalWrite(RELAY_1_PIN, HIGH); // Turn off Relay 1
    Serial.println("Relay 1 is OFF");
  }

  // Check the value of node2 and control Relay 2
  if (node2 == 0) {
    digitalWrite(RELAY_2_PIN, LOW); // Turn on Relay 2
    Serial.println("Relay 2 is ON");
  } else {
    digitalWrite(RELAY_2_PIN, HIGH); // Turn off Relay 2
    Serial.println("Relay 2 is OFF");
  }

  // Check the value of node3 and control Relay 3
  if (node3 == 0) {
    digitalWrite(RELAY_3_PIN, LOW); // Turn on Relay 3
    Serial.println("Relay 3 is ON");
  } else {
    digitalWrite(RELAY_3_PIN, HIGH); // Turn off Relay 3
    Serial.println("Relay 3 is OFF");
  }

  // Check the value of node4 and control Relay 4
  if (node4 == 0) {
    digitalWrite(RELAY_4_PIN, LOW); // Turn on Relay 4
    Serial.println("Relay 4 is ON");
  } else {
    digitalWrite(RELAY_4_PIN, HIGH); // Turn off Relay 4
    Serial.println("Relay 4 is OFF");
  }

  delay(1000); // Delay between each iteration
}
