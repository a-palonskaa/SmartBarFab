#pragma once

namespace Config {
    // WiFi
   inline const String SSID = "Aliffka";
   inline const String Password = "19fuckyoubitch1041";

    // Telegram
   inline const String BotToken = "8219573502:AAEs7DSS56JjHXstaEAlS3rX03xJQ5YI4ME";
   inline const String BotPassword = "meow";

    // Serial
    inline const int SerialBaud = 115200;

    // Delay
    inline const int MessageProcessingDelay = 50;
    inline const int WifiConnectionDelay = 300;

    // Limits
    inline const int MAX_USERS = 10;
    inline const int MAX_COCKTAILS = 20;
    inline const int MAX_SESSIONS = 10;
}

