#include "GrtDecoder.h"

// Helper to combine high + low bytes into a 16-bit value
static inline uint16_t hiLo(uint8_t h, uint8_t l) {
    return (uint16_t)h << 8 | l;
}

// Helper for signed 16-bit from high+low (2's complement)
static inline int16_t hiLoSigned(uint8_t h, uint8_t l) {
    return (int16_t)((uint16_t)h << 8 | l);
}

GrtDecoder::GrtDecoder() {}

void GrtDecoder::reset() {
    _state       = State::WAIT_HEADER_1;
    _dataIndex   = 0;
    memset(&_lastFrame, 0, sizeof(_lastFrame));
}

void GrtDecoder::feedByte(uint8_t b) {
    switch (_state) {
        case State::WAIT_HEADER_1:
            if (b == HEADER_1) _state = State::WAIT_HEADER_2;
            break;

        case State::WAIT_HEADER_2:
            if (b == HEADER_2) {
                _state = State::WAIT_HEADER_3;
            } else {
                // Reset — didn't get FF after FE
                _headerResets++;
                reset();  // clear stale decoded fields too
                _state = (b == HEADER_1) ? State::WAIT_HEADER_2 : State::WAIT_HEADER_1;
            }
            break;

        case State::WAIT_HEADER_3:
            if (b == HEADER_3) {
                // Full header found, start collecting data
                _state = State::COLLECT_DATA;
                _dataIndex = 0;
                memset(&_lastFrame.raw, 0, sizeof(_lastFrame.raw));
            } else {
                _headerResets++;
                reset();  // clear stale decoded fields too
                _state = (b == HEADER_1) ? State::WAIT_HEADER_2 : State::WAIT_HEADER_1;
            }
            break;

        case State::COLLECT_DATA:
            if (_dataIndex < DATA_LENGTH) {
                _lastFrame.bytes[_dataIndex] = b;
                _dataIndex++;
            } else {
                // Overflow — should not happen, reset
                _headerResets++;
                reset();  // clear stale decoded fields too
            }
            break;

        case State::WAIT_CHECKSUM:
            _lastFrame.checksumReceived = b;
            processFrame();
            reset();  // clear for next frame
            break;
    }
}

bool GrtDecoder::validateChecksum() {
    uint8_t sum = 0;
    for (size_t i = 0; i < DATA_LENGTH; i++) {
        sum += _lastFrame.bytes[i];
    }
    _lastFrame.checksumCalculated = (~sum) & 0xFF;
    return (_lastFrame.checksumReceived == _lastFrame.checksumCalculated);
}

void GrtDecoder::processFrame() {
    bool checksumOk = validateChecksum();

    // Parse all fields from raw bytes per the GRT spec layout
    auto& r = _lastFrame.raw;

    _lastFrame.tach          = hiLo(r.tachH, r.tachL);
    for (int i = 0; i < 6; i++) {
        _lastFrame.cht[i]    = hiLo(r.cht[i * 2], r.cht[i * 2 + 1]);
        _lastFrame.egt[i]    = hiLo(r.egt[i * 2], r.egt[i * 2 + 1]);
    }
    for (int i = 0; i < 8; i++) {
        switch (i) {
            case 0: _lastFrame.aux[0] = hiLo(r.aux1H, r.aux1L); break; // aux1
            case 1: _lastFrame.aux[1] = hiLo(r.aux2H, r.aux2L); break; // aux2
            case 2: _lastFrame.aux[2] = hiLo(r.aux3H, r.aux3L); break; // aux3
            case 3: _lastFrame.aux[3] = hiLo(r.aux4H, r.aux4L); break; // aux4
            case 4: _lastFrame.aux[4] = hiLo(r.aux5H, r.aux5L); break; // aux5 (TIT)
            case 5: _lastFrame.aux[5] = hiLo(r.aux6H, r.aux6L); break; // aux6 (TIT)
            default: break;
        }
    }
    _lastFrame.airspeed     = hiLo(r.airspeedH, r.airspeedL);
    _lastFrame.altitude     = hiLoSigned(r.altH, r.altL);
    _lastFrame.voltage      = (float)hiLo(r.volthH, r.volthL);
    _lastFrame.fuelFlow     = (float)hiLo(r.fuelFH, r.fuelFL);
    _lastFrame.internalTemp  = r.unitT;
    _lastFrame.carbIcing     = r.carbT;
    _lastFrame.verticalSpeedSign = r.rocSign;
    _lastFrame.oatOffset     = r.oatH;
    _lastFrame.oilTemp       = hiLo(r.oilTH, r.oilTL);
    _lastFrame.oilPressure   = r.oilP;
    _lastFrame.coolantTemp   = hiLo(r.coolTH, r.coolTL);
    _lastFrame.hourmeter     = (float)hiLo(r.etih, r.etil);
    _lastFrame.fuelQuantity  = (float)hiLo(r.qtyH, r.qtyL);
    _lastFrame.flightHours   = r.hrs;
    _lastFrame.flightMinutes = r.mins;
    _lastFrame.flightSeconds = r.secs;
    _lastFrame.timeEmptyHrs  = r.endHrs;
    _lastFrame.timeEmptyMins = r.endMins;
    _lastFrame.baroSetting   = (float)hiLo(r.baroH, r.baroL);
    _lastFrame.tach2         = hiLo(r.tach2H, r.tach2L);

    // Mark frame validity
    _lastFrame.isValid = checksumOk;

    if (checksumOk) {
        _framesReceived++;
    } else {
        _checksumErrors++;
    }

    // Fire callback if set
    if (_frameCallback && checksumOk) {
        _frameCallback(_lastFrame);
    }
}

