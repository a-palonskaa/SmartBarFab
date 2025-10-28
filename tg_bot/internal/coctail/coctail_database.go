package coctail

import (
	"strings"
)

type Coctail int

const (
	UNDEFINED Coctail = iota
	LONGISLAND
	BLUELOGOON
	MOJITO
	PORNSTAR
	PINKYMONSTER
	SEXONTHEBICH
	MARGARITA
	MANHATTAN
	SUNRISE
	CUBALIBRE
	RUMCOKE
	CAPECODDER
	SCREWDRIVER
	SEABREEZE
	MADRASS
	TROPICALMIX
	BERRYCITRUS
	DOUBLE_TROUBLE
	CITRUSCOLA
	FRUITPUNCH
	VIRGINSUNRISE
	CHERRYCOKE
	VODKA
)

var Coctails = []Coctail{
	LONGISLAND,
	BLUELOGOON,
	MOJITO,
	PORNSTAR,
	PINKYMONSTER,
	SEXONTHEBICH,
	MARGARITA,
	MANHATTAN,
	SUNRISE,
	CUBALIBRE,
	RUMCOKE,
	CAPECODDER,
	SCREWDRIVER,
	SEABREEZE,
	MADRASS,
	TROPICALMIX,
	BERRYCITRUS,
	DOUBLE_TROUBLE,
	CITRUSCOLA,
	FRUITPUNCH,
	VIRGINSUNRISE,
	CHERRYCOKE,
	VODKA,
}

var TgCoctailNamesToIR = map[string]Coctail{
	"/longisland":    LONGISLAND,
	"/bluelagoon":    BLUELOGOON,
	"/mojito":        MOJITO,
	"/pornstar":      PORNSTAR,
	"/pinkymonster":  PINKYMONSTER,
	"/sexonthebich":  SEXONTHEBICH,
	"/margarita":     MARGARITA,
	"/manhattan":     MANHATTAN,
	"/sunrise":       SUNRISE,
	"/cubalibre":     CUBALIBRE,
	"/rumcoke":       RUMCOKE,
	"/capecodder":    CAPECODDER,
	"/screwdriver":   SCREWDRIVER,
	"/seabreeze":     SEABREEZE,
	"/madrass":       MADRASS,
	"/tropicalmix":   TROPICALMIX,
	"/berrycitrus":   BERRYCITRUS,
	"/doubletrouble": DOUBLE_TROUBLE,
	"/citruscola":    CITRUSCOLA,
	"/fruitpunch":    FRUITPUNCH,
	"/virginsunrise": VIRGINSUNRISE,
	"/cherrycoke":    CHERRYCOKE,
	"/vodka":         VODKA,
	"/i_dont_know":   PINKYMONSTER, // ХУЙНЯ
}

var CoctailToESPNames = map[Coctail]string{
	LONGISLAND:     "LONGISLAND",
	BLUELOGOON:     "BLUELOGOON",
	MOJITO:         "MOJITO",
	PORNSTAR:       "PORNSTAR",
	PINKYMONSTER:   "PINKYMONSTER",
	SEXONTHEBICH:   "SEXONTHEBICH",
	MARGARITA:      "MARGARITA",
	MANHATTAN:      "MANHATTAN",
	SUNRISE:        "SUNRISE",
	CUBALIBRE:      "CUBALIBRE",
	RUMCOKE:        "RUMCOKE",
	CAPECODDER:     "CAPECODDER",
	SCREWDRIVER:    "SCREWDRIVER",
	SEABREEZE:      "SEABREEZE",
	MADRASS:        "MADRASS",
	TROPICALMIX:    "TROPICALMIX",
	BERRYCITRUS:    "BERRYCITRUS",
	DOUBLE_TROUBLE: "DOUBLE_TROUBLE",
	CITRUSCOLA:     "CITRUSCOLA",
	FRUITPUNCH:     "FRUITPUNCH",
	VIRGINSUNRISE:  "VIRGINSUNRISE",
	CHERRYCOKE:     "CHERRYCOKE",
	VODKA:          "VODKA",
}

