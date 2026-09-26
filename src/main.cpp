#include <Adafruit_NeoPixel.h>
#include "esp_heap_caps.h"

#define PIN 48
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(115200);
    delay(1000);  // 等串口稳定

    Serial.println("=== BOOT ===");

    // 打印内存信息
    Serial.printf("Internal RAM free: %u bytes\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
    Serial.printf("PSRAM total: %u bytes\n", heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
    Serial.printf("PSRAM free : %u bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));

    pixels.begin();
    Serial.println("LED strip initialized");
}

void loop() {
    pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // 红
    pixels.show();
    delay(500);

    pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // 绿
    pixels.show();
    delay(500);

    pixels.setPixelColor(0, pixels.Color(0, 0, 255)); // 蓝
    pixels.show();
    delay(500);
}