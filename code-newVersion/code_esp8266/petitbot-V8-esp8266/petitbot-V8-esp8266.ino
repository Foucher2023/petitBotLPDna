//////////////
// Petit Bot //
///////////////
// Programme pédagogique pour contrôler le robot "Petit Bot"
// Licence CC-By-Sa

// =================== Constantes =========================
static constexpr uint8_t LED_PIN = 2;          // Broche de la LED intégrée (NodeMCU)
// 8 pour esp32
// 2 pour esp8266
static constexpr uint8_t DNS_PORT = 53;       // Port DNS
static constexpr uint8_t SERVER_PORT = 80;       // Port SERVER
static constexpr const char* BASE_URL = "petitbot"; // url pour téléphone [url].local 
static constexpr uint16_t EEPROM_SIZE = 256; // Taille de l'EEPROM
static constexpr uint8_t BUFFER_SIZE = 64;
static constexpr const char* DEFAULT_SSID = "Petitbot_V8_esp8266";
static constexpr uint8_t DEFAULT_VALUE_F_MEM = 255;
static constexpr uint8_t PIN_MOTOR1 = 4;          // Broche moteur
static constexpr uint8_t PIN_MOTOR2 = 5;          // Broche moteur
// 0 et 3 pour esp32
// 4 et 5 pour esp8266


// todo completer page à propos qui explique le projet V8
//navbar on ota 


// =================== Bibliothèques =========================

//#include <WiFi.h>             // pour esp32
//#include <WebServer.h>        // pour esp32
//#include <ESP32Servo.h>       // pour esp32
//#include <HTTPUpdateServer.h> // pour esp32
//#include <ESPmDNS.h>          // pour esp32 



#include <ESP8266WiFi.h>                                  // pour ESP8266
#include <ESP8266WebServer.h>                             // pour ESP8266
#include <Servo.h>                                        // pour ESP8266
#include <ESP8266HTTPUpdateServer.h> //ota                // pour ESP8266
#include <ESP8266mDNS.h> // pour ce connecter à base_url  // pour ESP8266

// pour les deux version de code 
#include <DNSServer.h>
#include <EEPROM.h>


// =================== Fichiers Externes ======================
// Styles CSS
#include "Style_des_pages/global_style.h"
#include "Style_des_pages/navbar_style.h"
#include "Style_des_pages/config_style.h"
#include "Style_des_pages/telecommande_style.h"
#include "Style_des_pages/programmer_style.h"
#include "Style_des_pages/connectionLimit_style.h"
#include "Style_des_pages/ota_style.h"

// Pages HTML
#include "Page_HTML/navbar.h"
#include "Page_HTML/telecommande.h"
#include "Page_HTML/config.h"
#include "Page_HTML/connection_limit.h"
#include "Page_HTML/programmer.h"
#include "Page_HTML/ota.h"
#include "Page_HTML/info.h"


// Scripts JavaScript
#include "Code_JS/config_js.h"
#include "Code_JS/telecommande_js.h"
#include "Code_JS/programmer_js.h"
#include "Code_JS/connectionLimit_js.h"
#include "Code_JS/ota_js.h"

// =================== Structures & Variables Globales ===========
struct DeviceConfig {
  char ssid[32] = {0};          // Initialisation à vide
  char password[32] = {0};      // Initialisation à vide
  bool ledState = false;        // État de la LED
  bool motorsFBInverted = false; // Inversion avant/arrière
  bool motorsLRInverted = false;
  uint8_t pinMotor1 = PIN_MOTOR1;        // Broche moteur 1 (D2)
  uint8_t pinMotor2 = PIN_MOTOR2;        // Broche moteur 2 (D1)
};

DeviceConfig config;
ESP8266WebServer server(SERVER_PORT); //esp8266
//WebServer server(SERVER_PORT); //esp32
DNSServer dnsServer;
Servo servoG;
Servo servoD;

ESP8266HTTPUpdateServer httpUpdater; //esp8266
//HTTPUpdateServer httpUpdater; //esp32

// =================== Gestion des broches des moteurs ===========
// Récupère les broches des servos (inversées si nécessaire)
void getServoPins(uint8_t &servoGPin, uint8_t &servoDPin) {
  if (config.motorsFBInverted) {
    servoGPin = config.pinMotor2;
    servoDPin = config.pinMotor1;
  } else {
    servoGPin = config.pinMotor1;
    servoDPin = config.pinMotor2;
  }
}

