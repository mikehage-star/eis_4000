#include <Arduino.h>

// Configuration
#define RS232_BAUD 9600          // Change to match your RS232 device (e.g., 4800, 19200, 115200)
#define UART_RX_PIN 19           // ESP32 Serial1 RX (GPIO19)
#define UART_TX_PIN 20           // ESP32 Serial1 TX (GPIO20)

void setup() {
  // Initialize USB serial (via onboard CP210x/CH340 adapter on most DevKit boards)
  Serial.begin(115200);
  // Initialize hardware UART for RS232 connection
  Serial1.begin(RS232_BAUD, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
  

  for (uint8_t i=200;i>0;i--) {
    Serial.println(i);
  }
  Serial.println(F("\n=== ESP32 DevKit RS232 <-> USB Forwarder ==="));
  Serial.print(F("USB Baud: 115200 | UART Baud: "));
  Serial.println(RS232_BAUD);
}

void loop() {
  // Forward from RS232 (UART) to USB
  if (Serial1.available()) {
    while (Serial1.available()) {
      char buf[3];
      snprintf(buf, sizeof(buf), "%02x", (unsigned char)Serial1.read());
      Serial.print(buf);
    }
  }

  // Optional: Forward from USB to RS232 (bidirectional)
  // Remove this block if you only want UART -> USB forwarding
  if (Serial.available()) {
    while (Serial.available()) {
      Serial1.write(Serial.read());
    }
  }
}
