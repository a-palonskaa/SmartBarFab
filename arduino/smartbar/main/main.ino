#include <Servo.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

constexpr uint32_t DELAY_BETWEEN_PROCESSING_MS = 1000;

constexpr uint8_t LED_STRIP_PIN = 8;
constexpr uint8_t LED_STRIP_ON_LEVEL  = HIGH;
constexpr uint8_t LED_STRIP_OFF_LEVEL = LOW;

constexpr uint16_t TIMER3_COMPARE_VALUE = 24999;

constexpr uint8_t START_BLINK_COUNT = 3;
constexpr uint8_t START_BLINK_INTERVAL_TICKS = 1;

constexpr uint8_t FINISH_BLINK_COUNT = 2;
constexpr uint8_t FINISH_BLINK_INTERVAL_TICKS = 2;

constexpr uint8_t MAX_LED_TICKS_PENDING = 20;

enum LedMode : uint8_t {
  LED_MODE_STEADY_ON = 0,
  LED_MODE_BLINKING  = 1,
};

volatile uint8_t ledTicksPending = 0;

LedMode ledMode = LED_MODE_STEADY_ON;
bool ledIsOn = true;

uint8_t ledTogglesRemaining = 0;
uint8_t ledBlinkIntervalTicks = 1;
uint8_t ledBlinkTickCounter = 0;

ISR(TIMER3_COMPA_vect) {
  if (ledTicksPending < MAX_LED_TICKS_PENDING) {
    ledTicksPending++;
  }
}

void setLedStrip(bool enabled) {
  ledIsOn = enabled;
  digitalWrite(LED_STRIP_PIN, enabled ? LED_STRIP_ON_LEVEL : LED_STRIP_OFF_LEVEL);
}

uint8_t takeLedTicks() {
  uint8_t ticks = 0;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    ticks = ledTicksPending;
    ledTicksPending = 0;
  }

  return ticks;
}

void finishLedBlinking() {
  ledMode = LED_MODE_STEADY_ON;
  ledTogglesRemaining = 0;
  ledBlinkTickCounter = 0;
  setLedStrip(true);
}

void startLedBlink(uint8_t blinkCount, uint8_t intervalTicks) {
  if (blinkCount == 0) {
    finishLedBlinking();
    return;
  }

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    ledTicksPending = 0;
  }

  ledMode = LED_MODE_BLINKING;
  ledTogglesRemaining = blinkCount * 2;
  ledBlinkIntervalTicks = intervalTicks;
  ledBlinkTickCounter = 0;

  setLedStrip(true);
}

void updateLedOneTimerTick() {
  if (ledMode == LED_MODE_STEADY_ON) {
    if (!ledIsOn) {
      setLedStrip(true);
    }
    return;
  }

  if (ledMode != LED_MODE_BLINKING) {
    finishLedBlinking();
    return;
  }

  ledBlinkTickCounter++;

  if (ledBlinkTickCounter < ledBlinkIntervalTicks) {
    return;
  }

  ledBlinkTickCounter = 0;

  if (ledTogglesRemaining == 0) {
    finishLedBlinking();
    return;
  }

  setLedStrip(!ledIsOn);
  ledTogglesRemaining--;

  if (ledTogglesRemaining == 0) {
    finishLedBlinking();
  }
}

void updateLedStrip() {
  uint8_t ticks = takeLedTicks();

  while (ticks > 0) {
    updateLedOneTimerTick();
    ticks--;
  }
}

void delayWithLedUpdate(uint32_t delayMs) {
  const uint32_t startMs = millis();

  while (millis() - startMs < delayMs) {
    updateLedStrip();
    delay(1);
  }
}

void onPouringStarted() {
  startLedBlink(START_BLINK_COUNT, START_BLINK_INTERVAL_TICKS);
}

void onPouringFinished() {
  startLedBlink(FINISH_BLINK_COUNT, FINISH_BLINK_INTERVAL_TICKS);
}

