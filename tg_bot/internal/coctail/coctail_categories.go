package coctail

import (
	"errors"
	"time"

	"github.com/rs/zerolog/log"
)

//---------------Alco-------------------

type AlcoCategory string

const (
	AlcoCategoryAlcoFree   AlcoCategory = "alco-free"
	AlcoCategoryLightAlco  AlcoCategory = "light"
	AlcoCategoryStrongAlco AlcoCategory = "hard"
)

func (c AlcoCategory) String() string {
	return string(c)
}

var alcoCategoryMap = map[string]AlcoCategory{
	AlcoCategoryAlcoFree.String():   AlcoCategoryAlcoFree,
	AlcoCategoryLightAlco.String():  AlcoCategoryLightAlco,
	AlcoCategoryStrongAlco.String(): AlcoCategoryStrongAlco,
}

func GetAlcoCategoryFromString(s string) (AlcoCategory, error) {
	cat, ok := alcoCategoryMap[s]
	if !ok {
		return "", errors.New("unknown category")
	}
	return cat, nil
}

var DrinksByAlcoCategory = map[AlcoCategory][]Coctail{
	AlcoCategoryAlcoFree: {
		FRUITPUNCH,
		VIRGINSUNRISE,
	},
	AlcoCategoryLightAlco: {
		SEXONTHEBICH,
		PORNSTAR,
		CUBALIBRE,
		MANHATTAN,
		SUNRISE,
		MOJITO,
		MARGARITA,
		CITRUSCOLA,
		TROPICALMIX,
		MADRASS,
		SEABREEZE,
		CAPECODDER,
		SCREWDRIVER,
		BLUELOGOON,
	},
	AlcoCategoryStrongAlco: {
		VODKA,
		LONGISLAND,
		PINKYMONSTER,
		BERRYCITRUS,
		DOUBLE_TROUBLE,
		RUMCOKE,
	},
}

//---------------Holiday-------------------

type HolidayCategory string

type HolidayCategoryDates struct {
	FromMonth time.Month
	FromDay   int
	ToMonth   time.Month
	ToDay     int
}

const (
	HolidayInternationalCoffeeDay HolidayCategory = "coffee_day"
	HolidayNewYear                HolidayCategory = "new_year"
	HolidayValentines             HolidayCategory = "valentines"
	HolidayWomenDay               HolidayCategory = "women_day"
	HolidayMaslenitsa             HolidayCategory = "maslenitsa"
	HolidayHugDay                 HolidayCategory = "hug_day"
	HolidayFathersDay             HolidayCategory = "fathers_day"
	HolidayChampagneDay           HolidayCategory = "champagne_day"
	HolidayOrangeJuiceDay         HolidayCategory = "orange_juice_day"
	HolidayWineDay                HolidayCategory = "wine_day"
	HolidayRumDay                 HolidayCategory = "rum_day"
	HolidayCocktailDay            HolidayCategory = "cocktail_day"
	HolidayOktoberfest            HolidayCategory = "oktoberfest"
	HolidayPomegranateDay         HolidayCategory = "pomegranate_day"
	HolidayHalloween              HolidayCategory = "halloween"
	HolidaySummer                 HolidayCategory = "summer"
	HolidayChristmas              HolidayCategory = "christmas"
)

