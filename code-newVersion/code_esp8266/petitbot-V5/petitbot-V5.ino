//////////////
// Petit Bot //
///////////////
// Un programme pedagogique des petits debrouillards ?=+ pour gerer le robot "Petit Bot"
// Voir sur http://wikidebrouillard.org/index.php?title=Petit_Bot_un_robot_controlable_en_Wifi
// Ce programme est inspire de : http://www.esp8266.com/viewtopic.php?f=29&t=6419#sthash.gd1tJhwU.dpuf
// Sous licence CC-By-Sa
// Crée et utilisé par des gens bien

// =================== Bibliothèques =========================
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <Servo.h>

// =================== Fichiers Externe ======================
#include "Style_des_pages/global_style.h"
#include "Style_des_pages/navbar_style.h"
#include "Style_des_pages/config_style.h"
#include "Style_des_pages/telecommande_style.h"
#include "Style_des_pages/programmer_style.h"
#include "Style_des_pages/connectionLimit_style.h"


#include "Page_HTML/telecommande.h"
#include "Page_HTML/config.h"
#include "Page_HTML/connection_limit.h"
#include "Page_HTML/loading.h"
#include "Page_HTML/programmer.h"

#include "code_JS/config_js.h"
#include "code_JS/telecommande_js.h"
#include "code_JS/programmer_js.h"
#include "code_JS/connectionLimit_js.h"

// =================== Structures & Variables Globales =======
struct DeviceConfig {
  char ssid[32];          // SSID du réseau WiFi
  char password[32];      // Mot de passe du réseau WiFi
  bool ledState;          // État de la LED (allumée/éteinte)
  bool motorsFBInverted;  // Inversion des moteurs avant/arrière
  bool motorsLRInverted;  // Inversion des moteurs gauche/droite
  unsigned int pinMotor1;
  unsigned int pinMotor2;
};

DeviceConfig config;  // Instance de la configuration

// =================== Broches & Objets ======================
ESP8266WebServer server(80);  // Serveur web sur le port 80
DNSServer dnsServer;          // Serveur DNS pour le portail captif
const byte DNS_PORT = 53;     // Port DNS

#define LED_PIN 2  // Broche de la LED intégrée (NodeMCU)

int pin_servoG;  // Broche du servo gauche
int pin_servoD;  // Broche du servo droit
Servo servoG;    // Objet Servo pour le servo gauche
Servo servoD;    // Objet Servo pour le servo droit

bool isRedirectToLimitConnect = false;

// =================== Gestion de l'EEPROM ===================
// Efface entièrement l'EEPROM
void clearEEPROM() {
  Serial.println("Clearing EEPROM...");
  for (unsigned int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 255);  // Écrit 255 dans chaque octet
  }
  EEPROM.commit();  // Sauvegarde les modifications
  Serial.println("EEPROM cleared!");
}

// Sauvegarde la configuration dans l'EEPROM
void saveConfig() {
  EEPROM.begin(sizeof(DeviceConfig));
  EEPROM.put(0, config);  // Sauvegarde la structure config
  EEPROM.commit();        // Valide les modifications
  Serial.println("Config saved to EEPROM.");
}

// Charge la configuration depuis l'EEPROM
void loadConfig() {
  EEPROM.begin(sizeof(DeviceConfig));
  EEPROM.get(0, config);   // Récupère la structure config
  config.ssid[31] = '\0';  // Assure la terminaison nulle
  config.password[31] = '\0';
  if (config.pinMotor1 == 4294967295 || config.pinMotor2 == 4294967295) {
    config.pinMotor1 = 4;  // Valeur par défaut pour pinMotor1 (D2)
    config.pinMotor2 = 5;  // Valeur par défaut pour pinMotor2 (D1)
  }
  pin_servoG = config.pinMotor1;
  pin_servoD = config.pinMotor2;
  reverseMotorFB();
  Serial.println("Config loaded from EEPROM.");
}

