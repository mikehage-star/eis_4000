#pragma once

#include <Arduino.h>
#include <functional>

// GRT EIS-4000 / Model 4000/6000 protocol decoder
// Header: FE FF FE (3 bytes)
// Data:   68 bytes of sensor fields
// Checksum: 1 byte = complement of sum of all 68 data bytes

struct GrtFrame {
    uint16_t tach;          // Tachometer RPM, resolution 1
    uint16_t tach2;         // Tachometer 2 RPM, resolution 1
    uint16_t cht[6];        // CHT 1-6, resolution 1 deg F
    uint16_t egt[6];        // EGT 1-6, resolution 1 deg F
    uint16_t aux[8];        // AUX 1-8 (AUX5/6 = TIT when equipped)
    uint16_t airspeed;      // Airspeed (same units as EIS display)
    int16_t  altitude;      // Altitude in tens of feet, 2's complement
    float    voltage;       // Resolution 0.1 V
    float    fuelFlow;      // Fuel flow GPH, resolution 0.1
    uint16_t oilTemp;       // Oil temperature, resolution 1 deg F
    uint16_t oilPressure;   // Oil pressure, resolution 1
    uint16_t coolantTemp;   // Coolant temp (or TACH3 if configured)
    float    hourmeter;     // Hourmeter hours, resolution 0.1 h
    float    fuelQuantity;  // Fuel quantity gallons, resolution 0.1 gal
    uint8_t  flightHours;   // Flight timer hours
    uint8_t  flightMinutes; // Flight timer minutes
    uint8_t  flightSeconds; // Flight timer seconds
    uint8_t  timeEmptyHrs;  // Time until empty - hours
    uint8_t  timeEmptyMins; // Time until empty - minutes
    float    baroSetting;   // Barometric setting, resolution 0.01 inHg
    int8_t   verticalSpeedSign; // V/S sign: +1 = up, -1 = down (100 fpm res)
    uint8_t  internalTemp;  // Internal instrument temperature
    int8_t   carbIcing;     // Carb icing indicator, 2's complement
    uint8_t  oatOffset;     // OAT offset value (+50 to get actual), unsigned

    bool isValid = false;
    uint16_t checksumReceived = 0;
    uint16_t checksumCalculated = 0;

    // Raw bytes for checksum calculation and field access (must match spec layout exactly)
    union {
        struct {
            uint8_t tachH, tachL;
            uint8_t cht[12];       // 6 × 2-byte CHT
            uint8_t egt[12];       // 6 × 2-byte EGT
            uint8_t aux5H, aux5L;
            uint8_t aux6H, aux6L;
            uint8_t airspeedH, airspeedL;
            uint8_t altH, altL;
            uint8_t volthH, volthL;
            uint8_t fuelFH, fuelFL;
            uint8_t unitT;         // Internal instrument temperature
            int8_t  carbT;         // Carb icing (2's complement)
            int8_t  rocSign;       // V/S sign (2's complement)
            uint8_t oatH;          // OAT offset (unsigned 8-bit, +50)
            uint8_t oilTH, oilTL;
            uint8_t oilP;
            uint8_t aux1H, aux1L;
            uint8_t aux2H, aux2L;
            uint8_t aux3H, aux3L;
            uint8_t aux4H, aux4L;
            uint8_t coolTH, coolTL;
            uint8_t etih, etil;
            uint8_t qtyH, qtyL;
            uint8_t hrs;
            uint8_t mins;
            uint8_t secs;
            uint8_t endHrs;
            uint8_t endMins;
            uint8_t baroH, baroL;
            uint8_t tach2H, tach2L;
            uint8_t spare;
        } raw;
        uint8_t bytes[68];  // Flat byte access at same memory offset
    };

    // Scale factors for display output
    int16_t getAltitudeFeet() const { return altitude * 10; }
    float   getVoltage()      const { return voltage * 0.1f; }
    float   getFuelFlowGPH()  const { return fuelFlow * 0.1f; }
    float   getFuelQtyGal()   const { return fuelQuantity * 0.1f; }
    float   getHourmeterHrs() const { return hourmeter * 0.1f; }
    float   getBaroInHg()     const { return baroSetting * 0.01f; }
    int16_t getVerticalSpeedFpm() const { return verticalSpeedSign * 100; }

    // Helper to format a single field as "NAME=value" with optional unit suffix
    String getField(uint8_t index) const;
};

class GrtDecoder {
public:
    using FrameCallback = std::function<void(const GrtFrame&)>;

    GrtDecoder();

    // Feed one byte at a time from the RS232 stream
    void feedByte(uint8_t b);

    // Reset decoder state machine and clear last frame data
    void reset();

    // Set callback for when a complete valid frame is received
    void onFrame(FrameCallback cb) { _frameCallback = cb; }

    // Get last decoded frame (caller must check isValid)
    const GrtFrame& getLastFrame() const { return _lastFrame; }

    // Statistics
    uint32_t getFramesReceived()  const { return _framesReceived; }
    uint32_t getChecksumErrors()  const { return _checksumErrors; }
    uint32_t getHeaderResets()    const { return _headerResets; }

private:
    enum class State : uint8_t {
        WAIT_HEADER_1,
        WAIT_HEADER_2,
        WAIT_HEADER_3,
        COLLECT_DATA,
        WAIT_CHECKSUM
    };

    void processFrame();
    bool validateChecksum();

    State _state = State::WAIT_HEADER_1;
    uint8_t _dataIndex = 0;
    GrtFrame _lastFrame;
    FrameCallback _frameCallback = nullptr;

    // Statistics
    uint32_t _framesReceived = 0;
    uint32_t _checksumErrors = 0;
    uint32_t _headerResets = 0;

    // Header sync pattern
    static constexpr uint8_t HEADER_1 = 0xFE;
    static constexpr uint8_t HEADER_2 = 0xFF;
    static constexpr uint8_t HEADER_3 = 0xFE;
    static constexpr size_t DATA_LENGTH = 68;
};