var HolidayCategoryTiming = map[HolidayCategory]HolidayCategoryDates{
	HolidayFathersDay: {
		FromMonth: time.June, FromDay: 15,
		ToMonth: time.June, ToDay: 21,
	},
	HolidayChampagneDay: {
		FromMonth: time.October, FromDay: 25,
		ToMonth: time.October, ToDay: 27,
	},
	HolidayOrangeJuiceDay: {
		FromMonth: time.May, FromDay: 4,
		ToMonth: time.May, ToDay: 5,
	},
	HolidayWineDay: {
		FromMonth: time.May, FromDay: 25,
		ToMonth: time.May, ToDay: 26,
	},
	HolidayRumDay: {
		FromMonth: time.August, FromDay: 15,
		ToMonth: time.August, ToDay: 17,
	},
	HolidayCocktailDay: {
		FromMonth: time.May, FromDay: 13,
		ToMonth: time.May, ToDay: 14,
	},
	HolidayOktoberfest: {
		FromMonth: time.September, FromDay: 16,
		ToMonth: time.October, ToDay: 3,
	},
	HolidayPomegranateDay: {
		FromMonth: time.November, FromDay: 26,
		ToMonth: time.November, ToDay: 27,
	},
	HolidayHugDay: {
		FromMonth: time.January, FromDay: 21,
		ToMonth: time.January, ToDay: 22,
	},
	HolidayInternationalCoffeeDay: {
		FromMonth: time.October, FromDay: 1,
		ToMonth: time.October, ToDay: 2,
	},
	HolidayNewYear: {
		FromMonth: time.December, FromDay: 25,
		ToMonth: time.January, ToDay: 10,
	},
	HolidayValentines: {
		FromMonth: time.February, FromDay: 13,
		ToMonth: time.February, ToDay: 15,
	},
	HolidayMaslenitsa: {
		FromMonth: time.February, FromDay: 20,
		ToMonth: time.March, ToDay: 3,
	},
	HolidayWomenDay: {
		FromMonth: time.March, FromDay: 7,
		ToMonth: time.March, ToDay: 9,
	},
	HolidaySummer: {
		FromMonth: time.June, FromDay: 1,
		ToMonth: time.August, ToDay: 31,
	},
	HolidayHalloween: {
		FromMonth: time.October, FromDay: 20,
		ToMonth: time.November, ToDay: 2,
	},
	HolidayChristmas: {
		FromMonth: time.December, FromDay: 20,
		ToMonth: time.December, ToDay: 26,
	},
}

var DrinksByHolidayCategory = map[HolidayCategory][]Coctail{
	HolidayNewYear: {
		DOUBLE_TROUBLE,
		BLUELOGOON,
		LONGISLAND,
		SCREWDRIVER,
	},
	HolidayValentines: {
		PINKYMONSTER,
		TROPICALMIX,
		PORNSTAR,
	},
	HolidayWomenDay: {
		PINKYMONSTER,
		CAPECODDER,
	},
	HolidayMaslenitsa: {
		LONGISLAND,
		TROPICALMIX,
		SCREWDRIVER,
	},
	HolidayHugDay: {
		PINKYMONSTER,
		VIRGINSUNRISE,
	},
	HolidayFathersDay: {
		DOUBLE_TROUBLE,
		LONGISLAND,
		MANHATTAN,
	},
	HolidayChampagneDay: {
		BLUELOGOON,
	},
	HolidayOrangeJuiceDay: {
		TROPICALMIX,
		VIRGINSUNRISE,
	},
	HolidayWineDay: {
		DOUBLE_TROUBLE,
		MARGARITA,
	},
	HolidayRumDay: {
		LONGISLAND,
		RUMCOKE,
		CAPECODDER,
	},
	HolidayCocktailDay: {
		BLUELOGOON,
		TROPICALMIX,
		SCREWDRIVER,
	},
	HolidayOktoberfest: {
		DOUBLE_TROUBLE,
		MANHATTAN,
		RUMCOKE,
	},
	HolidayPomegranateDay: {
		PINKYMONSTER,
		TROPICALMIX,
	},
	HolidayHalloween: {
		DOUBLE_TROUBLE,
		BLUELOGOON,
		RUMCOKE,
	},
	HolidaySummer: {
		TROPICALMIX,
		LONGISLAND,
		SCREWDRIVER,
	},
	HolidayChristmas: {
		BLUELOGOON,
		PINKYMONSTER,
		TROPICALMIX,
	},
}

func GetCurrentHolidays(date time.Time) []HolidayCategory {
	var result []HolidayCategory
	for cat, rng := range HolidayCategoryTiming {
		start := time.Date(date.Year(), rng.FromMonth, rng.FromDay, 0, 0, 0, 0, time.UTC)
		end := time.Date(date.Year(), rng.ToMonth, rng.ToDay, 23, 59, 59, 0, time.UTC)

		if end.Before(start) {
			end = end.AddDate(1, 0, 0)
		}

		if (date.After(start) && date.Before(end)) || date.Equal(start) || date.Equal(end) {
			result = append(result, cat)
		}
	}
	return result
}

