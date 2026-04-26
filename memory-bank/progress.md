# Progress: EIS-4000

## What Works
- [x] PlatformIO project initialized for ESP32 DevKit
- [x] Serial monitor output (USB, 115200 baud)
- [x] UART1 initialization on GPIO 19/20 (RS232, 9600 baud)
- [x] Bidirectional byte forwarding between USB↔UART1
- [x] Fixed hex output bug: `Serial.write("%2x", ...)` → proper `%02x` formatting via snprintf

## What's Left to Build
- [ ] Requirements definition (in progress)
- [ ] Determine if transparent bridge is sufficient or intelligent processing needed
- [ ] Protocol parsing layer (if structured RS232 protocol identified)
- [ ] Command interface / CLI (if interactive control needed)
- [ ] Error handling: buffer overflow, framing errors, timeout management
- [ ] Configuration system: configurable baud rate, pin mapping, mode selection

## Current Status
**Phase:** Requirements gathering & project scaffolding

Memory Bank documentation is initialized. Code compiles and runs but has a known formatting bug in the UART→USB forwarding path.

## Known Issues
1. **No flow control:** High baud rates or bursty data may cause buffer overflows.
2. **Blocking loops:** `while(available())` blocks the reverse direction while draining one side — could miss data arriving on the other port.

## Next Milestones
1. **M1:** ✅ Fixed hex output bug, verified serial forwarding compiles
2. **M2:** Define EIS-4000 requirements and target protocol(s)
3. **M3:** Implement required feature set based on M2 decisions
