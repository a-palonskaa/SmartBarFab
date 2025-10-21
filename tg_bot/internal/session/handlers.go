package session

import (
	"fmt"
	"strconv"
	"time"

	tgbotapi "github.com/go-telegram-bot-api/telegram-bot-api/v5"

	"github.com/a-palonskaa/SmartBar/tg_bot/internal/coctail"
	"github.com/a-palonskaa/SmartBar/tg_bot/internal/msg"
	"github.com/a-palonskaa/SmartBar/tg_bot/internal/recommendation"
)

func (us *UserSessions) HandleMessage(bot *tgbotapi.BotAPI, msg *tgbotapi.Message) {
	chatID := msg.Chat.ID
	text := msg.Text
	session := us.GetSession(chatID)

	switch session.State {
	case WAIT_PASSWORD:
		us.HandleWaitPassword(bot, chatID, text)
	case AUTHORIZED:
		us.HandleAuthorized(bot, chatID, text)
	case WAIT_DRINK:
		us.HandleWaitDrink(bot, chatID, text)
	case RECOMMENDATION_STEP1:
		us.HandleRecommendationStep1(bot, chatID, text)
	}
}

func (us *UserSessions) HandleWaitPassword(bot *tgbotapi.BotAPI, chatID int64, text string) {
	if text == "/start" {
		bot.Send(tgbotapi.NewMessage(chatID, msg.MsgStart.String()))
		return
	}

	if len(text) > 10 && text[:10] == "/password " {
		password := text[10:]
		if password == us.Password {
			us.GetSession(chatID).State = AUTHORIZED
			bot.Send(tgbotapi.NewMessage(chatID, msg.MsgPasswordOK.String()))
			bot.Send(tgbotapi.NewMessage(chatID, msg.MsgAuthorizedCmdOptions.String()))
		} else {
			bot.Send(tgbotapi.NewMessage(chatID, msg.MsgPasswordFail.String()))
		}
		return
	}

	bot.Send(tgbotapi.NewMessage(chatID, msg.MsgWrongCmdAfterStart.String()))
}

func (us *UserSessions) HandleAuthorized(bot *tgbotapi.BotAPI, chatID int64, text string) {
	if text == "/make_drink" {
		us.GetSession(chatID).State = WAIT_DRINK
		bot.Send(tgbotapi.NewMessage(chatID, msg.MsgMakeDrink.String()))
		return
	}

	if text == "/recommend_drink" {
		us.RecommendDrink(bot, chatID)
		return
	}

	if len(text) > 13 && text[:13] == "/delay_drink " {
		delayMin, _ := strconv.Atoi(text[13:])
		us.scheduleDrink(chatID, delayMin)
		us.GetSession(chatID).State = WAIT_DRINK
		bot.Send(tgbotapi.NewMessage(chatID, msg.MsgMakeDrink.String()))
		return
	}

	bot.Send(tgbotapi.NewMessage(chatID, msg.MsgWrongCommandAuthorized.String()))
	bot.Send(tgbotapi.NewMessage(chatID, msg.MsgAuthorizedCmdOptions.String()))
}

func (us *UserSessions) HandleWaitDrink(bot *tgbotapi.BotAPI, chatID int64, text string) {
	drink, ok := coctail.TgCoctailNamesToIR[text]
	if !ok {
		bot.Send(tgbotapi.NewMessage(chatID, msg.MsgMakeDrink.String()))
		return
	}

	session := us.GetSession(chatID)
	session.ScheduledDrink = drink
	session.State = DONE //ХУЙНЯ -
	bot.Send(tgbotapi.NewMessage(chatID, fmt.Sprintf("%s '%s'", msg.MsgWaitDrink, coctail.CoctailToNames[drink])))
}

func (us *UserSessions) scheduleDrink(chatID int64, delayMin int) {
	session := us.GetSession(chatID)
	if delayMin > 0 {
		session.ScheduledTime = time.Now().Add(time.Duration(delayMin) * time.Minute)
	} else {
		session.ScheduledTime = time.Now()
	}
}

func (us *UserSessions) RecommendDrink(bot *tgbotapi.BotAPI, chatID int64) {
	buttons := tgbotapi.NewReplyKeyboard(
		tgbotapi.NewKeyboardButtonRow(
			tgbotapi.NewKeyboardButton(coctail.AlcoCategoryAlcoFree.String()),
			tgbotapi.NewKeyboardButton(coctail.AlcoCategoryLightAlco.String()),
			tgbotapi.NewKeyboardButton(coctail.AlcoCategoryStrongAlco.String()),
		),
	)

	msg := tgbotapi.NewMessage(chatID, "на зоже, легкий или покрепче?")
	msg.ReplyMarkup = buttons
	bot.Send(msg)

	session := us.GetSession(chatID)
	session.State = RECOMMENDATION_STEP1
}

func (us *UserSessions) HandleRecommendationStep1(bot *tgbotapi.BotAPI, chatID int64, text string) {
	session := us.GetSession(chatID)

	category, err := coctail.GetCategoryFromString(text)
	if err != nil {
		bot.Send(tgbotapi.NewMessage(chatID, msg.MsgWrongInput.String()))
		return
	}

	drinks, ok := coctail.DrinksByAlcoCategory[category]
	if !ok || len(drinks) == 0 {
		bot.Send(tgbotapi.NewMessage(chatID, msg.MsgWrongInput.String()))
		return
	}

	session.ScheduledDrink = recommendation.Recommend(drinks)
	session.State = AUTHORIZED

	ms := tgbotapi.NewMessage(chatID, fmt.Sprintf("%s '%s'", msg.MsgWaitDrink, coctail.CoctailToNames[session.ScheduledDrink]))
	removeKeyboard := tgbotapi.NewRemoveKeyboard(true)
	ms.ReplyMarkup = removeKeyboard
	bot.Send(ms)
}
