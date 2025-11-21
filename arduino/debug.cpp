#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

enum Drinks {
    Vodka          = 0,
    Rum            = 1,
    Cola           = 2,
    OrangeJuice    = 3,
    PineappleJuice = 4,
    CherryJuice    = 5,
};

vector<string> drinkNames = {
    "Vodka", "Rum", "Cola", "OrangeJuice", "PineappleJuice", "CherryJuice"
};

int motorCounts[] = {0,0,0,0,0,0};
int timePortion[] = {15060,14820,14750,14810,15270,15510,16070,16290,17270,17730,18730,19780,20860,25680};

const float stepAngle = 1.8;
const int stepsPerRev = int(360.0 / stepAngle);
const float degreesToMove = 60.0;
const int microstep = 1;

long stepsNeeded = (long)((stepsPerRev * (degreesToMove / 360.0)) * microstep);


// -------------------- DEBUG FUNCTIONS --------------------

void poorLiquid(Drinks d, int portion) {
    cout << "[POUR] " << drinkNames[d]
         << "  portion = " << portion << "  time = ";

    int start = motorCounts[d];
    int end = motorCounts[d] + portion;

    int total = 0;
    for (int i = start; i < end; i++) {
        cout << timePortion[i] << " ";
        total += timePortion[i];
    }

    cout << "| TOTAL = " << total << " ms" << endl;

    motorCounts[d] += portion;
}

void stepMotor(long steps) {
    cout << "[STEP] steps = " << steps << endl;
}

// -------------------- COCKTAILS --------------------

void cookLONGISLAND(){ poorLiquid(Vodka,1); stepMotor(stepsNeeded); poorLiquid(Rum,1); stepMotor(stepsNeeded); poorLiquid(Cola,1); stepMotor(stepsNeeded); poorLiquid(OrangeJuice,1); stepMotor(3*stepsNeeded); }
void cookBLUELOGOON(){ poorLiquid(Vodka,1); stepMotor(4*stepsNeeded); poorLiquid(PineappleJuice,1); stepMotor(stepsNeeded); poorLiquid(CherryJuice,1); stepMotor(stepsNeeded); }
void cookMOJITO(){ stepMotor(stepsNeeded); poorLiquid(Rum,1); stepMotor(stepsNeeded); poorLiquid(Cola,1); stepMotor(2*stepsNeeded); poorLiquid(PineappleJuice,2); stepMotor(2*stepsNeeded); }
void cookPORNSTAR(){ stepMotor(stepsNeeded); poorLiquid(Rum,2); stepMotor(3*stepsNeeded); poorLiquid(PineappleJuice,1); stepMotor(stepsNeeded); poorLiquid(CherryJuice,1); stepMotor(stepsNeeded); }
void cookPINKYMONSTER(){ poorLiquid(Vodka,1); stepMotor(stepsNeeded); poorLiquid(Rum,1); stepMotor(2*stepsNeeded); poorLiquid(OrangeJuice,1); stepMotor(stepsNeeded); poorLiquid(PineappleJuice,1); stepMotor(stepsNeeded); }
void cookSEXONTHEBICH(){ poorLiquid(Vodka,1); stepMotor(stepsNeeded); poorLiquid(OrangeJuice,1); stepMotor(3*stepsNeeded); poorLiquid(PineappleJuice,1); stepMotor(stepsNeeded); poorLiquid(CherryJuice,1); stepMotor(stepsNeeded); }
void cookMARGARITA(){ poorLiquid(Vodka,1); stepMotor(stepsNeeded); poorLiquid(OrangeJuice,1); stepMotor(3*stepsNeeded); poorLiquid(PineappleJuice,1); stepMotor(2*stepsNeeded); }
void cookMANHATTAN(){ stepMotor(stepsNeeded); poorLiquid(Rum,1); stepMotor(stepsNeeded); poorLiquid(Cola,1); stepMotor(3*stepsNeeded); poorLiquid(CherryJuice,1); stepMotor(stepsNeeded); }
void cookSUNRISE(){ stepMotor(stepsNeeded); poorLiquid(Rum,1); stepMotor(2*stepsNeeded); poorLiquid(OrangeJuice,1); stepMotor(2*stepsNeeded); poorLiquid(CherryJuice,1); stepMotor(stepsNeeded); }
void cookCUBALIBRE(){ stepMotor(stepsNeeded); poorLiquid(Rum,1); stepMotor(stepsNeeded); poorLiquid(Cola,1); stepMotor(stepsNeeded); poorLiquid(OrangeJuice,1); stepMotor(3*stepsNeeded); }
void cookRUMCOKE(){ stepMotor(stepsNeeded); poorLiquid(Rum,1); stepMotor(stepsNeeded); poorLiquid(Cola,2); stepMotor(4*stepsNeeded); }
void cookCAPECODDER(){ poorLiquid(Vodka,1); stepMotor(5*stepsNeeded); poorLiquid(CherryJuice,2); stepMotor(stepsNeeded); }
void cookSCREWDRIVER(){ poorLiquid(Vodka,1); stepMotor(3*stepsNeeded); poorLiquid(OrangeJuice,2); stepMotor(3*stepsNeeded); }
void cookSEABREEZE(){ poorLiquid(Vodka,1); stepMotor(4*stepsNeeded); poorLiquid(PineappleJuice,1); stepMotor(stepsNeeded); poorLiquid(CherryJuice,1); stepMotor(stepsNeeded); }
void cookMADRASS(){ poorLiquid(Vodka,1); stepMotor(3*stepsNeeded); poorLiquid(OrangeJuice,1); stepMotor(2*stepsNeeded); poorLiquid(CherryJuice,1); stepMotor(stepsNeeded); }
void cookTROPICALMIX(){ stepMotor(stepsNeeded); poorLiquid(Rum,1); stepMotor(3*stepsNeeded); poorLiquid(PineappleJuice,1); stepMotor(stepsNeeded); poorLiquid(CherryJuice,1); stepMotor(stepsNeeded); }
void cookBERRYCITRUS(){ poorLiquid(Vodka,1); stepMotor(stepsNeeded); poorLiquid(Rum,1); stepMotor(2*stepsNeeded); poorLiquid(OrangeJuice,1); stepMotor(2*stepsNeeded); poorLiquid(CherryJuice,1); stepMotor(stepsNeeded); }
void cookDOUBLE_TROUBLE(){ poorLiquid(Vodka,1); stepMotor(stepsNeeded); poorLiquid(Rum,1); stepMotor(4*stepsNeeded); poorLiquid(CherryJuice,1); stepMotor(stepsNeeded); }
void cookCITRUSCOLA(){ poorLiquid(Vodka,1); stepMotor(2*stepsNeeded); poorLiquid(Cola,1); stepMotor(stepsNeeded); poorLiquid(OrangeJuice,1); stepMotor(3*stepsNeeded); }
void cookFRUITPUNCH(){ stepMotor(3*stepsNeeded); poorLiquid(OrangeJuice,1); stepMotor(stepsNeeded); poorLiquid(PineappleJuice,1); stepMotor(stepsNeeded); poorLiquid(CherryJuice,1); stepMotor(stepsNeeded); }
void cookVIRGINSUNRISE(){ stepMotor(3*stepsNeeded); poorLiquid(OrangeJuice,2); stepMotor(2*stepsNeeded); poorLiquid(CherryJuice,2); stepMotor(stepsNeeded); }
void cookCHERRYCOKE(){ stepMotor(2*stepsNeeded); poorLiquid(Cola,2); stepMotor(3*stepsNeeded); poorLiquid(CherryJuice,2); stepMotor(stepsNeeded); }
void cookVODKA(){ poorLiquid(Vodka,2); stepMotor(6*stepsNeeded); }


