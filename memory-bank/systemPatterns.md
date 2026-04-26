# System Patterns: EIS-4000

## Architecture
Simple single-loop embedded application with bidirectional serial forwarding.

```
[RS232 Device] ↔ (GPIO 19 RX / GPIO 20 TX) ↔ ESP32 Serial1
                                              ↕
                                    [ESP32 Core]
                                              ↕
                              (CP210x/CH340 USB-to-Serial)
                                              ↕
                                    [USB Host / PC]
```

## Current Pattern: Polling Forwarder
- **Main loop** polls `Serial1.available()` and forwards bytes to `Serial` (USB).
- **Reverse direction** polls `Serial.available()` and forwards bytes to `Serial1` (RS232).
- Both directions use blocking `while(available())` loops — drain the buffer in one iteration.

## Design Characteristics
- **Synchronous:** No RTOS tasks, no interrupts for serial data.
- **Single priority:** All forwarding happens in `loop()` with equal weight.
- **No buffering strategy:** Data is forwarded byte-by-byte as it arrives.

## Key Technical Decisions
1. **ESP32 DevKit + Arduino** — chosen for rapid prototyping and hardware availability.
2. **Hardware UART (Serial1)** over SoftwareSerial — reliable, no timing issues at any baud rate.
3. **GPIO 19/20 for Serial1** — standard ESP32 alternate pins; can be changed via `begin()` parameters.

## Planned Patterns (TBD)
- [ ] Protocol parser state machine (if RS232 uses a structured protocol)
- [ ] Command interpreter (if interactive CLI over serial is needed)
- [ ] Ring buffer + interrupt-driven UART (for high-throughput scenarios)
- [ ] Task-based architecture (FreeRTOS tasks for each direction)
