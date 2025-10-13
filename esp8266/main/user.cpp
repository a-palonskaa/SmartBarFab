#include "user.h"
#include <UniversalTelegramBot.h>

extern UniversalTelegramBot bot;

UserSession* UserStateManager::getSession(String chatId) {
    UserSession* session = nullptr;
    for (int i = 0; i < sessions_amount_; i++) {
        if (sessions_[i].chatId == chatId) {
            session = &sessions_[i];
            break;
        }
    }
    if (!session && sessions_amount_ < Config::MAX_SESSIONS) {
        sessions_[sessions_amount_].chatId = chatId;
        sessions_[sessions_amount_].state = WAIT_PASSWORD;
        sessions_[sessions_amount_].scheduledTime = 0;
        sessions_[sessions_amount_].scheduledDrink = "";
        return &sessions_[sessions_amount_++];
    }
    return session;
}

void UserStateManager::updateSessionState(String chatId, SessionState newState) {
    getSession(chatId)->state = newState;
}


SessionState UserStateManager::getSessionState(String chatId) {
    return getSession(chatId)->state;
}

// ХУЙНЯ - мапу id - struct
bool UserStateManager::isAuthorized(String chatId) {
    for (int i = 0; i < sessions_amount_; i++) {
        if (sessions_[i].chatId == chatId) {
            return sessions_[i].state != WAIT_PASSWORD; // ХУЙНЯ юзай get session
        }
    }
    return false;
}

void UserStateManager::scheduleDrink(String chatId, unsigned long delayMinutes) {
    UserSession* session = getSession(chatId);
    if (delayMinutes > 0) {
        session->scheduledTime = millis() + (delayMinutes * 60 * 1000);
    } else {
        session->scheduledTime = millis();
    }
}

void UserStateManager::nameScheduledDrink(String chatId, String drink) {
    UserSession* session = getSession(chatId);
    session->scheduledDrink = drink;
}

void UserStateManager::checkScheduledDrinks() {
    unsigned long currentTime = millis();

    for (int i = 0; i < sessions_amount_; i++) {
        if (sessions_[i].state == DONE &&
            sessions_[i].scheduledTime > 0 && sessions_[i].scheduledTime <= currentTime) {
            sendToArduino(sessions_[i].scheduledDrink);

            // ХУЙНЯ -  хочется делать так bot.sendMessage(sessions[i].chatId, "готово");
            // но это ответсвенность не юзера а бот хэндлера, надо ручку создать какую-то

            sessions_[i].scheduledTime = 0;
            sessions_[i].scheduledDrink = "";
            sessions_[i].state = AUTHORIZED;
        }
    }
}

void UserStateManager::sendToArduino(String command) {
    Serial.println(command);
}
