#include <Servo.h>
#include <avr/interrupt.h>

constexpr int DELAY_BETWEEN_PROCESSING = 1000;

enum Drinks
{
    Vodka = 0,
    Rum = 1,
    Cola = 2,
    OrangeJuice = 3,
    PineappleJuice = 4,
    CherryJuice = 5,
};

//--------------------constants for smartbar--------------------
const int maxQueueSize = 10; // Maximum size of the command queue
String commandQueue[maxQueueSize];
int commandQueueStart = 0; // Pointer to the start of the queue
int commandQueueEnd = 0;   // Pointer to the end of the queue
int commandQueueCount = 0; // Keeps track of the number of elements in the queue

const int DIR_PIN = 8;  // direction
const int STEP_PIN = 9; // step (pulse)
const int ENABLE_PIN = 10;
const float STEP_ANGLE = 1.8;                      // degrees per full step
const int stepsPerRev = (int)(360.0 / STEP_ANGLE); // 200
const float degreesToMove = 600.0;
const int microstep = 1; // 1 = full step, 2 = half, 4 = 1/4, 8 = 1/8, 16 = 1/16

// delays for speed
const unsigned long stepPulseWidth = 2000; // STEP pulse width (us)
const unsigned long stepDelay = 10000;     // delay between pulses (us)

long stepsNeeded = 0;

//--------------------end constants for smartbar--------------------

Servo m1, m2, m3, m4, m5, m6;
Servo *drinkMotors[] = {&m1, &m2, &m3, &m4, &m5, &m6};
int motorCounts[] = {0, 0, 0, 0, 0, 0};
int timePortion[] = {15060, 14820, 14750, 14810, 15270, 15510, 16070, 16290, 17270, 17730, 18730, 19780, 20860, 25680};

void initMotors()
{
    m1.attach(4);
    m2.attach(7);
    m3.attach(2);
    m4.attach(6);
    m5.attach(3);
    m6.attach(5);
}

void initSerialPorts()
{
    Serial.begin(9600);
    Serial1.begin(115200); // RX1 = Pin 19, TX1 = Pin 18
}

const int R_PIN = A0; // analog Arduino UNO pins (as they can be used as GPIO)
const int G_PIN = A1;
const int B_PIN = A2;

volatile bool timerTick = false; // timer used for blinking, changes in ISR

bool blinkActive = false;
int blinkLeft = 0; // 1 blink = 2 changes (on -> off, off -> on)
int blinkTicks = 0;
int blinkPeriod = 10; // ticks before toggles,  ~100ms

int blinkPin = B_PIN; // color to blink with
bool blinkState = true;

void lightOff()
{
    digitalWrite(R_PIN, LOW);
    digitalWrite(G_PIN, LOW);
    digitalWrite(B_PIN, LOW);
}

void lightColor(int pin)
{
    lightOff();
    digitalWrite(pin, HIGH);
}

void initLighting()
{
    pinMode(R_PIN, OUTPUT);
    pinMode(G_PIN, OUTPUT);
    pinMode(B_PIN, OUTPUT);

    lightColor(G_PIN);
}

ISR(TIMER2_COMPA_vect) // interrupt handler from Timer2, COMPA = compare A
{
    timerTick = true;
}

void initLightTimer2() // setting for Timer2 func
{
    noInterrupts();

    TCCR2A = 0; // Timer Counter Control Register A (bits COM2A1 COM2A0 COM2B1 COM2B0 ... WGM21 WGM20)
    TCCR2B = 0; // Timer Counter Control Register A (bits WGM22 CS22 CS21 CS20)
    TCNT2 = 0;  // Timer Counter Register (coints 0, 1, 2, 3, ..)

    OCR2A = 155; // Output  Compare Register A
    // if TCNT2 == OCR2A -> Compare Match A
    TCCR2A |= (1 << WGM21);                            // CTC mode  (Clear Timer on Compare Match)
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // prescaler for Timer2
    TIMSK2 |= (1 << OCIE2A);                           // Timer Counter2 Interrupt Mask Register

    interrupts();
}

void startBlink(int pin, int count, int periodTicks)
{
    blinkActive = true;
    blinkPin = pin;
    blinkLeft = count * 2;
    blinkPeriod = periodTicks;
    blinkTicks = 0;
    blinkState = true;

    lightColor(blinkPin);
}

void updateLightning()
{
    if (!timerTick)
    {
        return;
    }

    timerTick = false;

    if (!blinkActive)
    {
        return;
    }

    blinkTicks++;

    if (blinkTicks < blinkPeriod)
    {
        return;
    }

    blinkTicks = 0;
    blinkState = !blinkState;

    if (blinkState)
    {
        lightColor(blinkPin);
    }
    else
    {
        lightOff();
    }

    blinkLeft--;

    if (blinkLeft <= 0)
    {
        blinkActive = false;
        lightColor(G_PIN);
    }
}

void delayWithLightUpdate(unsigned long delayMs)
{
    unsigned long start = millis();

    while (millis() - start < delayMs)
    {
        updateLightning();
        delay(1);
    }
}

