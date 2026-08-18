#ifndef PROGRAMMER_H
#define PROGRAMMER_H

const char PROGRAMMER_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <title>Programmer Petit Bot</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/global.css">
  <link rel="stylesheet" href="/programmer.css">
</head>
<body>
<div id="navbar-placeholder"></div>
  <main class="container">
    <h1>Programmer Petit Bot</h1>

    <!-- Panneau des blocs disponibles -->
    <section class="blocks-panel">
      <h2 class="program-title">Blocs disponibles</h2>
      <div class="blocks-grid">
        <!-- Bloc Avancer -->
        <div class="block-container">
          <div class="block" draggable="true" data-block-type="FORWARD" id="forward-block">
            <strong>Avancer</strong>
          </div>
          <button class="place-btn" data-action="place" data-block-id="forward-block">Placer</button>
          <p class="infoInBlack" id="NumbForward"></p>
        </div>

        <!-- Bloc Reculer -->
        <div class="block-container">
          <div class="block" draggable="true" data-block-type="REVERSE" id="reverse-block">
            <strong>Reculer</strong>
          </div>
          <button class="place-btn" data-action="place" data-block-id="reverse-block">Placer</button>
          <p class="infoInBlack" id="NumbReverse"></p>
        </div>

        <!-- Bloc Gauche -->
        <div class="block-container">
          <div class="block" draggable="true" data-block-type="LEFT" id="left-block">
            <strong>Gauche</strong>
          </div>
          <button class="place-btn" data-action="place" data-block-id="left-block">Placer</button>
          <p class="infoInBlack" id="NumbLeft"></p>
        </div>

        <!-- Bloc Droite -->
        <div class="block-container">
          <div class="block" draggable="true" data-block-type="RIGHT" id="right-block">
            <strong>Droite</strong>
          </div>
          <button class="place-btn" data-action="place" data-block-id="right-block">Placer</button>
          <p class="infoInBlack" id="NumbRight"></p>
        </div>

        <!-- Bloc Stop -->
        <div class="block-container">
          <div class="block" draggable="true" data-block-type="STOP" id="stop-block">
            <strong>Stop</strong>
          </div>
          <button class="place-btn" data-action="place" data-block-id="stop-block">Placer</button>
          <p class="infoInBlack" id="NumbStop"></p>
        </div>

        <!-- Bloc Boucle -->
        <div class="block-container">
          <div class="block" draggable="true" data-block-type="LOOP" id="loop-block">
            <strong>Boucle</strong>
            <input class="inputBox" type="number" id="loop-iterations" value="3" min="1"> fois
          </div>
          <button class="place-btn" data-action="place-loop">Placer</button>
          <p class="infoInBlack" id="NumbLoop"></p>
        </div>

        <!-- Bloc Attendre -->
        <div class="block-container">
          <div class="block" draggable="true" data-block-type="WAIT" id="wait-block">
            <strong>Attendre</strong>
            <input class="inputBox" type="number" id="wait-time" value="1" min="1" max="100" step="1"> seconde(s)
          </div>
          <button class="place-btn" data-action="place-wait">Placer</button>
          <p class="infoInBlack" id="NumbWait"></p>
        </div>
      </div>
    </section>

    <!-- Panneau du programme -->
    <section class="program-panel">
      <h2 class="program-title">Programme</h2>
      <p class="infoInBlack" id="NumbBlock"></p>
      <div id="program-area" data-action="drop-zone"></div>

      <!-- Contrôles -->
      <div class="controls">
        <div class="main-buttons">
          <button id="run-btn" data-action="run">Exécuter</button>
          <button id="stop-btn" data-action="stop">Stop</button>
          <button id="clear-btn" data-action="clear">Effacer la zone de programme</button>
        </div>
        <div class="secondary-buttons">
          <button id="save-btn" data-action="save-session">Sauvegarder dans le navigateur</button>
          <button id="clear2-btn" data-action="clear-session">Effacer la sauvegarde</button>
        </div>
        <div class="tertiary-buttons">
          <button id="export-btn" data-action="export">Exporter le programme</button>
          <button id="load-btn" data-action="load">Charger le programme</button>
        </div>
      </div>
    </section>
  </main>

  <script src="/programmer.js"></script>
</body>
</html>
)rawliteral";
#endif
