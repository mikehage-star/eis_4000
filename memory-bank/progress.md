# Progress: EIS-4000

## What Works
- [x] PlatformIO project initialized for ESP32 DevKit
- [x] Serial monitor output (USB, 115200 baud)
- [x] UART1 initialization on GPIO 19/20 (RS232, 9600 baud)
- [x] Bidirectional byte forwarding between USB↔UART1
- [x] GRT Model 4000/6000 protocol decoder (`lib/grt_decode/GrtDecoder`) — parses all 68-byte frames with checksum validation
- [x] Frame callback system for decoded telemetry data
- [x] Non-blocking UART reading (no more blocking `while(available())` loops)
- [x] ESP32 RTOS yield() in loop() for WiFi/watchdog stability
- [x] Decoder reset() method — clears stale fields on all error/sync paths

## What's Left to Build

### M1: Fix bugs + stable RS232 forwarding ✅ COMPLETE
- Fixed blocking loops → non-blocking single-byte reads
- Added yield() for ESP32 RTOS stability  
- Added decoder reset() for consistent state management
- Removed debug countdown timer from setup()

### M2: WiFi AP + JSON telemetry server (NEXT)
- [ ] Add WiFi AP mode with captive portal
- [ ] HTTP server serving responsive web dashboard (HTML/CSS/JS)
- [ ] WebSocket or polling endpoint for decoded frame data every 5s
- [ ] Dashboard gauges: RPM, CHT(×4), EGT(×4), hydraulic pressure, voltage, altitude

### M3: Configuration system
- [ ] Web-based config page: WiFi settings, UART baud, display channels
- [ ] NVS persistence for config
- [ ] Frame quality stats (checksum errors, frames/sec)

## Current Status
**Phase:** M1 complete — ready to start M2 (WiFi AP + web dashboard)
**Branch:** `feature/m1_fix_bugs` pushed to origin

## Known Issues
1. **No flow control:** High baud rates or bursty data may cause buffer overflows.
2. **No WiFi yet:** Currently serial-only; no network connectivity for remote monitoring.

## Next Milestones
1. **M1:** ✅ Complete — stable RS232 decoding with non-blocking I/O
2. **M2:** WiFi AP + responsive web dashboard (phone/iPad target, 5s update rate)
3. **M3:** Configuration system with NVS persistence
