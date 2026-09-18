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
- [x] WiFi AP mode (SSID: EIS-4000, IP: 192.168.4.1)
- [x] HTTP server with `/` (dashboard) and `/api/telemetry` (JSON) endpoints
- [x] Responsive web dashboard — dark cockpit theme, 4-cylinder CHT/EGT grid, zone coloring
- [x] JS polling at 2s interval with stale-data detection
- [x] JSON telemetry includes all 6 CHT/EGT slots (future-proofed) + stats

## What's Left to Build

### M1: Fix bugs + stable RS232 forwarding ✅ COMPLETE
- Fixed blocking loops → non-blocking single-byte reads
- Added yield() for ESP32 RTOS stability  
- Added decoder reset() for consistent state management
- Removed debug countdown timer from setup()

### M2: WiFi AP + JSON telemetry server ✅ COMPLETE
- [x] WiFi AP mode (192.168.4.1/24)
- [x] HTTP server serving responsive web dashboard (HTML/CSS/JS)
- [x] Polling endpoint (`/api/telemetry`) for decoded frame data every 2s
- [x] Dashboard gauges: RPM, CHT(×4), EGT(×4), oil temp/pressure, voltage, altitude, fuel
- [x] Zone-based color coding (green/yellow/red) per gauge
- [x] Stale data detection (3 unchanged polls → STALE indicator)
- [x] Builds clean: RAM 13.8%, Flash 58.4%

### M3: Configuration system
- [ ] Web-based config page: WiFi settings, UART baud, display channels
- [ ] NVS persistence for config
- [ ] Frame quality stats (checksum errors, frames/sec)

## Current Status
**Phase:** M2 complete — ready to start M3 (Configuration system) or hardware smoke test
**Branch:** `feature/m1_fix_bugs` (M2 changes pending commit)

## Known Issues
1. **No flow control:** High baud rates or bursty data may cause buffer overflows.
2. **Single HTML file:** Dashboard is ~4KB in flash; fine for now but may need optimization if M3 adds more pages.

## Next Milestones
1. **M1:** ✅ Complete — stable RS232 decoding with non-blocking I/O
2. **M2:** ✅ Complete — WiFi AP + responsive web dashboard (2s update, zone coloring)
3. **M3:** Configuration system with NVS persistence
4. **Smoke test:** Flash to ESP32, connect phone to EIS-4000 AP, verify dashboard