// Met à jour les broches des moteurs (appelée UNIQUEMENT ici)
void updateMotorPins(uint8_t newPin1, uint8_t newPin2) {
  if (newPin1 == newPin2) {
    Serial.println("Erreur: Les broches des moteurs doivent être différentes !");
    return;
  }
  config.pinMotor1 = newPin1;
  config.pinMotor2 = newPin2;
  saveConfig();
  Serial.print("Broches mises à jour: Moteur1 = ");
  Serial.print(config.pinMotor1);
  Serial.print(", Moteur2 = ");
  Serial.println(config.pinMotor2);
}

// =================== Gestion de l'EEPROM ===================
void clearEEPROM() {
  Serial.println("Clearing EEPROM...");
  EEPROM.begin(EEPROM_SIZE);
  for (uint16_t i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, DEFAULT_VALUE_F_MEM);
  }
  EEPROM.commit();
  Serial.println("EEPROM cleared!");
}

void saveConfig() {
  EEPROM.put(0, config);
  EEPROM.commit();
  Serial.println("Config saved to EEPROM.");
}

void loadConfig() {
  EEPROM.begin(sizeof(DeviceConfig));
  EEPROM.get(0, config);

  // Vérifie la validité de la configuration
  config.ssid[sizeof(config.ssid) - 1] = '\0';
  config.password[sizeof(config.password) - 1] = '\0';

  if (strlen(config.ssid) == 0 || strlen(config.ssid) >= sizeof(config.ssid) ||
      config.pinMotor1 == DEFAULT_VALUE_F_MEM || config.pinMotor2 == DEFAULT_VALUE_F_MEM) {
    setDefaultConfig();
    saveConfig();
  }
  Serial.println("Config loaded from EEPROM.");
}

// =================== Configuration par défaut ==============
void setDefaultConfig() {
  clearEEPROM();
  strcpy(config.ssid, DEFAULT_SSID);
  strcpy(config.password, "");
  config.ledState = false;
  config.motorsLRInverted = false;
  config.motorsFBInverted = false;
  updateMotorPins(PIN_MOTOR1, PIN_MOTOR2); // Broches par défaut: D2 (GPIO4) et D1 (GPIO5)
  Serial.println("Default config set.");
}

// =================== Gestion des moteurs ===================
void attachServos() {
  uint8_t gPin, dPin;
  getServoPins(gPin, dPin);
  servoG.attach(gPin);
  servoD.attach(dPin);
}

// =================== Gestion du WiFi =======================
void setWifi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(config.ssid);
  IPAddress apIP = WiFi.softAPIP();
  dnsServer.start(DNS_PORT, "*", apIP);
  Serial.print("WiFi AP started. SSID: ");
  Serial.println(config.ssid);
  Serial.print("IP: ");
  Serial.println(apIP);
  MDNS.begin(BASE_URL);
}

// =================== Handlers HTTP =======================
void handleGetValueEEPROM() {
char buffer[BUFFER_SIZE];
snprintf(buffer, sizeof(buffer), "%d,%d,%d,%d,%d,%s",
         config.ledState, config.motorsFBInverted, config.motorsLRInverted,
         config.pinMotor1, config.pinMotor2, config.ssid);
server.send(200, "text/plain", buffer);
}


void handleGetStaticValue() {
  if (!server.hasArg("param")) {
    Serial.println("Erreur : Argument 'param' manquant");
    server.send(400, "text/plain", "Erreur : Argument 'param' manquant");
    return;
  }
  String wantedStaticVal = server.arg("param");
  Serial.print("Paramètre demandé : ");
  Serial.println(wantedStaticVal);

  if (wantedStaticVal == "LED_PIN") {
    server.send(200, "text/plain", String(LED_PIN));
  }
  else if (wantedStaticVal == "BASE_URL") {
    server.send(200, "text/plain", String(BASE_URL));
  }
  else {
    Serial.println("Paramètre inconnu");
    server.send(400, "text/plain", "Erreur : Paramètre inconnu");
  }
};

