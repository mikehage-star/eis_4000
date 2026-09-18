# Active Context: EIS-4000

## Current Focus
M2 just completed — WiFi AP + web dashboard. Ready for hardware smoke test or M3 (configuration system).

## Recent Changes
- **2026-04-26:** Initialized PlatformIO project with ESP32 DevKit + Arduino framework.
- **2026-04-26:** Created initial `src/main.cpp` — bidirectional UART↔USB serial forwarder.
- **2026-04-26:** Created Memory Bank documentation structure.
- **2026-09-17:** M1 complete — fixed blocking loops, added yield(), decoder reset().
- **2026-09-17:** M2 complete — added WiFi AP (192.168.4.1), WebServer, JSON telemetry endpoint, responsive dark-theme dashboard with 4-cyl CHT/EGT grid, zone color coding, and stale-data detection.

## Next Steps
1. Flash to hardware and smoke-test dashboard on a phone/laptop
2. Verify GRT telemetry flows correctly through the JSON endpoint
3. Start M3: Web-based config page (WiFi settings, UART baud) with NVS persistence
4. (Optional) Add `test/` unit tests for GrtDecoder

## Active Decisions & Considerations
- **4-cylinder Lycoming:** Dashboard shows CHT 1-4 and EGT 1-4; JSON sends all 6 slots for future-proofing.
- **Polling rate:** 2s (not 5s) for smoother feel. ESP32 handles the load fine (RAM at 13.8%).
- **Zone thresholds hardcoded in JS:** CHT danger >400°F, EGT danger >1550°F, oil press danger <30 PSI, voltage danger <13V. These should be configurable in M3.
- **Dashboard file size:** ~4KB raw HTML in flash — leaves plenty of room for M3 config pages.
- **WiFi AP (not STA):** Keeps the dashboard available without external network. Could add STA mode in M3 for remote access.

## Open Questions
- Should the AP be persistent or switchable to STA mode for remote monitoring? (M3)
- Are the zone thresholds correct for this specific airframe/engine? May need per-airframe config.
- Do we need a mobile PWA manifest for add-to-homescreen on phones?