// =================== Configuration par défaut ==============
// Réinitialise la configuration aux valeurs par défaut
void setDefaultConfig() {
  clearEEPROM();
  //petitbot V5.3
    //D1 (GPIO5)
    //D2 (GPIO4)
    //D3 (GPIO0)
    //D4 (GPIO2)
    //D5 (GPIO14)
    //D6 (GPIO12)
    //D7 (GPIO13)
    //D8 (GPIO15)
  strcpy(config.ssid, "Petitbot N°jojo");  // SSID par défaut
  strcpy(config.password, "");          // Pas de mot de passe
  config.ledState = false;              // LED éteinte
  config.motorsLRInverted = false;      // Pas d'inversion gauche/droite
  config.motorsFBInverted = true;      // Pas d'inversion avant/arrière
  config.pinMotor1 = 4;
  config.pinMotor2 = 5;
  pin_servoG = config.pinMotor1;
  pin_servoD = config.pinMotor2;
  Serial.println("Default config set.");
}

// =================== Gestion des moteurs ===================
// Inverse les broches des moteurs avant/arrière si nécessaire
void reverseMotorFB() {
  if (config.motorsFBInverted) {
    pin_servoG = config.pinMotor2;
    pin_servoD = config.pinMotor1;
  } else {
    pin_servoG = config.pinMotor1;
    pin_servoD = config.pinMotor2;
  }
}

// =================== Gestion du WiFi =======================
// Configure le WiFi en mode Point d'Accès (AP)
void setWifi() {
  WiFi.disconnect(true);                        // Déconnecte toute connexion existante
  WiFi.mode(WIFI_OFF);                          // Désactive le WiFi
  WiFi.mode(WIFI_AP);                           // Active le mode AP
  WiFi.softAP(config.ssid);  // Crée un réseau avec le SSID configuré
  IPAddress apIP = WiFi.softAPIP();             // Récupère l'IP du point d'accès
  dnsServer.start(DNS_PORT, "*", apIP);         // Démarre le serveur DNS
}

// =================== Gestion des requêtes HTTP =============
// Renvoie l'état actuel (LED, inversion des moteurs)
void handleGetValueEEPROM() {
  String state = String(config.ledState) + "," + String(config.motorsFBInverted) + "," + String(config.motorsLRInverted) + "," + String(config.pinMotor1) + "," + String(config.pinMotor2) + "," + String(config.ssid);
  server.send(200, "text/plain", state);
}

