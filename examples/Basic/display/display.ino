/**
 * @file display.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief M5StickCPlus2 Display Test
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

void draw_function(LovyanGFX* gfx) {
    int x      = rand() % gfx->width();
    int y      = rand() % gfx->height();
    int r      = (gfx->width() >> 4) + 2;
    uint16_t c = rand();
    gfx->fillRect(x - r, y - r, r * 2, r * 2, c);
}

void setup() {
    auto cfg = M5.config();
    StickCP2.begin(cfg);
    int textsize = StickCP2.Display.height() / 60;
    if (textsize == 0) {
        textsize = 1;
    }
    StickCP2.Display.setTextSize(textsize);
}

void loop() {
    int x      = rand() % StickCP2.Display.width();
    int y      = rand() % StickCP2.Display.height();
    int r      = (StickCP2.Display.width() >> 4) + 2;
    uint16_t c = rand();
    StickCP2.Display.fillCircle(x, y, r, c);
    draw_function(&StickCP2.Display);
}
