# Tech Context: EIS-4000

## Platform & Framework
- **MCU:** ESP32 (ESP-WROOM-32 or compatible)
- **Board:** esp32dev (PlatformIO board definition)
- **Framework:** Arduino (espressif32@6.8.1)
- **IDE/Build System:** PlatformIO

## Hardware Configuration
| Interface | Pin(s) | Purpose |
|-----------|--------|---------|
| Serial (USB) | Built-in CP210x/CH340 | USB-to-Serial monitor/debug |
| Serial1 (UART) | RX: GPIO 19, TX: GPIO 20 | RS232 communication |

## Software Dependencies
- ESP32 Arduino core (espressif32@6.8.1)
- No external libraries currently used

## Build Configuration (`platformio.ini`)
```ini
[env:esp32-devkit]
platform = espressif32@6.8.1
board = esp32dev
framework = arduino
upload_protocol = esptool
monitor_port=/dev/ttyUSB0
monitor_speed = 115200
build_flags = -DCORE_DEBUG_LEVEL=0
```

## Development Setup
- **Upload:** ESP bootloader via `/dev/ttyUSB0`
- **Monitor:** Serial monitor at 115200 baud (USB), 9600 baud (UART1)
- **Debugging:** CORE_DEBUG_LEVEL set to 0 (disabled); can be increased for ESP32 core logs

## Known Issues in Current Code
1. `Serial.write("%2x", ...)` — `Serial.write()` does not support printf-style formatting; this prints literal `%2x` instead of hex values. Should use `Serial.print(..., HEX)` or a custom formatter.
2. No flow control on UART forwarding (potential data loss at high throughput).
3. Blocking `while(available())` loops — no timeout handling for bidirectional forwarding.

## Potential Future Tech Additions
- Software serial libraries if more UART channels needed
- Protocol parsing libraries (Modbus, custom binary protocols)
- Storage (SPIFFS/FFat) for data logging
- WiFi/BLE capabilities (ESP32 native, not currently used)
