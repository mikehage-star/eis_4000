# EFIS Constitution

## Core Principles

### I. Safety-First Design
All display and output logic must meet aviation-grade reliability standards; fail-safe defaults always available on power loss, watchdog reset, or sensor failure. No feature may compromise primary flight instrument availability. UI elements that are non-critical (settings menus, diagnostics) must never block or delay critical gauge rendering.

### II. Hardware Abstraction
Clear separation between hardware drivers and application logic for portability across ESP32 variants and display modules. All peripheral interactions (SPI display, I2C sensors, ADC inputs) must go through defined abstraction layers. Direct register access is prohibited outside of driver implementations.

### III. Real-Time Determinism
Display refresh and sensor polling must meet strict timing budgets; no blocking operations on critical paths. The main render loop must complete within one frame budget (target: 60 Hz / ~16.7 ms per frame). Long-running operations (file I/O, network, heavy computation) must be offloaded to background tasks with appropriate synchronization.

### IV. Test-First (NON-NEGOTIABLE)
Unit tests for all logic layers before integration; hardware-in-the-loop testing before any flight deployment. TDD enforced: tests written and reviewed → tests fail → implement until green → refactor. Simulation harness required for all instrument computation logic to enable CI testing without physical hardware.

### V. AI Safety Boundary
AI/ML features are auxiliary only; primary instrument data never derived from AI inference. Any neural network or ML model output must be validated against physics-based sensor fusion results before display. AI-assisted development tools (code generation, refactoring) are permitted but all generated code requires manual review for safety-critical paths.

## Technology Stack & Constraints

- **MCU**: ESP32-P4 with 7-inch display
- **Framework**: ESP-IDF or Arduino framework (document choice per module)
- **Language**: C for critical paths, Python/C++ for build/test tooling
- **Build System**: CMake (ESP-IDF standard)
- **CI/CD**: Automated unit tests on every commit; hardware-in-the-loop nightly
- **Version Control**: Git with sequential branch numbering per `.specify/init-options.json`

## Development Workflow

### Code Quality Gates
- All changes require passing unit tests before merge
- Safety-critical modules require additional code review beyond standard PR process
- No direct commits to main; all work via feature branches (`speckit.git.feature`)
- Merge requires: CI green + at least one reviewer approval

### Documentation Requirements
- Every module must have a README describing its interface, dependencies, and testing approach
- Public APIs must include doxygen-style comments
- Hardware pin mappings documented in `docs/hardware/` with revision tracking

## Governance

This constitution supersedes all other development practices for the EFIS project. Amendments require:
1. Proposed change documented as a comment update to this file
2. Impact assessment on existing code and safety assumptions
3. Approval from project lead before merging

All PRs and reviews must verify compliance with these principles. Complexity must be justified — if a simpler approach satisfies all constraints, it is preferred. Use `.specify/workflows/speckit/workflow.yml` for the standard development workflow (specify → plan → tasks → implement).

**Version**: 1.0.0 | **Ratified**: 2026-04-20 | **Last Amended**: 2026-04-20
