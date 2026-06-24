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

void setup() {
  Serial.begin(115200);
  EEPROM.begin(sizeof(DeviceConfig)); // Initialize EEPROM with a size of 512 bytes (adjust if needed)

  // Erase EEPROM by writing 0xFF (empty value) to all addresses
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0xFF);
  }

  EEPROM.commit(); // Save changes to EEPROM
  Serial.println("EEPROM erased!");
}

void loop() {
  // Nothing to do here
}