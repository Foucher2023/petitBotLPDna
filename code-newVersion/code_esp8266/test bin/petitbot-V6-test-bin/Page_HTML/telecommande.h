#ifndef TELECOMMANDE_H
#define TELECOMMANDE_H

const char TELECOMMANDE_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <title>PetitBot - Télécommande</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/navbar.css">
  <link rel="stylesheet" href="/global.css">
  <link rel="stylesheet" href="/telecommande.css">
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
          <li><a href="/config">Configuration</a></li>
          <li class="active"><a href="telecommande.html">Télécommande</a></li>
          <li><a href="/programmer">Programmer</a></li>
        </ul>
      </div>
    </div>
  </nav>

  <main>
    <h1 id="titleRemote">Télécommande PetitBot</h1>

    <div class="dpad">
      <button class="btnDir" id="forwardBtn" data-action="FORWARD" aria-label="Avancer">
        <span class="bigArrow">▲</span>
      </button>

      <div class="middle">
        <button class="btnDir" id="leftBtn" data-action="LEFT" aria-label="Tourner à gauche">
          <span class="bigArrow">◀</span>
        </button>
        <button class="btnDir" id="rightBtn" data-action="RIGHT" aria-label="Tourner à droite">
          <span class="bigArrow">▶</span>
        </button>
      </div>

      <button class="btnDir" id="reverseBtn" data-action="REVERSE" aria-label="Reculer">
        <span class="bigArrow">▼</span>
      </button>
    </div>

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
