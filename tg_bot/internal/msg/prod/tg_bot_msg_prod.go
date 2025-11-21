package msg_prod

import (
	"github.com/a-palonskaa/SmartBar/tg_bot/internal/coctail"
)

type Msg string

const (
	MsgStart                  Msg = "Welcome to SmartBar project🙌\nавторизуйтесь через /password [пароль]"
	MsgWrongCmdAfterStart     Msg = "для авторизации введите пароль через /password [пароль]"
	MsgPasswordOK             Msg = "вы авторизовались✅"
	MsgPasswordFail           Msg = "неверный ключ авторизации, попробуйте еще раз"
	MsgWrongCommandAuthorized Msg = "проверьте соответсвие введенной команды списку доступных"
	MsgAuthorizedCmdOptions   Msg = "доступны следующие команды:\n/make_drink\n/delay_drink [минуты]\n/recommend_drink"
	MsgWaitDrink              Msg = "совсем скоро вы насладитесь необыкновенным "
	MsgWrongInput             Msg = "данный ввод нами не обрабатывается"
	MsgChooseOccasion         Msg = "выберите наиболее релевантный повод"
	MsgChooseAlcoCategory     Msg = "предпочитаете безалкогольный, легкий или крепкий коктейль?"
)

var (
	MsgMakeDrink Msg = Msg("доступные коктейли:\n" + coctail.BuildCoctailList())
)

func (m Msg) String() string {
	return string(m)
}
