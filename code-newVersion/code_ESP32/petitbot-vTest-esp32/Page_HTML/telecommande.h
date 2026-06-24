#ifndef TELECOMMANDE_H
#define TELECOMMANDE_H

const char TELECOMMANDE_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP8266 Telecommande</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/navbar.css">
  <link rel="stylesheet" href="/global.css">
  <link rel="stylesheet" href="/telecommande.css">
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
						<li class="active"><a href="telecommande.html">Télécommande</a></li>			
					  <li><a href="/programmer">Programmer</a></li>
            </ul>
				</div>
			</div>
		</nav>

  <h1>télécommande de controle</h1>

  <div class="dpad">
    <button class="btnDir" id="forwardBtn"
        onmousedown="controlMotor('FORWARD')"
        onmouseup="stopMotor()"
        ontouchstart="controlMotor('FORWARD')"
        ontouchend="stopMotor()">
        <span class="bigArrow">▲</span>
    </button>
  <div class="middle">
    <button class="btnDir" id="leftBtn"
        onmousedown="controlMotor('LEFT')"
        onmouseup="stopMotor()"
        ontouchstart="controlMotor('LEFT')"
        ontouchend="stopMotor()">
        <span class="bigArrow">◀</span>
    </button>    
    <button class="btnDir" id="rightBtn"
        onmousedown="controlMotor('RIGHT')"
        onmouseup="stopMotor()"
        ontouchstart="controlMotor('RIGHT')"
        ontouchend="stopMotor()">
        <span class="bigArrow">▶</span>
    </button> 
  </div>

 <button class="btnDir" id="reverseBtn"
  onmousedown="controlMotor('REVERSE')"
  onmouseup="stopMotor()"
  ontouchstart="controlMotor('REVERSE')"
  ontouchend="stopMotor()">
  <span class="bigArrow">▼</span>
</button></div>

</body>
<script src="/telecommande.js"></script>
</html>
)rawliteral";

#endif
