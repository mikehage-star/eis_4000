# Progress: EIS-4000

## What Works
- [x] PlatformIO project initialized for ESP32 DevKit
- [x] Serial monitor output (USB, 115200 baud)
- [x] UART1 initialization on GPIO 19/20 (RS232, 9600 baud)
- [x] Bidirectional byte forwarding between USB↔UART1
- [x] Fixed hex output bug: `Serial.write("%2x", ...)` → proper `%02x` formatting via snprintf
- [x] Git initialized on `master` branch with initial commit
- [x] GitHub repo created and code pushed to https://github.com/mikehage-star/eis_4000
- [x] `dev` branch created with branch protection (requires 1 PR approval before merge)

## Workflow
**Branch strategy:** All tasks create a new feature branch from `dev`. Every task requires a pull request that must be approved by the user before merging into `dev`.

```
master ← dev ← feature/xxx → PR → review/approve → merge to dev
```

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
2. **M2:** ✅ Git repo created and pushed to GitHub (mikehage-star/eis_4000)
3. **M3:** Define EIS-4000 requirements and target protocol(s) → create PR from feature branch
4. **M4:** Implement required feature set based on M3 decisions → create PR from feature branch
