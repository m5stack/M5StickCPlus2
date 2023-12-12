/**
 * @file buzzer.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief M5StickCPlus2 Buzzer Test
 * @version 0.1
 * @date 2023-12-09
 *
 *
 * @Hardwares: M5StickCPlus2
 * @Platform Version: Arduino M5Stack Board Manager v2.0.9
 * @Dependent Library:
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5Unified: https://github.com/m5stack/M5Unified
 * M5StickCPlus2: https://github.com/m5stack/M5StickCPlus2
 */

#include "M5StickCPlus2.h"

void setup() {
    auto cfg = M5.config();
    StickCP2.begin(cfg);
    StickCP2.Display.setRotation(1);
    StickCP2.Display.setTextColor(GREEN);
    StickCP2.Display.setTextDatum(middle_center);
    StickCP2.Display.setTextFont(&fonts::Orbitron_Light_24);
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.drawString("Buzzer Test", StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 2);
}

void loop() {
    StickCP2.Speaker.tone(10000, 100);
    delay(1000);
    StickCP2.Speaker.tone(4000, 20);
    delay(1000);
}
