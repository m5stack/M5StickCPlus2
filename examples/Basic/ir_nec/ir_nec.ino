/**
 * @file ir_nec.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief M5StickCPlus2 IR NEC test
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
 * IRremote: https://github.com/Arduino-IRremote/Arduino-IRremote
 */

#define DISABLE_CODE_FOR_RECEIVER  // Disables restarting receiver after each
                                   // send. Saves 450 bytes program memory and
                                   // 269 bytes RAM if receiving functions are
                                   // not used.
#define SEND_PWM_BY_TIMER
#define IR_TX_PIN 19

#include "M5StickCPlus2.h"
#include <IRremote.hpp>  // include the library

uint8_t sCommand = 0x34;
uint8_t sRepeats = 0;

void setup() {
    auto cfg = M5.config();
    StickCP2.begin(cfg);
    StickCP2.Display.setRotation(1);
    StickCP2.Display.setTextColor(GREEN);
    StickCP2.Display.setTextDatum(middle_center);
    StickCP2.Display.setTextFont(&fonts::Orbitron_Light_24);
    StickCP2.Display.setTextSize(1);

    IrSender.begin(DISABLE_LED_FEEDBACK);  // Start with IR_SEND_PIN as send pin
    IrSender.setSendPin(IR_TX_PIN);
}

void loop() {
    Serial.println();
    Serial.print(F("Send now: address=0x1111, command=0x"));
    Serial.print(sCommand, HEX);
    Serial.print(F(", repeats="));
    Serial.print(sRepeats);
    Serial.println();

    StickCP2.Display.clear();
    StickCP2.Display.drawString("IR NEC SEND", StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 2 - 40);

    StickCP2.Display.drawString("ADDR:0x1111", StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 2);

    StickCP2.Display.drawString("CMD:0x" + String(sCommand, HEX),
                                StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 2 + 40);

    Serial.println(F("Send standard NEC with 16 bit address"));

    StickCP2.Display.fillCircle(32, 105, 8, GREEN);
    IrSender.sendNEC(0x1111, sCommand, sRepeats);
    // IrSender.sendOnkyo(0x1111, 0x2223, sRepeats);
    /*
     * Increment send values
     */
    sCommand += 1;
    delay(500);
    StickCP2.Display.fillCircle(32, 105, 8, YELLOW);
    delay(500);
}
