#pragma once

#include <Arduino.h>
#include "config.h"

enum SessionState {
    WAIT_PASSWORD,
    AUTHORIZED,
    WAIT_DRINK,
    DONE
};

struct UserSession {
    String chatId;
    SessionState state;
    unsigned long scheduledTime;
    String scheduledDrink;
};

class UserStateManager {
public:
    UserSession* getSession(String chatId);
    void updateSessionState(String chatId, SessionState newState);
    SessionState getSessionState(String chatId);

    bool isAuthorized(String chatId);

    void scheduleDrink(String chatId, unsigned long delayMinutes);
    void nameScheduledDrink(String chatId, String drink);
    void checkScheduledDrinks();
private:
    int sessions_amount_{0};
    UserSession sessions_[Config::MAX_SESSIONS];

    void sendToArduino(String command);
};