void handleUpdateState() {
  Serial.print("param: "); Serial.println(server.arg("param"));
  Serial.print("value: "); Serial.println(server.arg("value"));
  if (!server.hasArg("param") || !server.hasArg("value")) {
    server.send(400, "text/plain", "Bad Request: Missing param or value");
    return;
  }

  String param = server.arg("param");
  String value = server.arg("value");

  if (param == "ledState") {
    config.ledState = value.toInt();
    digitalWrite(LED_PIN, config.ledState);
  } else if (param == "motorBF") {
    config.motorsFBInverted = value.toInt();
  } else if (param == "motorLR") {
    config.motorsLRInverted = value.toInt();
  } else {
    server.send(400, "text/plain", "Invalid parameter");
    return;
  }

  saveConfig();
  server.send(200, "text/plain", "OK");
}

void handleTelecommande() {
  if (!server.hasArg("val")) {
    server.send(400, "text/plain", "Missing arg 'val'");
    return;
  }
  attachServos();
  String dir = server.arg("val");
  int g = 0;
  int d = 0;
  if (dir == "FORWARD") {
    g = 180;
    d = 0;
  } else if (dir == "REVERSE") {
    g = 0;
    d = 180;
  } else if (dir == "LEFT") {
    g = config.motorsLRInverted ? 180 : 0;
    d = config.motorsLRInverted ? 180 : 0;
  } else if (dir == "RIGHT") {
    g = config.motorsLRInverted ? 0 : 180;
    d = config.motorsLRInverted ? 0 : 180;
  } else if (dir == "STOP") {
    servoG.detach();
    servoD.detach();
    server.send(200, "text/plain", "STOP");
    return;
  } else {
    server.send(400, "text/plain", "Unknown direction");
    return;
  }

  servoG.write(g);
  servoD.write(d);
  server.send(200, "text/plain", "OK: " + dir);
}

void handleStopMotors() {
  servoG.detach();
  servoD.detach();
  server.send(200, "text/plain", "STOP");
}

void handleUpdateConfig() {
  if (server.hasArg("ssid")) {
    String newSSID = server.arg("ssid");
    strncpy(config.ssid, newSSID.c_str(), sizeof(config.ssid) - 1);
    config.ssid[sizeof(config.ssid) - 1] = '\0';
    saveConfig();
    server.send(200, "text/plain", "SSID updated");
    delay(2000);
    setWifi();
  }
  else if (server.hasArg("pinMotor1") && server.hasArg("pinMotor2")) {
    uint8_t newPin1 = server.arg("pinMotor1").toInt();
    uint8_t newPin2 = server.arg("pinMotor2").toInt();
    updateMotorPins(newPin1, newPin2); // <-- UNIQUE POINT DE MODIFICATION DES BROCHES
    server.send(200, "text/plain", "Motor pins updated");
  }
  else if (server.hasArg("reset")) {
    setDefaultConfig();
    digitalWrite(LED_PIN, config.ledState);
    setWifi();
    server.send(200, "text/plain", "Reset to default config");
  }
  else {
    server.send(400, "text/plain", "Bad Request: No valid arguments");
  }
}

void handleGetNumConnections(){
  int numConnections = WiFi.softAPgetStationNum();
  server.send(200, "text/plain", String(numConnections));
}

void handleRestartWiFi() {
  WiFi.softAPdisconnect(true);
  WiFi.disconnect(true); 
  delay(4000);
  server.stop();
  delay(5000);
  setWifi();
  server.begin();
};

// ==================== Routage des fonctions qui réponde a un appel externe ==================

