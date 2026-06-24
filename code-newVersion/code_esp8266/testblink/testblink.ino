#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>  // Required for OTA
#include <EEPROM.h>

struct DeviceConfig {
  char ssid[32] = "testblink";     // Default SSID
  char password[32] = "";         // Default password (empty)
  bool ledState = false;
  bool motorsFBInverted = false;
  bool motorsLRInverted = false;
  unsigned int pinMotor1 = 4;    // D2
  unsigned int pinMotor2 = 5;    // D1
};

DeviceConfig config;
ESP8266WebServer server(80);      // Web server for OTA
ESP8266HTTPUpdateServer httpUpdater;  // OTA updater

const int ledPin = LED_BUILTIN;   // Use built-in LED (D4/GPIO2 on NodeMCU)

void setup() {
  Serial.begin(115200);
  delay(100);  // Give time for serial to initialize

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Initialize EEPROM
  EEPROM.begin(sizeof(DeviceConfig));

  // Clear EEPROM (only once, not in loop!)
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0xFF);
  }
  EEPROM.commit();
  Serial.println("EEPROM erased!");

  // Start WiFi in AP mode
  Serial.println("Creating WiFi access point...");
  WiFi.softAP(config.ssid, config.password);

  Serial.print("SSID: ");
  Serial.println(config.ssid);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  // Setup OTA
  httpUpdater.setup(&server, "/update", "admin", "admin");  // Change credentials!

  // Start server
  server.begin();

  // Add OTA page route
  server.on("/ota", []() {
    if (server.client().remoteIP().toString() == "192.168.4.2" ) {
      server.send(200, "text/html",
        "<form method='POST' action='/update' enctype='multipart/form-data'>"
        "<input type='file' name='file' accept='.bin'>"
        "<button type='submit'>Update</button>"
        "</form>");
    } else {
        String response = "Access denied. Client IP: " + server.client().remoteIP().toString() +", AP IP: " + WiFi.softAPIP().toString();
        server.send(403, "text/plain", response);      
    }
  });
}

void loop() {
  server.handleClient();  // ✅ CRITICAL for OTA to work!

  // Your existing LED blink code
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}