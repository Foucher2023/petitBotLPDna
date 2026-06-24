#ifndef CONFIG_H
#define CONFIG_H

const char CONFIG_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>petitbot Config</title>
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
  <h1>État de la LED</h1>
  <form>
    <label class="button" id="btnLed-on"><input type="radio" name="stateLed" onchange="controlLed('ON')">ON</label>
    <label class="button" id="btnLed-off"><input type="radio" name="stateLed" onchange="controlLed('OFF')">OFF</label>
  </form>

  <h1>Moteurs inversés avant/arrière</h1>
  <form>
    <label class="button" id="btnMotor-invert-yes"><input type="radio" name="stateMotor" onchange="motorInvertedFrontBack('YES')">OUI</label>
    <label class="button" id="btnMotor-invert-no"><input type="radio" name="stateMotor" onchange="motorInvertedFrontBack('NO')">NON</label>
  </form>

  <h1>Moteurs inversés gauche/droite</h1>
  <form>
    <label class="button" id="btnMotor-invert-left-yes"><input type="radio" name="stateMotorLeftRight" onchange="motorInvertedLeftRight('YES')">OUI</label>
    <label class="button" id="btnMotor-invert-left-no"><input type="radio" name="stateMotorLeftRight" onchange="motorInvertedLeftRight('NO')">NON</label>
  </form>


  <h1>Changer les pin des moteurs </h1>

<p id="statusPinMotorSet"></p>

<label for="pinMotor1">Broche Moteur 1 :</label>
<select id="pinMotor1">
    <option value="">pas de modification</option>
    <option value="D1">D1 (GPIO5)</option>
    <option value="D2">D2 (GPIO4)</option>
    <option value="D3">D3 (GPIO0)</option>
    <option value="D4">D4 (GPIO2)</option>
    <option value="D5">D5 (GPIO14)</option>
    <option value="D6">D6 (GPIO12)</option>
    <option value="D7">D7 (GPIO13)</option>
    <option value="D8">D8 (GPIO15)</option>
</select><br>

<label for="pinMotor2">Broche Moteur 2 :</label>
<select id="pinMotor2">
    <option value="">pas de modification</option>
    <option value="D1">D1 (GPIO5)</option>
    <option value="D2">D2 (GPIO4)</option>
    <option value="D3">D3 (GPIO0)</option>
    <option value="D4">D4 (GPIO2)</option>
    <option value="D5">D5 (GPIO14)</option>
    <option value="D6">D6 (GPIO12)</option>
    <option value="D7">D7 (GPIO13)</option>
    <option value="D8">D8 (GPIO15)</option>
</select>
<br>
<button id="updateButton" class="button" onclick="updatePinMotor()">Changer les Pins</button>
<p id="statusGPIO"></p>
  <h1>Changer le SSID du PetitBot</h1>
  <input type="text" id="ssidInput" placeholder="Nouveau nom" maxlength="30">
  <button class="button" onclick="updateSSID()">Valider</button>
  <p id="statusSSID"></p>

  <label class="button" id="reset-btn" onclick="resetConfig()">Réinitialiser</label>

<script src="/config.js"></script>
</body>
</html>
)rawliteral";

#endif