func GetRecommendationByHoliday() []Coctail {
	holidays := GetCurrentHolidays(time.Now())
	log.Info().Msgf("holiday: %v", holidays) // DEBUG: debug logs to test recommendation
	holidayDrinks := make([]Coctail, 0)
	for _, holiday := range holidays {
		holidayDrinks = append(holidayDrinks, DrinksByHolidayCategory[holiday]...)
	}
	return holidayDrinks
}

var alcoMap = map[string]AlcoCategory{
	AlcoCategoryAlcoFree.String():   AlcoCategoryAlcoFree,
	AlcoCategoryLightAlco.String():  AlcoCategoryLightAlco,
	AlcoCategoryStrongAlco.String(): AlcoCategoryStrongAlco,
}

//---------------Ocasion-------------------

type OccasionCategory string

const (
	OccasionAlkocoding     OccasionCategory = "alkocoding"
	OccasionRelax          OccasionCategory = "relax-deadline"
	OccasionDormParty      OccasionCategory = "1st_dormitory_party"
	OccasionMovieNight     OccasionCategory = "movie_night"
	OccasionNightResearch  OccasionCategory = "night_research"
	OccasionAllnighter     OccasionCategory = "allnighter_exam"
	OccasionFiztechParty   OccasionCategory = "fiztech_party"
	OccasionAfterDiff      OccasionCategory = "after_differential_equations"
	OccasionBrainstorm     OccasionCategory = "brainstorm"
	OccasionWannaGetWasted OccasionCategory = "wanna_get_wasted"
)

func (c OccasionCategory) String() string {
	return string(c)
}

var occasionCategoryMap = map[string]OccasionCategory{
	OccasionAlkocoding.String():     OccasionAlkocoding,
	OccasionRelax.String():          OccasionRelax,
	OccasionDormParty.String():      OccasionDormParty,
	OccasionMovieNight.String():     OccasionMovieNight,
	OccasionNightResearch.String():  OccasionNightResearch,
	OccasionAllnighter.String():     OccasionAllnighter,
	OccasionFiztechParty.String():   OccasionFiztechParty,
	OccasionAfterDiff.String():      OccasionAfterDiff,
	OccasionBrainstorm.String():     OccasionBrainstorm,
	OccasionWannaGetWasted.String(): OccasionWannaGetWasted,
}

func GetOccasionCategoryFromString(s string) (OccasionCategory, error) {
	cat, ok := occasionCategoryMap[s]
	if !ok {
		return "", errors.New("unknown category")
	}
	return cat, nil
}

var DrinksByOccasionCategory = map[OccasionCategory][]Coctail{
	OccasionAlkocoding: {
		CITRUSCOLA,
		DOUBLE_TROUBLE,
		MOJITO,
	},
	OccasionRelax: {
		BLUELOGOON,
		MADRASS,
		SEABREEZE,
	},
	OccasionDormParty: {
		SEXONTHEBICH,
		FRUITPUNCH,
		PORNSTAR,
	},
	OccasionMovieNight: {
		MANHATTAN,
		SUNRISE,
		VIRGINSUNRISE,
	},
	OccasionNightResearch: {
		LONGISLAND,
		RUMCOKE,
		VODKA,
	},
	OccasionAllnighter: {
		DOUBLE_TROUBLE,
		CITRUSCOLA,
		VODKA,
	},
	OccasionFiztechParty: {
		TROPICALMIX,
		PINKYMONSTER,
		BERRYCITRUS,
	},
	OccasionAfterDiff: {
		VODKA,
		LONGISLAND,
		RUMCOKE,
	},
	OccasionBrainstorm: {
		MOJITO,
		MARGARITA,
		CAPECODDER,
	},
	OccasionWannaGetWasted: {
		LONGISLAND,
		DOUBLE_TROUBLE,
		VODKA,
		PINKYMONSTER,
	},
}
