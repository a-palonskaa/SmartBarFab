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
	"/i_dont_know":   PINKYMONSTER,
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
}

// DEBUG - чатгпт уебан не умеет до 6 считать - перепроверить
var CoctailDescription = map[Coctail]string{
	LONGISLAND:     "Vodka, Rum, Triple sec, Cola or juice – strong and refreshing cocktail",
	BLUELOGOON:     "Vodka, Blue Curacao liqueur, Lemonade – bright blue cocktail",
	MOJITO:         "Rum, Lime, Sugar, Mint, Soda – classic Cuban cocktail",
	PORNSTAR:       "Vodka, Passion fruit, Champagne – sweet tropical cocktail",
	PINKYMONSTER:   "Vodka, Rum, Cranberry juice, Orange juice – fruity cocktail",
	SEXONTHEBICH:   "Vodka, Peach liqueur, Juice – sweet and strong",
	MARGARITA:      "Rum, Triple sec, Lime – sour and sweet classic",
	MANHATTAN:      "Whiskey, Sweet vermouth, Bitters – strong cocktail",
	SUNRISE:        "Rum, Orange juice, Grenadine – beautiful gradient",
	CUBALIBRE:      "Rum, Cola, Lime – simple and popular",
	RUMCOKE:        "Rum, Cola – classic combination",
	CAPECODDER:     "Vodka, Cranberry juice – light and tart",
	SCREWDRIVER:    "Vodka, Orange juice – simple and refreshing",
	SEABREEZE:      "Vodka, Cranberry juice, Grapefruit juice – tart and fruity",
	MADRASS:        "Vodka, Cranberry juice, Orange juice – similar to Sea Breeze",
	TROPICALMIX:    "Rum, Orange juice, Cranberry juice – tropical mix",
	BERRYCITRUS:    "Vodka, Tequila, Cranberry juice, Orange juice – berry-citrus cocktail",
	DOUBLE_TROUBLE: "Vodka, Rum, Cranberry juice – strong fruity drink",
	CITRUSCOLA:     "Vodka, Orange juice, Cola – light and refreshing",
}

func BuildCoctailList() string {
	var sb strings.Builder
	for name := range TgCoctailNamesToIR {
		sb.WriteString(name)
		sb.WriteString("\n")
	}
	return sb.String()
}
