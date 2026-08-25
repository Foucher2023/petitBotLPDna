#ifndef TELECOMMANDE_H
#define TELECOMMANDE_H

const char TELECOMMANDE_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <title>PetitBot - Télécommande</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/global.css">
  <link rel="stylesheet" href="/telecommande.css">
</head>
<body>
<div id="navbar-placeholder"></div>
  <main>
    <h1 id="titleRemote">Télécommande PetitBot</h1>

    <!-- ============= les différents Bouttons de Contrôles du ROBOT ============= -->

    <div class="dpad">
    <!-- Boutton AVANCER -->
      <button class="btnDir" id="forwardBtn" data-action="FORWARD" aria-label="Avancer">
        <span class="bigArrow">▲</span>
      </button>

      <div class="middle">
        <!-- Boutton GAUCHE -->
        <button class="btnDir" id="leftBtn" data-action="LEFT" aria-label="Tourner à gauche">
          <span class="bigArrow">◀</span>
        </button>

        <!-- Boutton DROITE -->
        <button class="btnDir" id="rightBtn" data-action="RIGHT" aria-label="Tourner à droite">
          <span class="bigArrow">▶</span>
        </button>
      </div>

      <!-- Boutton RECULER -->
      <button class="btnDir" id="reverseBtn" data-action="REVERSE" aria-label="Reculer">
        <span class="bigArrow">▼</span>
      </button>
    </div>

    <!-- Boutton STOP -->
    <div class="stop-container">
      <button class="btnStop" id="stopBtn" data-action="STOP" aria-label="Arrêt complet">
        STOP
      </button>
    </div>
  </main>

  <script src="/telecommande.js"></script>
</body>
</html>
)rawliteral";
#endif