// #include <string.h>
#include "bot_handler.h"
#include "config.h"

namespace BotMessages {
    extern const String WelcomeMessage = "sosite-suki пиздатый проект вас приветствует\n"
                  "авторизуйтесь для использования /password [password]\n"
                  "yours aliffka❤️";

    extern const String WaitPasswordUnknownCase  = "authorize";
    extern const String AuthorizedUnknownCase = "доступные команды:\n"
                                                "/make_drink\n"
                                                "/delay_drink";

    extern const String ChooseDrink = "доступные коктейли:\n"
                                      "/pinakolada\n"
                                      "/pornostar\n"
                                      "/pinkymonster\n"
                                      "/i_dont_know - если доверяете нашему вкусу";

    extern const String PasswordCorrect = "u passed authorization";
    extern const String PasswordIncorrect = "unluck, my friend, try again";
}

// ХУЙНЯ - вынести в отдельную либу, сделать мапами, добавить description и хэлперы-трансляторы
namespace ArduinoCommands {
    extern const String PINAKOLADA   = "PINAKOLADA";   // "/pinakolada"
    extern const String PORNOSTAR    = "REDMARY";      // "/pornostar"
    extern const String PINKYMONSTER = "PINKYMONSTER"; // "/pinkymonster"
}

void BotHandler::processMessage(String chatId, String text) {
    SessionState state = user_state_manager_.getSessionState(chatId);

    switch (state) {
        case WAIT_PASSWORD:
            handleWaitPassword(chatId, text);
            break;
        case AUTHORIZED:
            handleAuthorized(chatId, text);
            break;
        case WAIT_DRINK:
            handleWaitDrink(chatId, text);
            break;
    }
}

void BotHandler::handleWaitPassword(String chatId, String text) {
    if (text.startsWith("/password ")) {
        String password = text.substring(10);
        handlePasswordCommand(chatId, password);
    } else if (text == "/start") {
        handleStartCommand(chatId);
    } else {
        bot_.sendMessage(chatId, BotMessages::WaitPasswordUnknownCase);
    }
}

void BotHandler::handleAuthorized(String chatId, String text) {
    if (text == "/make_drink") {
        handleMakeDrinkCommand(chatId);
    } else if (text.startsWith("/delay_drink")) {
        String minutes = text.substring(13);
        handleDelayDrinkCommand(chatId, minutes);
    } else {
        bot_.sendMessage(chatId, BotMessages::AuthorizedUnknownCase);
    }
}

void BotHandler::handleStartCommand(String chatId) {
    user_state_manager_.getSession(chatId);
    bot_.sendMessage(chatId, BotMessages::WelcomeMessage);
}

void BotHandler::handlePasswordCommand(String chatId, String password) {
    if (password == Config::BotPassword) {
        user_state_manager_.updateSessionState(chatId, AUTHORIZED);
        bot_.sendMessage(chatId, BotMessages::PasswordCorrect);
        bot_.sendMessage(chatId, BotMessages::AuthorizedUnknownCase); //ХУЙНЯ - naming хочется велкоминг аусорайзд как-то
    } else {
        bot_.sendMessage(chatId, BotMessages::PasswordIncorrect);
    }
}

void BotHandler::handleMakeDrinkCommand(String chatId) {
    user_state_manager_.updateSessionState(chatId, WAIT_DRINK);
    user_state_manager_.scheduleDrink(chatId, 0);
    bot_.sendMessage(chatId, BotMessages::ChooseDrink);
}

void BotHandler::handleDelayDrinkCommand(String chatId, String minutes) {
    user_state_manager_.updateSessionState(chatId, WAIT_DRINK);
    user_state_manager_.scheduleDrink(chatId, minutes.toInt());
    bot_.sendMessage(chatId, BotMessages::ChooseDrink);
}

void BotHandler::handleWaitDrink(String chatId, String drinkCommand) {
    String arduinoCommand = "";

    if (drinkCommand == "/pinakolada") {
        arduinoCommand = ArduinoCommands::PINAKOLADA;
    } else if (drinkCommand == "/pornostart") {
        arduinoCommand = ArduinoCommands::PORNOSTAR;
    } else if (drinkCommand == "/pinkymonster") {
        arduinoCommand = ArduinoCommands::PINKYMONSTER;
    } else if (drinkCommand == "/i_dont_know") {
        arduinoCommand = ArduinoCommands::PINKYMONSTER; //ХУЙНЯ - some kind of reccomendation system here
    } else {
        bot_.sendMessage(chatId, BotMessages::ChooseDrink);
        return;
    }

    user_state_manager_.nameScheduledDrink(chatId, arduinoCommand);
    user_state_manager_.updateSessionState(chatId, DONE);
}
