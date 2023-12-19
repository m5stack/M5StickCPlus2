/**
 * @file mic.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief M5StickCPlus2 Microphone Record Test
 * @version 0.1
 * @date 2023-12-19
 *
 *
 * @Hardwares: M5StickCPlus2
 * @Platform Version: Arduino M5Stack Board Manager v2.0.9
 * @Dependent Library:
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5Unified: https://github.com/m5stack/M5Unified
 */

#include <M5StickCPlus2.h>

static constexpr const size_t record_number     = 200;
static constexpr const size_t record_length     = 240;
static constexpr const size_t record_size       = record_number * record_length;
static constexpr const size_t record_samplerate = 44100;
static int16_t prev_y[record_length];
static int16_t prev_h[record_length];
static size_t rec_record_idx  = 2;
static size_t draw_record_idx = 0;
static int16_t *rec_data;

#define PIN_CLK  0
#define PIN_DATA 34

void setup(void) {
    auto cfg = M5.config();

    StickCP2.begin(cfg);
    StickCP2.Display.startWrite();
    StickCP2.Display.setRotation(1);
    StickCP2.Display.setTextDatum(top_center);
    StickCP2.Display.setTextColor(WHITE);
    StickCP2.Display.setFont(&fonts::FreeSansBoldOblique12pt7b);

    rec_data = (typeof(rec_data))heap_caps_malloc(record_size *

                                                      sizeof(int16_t),
                                                  MALLOC_CAP_8BIT);
    memset(rec_data, 0, record_size * sizeof(int16_t));
    StickCP2.Speaker.setVolume(255);

    /// Since the microphone and speaker cannot be used at the same time,
    // turn
    /// off the speaker here.
    StickCP2.Speaker.end();
    StickCP2.Mic.begin();

    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate = 44100,
        .bits_per_sample =
            I2S_BITS_PER_SAMPLE_16BIT,  // is fixed at 12bit, stereo, MSB
        .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
        .communication_format =
            I2S_COMM_FORMAT_STAND_I2S,  // Set the format of the communication.
#else                                   // 设置通讯格式
        .communication_format = I2S_COMM_FORMAT_I2S,
#endif
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count    = 3,
        .dma_buf_len      = 256,
    };

    i2s_pin_config_t pin_config;
#if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0))
    pin_config.mck_io_num = I2S_PIN_NO_CHANGE;
#endif
    pin_config.bck_io_num   = I2S_PIN_NO_CHANGE;
    pin_config.ws_io_num    = PIN_CLK;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num  = PIN_DATA;

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);

    StickCP2.Display.fillCircle(70, 15, 8, RED);
    StickCP2.Display.drawString("REC", 120, 3);
}

void loop(void) {
    StickCP2.update();

    if (StickCP2.Mic.isEnabled()) {
        static constexpr int shift = 6;
        auto data                  = &rec_data[rec_record_idx * record_length];
        if (StickCP2.Mic.record(data, record_length, record_samplerate)) {
            data = &rec_data[draw_record_idx * record_length];

            int32_t w = StickCP2.Display.width();
            if (w > record_length - 1) {
                w = record_length - 1;
            }
            for (int32_t x = 0; x < w; ++x) {
                StickCP2.Display.writeFastVLine(x, prev_y[x], prev_h[x],
                                                TFT_BLACK);
                int32_t y1 = (data[x] >> shift);
                int32_t y2 = (data[x + 1] >> shift);
                if (y1 > y2) {
                    int32_t tmp = y1;
                    y1          = y2;
                    y2          = tmp;
                }
                int32_t y = ((StickCP2.Display.height()) >> 1) + y1;
                int32_t h = ((StickCP2.Display.height()) >> 1) + y2 + 1 - y;
                prev_y[x] = y;
                prev_h[x] = h;
                StickCP2.Display.writeFastVLine(x, prev_y[x], prev_h[x], WHITE);
            }

            StickCP2.Display.display();
            StickCP2.Display.fillCircle(70, 15, 8, RED);
            StickCP2.Display.drawString("REC", 120, 3);
            if (++draw_record_idx >= record_number) {
                draw_record_idx = 0;
            }
            if (++rec_record_idx >= record_number) {
                rec_record_idx = 0;
            }
        }
    }

    if (StickCP2.BtnA.wasHold()) {
        auto cfg               = StickCP2.Mic.config();
        cfg.noise_filter_level = (cfg.noise_filter_level + 8) & 255;
        StickCP2.Mic.config(cfg);
        StickCP2.Display.clear();
        StickCP2.Display.fillCircle(70, 15, 8, GREEN);
        StickCP2.Display.drawString("NF:" + String(cfg.noise_filter_level), 120,
                                    3);

    } else if (StickCP2.BtnA.wasClicked()) {
        if (StickCP2.Speaker.isEnabled()) {
            StickCP2.Display.clear();
            while (StickCP2.Mic.isRecording()) {
                delay(1);
            }
            /// Since the microphone and speaker cannot be used at the same
            /// time, turn off the microphone here.
            StickCP2.Mic.end();
            StickCP2.Speaker.begin();

            StickCP2.Display.fillTriangle(70 - 8, 15 - 8, 70 - 8, 15 + 8,
                                          70 + 8, 15, 0x1c9f);
            StickCP2.Display.drawString("PLAY", 120, 3);
            int start_pos = rec_record_idx * record_length;
            if (start_pos < record_size) {
                StickCP2.Speaker.playRaw(&rec_data[start_pos],
                                         record_size - start_pos,
                                         record_samplerate, false, 1, 0);
            }
            if (start_pos > 0) {
                StickCP2.Speaker.playRaw(rec_data, start_pos, record_samplerate,
                                         false, 1, 0);
            }
            do {
                delay(1);
                StickCP2.update();
            } while (StickCP2.Speaker.isPlaying());

            /// Since the microphone and speaker cannot be used at the same
            /// time, turn off the speaker here.
            StickCP2.Speaker.end();
            StickCP2.Mic.begin();

            StickCP2.Display.clear();
            StickCP2.Display.fillCircle(70, 15, 8, RED);
            StickCP2.Display.drawString("REC", 120, 3);
        }
    }
}
