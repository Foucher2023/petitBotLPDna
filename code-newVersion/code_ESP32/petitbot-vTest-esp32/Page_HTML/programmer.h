#ifndef PROGRAMMER_H
#define PROGRAMMER_H

const char PROGRAMMER_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Programmer Petit Bot</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="/navbar.css">
  <link rel="stylesheet" href="/global.css">
  <link rel="stylesheet" href="/programmer.css">
</head>
<body>
<nav class="navbar navbar-inverse">
			<div class="container-fluid">
				<!--Bouton hamburger pour petits écrans -->
				<div class="navbar-header">
					<button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#myNavbar">
						<span class="icon-bar"></span>
						<span class="icon-bar"></span>
						<span class="icon-bar"></span>
					</button>
					<a class="navbar-brand" href="index.html">PetitBot</a>
				</div>

				<!--Contenu de la nav qui se replie sur mobile-->  
				<div class="collapse navbar-collapse" id="myNavbar">
					<ul class="nav navbar-nav">
						<li><a href="/config">configuration</a></li>
						<li><a href="telecommande.html">Télécommande</a></li>			
					  <li class="active"><a href="/programmer">Programmer</a></li>
            </ul>
				</div>
			</div>
		</nav>

  <h1>Programmer Petit Bot</h1>

  <div class="container">
    <div class="blocks-panel">
      <h2 class="program-title">Blocs disponibles</h2>
      <!-- Bloc Avancer -->
      <div class="block-container">
        <div class="block" draggable="true" ondragstart="drag(event)" id="forward-block">
          <strong>Avancer</strong>
          <span class="hide">FORWARD</span>
        </div>
        <button class="place-btn" onclick="placeBlock('forward-block')">Placer</button>
      </div>

      <!-- Bloc Reculer -->
      <div class="block-container">
        <div class="block" draggable="true" ondragstart="drag(event)" id="reverse-block">
          <strong>Reculer</strong>
          <span class="hide">REVERSE</span>
        </div>
        <button class="place-btn" onclick="placeBlock('reverse-block')">Placer</button>
      </div>

      <!-- Bloc Gauche -->
      <div class="block-container">
        <div class="block" draggable="true" ondragstart="drag(event)" id="left-block">
          <strong>Gauche</strong>
          <span class="hide">LEFT</span>
        </div>
        <button class="place-btn" onclick="placeBlock('left-block')">Placer</button>
      </div>

     
      <!-- Bloc Droite -->
      <div class="block-container">
        <div class="block" draggable="true" ondragstart="drag(event)" id="right-block">
          <strong>Droite</strong>
          <span class="hide">RIGHT</span>
        </div>
        <button class="place-btn" onclick="placeBlock('right-block')">Placer</button>
      </div>

        <!-- Bloc Stop -->
      <div class="block-container">
        <div class="block" draggable="true" ondragstart="drag(event)" id="stop-block">
          <strong>Stop</strong>
          <span class="hide">STOP</span>
        </div>
        <button class="place-btn" onclick="placeBlock('stop-block')">Placer</button>
      </div>

      <!-- Bloc Attendre -->
      <div class="block-container">
        <div class="block" draggable="true" ondragstart="drag(event)" id="wait-block">
          <strong>Attendre</strong>
          <input type="number" id="wait-time" value="1" min="1" max="100" step="1"> seconde(s)
        </div>
        <button class="place-btn" onclick="placeWaitBlock()">Placer</button>
      </div>
    </div>


    </div>

    

    <div class="program-panel">
      <h2 class="program-title">Programme</h2>
      <div id="program-area" ondrop="drop(event)" ondragover="allowDrop(event)">
        <!-- Les blocs seront déposés ici -->
      </div>
      <div class="controls">
        <div class="main-buttons">
          <button id="run-btn" onclick="runProgram()">Exécuter</button>
          <button id="stop-btn" onclick="stopProgram()">Stop</button>
        </div>
        <div class="secondary-buttons">
          <button id="clear-btn" onclick="clearProgram()">Effacer</button>
          <button id="save-btn" onclick="saveProgram()">Sauvegarder</button>
          <button id="load-btn" onclick="loadProgram()">Charger</button>
        </div>
      </div>
    </div>
  </div>

  <script src="/programmer.js"></script>
</body>
</html>
)rawliteral";
#endif
