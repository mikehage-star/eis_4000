# Active Context: EIS-4000

## Current Focus
Setting up project structure and Memory Bank documentation. Requirements are still being defined.

## Recent Changes
- **2026-04-26:** Initialized PlatformIO project with ESP32 DevKit + Arduino framework.
- **2026-04-26:** Created initial `src/main.cpp` — bidirectional UART↔USB serial forwarder.
- **2026-04-26:** Created Memory Bank documentation structure.

## Next Steps
1. Define project requirements and use case for EIS-4000.
2. Fix known bug in `main.cpp`: `Serial.write("%2x", ...)` doesn't do hex formatting.
3. Determine if the serial forwarder is sufficient or if protocol parsing/processing is needed.

## Active Decisions & Considerations
- **Requirements phase:** No firm feature set yet — need to understand what EIS-4000 communicates with and how.
- **Baud rate:** Currently hardcoded at 9600 for UART1; may need to be configurable or protocol-dependent.
- **Hex output bug:** The `%2x` format in `Serial.write()` is a known issue — needs fixing before deployment.

## Open Questions
- What device/system sits on the RS232 side?
- Is data structured (commands/responses) or raw streaming?
- Any latency, throughput, or reliability requirements?
- Should this be a transparent bridge or an intelligent gateway?
