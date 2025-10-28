package session

import (
	"os"
	"sync"
	"time"

	"github.com/a-palonskaa/SmartBar/tg_bot/internal/coctail"
	"github.com/a-palonskaa/SmartBar/tg_bot/internal/config"
)

type SessionState int

const (
	WAIT_PASSWORD SessionState = iota
	AUTHORIZED
	WAIT_DRINK
	RECOMMENDATION_STEP1
	RECOMMENDATION_STEP2
	DONE
)

type UserSession struct {
	State            SessionState
	ScheduledTime    time.Time
	ScheduledDrink   coctail.Coctail
	RecommenedDrinks [][]coctail.Coctail
}

type UserSessions struct {
	Sessions map[int64]*UserSession
	Password string
	Mu       sync.Mutex
}

func NewUserSessions() *UserSessions {
	return &UserSessions{
		Sessions: make(map[int64]*UserSession, 0),
		Password: os.Getenv(config.BotPasswordEnv),
	}
}

func (us *UserSessions) GetSession(chatID int64) *UserSession {
	us.Mu.Lock()
	defer us.Mu.Unlock()

	if session, ok := us.Sessions[chatID]; ok {
		return session
	}

	s := &UserSession{State: WAIT_PASSWORD}
	us.Sessions[chatID] = s
	return s
}
