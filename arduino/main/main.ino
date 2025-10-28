#include <Servo.h>

constexpr int DELAY_BETWEEN_PROCESSING = 1000;

enum Drinks {
    Vodka          = 0,
    Rum            = 1,
    Cola           = 2,
    OrangeJuice    = 3,
    PineappleJuice = 4,
    CherryJuice    = 5,
};

const int maxQueueSize = 10;  // Maximum size of the command queue
String commandQueue[maxQueueSize];
int commandQueueStart = 0;  // Pointer to the start of the queue
int commandQueueEnd = 0;    // Pointer to the end of the queue
int commandQueueCount = 0;  // Keeps track of the number of elements in the queue

Servo m1, m2, m3, m4, m5, m6;

int flowPin1 = 2;
int flowPin2 = 3;
int flowPin3 = 4;
int flowPin4 = 5;
int flowPin5 = 6;
int flowPin6 = 7;

float counter1, counter2, counter3, counter4, counter5, counter6;

float* drinkFlowmeters[] = { &counter1, &counter2, &counter3, &counter4, &counter5, &counter6 };

void count1() { counter1++; }
void count2() { counter2++; }
void count3() { counter3++; }
void count4() { counter4++; }
void count5() { counter5++; }
void count6() { counter6++; }

void measure(float targetVolume, float* counter) {
  *counter = 0;
  uint32_t varTime = 0;
  float varQ = 0, varV = 0;

  while (varV < targetVolume) {
    varQ = *counter / ((float)*counter * 5.9f + 4570.0f);
    *counter = 0;
    varTime = millis();
    varV += varQ;
    delay(10);
  }
}

void initFlowSensors() {
  uint8_t interruptPin1 = digitalPinToInterrupt(flowPin1);
  attachInterrupt(interruptPin1, count1, RISING);

  uint8_t interruptPin2 = digitalPinToInterrupt(flowPin2);
  attachInterrupt(interruptPin2, count2, RISING);

  uint8_t interruptPin3 = digitalPinToInterrupt(flowPin3);
  attachInterrupt(interruptPin3, count3, RISING);

  uint8_t interruptPin4 = digitalPinToInterrupt(flowPin4);
  attachInterrupt(interruptPin4, count4, RISING);

  uint8_t interruptPin5 = digitalPinToInterrupt(flowPin5);
  attachInterrupt(interruptPin5, count5, RISING);

  uint8_t interruptPin6 = digitalPinToInterrupt(flowPin6);
  attachInterrupt(interruptPin6, count6, RISING);
}

Servo* drinkMotors[] = { &m1, &m2, &m3, &m4, &m5, &m6 };

void initMotors() {
  m1.attach(9);
  m2.attach(10);
  m3.attach(11);
  m4.attach(12);
  m5.attach(13);
  m6.attach(14);
}

void initSerialPorts() {
  Serial.begin(9600);
  Serial1.begin(115200);  // RX1 = Pin 19, TX1 = Pin 18
}

void poorLiquid(Drinks liquidType, int liquidVolume) {
  drinkMotors[liquidType]->write(0);
  delay(200);

  measure(liquidVolume, drinkFlowmeters[liquidType]);

  drinkMotors[liquidType]->write(180);
  delay(200);
}

//----------cocktail functions----------
void cookLONGISLAND() {
  poorLiquid(Drinks::Vodka, 50);
  // Add other ingredients here
}

void cookBLUELOGOON() {
  // Logic for Blue Lagoon
}

void cookMOJITO() {
  // Logic for Mojito
}

void cookPORNSTAR() {
  // Logic for Pornstar
}

void cookPINKYMONSTER() {
  // Logic for Pinky Monster
}

void cookSEXONTHEBICH() {
  // Logic for Sex on the Bich
}

void cookMARGARITA() {
  // Logic for Margarita
}

void cookMANHATTAN() {
  // Logic for Manhattan
}

void cookSUNRISE() {
  // Logic for Sunrise
}

void cookCUBALIBRE() {
  // Logic for Cuba Libre
}

void cookRUMCOKE() {
  // Logic for Rum & Coke
}

void cookCAPECODDER() {
  // Logic for Cape Codder
}

void cookSCREWDRIVER() {
  // Logic for Screwdriver
}

void cookSEABREEZE() {
  // Logic for Sea Breeze
}

void cookMADRASS() {
  // Logic for Madrass
}

void cookTROPICALMIX() {
  // Logic for Tropical Mix
}

void cookBERRYCITRUS() {
  // Logic for Berry Citrus
}

void cookDOUBLE_TROUBLE() {
  // Logic for Double Trouble
}

void cookCITRUSCOLA() {
  // Logic for Citrus Cola
}

void cookFRUITPUNCH() {
  // Logic for Fruit Punch
}

void processCommand(String command) {
    Serial.println("Processing: " + command);

    if (command == "LONGISLAND") {
        cookLONGISLAND();
    } else if (command == "BLUELOGOON") {

    } else if (command == "MOJITO") {

    } else if (command == "PORNSTAR") {

    } else if (command == "PINKYMONSTER") {

    } else if (command == "SEXONTHEBICH") {

    } else if (command == "MARGARITA") {

    } else if (command == "MANHATTAN") {

    } else if (command == "SUNRISE") {

    } else if (command == "CUBALIBRE") {

    } else if (command == "RUMCOKE") {

    } else if (command == "CAPECODDER") {

    } else if (command == "SCREWDRIVER") {

    } else if (command == "SEABREEZE") {

    } else if (command == "MADRASS") {

    } else if (command == "TROPICALMIX") {

    } else if (command == "BERRYCITRUS") {

    } else if (command == "DOUBLE_TROUBLE") {

    } else if (command == "CITRUSCOLA") {

    } else if (command == "FRUITPUNCH") {

    } else if (command == "VIRGINSUNRISE") {

    } else if (command == "CHERRYCOKE") {

    } else if (command == "VODKA") {

    } else {
        Serial.println("unknown coctail " + command); // DEBUG
    }
}

void addCommandToQueue(String command) {
  if (commandQueueCount < maxQueueSize) {
    commandQueue[commandQueueEnd] = command;
    commandQueueEnd = (commandQueueEnd + 1) % maxQueueSize;
    commandQueueCount++;
  }
}

void setup() {
  initMotors();
  initSerialPorts();
  initFlowSensors();
}

void loop() {
  if (Serial1.available()) {
    String command = Serial1.readString();
    command.trim();
    addCommandToQueue(command);
  }

  if (commandQueueCount > 0) {
    String commandToProcess = commandQueue[commandQueueStart];
    commandQueueStart = (commandQueueStart + 1) % maxQueueSize;
    commandQueueCount--;
    processCommand(commandToProcess);
  }

  delay(DELAY_BETWEEN_PROCESSING);
}
