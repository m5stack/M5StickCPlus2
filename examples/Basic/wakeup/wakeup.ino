/**
 * @file M5StickCPlus2.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief M5StickCPlus2 Timer Wakeup Test
 * @version 0.1
 * @date 2023-12-12
 *
 *
 * @Hardwares: M5StickCPlus2
 * @Platform Version: Arduino M5Stack Board Manager v2.0.9
 * @Dependent Library:
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5Unified: https://github.com/m5stack/M5Unified
 * M5StickCPlus2: https://github.com/m5stack/M5StickCPlus2
 */

#include <M5StickCPlus2.h>

void setup(void) {
    auto cfg = M5.config();
    StickCP2.begin(cfg);

    StickCP2.Display.setRotation(1);
    StickCP2.Display.setTextColor(GREEN);
    StickCP2.Display.setTextDatum(middle_center);
    StickCP2.Display.setTextFont(&fonts::Orbitron_Light_24);
    StickCP2.Display.setTextSize(1);

    Serial.println("Press Btn to sleep");
    Serial.println("After 5s Wakeup");

    StickCP2.Display.drawString("BtnA Sleep 5s", StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 2);
}

void loop(void) {
    StickCP2.update();

    if (StickCP2.BtnA.wasPressed()) {
        StickCP2.Power.timerSleep(5);
        // StickCP2.Power.timerSleep(const rtc_time_t& time);
        // StickCP2.Power.timerSleep(const rtc_date_t& date, const rtc_time_t&
        // time);
        // StickCP2.Power.powerOff(); shutdown
    }
}
