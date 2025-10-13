#pragma once

#include <Arduino.h>
#include <UniversalTelegramBot.h>

#include "user.h"

class BotHandler {
public:
    BotHandler(UniversalTelegramBot* bot, UserStateManager* usm) : bot_(*bot), user_state_manager_(*usm) {} // ХУЙНЯ

    void processMessage(String chatId, String text);

private:
    void handleWaitPassword(String chatId, String text);
    void handleAuthorized(String chatId, String text);
    void handleWaitTime(String chatId, String text);
    void handleWaitDrink(String chatId, String text);

    void handleStartCommand(String chatId);
    void handlePasswordCommand(String chatId, String password);
    void handleMakeDrinkCommand(String chatId);
    void handleDelayDrinkCommand(String chatId, String minutes);
    void handleDrinkSelection(String chatId, String drinkCommand);

private:
    UniversalTelegramBot bot_;
    UserStateManager user_state_manager_;
};