var CoctailToNames = map[Coctail]string{
	LONGISLAND:     "longisland",
	BLUELOGOON:     "bluelogoon",
	MOJITO:         "mojito",
	PORNSTAR:       "pornstar",
	PINKYMONSTER:   "pinkymonster",
	SEXONTHEBICH:   "sexonthebich",
	MARGARITA:      "margarita",
	MANHATTAN:      "manhattan",
	SUNRISE:        "sunrise",
	CUBALIBRE:      "cubalibre",
	RUMCOKE:        "rumcoke",
	CAPECODDER:     "capecodder",
	SCREWDRIVER:    "screwdriver",
	SEABREEZE:      "seabreeze",
	MADRASS:        "madrass",
	TROPICALMIX:    "tropicalmix",
	BERRYCITRUS:    "berrycitrus",
	DOUBLE_TROUBLE: "double_trouble",
	CITRUSCOLA:     "citruscola",
	FRUITPUNCH:     "fruitpunch",
	VIRGINSUNRISE:  "virginsunrise",
	CHERRYCOKE:     "cherrycoke",
	VODKA:          "vodka",
}

// SEX: Vodka, Rum, Cola, Orange juice, Pineapple juice, Cherry juice
var CoctailDescription = map[Coctail]string{
	LONGISLAND:     "Vodka, Rum, Cola, Orange juice – strong and refreshing mix in the Long Island style",
	BLUELOGOON:     "Vodka, Pineapple juice, Cherry juice – bright tropical cocktail without liqueurs",
	MOJITO:         "Rum, Pineapple juice, Cola – light version without mint, with a tropical twist",
	PORNSTAR:       "Vodka, Pineapple juice, Cherry juice – sweet and fruity cocktail",
	PINKYMONSTER:   "Vodka, Rum, Cherry juice, Orange juice – fruity and slightly tangy mix",
	SEXONTHEBICH:   "Vodka, Orange juice, Pineapple juice, Cherry juice – sweet and vibrant tropical cocktail",
	MARGARITA:      "Rum, Orange juice, Pineapple juice – smooth citrus-tropical blend",
	MANHATTAN:      "Rum, Cola, Cherry juice – sweet and strong variation in Manhattan style",
	SUNRISE:        "Rum, Orange juice, Cherry juice – beautiful layered cocktail similar to Tequila Sunrise",
	CUBALIBRE:      "Rum, Cola, Orange juice – simple and popular variation of Cuba Libre",
	RUMCOKE:        "Rum, Cola – classic combination",
	CAPECODDER:     "Vodka, Cherry juice – light and refreshing cocktail",
	SCREWDRIVER:    "Vodka, Orange juice – simple and refreshing cocktail",
	SEABREEZE:      "Vodka, Pineapple juice, Cherry juice – fruity and refreshing variation",
	MADRASS:        "Vodka, Orange juice, Cherry juice – sweet citrus and berry flavor",
	TROPICALMIX:    "Rum, Pineapple juice, Cherry juice – tropical mix with rich flavor",
	BERRYCITRUS:    "Vodka, Rum, Cherry juice, Orange juice – citrus and berry fusion",
	DOUBLE_TROUBLE: "Vodka, Rum, Cherry juice – strong and fruity drink",
	CITRUSCOLA:     "Vodka, Orange juice, Cola – unusual and refreshing combination",
	FRUITPUNCH:     "Orange juice, Pineapple juice, Cherry juice – tropical and refreshing fruit punch",
	VIRGINSUNRISE:  "Orange juice, Cherry juice – a non-alcoholic version of the classic Tequila Sunrise",
	CHERRYCOKE:     "Cherry juice, Cola - refreshing daily mix",
	VODKA:          "Vodka - simple vodka",
}

func BuildCoctailList() string {
	var sb strings.Builder
	for name := range TgCoctailNamesToIR {
		sb.WriteString(name)
		sb.WriteString("\n")
	}
	return sb.String()
}
