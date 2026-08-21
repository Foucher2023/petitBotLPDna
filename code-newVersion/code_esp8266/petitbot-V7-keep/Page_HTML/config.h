#ifndef CONFIG_H
#define CONFIG_H

const char CONFIG_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <title>PetitBot - Configuration</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/global.css">
  <link rel="stylesheet" href="/config.css">
</head>
<body>
  <div id="navbar-placeholder"></div>
  <main>
    <h1>Configuration du PetitBot</h1>

    <!-- ======== État de la LED ======== -->
    
    <section class="config-section">
      <h2>État de la LED</h2>
      <form class="config-form">
        <fieldset>
          <label class="button" data-action="led" data-value="ON" id="btnLed-ON">
  <input type="radio" name="stateLed" value="ON">
  ON
</label>
<label class="button" data-action="led" data-value="OFF" id="btnLed-OFF">
  <input type="radio" name="stateLed" value="OFF">
  OFF
</label>
        </fieldset>
      </form>
    </section>

    <!-- ======== Inversion des moteurs avant/arrière ======== -->

    <section class="config-section">
      <h2>Moteurs inversés avant/arrière</h2>
      <form class="config-form">
        <fieldset>
          <label class="button" data-action="motorBF" data-value="YES" id="btnMotorBF-YES">
  <input type="radio" name="stateMotor" value="YES">
  OUI
</label>
<label class="button" data-action="motorBF" data-value="NO" id="btnMotorBF-NO">
  <input type="radio" name="stateMotor" value="NO">
  NON
</label>
        </fieldset>
      </form>
    </section>

    <!-- ======== Inversion des moteurs gauche/droite ======== -->

    <section class="config-section">
      <h2>Moteurs inversés gauche/droite</h2>
      <form class="config-form">
        <fieldset>
          <label class="button" data-action="motorLR" data-value="YES" id="btnMotorLR-YES">
  <input type="radio" name="stateMotorLeftRight" value="YES">
  OUI
</label>
<label class="button" data-action="motorLR" data-value="NO" id="btnMotorLR-NO">
  <input type="radio" name="stateMotorLeftRight" value="NO">
  NON
</label>
        </fieldset>
      </form>
    </section>

    <!-- ======== Broches des moteurs ======== -->  

    <section class="config-section">
      <h2>Broches des moteurs</h2>
      <p id="statusPinMotorSet"></p>
      <div class="pin-selectors">
        <div class="pin-selector">
          <label for="pinMotor1">Broche Moteur 1 :</label>
          <select id="pinMotor1" data-action="pin-select">
            <option value="">Pas de modification</option>
            <option value="D1">D1 (GPIO5)</option>
            <option value="D2">D2 (GPIO4)</option>
            <option value="D3">D3 (GPIO0)</option>
            <option value="D4">D4 (GPIO2)</option>
            <option value="D5">D5 (GPIO14)</option>
            <option value="D6">D6 (GPIO12)</option>
            <option value="D7">D7 (GPIO13)</option>
            <option value="D8">D8 (GPIO15)</option>
          </select>
        </div>
        <div class="pin-selector">
          <label for="pinMotor2">Broche Moteur 2 :</label>
          <select id="pinMotor2" data-action="pin-select">
            <option value="">Pas de modification</option>
            <option value="D1">D1 (GPIO5)</option>
            <option value="D2">D2 (GPIO4)</option>
            <option value="D3">D3 (GPIO0)</option>
            <option value="D4">D4 (GPIO2)</option>
            <option value="D5">D5 (GPIO14)</option>
            <option value="D6">D6 (GPIO12)</option>
            <option value="D7">D7 (GPIO13)</option>
            <option value="D8">D8 (GPIO15)</option>
          </select>
        </div>
      </div>
      <button id="updateButton" class="button" data-action="update-pins">Changer les broches</button>
      <p id="statusGPIO"></p>
    </section>
    <!-- ======== SSID ======== -->
    

    <section class="config-section">
      <h2>Nouveau nom du réseau (SSID)</h2>
      <div class="ssid-input">
        <input type="text" id="ssidInput" placeholder="Nouveau nom" maxlength="30" aria-label="Nom du réseau">
        <button class="button" data-action="update-ssid">Valider</button>
      </div>
      <p id="statusSSID"></p>
    </section>

    <!-- ======== Réinitialisation ======== -->

    <section class="config-section">
      <h2>Réinitialisation du petit-bot aux paramètres de départ</h2>
      <button class="button" id="reset-btn" data-action="reset">Réinitialiser la configuration</button>
    </section>

     <!-- ======== OTA ======== --> 

    <div class="ota-update-button-container" id="otaButtonContainer" style="display: none;">
      <section class="config-section">
        <h2>Mettre a jour le logiciel du petit-bot</h2>
        <a href="/ota" class="ota-update-button">Mise à jour</a>
      </section>
    </div>
  </main>

  <script src="/config.js"></script>
</body>
</html>
)rawliteral";
#endif