void onPouringStarted()
{
    startBlink(B_PIN, 3, 10);
}

void onPouringFinished()
{
    startBlink(R_PIN, 2, 20);
}

void poorLiquid(Drinks liquidType, int portion)
{
    int number = motorCounts[liquidType];
    if (number + portion > 14)
    {
        Serial.println("ran out of liquid");
        return;
    }

    int time = 0;
    for (int n = number - 1; n < number + portion; n++)
    {
        time += timePortion[n];
    }

    motorCounts[liquidType] += portion;

    onPouringStarted(); // blinking blue when starting to pour our drink

    drinkMotors[liquidType]->write(0);
    drinkMotors[liquidType]->write(100);

    delayWithLightUpdate(time); // prevents light from blinking

    drinkMotors[liquidType]->write(0);

    onPouringFinished(); // blinking red when finishing
}

//---------------lower servo---------------

void iniLowerServo()
{
    pinMode(DIR_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);

    digitalWrite(ENABLE_PIN, LOW);
    digitalWrite(DIR_PIN, LOW); // HIGH if for another direction

    stepsNeeded = (long)((stepsPerRev * (degreesToMove / 360.0)) * microstep);
}

void stepMotor(long steps)
{
    for (long i = 0; i < steps; ++i)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(stepPulseWidth);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(stepDelay);
    }
}

//----------cocktail functions----------
void cookLONGISLAND()
{
    poorLiquid(Drinks::Vodka, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Rum, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Cola, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::OrangeJuice, 1);
    stepMotor(3 * stepsNeeded);
}

void cookBLUELOGOON()
{
    poorLiquid(Drinks::Vodka, 1);
    stepMotor(4 * stepsNeeded);
    poorLiquid(Drinks::PineappleJuice, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 1);
    stepMotor(stepsNeeded);
}

void cookMOJITO()
{
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Rum, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Cola, 1);
    stepMotor(2 * stepsNeeded);
    poorLiquid(Drinks::PineappleJuice, 2);
    stepMotor(2 * stepsNeeded);
}

void cookPORNSTAR()
{
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Rum, 2);
    stepMotor(3 * stepsNeeded);
    poorLiquid(Drinks::PineappleJuice, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 1);
    stepMotor(stepsNeeded);
}

void cookPINKYMONSTER()
{
    poorLiquid(Drinks::Vodka, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Rum, 1);
    stepMotor(2 * stepsNeeded);
    poorLiquid(Drinks::OrangeJuice, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::PineappleJuice, 1);
    stepMotor(stepsNeeded);
}

void cookSEXONTHEBICH()
{
    poorLiquid(Drinks::Vodka, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::OrangeJuice, 1);
    stepMotor(3 * stepsNeeded);
    poorLiquid(Drinks::PineappleJuice, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 1);
    stepMotor(stepsNeeded);
}

void cookMARGARITA()
{
    poorLiquid(Drinks::Vodka, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::OrangeJuice, 1);
    stepMotor(3 * stepsNeeded);
    poorLiquid(Drinks::PineappleJuice, 1);
    stepMotor(2 * stepsNeeded);
}

void cookMANHATTAN()
{
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Rum, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Cola, 1);
    stepMotor(3 * stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 1);
    stepMotor(stepsNeeded);
}

void cookSUNRISE()
{
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Rum, 1);
    stepMotor(2 * stepsNeeded);
    poorLiquid(Drinks::OrangeJuice, 1);
    stepMotor(2 * stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 1);
    stepMotor(stepsNeeded);
}

void cookCUBALIBRE()
{
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Rum, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Cola, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::OrangeJuice, 1);
    stepMotor(3 * stepsNeeded);
}

void cookRUMCOKE()
{
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Rum, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Cola, 2);
    stepMotor(4 * stepsNeeded);
}

void cookCAPECODDER()
{
    poorLiquid(Drinks::Vodka, 1);
    stepMotor(5 * stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 2);
    stepMotor(stepsNeeded);
}

void cookSCREWDRIVER()
{
    poorLiquid(Drinks::Vodka, 1);
    stepMotor(3 * stepsNeeded);
    poorLiquid(Drinks::OrangeJuice, 2);
    stepMotor(3 * stepsNeeded);
}

void cookSEABREEZE()
{
    poorLiquid(Drinks::Vodka, 1);
    stepMotor(4 * stepsNeeded);
    poorLiquid(Drinks::PineappleJuice, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 1);
    stepMotor(stepsNeeded);
}

void cookMADRASS()
{
    poorLiquid(Drinks::Vodka, 1);
    stepMotor(3 * stepsNeeded);
    poorLiquid(Drinks::OrangeJuice, 1);
    stepMotor(2 * stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 1);
    stepMotor(stepsNeeded);
}

void cookTROPICALMIX()
{
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Rum, 1);
    stepMotor(3 * stepsNeeded);
    poorLiquid(Drinks::PineappleJuice, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 1);
    stepMotor(stepsNeeded);
}