void initLedStrip() {
  pinMode(LED_STRIP_PIN, OUTPUT);
  setLedStrip(true);
}

void initLedTimer3() {
  noInterrupts();

  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;

  OCR3A = TIMER3_COMPARE_VALUE;

  TCCR3B |= (1 << WGM32);
  TCCR3B |= (1 << CS31) | (1 << CS30);

  TIMSK3 |= (1 << OCIE3A);

  interrupts();
}

enum Drinks {
  Vodka          = 0,
  Rum            = 1,
  Cola           = 2,
  OrangeJuice    = 3,
  PineappleJuice = 4,
  CherryJuice    = 5,
};

const int maxQueueSize = 10;

String commandQueue[maxQueueSize];

int commandQueueStart = 0;
int commandQueueEnd = 0;
int commandQueueCount = 0;

Servo m1, m2, m3, m4, m5, m6;

Servo* drinkMotors[] = {
  &m1,
  &m2,
  &m3,
  &m4,
  &m5,
  &m6,
};

int flowPin1 = 2;
int flowPin2 = 3;
int flowPin3 = 4;
int flowPin4 = 5;
int flowPin5 = 6;
int flowPin6 = 7;

volatile uint32_t counter1 = 0;
volatile uint32_t counter2 = 0;
volatile uint32_t counter3 = 0;
volatile uint32_t counter4 = 0;
volatile uint32_t counter5 = 0;
volatile uint32_t counter6 = 0;

volatile uint32_t* drinkFlowmeters[] = {
  &counter1,
  &counter2,
  &counter3,
  &counter4,
  &counter5,
  &counter6,
};

void count1() { counter1++; }
void count2() { counter2++; }
void count3() { counter3++; }
void count4() { counter4++; }
void count5() { counter5++; }
void count6() { counter6++; }

uint32_t takeAndResetCounter(volatile uint32_t* counter) {
  uint32_t value = 0;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    value = *counter;
    *counter = 0;
  }

  return value;
}

void measure(float targetVolume, volatile uint32_t* counter) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    *counter = 0;
  }

  uint32_t varTime = 0;
  float varQ = 0.0f;
  float varV = 0.0f;

  while (varV < targetVolume) {
    const uint32_t pulses = takeAndResetCounter(counter);

    varQ = (float)pulses / ((float)pulses * 5.9f + 4570.0f);

    varTime = millis();
    (void)varTime;

    varV += varQ;

    delayWithLedUpdate(10);
  }
}

void attachFlowInterrupt(int pin, void (*isr)()) {
  pinMode(pin, INPUT_PULLUP);

  const int interruptPin = digitalPinToInterrupt(pin);

  if (interruptPin == NOT_AN_INTERRUPT) {
    Serial.print("WARNING: pin ");
    Serial.print(pin);
    Serial.println(" has no interrupt");
    return;
  }

  attachInterrupt(interruptPin, isr, RISING);
}

void initFlowSensors() {
  attachFlowInterrupt(flowPin1, count1);
  attachFlowInterrupt(flowPin2, count2);
  attachFlowInterrupt(flowPin3, count3);
  attachFlowInterrupt(flowPin4, count4);
  attachFlowInterrupt(flowPin5, count5);
  attachFlowInterrupt(flowPin6, count6);
}

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
  Serial1.begin(115200);
  Serial1.setTimeout(50);
}

void poorLiquid(Drinks liquidType, int liquidVolume) {
  onPouringStarted();

  drinkMotors[liquidType]->write(0);
  delayWithLedUpdate(200);

  measure(liquidVolume, drinkFlowmeters[liquidType]);

  drinkMotors[liquidType]->write(180);
  delayWithLedUpdate(200);

  onPouringFinished();
}

void cookLONGISLAND() {
  poorLiquid(Drinks::Vodka, 50);
}

void cookBLUELOGOON() {
}

void cookMOJITO() {
}

void cookPORNSTAR() {
}

void cookPINKYMONSTER() {
}

void cookSEXONTHEBICH() {
}

