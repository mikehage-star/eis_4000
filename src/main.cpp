#include <Arduino.h>
#include "GrtDecoder.h"

// Configuration
#define RS232_BAUD 9600          // Change to match your RS232 device (e.g., 4800, 19200, 115200)
#define UART_RX_PIN 19           // ESP32 Serial1 RX (GPIO19)
#define UART_TX_PIN 20           // ESP32 Serial1 TX (GPIO20)

// GRT EIS-4000 decoder instance
GrtDecoder grt;

void setup() {
    // Initialize USB serial (via onboard CP210x/CH340 adapter on most DevKit boards)
    Serial.begin(115200);
    // Initialize hardware UART for RS232 connection
    Serial1.begin(RS232_BAUD, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);

    delay(500);  // brief pause to let USB serial stabilize on host
    Serial.println(F("\n=== GRT EIS-4000 Decoder ==="));
    Serial.print(F("USB Baud: 115200 | RS232 Baud: "));
    Serial.println(RS232_BAUD);

    // Set up frame callback — prints decoded fields when a valid frame arrives
    grt.onFrame([](const GrtFrame& f) {
        String line = "";
        for (uint8_t i = 0; i < 38; i++) {
            if (!line.isEmpty()) line += " | ";
            line += f.getField(i);
        }
        Serial.println(line);

        // Print stats every 10 frames
        static uint32_t counter = 0;
        counter++;
        if (counter % 10 == 0) {
            Serial.print(F("[Stats] Frames: "));
            Serial.print(grt.getFramesReceived());
            Serial.print(F(" | Checksum errors: "));
            Serial.print(grt.getChecksumErrors());
            Serial.print(F(" | Header resets: "));
            Serial.println(grt.getHeaderResets());
        }
    });

    // Also forward USB -> RS232 for bidirectional communication (e.g., sending commands)
    Serial.println(F("Ready. Forwarding RS232 -> decoded output."));
}

void loop() {
    // Feed incoming RS232 bytes into the GRT decoder (non-blocking, one byte per iteration)
    if (Serial1.available()) {
        grt.feedByte(Serial1.read());
    }

    // Forward from USB to RS232 (bidirectional, non-blocking, one byte per iteration)
    if (Serial.available()) {
        Serial1.write(Serial.read());
    }

    // ESP32 RTOS yield — required for WiFi, watchdog, and background tasks
    yield();
}
