package msg

import (
	"github.com/a-palonskaa/SmartBar/tg_bot/internal/coctail"
)

type Msg string

const (
	MsgStart                  Msg = "sosite-suki пиздатый проект вас приветствует🙌\nавторизуйтесь через /password [пароль]\nYours aliffka ❤️"
	MsgWrongCmdAfterStart     Msg = "ах, котик, у тебя слишком большой... ввод..."
	MsgPasswordOK             Msg = "welcome to the club buddy"
	MsgPasswordFail           Msg = "unluck, my friend"
	MsgWrongCommandAuthorized Msg = "по клавишам научись попадать, наш дорогой пользователь💋"
	MsgAuthorizedCmdOptions   Msg = "у вас следующие возможности:\n/make_drink\n/delay_drink [минуты]\n/recommend_drink"
	MsgWaitDrink              Msg = "совсем скоро вы насладитесь необыкновенным "
	MsgWrongInput             Msg = "сладость, данный ввод нами не обрабатывается❤️"
	MsgChooseOccasion         Msg = "какой сегодня повод, приятель?"
	MsgChooseAlcoCategory     Msg = "на зоже, легкий или покрепче?"
)

var (
	MsgMakeDrink Msg = Msg("доступные коктейли:\n" + coctail.BuildCoctailList())
)

func (m Msg) String() string {
	return string(m)
}