void setupRoutes() {
  server.on("/get-values-EEPROM", handleGetValueEEPROM);
  server.on("/get-static-value",handleGetStaticValue);
  server.on("/update-state", handleUpdateState);
  server.on("/UseTelecommande", handleTelecommande);
  server.on("/stopMotors", handleStopMotors);
  server.on("/update-config", handleUpdateConfig);
  server.on("/getNumConnections", handleGetNumConnections);
  server.on("/restart-wifi", handleRestartWiFi);

// =================== Routage des pages =======================
    // Pages HTML
  server.on("/navbar", []() {
    server.send_P(200, "text/html", NAVBAR_HTML);
  });


  server.on("/telecommande", []() {
    if (WiFi.softAPgetStationNum() > 1) {
      server.send_P(200, "text/html", CONNECTION_LIMIT_PAGE);
    } else {
      server.send_P(200, "text/html", TELECOMMANDE_PAGE);
    }
  });

  server.on("/config", []() {
    if (WiFi.softAPgetStationNum() > 1) {
      server.send_P(200, "text/html", CONNECTION_LIMIT_PAGE);
    } else {
      server.send_P(200, "text/html", CONFIG_PAGE);
    }
  });

    server.on("/info", []() {
    if (WiFi.softAPgetStationNum() > 1 ) {
      server.send_P(200, "text/html", CONNECTION_LIMIT_PAGE);
    } else {
      server.send_P(200, "text/html", INFO_PAGE);
    }
  });

  server.on("/programmer", []() {
    if (WiFi.softAPgetStationNum() > 1) {
      server.send_P(200, "text/html", CONNECTION_LIMIT_PAGE);
    } else {
      server.send_P(200, "text/html", PROGRAMMER_PAGE);
    }
  });

  server.on("/connection-limit", []() {
      server.send_P(200, "text/html", CONNECTION_LIMIT_PAGE);
  });

  server.on("/ota", []() {
  // Only allow access from 192.168.4.1 or BASE URL  
  char urlHost[50];
  strcpy(urlHost, BASE_URL);
  strcat(urlHost, ".local");
  if (server.hostHeader() == "192.168.4.1" || server.hostHeader() == urlHost) {
    server.send_P(200, "text/html", OTA_PAGE);
  } else {
    char explainAccess [150];
    strcpy(explainAccess, "Acces refuser : la page de mise a jour (OTA) est seulement accessible via l'adresse 192.168.4.1 ou " );
    strcat(explainAccess, urlHost);
    server.send(403, "text/plain", explainAccess );
  }
});

  // Fichiers CSS
  server.on("/global.css", []()           { server.send_P(200, "text/css", GLOBAL_CSS); });
  server.on("/navbar.css", []()           { server.send_P(200, "text/css", NAVBAR_CSS); });
  server.on("/config.css", []()           { server.send_P(200, "text/css", CONFIG_CSS); });
  server.on("/telecommande.css", []()     { server.send_P(200, "text/css", TELECOMMANDE_CSS); });
  server.on("/programmer.css", []()       { server.send_P(200, "text/css", PROGRAMMER_CSS); });
  server.on("/connectionLimit.css", []()  { server.send_P(200, "text/css", CONNECT_LIMIT_CSS); });
  server.on("/ota.css", []()              { server.send_P(200, "text/css", OTA_CSS); });

  // Fichiers statiques JS
  server.on("/telecommande.js", []()    { server.send_P(200, "text/javascript", TELECOMMANDE_JS); });
  server.on("/config.js", []()          { server.send_P(200, "text/javascript", CONFIG_JS); });
  server.on("/programmer.js", []()      { server.send_P(200, "text/javascript", PROGRAMMER_JS); });
  server.on("/connectionLimit.js", []() { server.send_P(200, "text/javascript", CONNECT_LIMIT_JS); });
  server.on("/ota.js", []()             { server.send_P(200, "text/javascript", OTA_JS); });

  // Gestion des requêtes non trouvées
  server.onNotFound([]() {
    if (WiFi.softAPgetStationNum() > 1) {
      server.send_P(200, "text/html", CONNECTION_LIMIT_PAGE);
    } else {
      server.send_P(200, "text/html", TELECOMMANDE_PAGE);
    }
  });
}

void setupSerial() {
  Serial.begin(115200);
  Serial.println("\n=== PETIT BOT START ===");
}

void setupConfig() {
  loadConfig();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, config.ledState);
}

// =============================== INITIALISATION ======================================
void setup() {
  setupSerial();
  setupConfig();
  setWifi();
  setupRoutes();
  server.begin();
  httpUpdater.setup(&server, "/update", "admin", "admin"); //change the credencials 

  Serial.println("Web server started");
  Serial.print("SSID: "); Serial.println(config.ssid);
  Serial.print("LED: "); Serial.println(config.ledState ? "ON" : "OFF");
  Serial.print("Motors FB Inverted: "); Serial.println(config.motorsFBInverted ? "YES" : "NO");
  Serial.print("Motors LR Inverted: "); Serial.println(config.motorsLRInverted ? "YES" : "NO");
  Serial.print("Motor Pins: "); Serial.print(config.pinMotor1); Serial.print(", "); Serial.println(config.pinMotor2);
}

// ================================= Boucle principale =====================================
void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}