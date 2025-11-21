void setup() {
    Serial.begin(9600);
    Serial1.begin(115200);  //RX1=Pin19, TX1=Pin18

    Serial.println("Arduino Mega ready...");
    Serial.println("Waiting for commands from ESP8266...");
}

void loop() {
  if (Serial1.available()) {
    String command = Serial1.readString();
    command.trim();
    Serial.println("Received from ESP: '" + command + "'");
    processCommand(command);
  }
  delay(1000);
}

void processCommand(String command) {
  Serial.println("Processing command: " + command);

  if (command == "MOJITO") {
    Serial.println("=== MAKING MOJITO ===");
  }
  else if (command == "MARGARITA") {
    Serial.println("=== MAKING MARGARITA ===");
  }
  else {
    Serial.println("!!! UNKNOWN COMMAND: " + command);
  }

  Serial.println("Waiting for next command...");
}
