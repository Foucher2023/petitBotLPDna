#ifndef CONFIG_H
#define CONFIG_H

const char CONFIG_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <title>PetitBot - Configuration</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/navbar.css">
  <link rel="stylesheet" href="/global.css">
  <link rel="stylesheet" href="/config.css">
</head>
<body>
  <nav class="navbar navbar-inverse">
    <div class="container-fluid">
      <div class="navbar-header">
        <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#myNavbar">
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>
        <a class="navbar-brand" href="index.html">PetitBot</a>
      </div>
      <div class="collapse navbar-collapse" id="myNavbar">
        <ul class="nav navbar-nav">
          <li class="active"><a href="/config">Configuration</a></li>
          <li><a href="telecommande.html">Télécommande</a></li>
          <li><a href="/programmer">Programmer</a></li>
        </ul>
      </div>
    </div>
  </nav>

  <main>
    <h1>Configuration du PetitBot</h1>

    <!-- État de la LED -->
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

    <!-- Inversion des moteurs avant/arrière -->
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

    <!-- Inversion des moteurs gauche/droite -->
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

    <!-- Broches des moteurs -->
    <section class="config-section">
      <h2>Broches des moteurs</h2>
      <p id="statusPinMotorSet"></p>
      <div class="pin-selectors">
        <div class="pin-selector">
          <label for="pinMotor1">Broche Moteur 1 :</label>
          <select id="pinMotor1" data-action="pin-select">
              <option value="">pas de modification</option>
              <option value="A0">0 (GPIO0)</option>
              <option value="A1">1 (GPIO1)</option>
              <option value="A3">3 (GPIO3)</option>
              <option value="A4">4 (GPIO4)</option>
              <option value="A5">5 (GPIO5)</option>
              <option value="D6">6 (GPIO6)</option>
              <option value="D7">7 (GPIO7)</option>
              <option value="D10">10 (GPIO10)</option>
              <option value="D20">20 (GPIO20)</option>
              <option value="D21">21 (GPIO21)</option>
          </select>
        </div>
        <div class="pin-selector">
          <label for="pinMotor2">Broche Moteur 2 :</label>
          <select id="pinMotor2" data-action="pin-select">
              <option value="">pas de modification</option>
              <option value="A0">0 (GPIO0)</option>
              <option value="A1">1 (GPIO1)</option>
              <option value="A3">3 (GPIO3)</option>
              <option value="A4">4 (GPIO4)</option>
              <option value="A5">5 (GPIO5)</option>
              <option value="D6">6 (GPIO6)</option>
              <option value="D7">7 (GPIO7)</option>
              <option value="D10">10 (GPIO10)</option>
              <option value="D20">20 (GPIO20)</option>
              <option value="D21">21 (GPIO21)</option>
          </select>
        </div>
      </div>
      <button id="updateButton" class="button" data-action="update-pins">Changer les broches</button>
      <p id="statusGPIO"></p>
    </section>

    <!-- SSID -->
    <section class="config-section">
      <h2>Nom du réseau (SSID)</h2>
      <div class="ssid-input">
        <input type="text" id="ssidInput" placeholder="Nouveau nom" maxlength="30" aria-label="Nom du réseau">
        <button class="button" data-action="update-ssid">Valider</button>
      </div>
      <p id="statusSSID"></p>
    </section>

    <!-- Réinitialisation -->
    <section class="config-section">
      <button class="button" id="reset-btn" data-action="reset">Réinitialiser la configuration</button>
    </section>
  </main>

  <script src="/config.js"></script>
</body>
</html>
)rawliteral";
#endif