void cookMARGARITA() {
}

void cookMANHATTAN() {
}

void cookSUNRISE() {
}

void cookCUBALIBRE() {
}

void cookRUMCOKE() {
}

void cookCAPECODDER() {
}

void cookSCREWDRIVER() {
}

void cookSEABREEZE() {
}

void cookMADRASS() {
}

void cookTROPICALMIX() {
}

void cookBERRYCITRUS() {
}

void cookDOUBLE_TROUBLE() {
}

void cookCITRUSCOLA() {
}

void cookFRUITPUNCH() {
}

void processCommand(String command) {
  Serial.println("Processing: " + command);

  if (command == "LONGISLAND") {
    cookLONGISLAND();
  } else if (command == "BLUELOGOON") {
    cookBLUELOGOON();
  } else if (command == "MOJITO") {
    cookMOJITO();
  } else if (command == "PORNSTAR") {
    cookPORNSTAR();
  } else if (command == "PINKYMONSTER") {
    cookPINKYMONSTER();
  } else if (command == "SEXONTHEBICH") {
    cookSEXONTHEBICH();
  } else if (command == "MARGARITA") {
    cookMARGARITA();
  } else if (command == "MANHATTAN") {
    cookMANHATTAN();
  } else if (command == "SUNRISE") {
    cookSUNRISE();
  } else if (command == "CUBALIBRE") {
    cookCUBALIBRE();
  } else if (command == "RUMCOKE") {
    cookRUMCOKE();
  } else if (command == "CAPECODDER") {
    cookCAPECODDER();
  } else if (command == "SCREWDRIVER") {
    cookSCREWDRIVER();
  } else if (command == "SEABREEZE") {
    cookSEABREEZE();
  } else if (command == "MADRASS") {
    cookMADRASS();
  } else if (command == "TROPICALMIX") {
    cookTROPICALMIX();
  } else if (command == "BERRYCITRUS") {
    cookBERRYCITRUS();
  } else if (command == "DOUBLE_TROUBLE") {
    cookDOUBLE_TROUBLE();
  } else if (command == "CITRUSCOLA") {
    cookCITRUSCOLA();
  } else if (command == "FRUITPUNCH") {
    cookFRUITPUNCH();
  } else if (command == "VIRGINSUNRISE") {

  } else if (command == "CHERRYCOKE") {

  } else if (command == "VODKA") {

  } else {
    Serial.println("unknown coctail " + command);
  }
}

void addCommandToQueue(String command) {
  if (commandQueueCount < maxQueueSize) {
    commandQueue[commandQueueEnd] = command;
    commandQueueEnd = (commandQueueEnd + 1) % maxQueueSize;
    commandQueueCount++;
  } else {
    Serial.println("queue is full");
  }
}

void readSerialCommandIfAvailable() {
  if (!Serial1.available()) {
    return;
  }

  String command = Serial1.readString();
  command.trim();

  if (command.length() == 0) {
    return;
  }

  addCommandToQueue(command);
}

void processQueuedCommandIfNeeded() {
  static bool hasProcessedCommand = false;
  static uint32_t lastCommandProcessMs = 0;

  if (commandQueueCount == 0) {
    return;
  }

  const uint32_t nowMs = millis();

  if (hasProcessedCommand &&
      nowMs - lastCommandProcessMs < DELAY_BETWEEN_PROCESSING_MS) {
    return;
  }

  String commandToProcess = commandQueue[commandQueueStart];

  commandQueueStart = (commandQueueStart + 1) % maxQueueSize;
  commandQueueCount--;

  processCommand(commandToProcess);

  hasProcessedCommand = true;
  lastCommandProcessMs = millis();
}

void setup() {
  initLedStrip();

  initMotors();
  initSerialPorts();
  initFlowSensors();

  initLedTimer3();

  Serial.println("SmartBarFab started");
}

void loop() {
  updateLedStrip();

  readSerialCommandIfAvailable();
  processQueuedCommandIfNeeded();
}
