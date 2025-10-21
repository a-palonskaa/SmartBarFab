package coctail

import (
	"errors"
	"time"

	"github.com/rs/zerolog/log"
)

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

func GetCategoryFromString(s string) (AlcoCategory, error) {
	cat, ok := alcoCategoryMap[s]
	if !ok {
		return "", errors.New("unknown category")
	}
	return cat, nil
}

var DrinksByAlcoCategory = map[AlcoCategory][]Coctail{
	AlcoCategoryAlcoFree:   {PINKYMONSTER, TROPICALMIX},
	AlcoCategoryLightAlco:  {BLUELOGOON, LONGISLAND},
	AlcoCategoryStrongAlco: {DOUBLE_TROUBLE},
}

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
	HolidayNewYear:        {DOUBLE_TROUBLE, BLUELOGOON},
	HolidayValentines:     {PINKYMONSTER, TROPICALMIX},
	HolidayWomenDay:       {PINKYMONSTER},
	HolidayMaslenitsa:     {LONGISLAND, TROPICALMIX},
	HolidayHugDay:         {PINKYMONSTER},
	HolidayFathersDay:     {DOUBLE_TROUBLE, LONGISLAND},
	HolidayChampagneDay:   {BLUELOGOON},
	HolidayOrangeJuiceDay: {TROPICALMIX},
	HolidayWineDay:        {DOUBLE_TROUBLE},
	HolidayRumDay:         {LONGISLAND},
	HolidayCocktailDay:    {BLUELOGOON, TROPICALMIX},
	HolidayOktoberfest:    {DOUBLE_TROUBLE},
	HolidayPomegranateDay: {PINKYMONSTER},
	HolidayHalloween:      {DOUBLE_TROUBLE},
	HolidaySummer:         {TROPICALMIX, LONGISLAND},
	HolidayChristmas:      {BLUELOGOON},
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
	log.Info().Msgf("holiday: %v", holidays) //DEBUG

	holidayDrinks := make([]Coctail, 0)
	for _, holiday := range holidays {
		holidayDrinks = append(holidayDrinks, DrinksByHolidayCategory[holiday]...)
	}
	return holidayDrinks
}
