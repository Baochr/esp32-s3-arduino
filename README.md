# ESP32-S3 N16R8 Arduino 开发模板

基于 **CLion + PlatformIO + Arduino 框架** 的 ESP32-S3-N16R8 开发模板。

## 硬件参数

| 项目 | 值 |
|------|-----|
| 芯片 | ESP32-S3 (QFN56, revision v0.2) |
| Flash | 16MB (QIO 模式) |
| PSRAM | 8MB (Octal 模式, 80MHz) |
| CPU | 双核 240MHz |
| 板载 LED | GPIO48 (WS2812) |
| 串口芯片 | CH340 (`/dev/cu.wchusbserial*`) |

## 项目结构

    esp32-s3-arduino/
    ├── platformio.ini              # PlatformIO 项目配置
    ├── partitions/
    │   └── default_16MB.csv        # 16MB 分区表
    └── src/
        └── main.cpp                # 程序入口

## 快速开始

### 1. 复制模板

    cp -r ~/Documents/esp32-templates/arduino-n16r8 ~/CLionProjects/新项目名
    cd ~/CLionProjects/新项目名

### 2. 修改 `main.cpp`

```cpp
#include <Arduino.h>
#include "esp_heap_caps.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("=== BOOT ===");
    Serial.printf("PSRAM total: %u bytes\n", heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
}

void loop() {
    Serial.println("tick");
    delay(1000);
}
```

### 3. 构建与上传

在 CLion 底部 Terminal 执行：

    pio run -t upload

### 4. 查看串口日志

    pio device monitor

按 `Ctrl+C` 退出监视器。

## `platformio.ini` 配置说明

| 配置项 | 作用 |
|--------|------|
| `board_upload.flash_size = 16MB` | 指定 Flash 大小 |
| `board_build.partitions = default_16MB.csv` | 使用 16MB 分区表 |
| `board_build.arduino.memory_type = qio_opi` | Flash 用 QIO，PSRAM 用 OPI |
| `board_build.flash_mode = qio` | Flash 工作模式 |
| `board_build.psram_type = opi` | PSRAM 类型 |
| `board_build.extra_flags = -DBOARD_HAS_PSRAM` | 启用 PSRAM |
| `monitor_speed = 115200` | 串口监视器波特率，必须与 `Serial.begin()` 一致 |
| `lib_deps` | 依赖库，按需添加 |

## PSRAM 使用

Arduino 框架默认不会把 `malloc()` 分配到 PSRAM，需要显式指定：

```cpp
#include "esp_heap_caps.h"

// 从 PSRAM 分配 1MB
uint8_t *buf = (uint8_t *)heap_caps_malloc(1024 * 1024, MALLOC_CAP_SPIRAM);
if (buf == NULL) {
    Serial.println("PSRAM alloc failed");
}

// 用完释放
heap_caps_free(buf);
```

验证 PSRAM 是否可用：

```cpp
Serial.printf("PSRAM total: %u bytes\n", heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
Serial.printf("PSRAM free : %u bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
```

预期输出：`PSRAM total: 8388608 bytes`（8MB）。

## 常见问题

### 1. 串口乱码

- `monitor_speed` 必须和代码里 `Serial.begin()` 的波特率一致
- `setup()` 里加 `delay(1000)` 等串口稳定

### 2. 串口被占用

上传前先退出监视器（`Ctrl+C`），否则会报 `port is busy`。

### 3. 上传失败

- 检查 USB 线是否连接正常
- 确认 `monitor_port` 是否指向正确的设备（`pio device list` 查看）
- 必要时按住 BOOT 键，按一下 RST 键，再松开 BOOT，进入下载模式

### 4. PSRAM 显示为 0

- 确认 `platformio.ini` 里有 `board_build.extra_flags = -DBOARD_HAS_PSRAM`
- 确认 `board_build.psram_type = opi`
- 如果仍然为 0，可能是板子的 PSRAM 是 Quad 模式，改成 `qpi` 试试

### 5. `mode:DIO` 是否需要优化？

不需要。`mode:DIO` 是 Bootloader 阶段的临时模式，程序运行时会自动切换到 QIO。

## 调试（可选）

如果使用板子上的**原生 USB 口**（不是 CH340 口），可以在 `platformio.ini` 里加：

```ini
build_type = debug
debug_tool = esp-builtin
```

然后可以在 CLion 里单步调试。

## 参考

- [PlatformIO 文档](https://docs.platformio.org/)
- [Arduino-ESP32 文档](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
- [Adafruit NeoPixel 库](https://github.com/adafruit/Adafruit_NeoPixel)

