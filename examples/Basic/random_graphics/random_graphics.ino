/**
 * @file Random graphics.ino
 * @author Tinyu (tinyu@m5stack.com)
 * @brief M5StickCPlus2 Random graphics Test
 * @version 0.1
 * @date 2024-06-14
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
    StickCP2.Display.setRotation(3);
    StickCP2.Display.setTextColor(GREEN);
    StickCP2.Display.setTextDatum(middle_center);
    StickCP2.Display.setTextFont(&fonts::Orbitron_Light_24);
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.drawString("Random graphics", StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 2);
    delay(1500);
}

void loop() {
    int x         = rand() % StickCP2.Display.width();
    int y         = rand() % StickCP2.Display.height();
    int r         = (StickCP2.Display.width() >> 4) + 2;
    uint16_t c    = rand();
    int shapeType = rand() % 5;  // 0: Circle, 1: Rectangle
                                 // 2: Ellipse, 3: Triangle, 4: Line

    StickCP2.Display.clear();  // 清除屏幕

    switch (shapeType) {
        case 0:
            StickCP2.Display.fillCircle(x, y, r, c);
            break;
        case 1:
            StickCP2.Display.fillRect(x, y, r, r, c);
            break;
        case 2:
            StickCP2.Display.fillEllipse(x, y, r + 5, r, c);
            break;
        case 3:
            StickCP2.Display.fillTriangle(x, y, x + r, y + r, x - r, y + r, c);
            break;
        case 4:
            StickCP2.Display.drawLine(x, y, x + r, y + r, c);
            break;
    }
    delay(500);
}