// -------------------- COMMAND PROCESSOR --------------------

void processCommand(const string& cmd) {
    cout << "\n>>> COMMAND: " << cmd << endl;

    if      (cmd=="LONGISLAND") cookLONGISLAND();
    else if (cmd=="BLUELOGOON") cookBLUELOGOON();
    else if (cmd=="MOJITO") cookMOJITO();
    else if (cmd=="PORNSTAR") cookPORNSTAR();
    else if (cmd=="PINKYMONSTER") cookPINKYMONSTER();
    else if (cmd=="SEXONTHEBICH") cookSEXONTHEBICH();
    else if (cmd=="MARGARITA") cookMARGARITA();
    else if (cmd=="MANHATTAN") cookMANHATTAN();
    else if (cmd=="SUNRISE") cookSUNRISE();
    else if (cmd=="CUBALIBRE") cookCUBALIBRE();
    else if (cmd=="RUMCOKE") cookRUMCOKE();
    else if (cmd=="CAPECODDER") cookCAPECODDER();
    else if (cmd=="SCREWDRIVER") cookSCREWDRIVER();
    else if (cmd=="SEABREEZE") cookSEABREEZE();
    else if (cmd=="MADRASS") cookMADRASS();
    else if (cmd=="TROPICALMIX") cookTROPICALMIX();
    else if (cmd=="BERRYCITRUS") cookBERRYCITRUS();
    else if (cmd=="DOUBLE_TROUBLE") cookDOUBLE_TROUBLE();
    else if (cmd=="CITRUSCOLA") cookCITRUSCOLA();
    else if (cmd=="FRUITPUNCH") cookFRUITPUNCH();
    else if (cmd=="VIRGINSUNRISE") cookVIRGINSUNRISE();
    else if (cmd=="CHERRYCOKE") cookCHERRYCOKE();
    else if (cmd=="VODKA") cookVODKA();
    else cout << "UNKNOWN COCKTAIL!" << endl;
}


// -------------------- MAIN --------------------

int main() {
    cout << "SmartBar Debug Simulator\n";
    cout << "Enter cocktail name:\n";

    while (true) {
        string cmd;
        cout << "> ";
        getline(cin, cmd);

        transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

        processCommand(cmd);
    }

    return 0;
}
