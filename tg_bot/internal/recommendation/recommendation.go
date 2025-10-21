package recommendation

import (
	"math/rand"
	"time"

	"github.com/rs/zerolog/log"

	"github.com/a-palonskaa/SmartBar/tg_bot/internal/coctail"
)

func intersectDrinks(a, b []coctail.Coctail) []coctail.Coctail {
	set := make(map[coctail.Coctail]bool)
	for _, x := range a {
		set[x] = true
	}

	var result []coctail.Coctail
	for _, y := range b {
		if set[y] {
			result = append(result, y)
		}
	}
	return result
}

func RecommendByFilters(filters ...[]coctail.Coctail) []coctail.Coctail {
	if len(filters) == 0 {
		return nil
	}

	result := filters[0]
	for _, f := range filters[1:] {
		result = intersectDrinks(result, f)
	}

	if len(result) == 0 {
		for _, filter := range filters {
			result = append(result, filter...)
		}
	}

	return result
}

func Recommend(options ...[]coctail.Coctail) coctail.Coctail {
	filters := [][]coctail.Coctail{
		coctail.GetRecommendationByHoliday(),
	}
	filters = append(filters, options...)

	recommendations := RecommendByFilters(filters...)
	rand.New(rand.NewSource(time.Now().UnixNano()))

	log.Info().Msgf("recommendations: %v", recommendations)
	if len(recommendations) == 0 {
		return coctail.Coctails[rand.Intn(len(coctail.Coctails))]
	}
	return recommendations[rand.Intn(len(recommendations))]

}