void cookBERRYCITRUS()
{
    poorLiquid(Drinks::Vodka, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Rum, 1);
    stepMotor(2 * stepsNeeded);
    poorLiquid(Drinks::OrangeJuice, 1);
    stepMotor(2 * stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 1);
    stepMotor(stepsNeeded);
}

void cookDOUBLE_TROUBLE()
{
    poorLiquid(Drinks::Vodka, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::Rum, 1);
    stepMotor(4 * stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 1);
    stepMotor(stepsNeeded);
}

void cookCITRUSCOLA()
{
    poorLiquid(Drinks::Vodka, 1);
    stepMotor(2 * stepsNeeded);
    poorLiquid(Drinks::Cola, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::OrangeJuice, 1);
    stepMotor(3 * stepsNeeded);
}

void cookFRUITPUNCH()
{
    stepMotor(3 * stepsNeeded);
    poorLiquid(Drinks::OrangeJuice, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::PineappleJuice, 1);
    stepMotor(stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 1);
    stepMotor(stepsNeeded);
}

void cookVIRGINSUNRISE()
{
    stepMotor(3 * stepsNeeded);
    poorLiquid(Drinks::OrangeJuice, 2);
    stepMotor(2 * stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 2);
    stepMotor(stepsNeeded);
}

void cookCHERRYCOKE()
{
    stepMotor(2 * stepsNeeded);
    poorLiquid(Drinks::Cola, 2);
    stepMotor(3 * stepsNeeded);
    poorLiquid(Drinks::CherryJuice, 2);
    stepMotor(stepsNeeded);
}

void cookVODKA()
{
    poorLiquid(Drinks::Vodka, 2);
    stepMotor(6 * stepsNeeded);
}

void migRed()
{
    lightColor(R_PIN);
}

void migBlue()
{
    lightColor(B_PIN);
}

void migGreen()
{
    lightColor(G_PIN);
}

void processCommand(String command)
{
    Serial.println("Processing: " + command);

    if (command == "LONGISLAND")
    {
        cookLONGISLAND();
    }
    else if (command == "BLUELOGOON")
    {
        cookBLUELOGOON();
    }
    else if (command == "MOJITO")
    {
        cookMOJITO();
    }
    else if (command == "PORNSTAR")
    {
        cookPORNSTAR();
    }
    else if (command == "PINKYMONSTER")
    {
        cookPINKYMONSTER();
    }
    else if (command == "SEXONTHEBICH")
    {
        cookSEXONTHEBICH();
    }
    else if (command == "MARGARITA")
    {
        cookMARGARITA();
    }
    else if (command == "MANHATTAN")
    {
        cookMANHATTAN();
    }
    else if (command == "SUNRISE")
    {
        cookSUNRISE();
    }
    else if (command == "CUBALIBRE")
    {
        cookCUBALIBRE();
    }
    else if (command == "RUMCOKE")
    {
        cookRUMCOKE();
    }
    else if (command == "CAPECODDER")
    {
        cookCAPECODDER();
    }
    else if (command == "SCREWDRIVER")
    {
        cookSCREWDRIVER();
    }
    else if (command == "SEABREEZE")
    {
        cookSEABREEZE();
    }
    else if (command == "MADRASS")
    {
        cookMADRASS();
    }
    else if (command == "TROPICALMIX")
    {
        cookTROPICALMIX();
    }
    else if (command == "BERRYCITRUS")
    {
        cookBERRYCITRUS();
    }
    else if (command == "DOUBLE_TROUBLE")
    {
        cookDOUBLE_TROUBLE();
    }
    else if (command == "CITRUSCOLA")
    {
        cookCITRUSCOLA();
    }
    else if (command == "FRUITPUNCH")
    {
        cookFRUITPUNCH();
    }
    else if (command == "VIRGINSUNRISE")
    {
        cookVIRGINSUNRISE();
    }
    else if (command == "CHERRYCOKE")
    {
        cookCHERRYCOKE();
    }
    else if (command == "VODKA")
    {
        cookVODKA();
    }
    else
    {
        Serial.println("unknown coctail " + command); // DEBUG
    }

    migGreen(); // blinking with green once finished
    delayWithLightUpdate(1000);
}

void addCommandToQueue(String command)
{
    if (commandQueueCount < maxQueueSize)
    {
        commandQueue[commandQueueEnd] = command;
        commandQueueEnd = (commandQueueEnd + 1) % maxQueueSize;
        commandQueueCount++;
    }
}

void setup()
{
    initMotors();
    initSerialPorts();
    iniLowerServo();
    initLightning();
    initLightTimer2(); // app. timer2
}

void loop()
{
    updateLightning();

    if (Serial1.available())
    {
        String command = Serial1.readString();
        command.trim();
        addCommandToQueue(command);
    }

    if (commandQueueCount > 0)
    {
        String commandToProcess = commandQueue[commandQueueStart];
        commandQueueStart = (commandQueueStart + 1) % maxQueueSize;
        commandQueueCount--;
        processCommand(commandToProcess);
    }

    delayWithLightUpdate(DELAY_BETWEEN_PROCESSING);
}