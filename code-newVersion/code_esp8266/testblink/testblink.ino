#include <ESP8266WiFi.h>
#include <EEPROM.h>

struct DeviceConfig {
  char ssid[32];            // SSID du réseau WiFi
  char password[32];       // Mot de passe du réseau WiFi
  bool ledState ;    // État de la LED (allumée/éteinte)
  bool motorsFBInverted ;  // Inversion des moteurs avant/arrière
  bool motorsLRInverted ;  // Inversion des moteurs gauche/droite
  unsigned int pinMotor1 ;
  unsigned int pinMotor2;
};

DeviceConfig config;  

const char* ssid = "testblink";  // Nom du réseau WiFi
const char* password = "";       // Pas de mot de passe (réseau ouvert)

const int ledPin = 8; // Broche de la LED intégrée (D4/GPIO2 pour NodeMCU)

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  EEPROM.begin(sizeof(DeviceConfig)); // Initialize EEPROM with a size of 512 bytes (adjust if needed)


  // Configuration du point d'accès WiFi
  Serial.println("Création du point d'accès WiFi...");
  WiFi.softAP(ssid, password);

  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Adresse IP du point d'accès: ");
  Serial.println(WiFi.softAPIP());

   // Erase EEPROM by writing 0xFF (empty value) to all addresses
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0xFF);
  }

  EEPROM.commit(); // Save changes to EEPROM
  Serial.println("EEPROM erased!");
}

void loop() {
  digitalWrite(ledPin, HIGH);  // Allume la LED
  delay(500);                  // Attend 500 ms
  digitalWrite(ledPin, LOW);   // Éteint la LED
  delay(500);                  // Attend 500 ms
}
