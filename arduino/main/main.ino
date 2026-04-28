#include <Servo.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

// ============================================================================
// SmartBarFab main Arduino sketch
// ============================================================================
//
// LED strip behavior:
//   - in idle/normal state the LED strip is constantly ON;
//   - at the beginning of pouring it blinks 3 times;
//   - at the end of pouring it blinks 2 times;
//   - after any blink sequence it returns to constantly ON.
//
// Hardware timer requirement:
//   - Timer3 of ATmega2560 is configured manually in CTC mode;
//   - Timer3 generates an interrupt every 100 ms;
//   - ISR is intentionally short: it only accumulates timer ticks;
//   - LED strip state is updated from normal code, not inside ISR.
//
// Assumptions:
//   - Arduino Mega 2560;
//   - simple non-addressable LED strip;
//   - LED strip is controlled through MOSFET/transistor;
//   - LED_STRIP_PIN controls MOSFET gate/base;
//   - HIGH = LED strip ON, LOW = LED strip OFF.
//
// If your LED strip is connected to another pin, change LED_STRIP_PIN only.

// ============================================================================
// Global settings
// ============================================================================

constexpr uint32_t DELAY_BETWEEN_PROCESSING_MS = 1000;

// ============================================================================
// LED strip indication via hardware Timer3
// ============================================================================

constexpr uint8_t LED_STRIP_PIN = 8;

constexpr uint8_t LED_STRIP_ON_LEVEL  = HIGH;
constexpr uint8_t LED_STRIP_OFF_LEVEL = LOW;

// Timer3 configuration for Arduino Mega 2560:
//
// F_CPU      = 16 MHz
// Prescaler = 64
// Timer clk = 16 MHz / 64 = 250 kHz
//
// Desired period = 100 ms = 0.1 s
// Counts = 250000 * 0.1 = 25000
// OCR3A = 25000 - 1 = 24999
constexpr uint16_t TIMER3_COMPARE_VALUE = 24999;

// Timer3 tick = 100 ms.
constexpr uint8_t START_BLINK_COUNT = 3;
constexpr uint8_t START_BLINK_INTERVAL_TICKS = 1;   // 100 ms between toggles

constexpr uint8_t FINISH_BLINK_COUNT = 2;
constexpr uint8_t FINISH_BLINK_INTERVAL_TICKS = 2;  // 200 ms between toggles

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

  // One blink consists of two toggles:
  // ON -> OFF and OFF -> ON.
  ledTogglesRemaining = blinkCount * 2;

  ledBlinkIntervalTicks = intervalTicks;
  ledBlinkTickCounter = 0;

  // The blink sequence always starts from ON.
  // The first timer event will switch the strip OFF.
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

  // Normal state: LED strip is always ON.
  setLedStrip(true);
}

void initLedTimer3() {
  noInterrupts();

  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;

  OCR3A = TIMER3_COMPARE_VALUE;

  // CTC mode: Clear Timer on Compare Match.
  // Timer3 resets when TCNT3 reaches OCR3A.
  TCCR3B |= (1 << WGM32);

  // Prescaler = 64.
  TCCR3B |= (1 << CS31) | (1 << CS30);

  // Enable Timer3 Compare Match A interrupt.
  TIMSK3 |= (1 << OCIE3A);

  interrupts();
}

// ============================================================================
// Drinks, queue, servos, flowmeters
// ============================================================================

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

// NOTE:
// This preserves the pins from your original file.
//
// Important for Arduino Mega 2560:
// external interrupt pins are 2, 3, 18, 19, 20, 21.
// Pins 4, 5, 6, 7 are not external interrupt pins on Mega.
// This code prints a warning instead of blindly attaching an invalid interrupt.
//
// If you want all six flowmeters to work via hardware external interrupts,
// move flowPin3..flowPin6 to 18, 19, 20, 21 and move ESP8266 away from
// Serial1 pins 18/19, for example to Serial2 pins 16/17.
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

// ============================================================================
// Flowmeter measurement
// ============================================================================

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

    // Formula preserved from the original sketch, but now pulses are read
    // atomically because the counter is modified inside an interrupt.
    varQ = (float)pulses / ((float)pulses * 5.9f + 4570.0f);

    varTime = millis();
    (void)varTime;

    varV += varQ;

    // Original code had delay(10). This variant keeps the same delay,
    // but LED blinking remains alive during the blocking wait.
    delayWithLedUpdate(10);
  }
}

void attachFlowInterrupt(int pin, void (*isr)()) {
  pinMode(pin, INPUT_PULLUP);

  const int interruptPin = digitalPinToInterrupt(pin);

  if (interruptPin == NOT_AN_INTERRUPT) {
    Serial.print("WARNING: pin ");
    Serial.print(pin);
    Serial.println(" is not an external interrupt pin on this board");
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

// ============================================================================
// Motors and serial
// ============================================================================

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

  // RX1 = Pin 19, TX1 = Pin 18
  Serial1.begin(115200);

  // Prevent Serial1.readString() from blocking for too long.
  Serial1.setTimeout(50);
}

// ============================================================================
// Pouring
// ============================================================================

void poorLiquid(Drinks liquidType, int liquidVolume) {
  onPouringStarted();

  drinkMotors[liquidType]->write(0);
  delayWithLedUpdate(200);

  measure(liquidVolume, drinkFlowmeters[liquidType]);

  drinkMotors[liquidType]->write(180);
  delayWithLedUpdate(200);

  onPouringFinished();
}

// ============================================================================
// Cocktail functions
// ============================================================================

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

// ============================================================================
// Command processing
// ============================================================================

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
    Serial.println("WARNING: command queue is full");
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

// ============================================================================
// Arduino setup / loop
// ============================================================================

void setup() {
  initLedStrip();

  initMotors();
  initSerialPorts();
  initFlowSensors();

  initLedTimer3();

  Serial.println("SmartBarFab started");
  Serial.println("LED strip is controlled by hardware Timer3");
}

void loop() {
  updateLedStrip();

  readSerialCommandIfAvailable();
  processQueuedCommandIfNeeded();
}