// Met à jour un paramètre spécifique (LED ou inversion des moteurs)
void handleUpdateState() {
  if (server.hasArg("param") && server.hasArg("value")) {
    String param = server.arg("param");
    String value = server.arg("value");

    if (param == "ledState") {
      config.ledState = value.toInt();
      digitalWrite(LED_PIN, config.ledState);  // Met à jour la LED
    } else if (param == "motorBF") {
      Serial.println("Config is set:");
      Serial.println(value);
      Serial.println("======");
      config.motorsFBInverted = value.toInt();
    } else if (param == "motorLR") {
      config.motorsLRInverted = value.toInt();
    } else {
      server.send(400, "text/plain", "Paramètre invalide");
      return;
    }
    saveConfig();  // Sauvegarde la configuration
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

// =================== Gestion de la télécommande =============
// Contrôle les moteurs en fonction de la direction
void handleTelecommande() {
  if (!server.hasArg("val")) {
    server.send(400, "text/plain", "Missing arg 'val'");
    return;
  }

  // Attache les servos aux bonnes broches
  if (config.motorsFBInverted) {
    servoD.attach(pin_servoG);
    servoG.attach(pin_servoD);
  } else {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);
  }

  String dir = server.arg("val");
  int g = 0;  // Valeur pour le servo gauche
  int d = 0;  // Valeur pour le servo droit

  // Détermine les valeurs en fonction de la direction
  if (dir == "FORWARD") {
    g = 180;
    d = 0;
  } else if (dir == "REVERSE") {
    g = 0;
    d = 180;
  } else if (dir == "LEFT") {
    g = 0;
    d = 0;
    if (config.motorsLRInverted) {
      g = 180;
      d = 180;
    }
  } else if (dir == "RIGHT") {
    g = 180;
    d = 180;
    if (config.motorsLRInverted) {
      g = 0;
      d = 0;
    }
  } else {
    server.send(400, "text/plain", "Unknown direction");
    return;
  }

  // Applique les valeurs aux servos
  servoG.write(g);
  servoD.write(d);
  server.send(200, "text/plain", "OK: " + dir);
}

// =================== Arrêt des moteurs =====================
// Détache les servos pour les arrêter
void handleStopMotors() {
  if (config.motorsFBInverted) {
    servoD.attach(pin_servoG);
    servoG.attach(pin_servoD);
  } else {
    servoG.attach(pin_servoG);
    servoD.attach(pin_servoD);
  }
  servoG.detach();
  servoD.detach();
  server.send(200, "text/plain", "STOP");
}

// =================== Mise à jour de la configuration ========
// Met à jour le SSID ou réinitialise la configuration
void handleUpdateConfig() {
  if (server.hasArg("ssid")) {
    String newSSID = server.arg("ssid");
    strncpy(config.ssid, newSSID.c_str(), sizeof(config.ssid) - 1);
    config.ssid[sizeof(config.ssid) - 1] = '\0';  // Assure la terminaison nulle
    saveConfig();
    server.send(200, "text/plain", "SSID updated");
    delay(2000);  // Attend 2 secondes
    setWifi();    // Redémarre le WiFi avec le nouveau SSID
  } else if (server.hasArg("pinMotor1") && server.hasArg("pinMotor2")) {
    config.pinMotor1 = server.arg("pinMotor1").toInt();
    config.pinMotor2 = server.arg("pinMotor2").toInt();
    pin_servoG = config.pinMotor1;
    pin_servoD = config.pinMotor2;
    saveConfig();
    reverseMotorFB();  // Met à jour l'inversion si nécessaire
    server.send(200, "text/plain", "Motor pins updated");
  } else if (server.hasArg("reset")) {
    setDefaultConfig();
    digitalWrite(LED_PIN, config.ledState);
    reverseMotorFB();
    setWifi();
    server.send(200, "text/plain", "Reset to default config");
  } else if (server.args() == 0) {
    server.send(400, "text/plain", "No arguments provided");
  } else {
    server.send(200, "text/plain", "OK");
  }
}

// =================== Redémarrage du WiFi =====================
void handleRestartWiFi() {

  // Déconnecte tous les appareils
  WiFi.softAPdisconnect(false);
  isRedirectToLimitConnect = true;
  // Attend un court instant
  delay(2000);
  // Redémarre le point d'accès
  setWifi();
  server.send(200, "text/plain", "WiFi restarted");
}


// =================== Configuration du serveur web ==========
void setup() {
  Serial.begin(115200);
  Serial.println("\n=== NOUVEAU DÉBUT ===");

  loadConfig();  // Charge la configuration depuis l'EEPROM

  // Vérifie si la configuration est valide
  if (strlen(config.ssid) == 0 || strlen(config.ssid) == 31 || strlen(config.ssid) >= sizeof(config.ssid) || config.pinMotor1 == 4294967295 || config.pinMotor2 == 4294967295) {
    setDefaultConfig();
    saveConfig();
  } else {
    Serial.println("Config is set:");
    Serial.print("LED state: ");
    Serial.println(config.ledState ? "OFF" : "ON");
    Serial.print("SSID: ");
    Serial.println(config.ssid);
    Serial.print("Motors FB inverted: ");
    Serial.println(config.motorsFBInverted);
    Serial.print("Motors LR inverted: ");
    Serial.println(config.motorsLRInverted);
    Serial.print("Motor Left pin: ");
    Serial.println(config.pinMotor1);
    Serial.print("Motor Right pin: ");
    Serial.println(config.pinMotor2);
  }

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, config.ledState);
  reverseMotorFB();
  setWifi();

  // =================== Routage des requêtes =================
  server.on("/get-values-EEPROM", handleGetValueEEPROM);
  server.on("/update-state", handleUpdateState);
  server.on("/UseTelecommande", handleTelecommande);
  server.on("/stopMotors", handleStopMotors);
  server.on("/update-config", handleUpdateConfig);
  server.on("/restart-wifi", handleRestartWiFi);

  // =================== Pages web ===========================
  server.onNotFound([]() {
    if (WiFi.softAPgetStationNum() > 1) {
      // Redirect the client
      if (isRedirectToLimitConnect == false) {
        server.send_P(200, "text/html", CONNECTION_LIMIT_PAGE);
      }
    } else {
    server.sendHeader("Location", "http://192.168.4.1/telecommande");
    server.send(302, "text/plain", "");
    }
  });

  server.on("/telecommande", []() {
    if (WiFi.softAPgetStationNum() > 1) {
      // Redirect the client
      if (isRedirectToLimitConnect == false) {
        server.send_P(200, "text/html", CONNECTION_LIMIT_PAGE);
      }
    } else {
      server.send_P(200, "text/html", TELECOMMANDE_PAGE);
    }
  });

  server.on("/config", []() {
    if (WiFi.softAPgetStationNum() > 1) {
      // Redirect the client
      if (isRedirectToLimitConnect == false) {
        server.send_P(200, "text/html", CONNECTION_LIMIT_PAGE);
      }
    } else {
      server.send_P(200, "text/html", CONFIG_PAGE);
    }
  });

  server.on("/loading", []() {
    server.send(200, "text/html", LOADING_PAGE);
  });

  server.on("/connection-limit", []() {
    server.send(200, "text/html", CONNECTION_LIMIT_PAGE);
  });

  server.on("/programmer", []() {
    if (WiFi.softAPgetStationNum() > 1) {
      // Redirect the client
      if (isRedirectToLimitConnect == false) {
        server.send_P(200, "text/html", CONNECTION_LIMIT_PAGE);
      }
    } else {
      server.send_P(200, "text/html", PROGRAMMER_PAGE);
    }
  });

  // =================== Fichiers statiques CSS ===================
  server.on("/global.css", []() {
    server.send_P(200, "text/css", GLOBAL_CSS);
  });

  server.on("/navbar.css", []() {
    server.send_P(200, "text/css", NAVBAR_CSS);
  });

  server.on("/config.css", []() {
    server.send_P(200, "text/css", CONFIG_CSS);
  });

  server.on("/telecommande.css", []() {
    server.send_P(200, "text/css", TELECOMMANDE_CSS);
  });

  server.on("/programmer.css", []() {
    server.send_P(200, "text/css", PROGRAMMER_CSS);
  });

  server.on("/connectionLimit.css", []() {
    server.send_P(200, "text/css", CONNECT_LIMIT_CSS);
  });
  // =================== Fichiers statiques JS ===================
  server.on("/config.js", []() {
    server.send_P(200, "text/javascript", CONFIG_JS);
  });

  server.on("/telecommande.js", HTTP_GET, []() {
    server.send_P(200, "text/javascript", TELECOMMANDE_JS);
  });

  server.on("/programmer.js", []() {
    server.send_P(200, "text/javascript", PROGRAMMER_JS);
  });

  server.on("/connectionLimit.js", []() {
    server.send_P(200, "text/javascript", CONNECT_LIMIT_JS);
  });
  server.begin();
  Serial.println("Web server started");
}



// =================== Boucle principale =====================
void loop() {
  dnsServer.processNextRequest();  // Traite les requêtes DNS
  server.handleClient();           // Traite les requêtes HTTP
}