String GrtFrame::getField(uint8_t index) const {
    char buf[64];
    switch (index) {
        case 0:  snprintf(buf, sizeof(buf), "TACH=%u", tach); break;
        case 1:  snprintf(buf, sizeof(buf), "TACH2=%u", tach2); break;
        case 2:  snprintf(buf, sizeof(buf), "CHT1=%u", cht[0]); break;
        case 3:  snprintf(buf, sizeof(buf), "CHT2=%u", cht[1]); break;
        case 4:  snprintf(buf, sizeof(buf), "CHT3=%u", cht[2]); break;
        case 5:  snprintf(buf, sizeof(buf), "CHT4=%u", cht[3]); break;
        case 6:  snprintf(buf, sizeof(buf), "CHT5=%u", cht[4]); break;
        case 7:  snprintf(buf, sizeof(buf), "CHT6=%u", cht[5]); break;
        case 8:  snprintf(buf, sizeof(buf), "EGT1=%u", egt[0]); break;
        case 9:  snprintf(buf, sizeof(buf), "EGT2=%u", egt[1]); break;
        case 10: snprintf(buf, sizeof(buf), "EGT3=%u", egt[2]); break;
        case 11: snprintf(buf, sizeof(buf), "EGT4=%u", egt[3]); break;
        case 12: snprintf(buf, sizeof(buf), "EGT5=%u", egt[4]); break;
        case 13: snprintf(buf, sizeof(buf), "EGT6=%u", egt[5]); break;
        case 14: snprintf(buf, sizeof(buf), "AUX1=%u", aux[0]); break;
        case 15: snprintf(buf, sizeof(buf), "AUX2=%u", aux[1]); break;
        case 16: snprintf(buf, sizeof(buf), "AUX3=%u", aux[2]); break;
        case 17: snprintf(buf, sizeof(buf), "AUX4=%u", aux[3]); break;
        case 18: snprintf(buf, sizeof(buf), "TIT1=%u", aux[4]); break;
        case 19: snprintf(buf, sizeof(buf), "TIT2=%u", aux[5]); break;
        case 20: snprintf(buf, sizeof(buf), "AUX7=%u", aux[6]); break;
        case 21: snprintf(buf, sizeof(buf), "AUX8=%u", aux[7]); break;
        case 22: snprintf(buf, sizeof(buf), "AIRSPEED=%u", airspeed); break;
        case 23: snprintf(buf, sizeof(buf), "ALT=%dft", getAltitudeFeet()); break;
        case 24: snprintf(buf, sizeof(buf), "VOLT=%.1fV", getVoltage()); break;
        case 25: snprintf(buf, sizeof(buf), "FUEL_FLOW=%.1fGPH", getFuelFlowGPH()); break;
        case 26: snprintf(buf, sizeof(buf), "OIL_TEMP=%u", oilTemp); break;
        case 27: snprintf(buf, sizeof(buf), "OIL_PRESS=%u", oilPressure); break;
        case 28: snprintf(buf, sizeof(buf), "COOLANT=%u", coolantTemp); break;
        case 29: snprintf(buf, sizeof(buf), "HOURMETER=%.1fh", getHourmeterHrs()); break;
        case 30: snprintf(buf, sizeof(buf), "FUEL_QTY=%.1fgal", getFuelQtyGal()); break;
        case 31: snprintf(buf, sizeof(buf), "FLIGHT_TIME=%02u:%02u:%02u", flightHours, flightMinutes, flightSeconds); break;
        case 32: snprintf(buf, sizeof(buf), "TIME_EMPTY=%uh%um", timeEmptyHrs, timeEmptyMins); break;
        case 33: snprintf(buf, sizeof(buf), "BARO=%.2f", getBaroInHg()); break;
        case 34: snprintf(buf, sizeof(buf), "V/S=%dfpm", getVerticalSpeedFpm()); break;
        case 35: snprintf(buf, sizeof(buf), "INT_TEMP=%u", internalTemp); break;
        case 36: snprintf(buf, sizeof(buf), "CARB_ICING=%d", carbIcing); break;
        case 37: snprintf(buf, sizeof(buf), "OAT_OFFSET=%d", (int)oatOffset - 50); break;
        default: snprintf(buf, sizeof(buf), "FIELD_%u=???", index); break;
    }
    return String(buf);
}
