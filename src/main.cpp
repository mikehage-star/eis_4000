#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "GrtDecoder.h"
#include "dashboard_html.h"

// === Configuration ===
#define RS232_BAUD   9600
#define UART_RX_PIN  19
#define UART_TX_PIN  20
#define WIFI_SSID    "EIS-4000"
#define WIFI_PASS    "eis4000"

GrtDecoder grt;
WebServer server(80);

// ── HTTP handler: JSON telemetry ───────────────────────────────

void handleTelemetry() {
    const GrtFrame& f = grt.getLastFrame();
    String j;
    j.reserve(400);

    j += F("{\"valid\":");
    j += f.isValid ? F("true") : F("false");

    j += F(",\"tach\":");     j += f.tach;
    j += F(",\"ias\":");      j += f.airspeed;
    j += F(",\"alt\":");      j += f.getAltitudeFeet();

    j += F(",\"cht\":[");
    for (int i = 0; i < 6; i++) { if (i) j += ','; j += f.cht[i]; }
    j += F("]");

    j += F(",\"egt\":[");
    for (int i = 0; i < 6; i++) { if (i) j += ','; j += f.egt[i]; }
    j += F("]");

    j += F(",\"voltage\":");   j += String(f.getVoltage(), 1);
    j += F(",\"oiltemp\":");   j += f.oilTemp;
    j += F(",\"oilpress\":");  j += f.oilPressure;
    j += F(",\"coolant\":");   j += f.coolantTemp;
    j += F(",\"fflow\":");     j += String(f.getFuelFlowGPH(), 1);
    j += F(",\"fqty\":");      j += String(f.getFuelQtyGal(), 1);
    j += F(",\"hours\":");     j += String(f.getHourmeterHrs(), 1);
    j += F(",\"baro\":");      j += String(f.getBaroInHg(), 2);
    j += F(",\"vspeed\":");    j += f.getVerticalSpeedFpm();
    j += F(",\"oat\":");       j += (int)f.oatOffset - 50;

    j += F(",\"ft_h\":");     j += f.flightHours;
    j += F(",\"ft_m\":");     j += f.flightMinutes;
    j += F(",\"ft_s\":");     j += f.flightSeconds;
    j += F(",\"te_h\":");     j += f.timeEmptyHrs;
    j += F(",\"te_m\":");     j += f.timeEmptyMins;

    j += F(",\"stats\":{");
    j += F("\"frames\":");    j += grt.getFramesReceived();
    j += F(",\"errors\":");   j += grt.getChecksumErrors();
    j += F(",\"resets\":");   j += grt.getHeaderResets();
    j += F("}}");

    server.send(200, "application/json", j);
}

// ── Setup ──────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println(F("\n=== EIS-4000 M2: WiFi Telemetry Dashboard ==="));

    // RS232 UART
    Serial1.begin(RS232_BAUD, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
    Serial.print(F("UART1 (RS232): "));
    Serial.print(RS232_BAUD);
    Serial.print(F(" baud | RX=GPIO"));
    Serial.print(UART_RX_PIN);
    Serial.print(F(" TX=GPIO"));
    Serial.println(UART_TX_PIN);

    // WiFi Access Point (192.168.4.1)
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1),
                      IPAddress(192, 168, 4, 1),
                      IPAddress(255, 255, 255, 0));
    WiFi.softAP(WIFI_SSID, WIFI_PASS);
    IPAddress apIP = WiFi.softAPIP();

    Serial.print(F("WiFi AP: "));
    Serial.print(WIFI_SSID);
    Serial.print(F(" | IP: "));
    Serial.println(apIP.toString());
    Serial.print(F("Dashboard: http://"));
    Serial.println(apIP.toString());

    // HTTP server routes
    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", dashboardHtml);
    });
    server.on("/api/telemetry", HTTP_GET, handleTelemetry);
    server.onNotFound([]() {
        server.send(404, "text/plain", F("Not found"));
    });
    server.begin();
    Serial.println(F("HTTP server ready on port 80"));

    // GRT decoder callback — periodic serial log (every 10 s)
    grt.onFrame([](const GrtFrame& f) {
        static unsigned long lastLog = 0;
        if (millis() - lastLog >= 10000) {
            lastLog = millis();
            Serial.print(F("TACH="));   Serial.print(f.tach);
            Serial.print(F(" IAS="));   Serial.print(f.airspeed);
            Serial.print(F(" ALT="));   Serial.print(f.getAltitudeFeet());
            Serial.print(F(" CHT1="));  Serial.print(f.cht[0]);
            Serial.print(F(" EGT1="));  Serial.print(f.egt[0]);
            Serial.print(F(" V="));     Serial.print(f.getVoltage(), 1);
            Serial.print(F(" OILP="));  Serial.println(f.oilPressure);
        }
    });

    Serial.println(F("Ready. Connect to WiFi and open dashboard."));
}

// ── Loop ───────────────────────────────────────────────────────

void loop() {
    if (Serial1.available()) grt.feedByte(Serial1.read());
    if (Serial.available())   Serial1.write(Serial.read());
    server.handleClient();
    yield();
}
