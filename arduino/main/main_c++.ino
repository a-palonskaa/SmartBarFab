#include <Servo.h>
#include <map>

constexpr int DELAY_BETWEEN_PROCESSING = 1000;

enum Drinks {
    Vodka          = 0,
    Rum            = 1,
    Cola           = 2,
    OrangeJuice    = 3,
    PineappleJuice = 4,
    CherryJuice    = 5,
};

std::queue<String> commandQueue;

std::map<Drinks, Servo*> drinkMotorMap;
Servo m1, m2, m3, m4, m5, m6;

std::map<String, void(*)()> drinkMap;

std::map<Drinks, FlowSensor> flowSensorMap;
FlowSensor sensors[6] = {2, 3, 4, 5, 6, 7};

class FlowSensor {
private:
  uint8_t pin;
  volatile uint16_t counter;

public:
  FlowSensor(uint8_t sensorPin) : pin(sensorPin), counter(0) {}

  void begin() {
    uint8_t interruptPin = digitalPinToInterrupt(pin);
    attachInterrupt(interruptPin, [this]() { this->counter++; }, RISING);
  }

  void measure(float targetVolume) {
    counter = 0;
    uint32_t varTime = 0;
    float varQ = 0, varV = 0;

    while (varV < targetVolume) {
      varQ = counter / ((float)counter * 5.9f + 4570.0f);
      counter = 0;
      varTime = millis();
      varV += varQ;
      delay(10);
    }
  }

  uint16_t getCount() const { return counter; }
};

void initFlowSensors() {
    flowSensorMap[Vodka]          = FlowSensor{2};
    flowSensorMap[Rum]            = FlowSensor{3};
    flowSensorMap[Cola]           = FlowSensor{4};
    flowSensorMap[OrangeJuice]    = FlowSensor{5};
    flowSensorMap[PineappleJuice] = FlowSensor{6};
    flowSensorMap[CherryJuice]    = FlowSensor{7};

    flowSensorMap[Vodka].begin();
    flowSensorMap[Rum].begin();
    flowSensorMap[Cola].begin();
    flowSensorMap[OrangeJuice].begin();
    flowSensorMap[PineappleJuice].begin();
    flowSensorMap[CherryJuice].begin();
}

void initMotors() {
    m1.attach(9);
    m2.attach(10);
    m3.attach(11);
    m4.attach(12);
    m5.attach(13);
    m6.attach(14);

    drinkMotorMap[Vodka]          = &m1;
    drinkMotorMap[Rum]            = &m2;
    drinkMotorMap[Cola]           = &m3;
    drinkMotorMap[OrangeJuice]    = &m4;
    drinkMotorMap[PineappleJuice] = &m5;
    drinkMotorMap[CherryJuice]    = &m6;
}

void initSerialPorts() {
    Serial.begin(9600);
    Serial1.begin(115200);  //RX1 = Pin19, TX1 = Pin18
}

void initDrinkMap() {
    drinkMap["LONGISLAND"] = cookLONGISLAND;
    drinkMap["BLUELOGOON"] = cookBLUELOGOON;
    drinkMap["MOJITO"] = cookMOJITO;
    drinkMap["PORNSTAR"] = cookPORNSTAR;
    drinkMap["PINKYMONSTER"] = cookPINKYMONSTER;
    drinkMap["SEXONTHEBICH"] = cookSEXONTHEBICH;
    drinkMap["MARGARITA"] = cookMARGARITA;
    drinkMap["MANHATTAN"] = cookMANHATTAN;
    drinkMap["SUNRISE"] = cookSUNRISE;
    drinkMap["CUBALIBRE"] = cookCUBALIBRE;
    drinkMap["RUMCOKE"] = cookRUMCOKE;
    drinkMap["CAPECODDER"] = cookCAPECODDER;
    drinkMap["SCREWDRIVER"] = cookSCREWDRIVER;
    drinkMap["SEABREEZE"] = cookSEABREEZE;
    drinkMap["MADRASS"] = cookMADRASS;
    drinkMap["TROPICALMIX"] = cookTROPICALMIX;
    drinkMap["BERRYCITRUS"] = cookBERRYCITRUS;
    drinkMap["DOUBLE_TROUBLE"] = cookDOUBLE_TROUBLE;
    drinkMap["CITRUSCOLA"] = cookCITRUSCOLA;
    drinkMap["FRUITPUNCH"] = cookFRUITPUNCH;
    drinkMap["VIRGINSUNRISE"] = cookVIRGINSUNRISE;
    drinkMap["CHERRYCOKE"] = cookCHERRYCOKE;
    drinkMap["VODKA"] = cookVODKA;
}

func poorLiquid(liquidType Drinks, liquidVolume int) {
    drinkMotorMap[liquidType]->write(0);
    delay(200);

    flowSensorMap[liquidType].measure(liquidVolume);

    drinkMotorMap[liquidType]->write(180);
    delay(200);
}

//----------cook-coctails-func----------

func cookLONGISLAND() {
    poorLiquid(Drinks::Vodka, 0.05)
}

func cookBLUELOGOON() {

}

func cookMOJITO() {

}

func cookPORNSTAR() {

}

func cookPINKYMONSTER() {

}

func cookSEXONTHEBICH() {

}

func cookMARGARITA() {

}

func cookMANHATTAN() {

}

func cookSUNRISE() {

}

func cookCUBALIBRE() {

}

func cookRUMCOKE() {

}

func cookCAPECODDER() {

}

func cookSCREWDRIVER() {

}

func cookSEABREEZE() {

}

func cookMADRASS() {

}

func cookTROPICALMIX() {

}

func cookBERRYCITRUS() {

}

func cookDOUBLE_TROUBLE() {

}

func cookCITRUSCOLA() {

}

func cookFRUITPUNCH() {

}

func cookVIRGINSUNRISE() {

}

func cookCHERRYCOKE() {

}

func cookVODKA() {

}

void processCommand(String command) {
    Serial.println("procesings: " + command); // DEBUG

    auto drink = drinkMap.find(command);
    if (drink != drinkMap.end()) {
        drink->second();
    } else {
        Serial.println("unknown cocktail: " + command); // DEBUG
    }
}

void setup() {
    initMotors();
    initSerialPorts();
    initDrinkMap();
    initFlowSensors();
}

void loop() {
    if (Serial1.available()) {
        String command = Serial1.readString();
        command.trim();
        commandQueue.push(command);
    }

    if (!commandQueue.empty()) {
        String commandToProcess = commandQueue.front();
        commandQueue.pop();
        processCommand(commandToProcess);
    }

    delay(DELAY_BETWEEN_PROCESSING);
}
