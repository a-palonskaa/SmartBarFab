#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#include "config.h"
#include "user.h"
#include "bot_handler.h"

WiFiClientSecure client;
UniversalTelegramBot bot(Config::BotToken, client);
UserStateManager userStateManager;
BotHandler botHandler{&bot, &userStateManager};

void connectToWifi();
void handleTelegramMessages();
void sendWelcomeMessage();

void setup() {
    Serial.begin(Config::SerialBaud);
    Serial.println("[DEBUG]: started"); // DEBUG

    connectToWifi();
    Serial.println("[DEBUG]: connected"); // DEBUG

    client.setInsecure();
}

void loop() {
    handleTelegramMessages();
    delay(Config::MessageProcessingDelay);
}

void handleTelegramMessages() {
    size_t numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    for (size_t  i = 0; i < numNewMessages; i++) {
        Serial.println(bot.messages[i].text); // DEBUG
        botHandler.processMessage(String(bot.messages[i].chat_id), bot.messages[i].text);
    }
}

void connectToWifi() {
    WiFi.begin(Config::SSID, Config::Password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(Config::WifiConnectionDelay);
    }
}
