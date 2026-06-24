#ifndef TELECOMMANDE_JS_H
#define TELECOMMANDE_JS_H

const char TELECOMMANDE_JS[] PROGMEM = R"rawliteral(
let forwardBtn, leftBtn, rightBtn, reverseBtn;
let activeKey = null;

// Fonction unique pour le chargement de la page
window.onload = function() {

// Récupérer l'état depuis l'ESP
  fetch('/get-values-EEPROM')
    .then(response => response.text())
    .then(data => {
      // Séparer les valeurs par la virgule
      const [led, motorBF, motorLR,pinMotor1,pinMotor2,ssid] = data.split(',').map(String);
      sessionStorage.setItem('ssidName', ssid);
      setTitleRemote();
});



}

function setTitleRemote(){
    const ssidState = sessionStorage.getItem('ssidName');
    if (ssidState.length>2){
    const TitleElement = document.getElementById("titleRemote");
          TitleElement.textContent = `télécommande de controle : ${ssidState}`;
    }
}

// Initialisation des boutons après le chargement du DOM
document.addEventListener('DOMContentLoaded', function() {
 forwardBtn = document.getElementById('forwardBtn');
 leftBtn = document.getElementById('leftBtn');
 rightBtn = document.getElementById('rightBtn');
 reverseBtn = document.getElementById('reverseBtn');
 stopBtn = document.getElementById ('stopBtn');
});

// Fonction pour activer/désactiver le style quand on utilise les fleches 
function setActiveButton(button, isActive) {
  if (button != stopBtn) 
  {
    if (isActive) {
    button.classList.add('button-active');
    } else {
      button.classList.remove('button-active');
    }
  }else 
  {
    if (isActive) {
      button.classList.add('btnStop-active');
    } else {
      button.classList.remove('btnStop-active');
    }
  }
}

function actionOfbutton (button,state,direction)
{
  setActiveButton(button,state);
  if (direction != NULL)
    {controlMotor(direction);}
  else {stopMotor();}
}

// Contrôle des moteurs via les boutons
function controlMotor(direction) {
  fetch('/UseTelecommande?val=' + direction);
}

// Arrêt des moteurs
function stopMotor() {
  fetch('/stopMotors');
}

// Gestion des touches clavier
document.addEventListener('keydown', function(event) {
  // Empêche le défilement pour les touches directionnelles et espace
  if ([37, 38, 39, 40, 32].includes(event.keyCode)) {
    event.preventDefault(); // Empêche le comportement par défaut (défilement)
  }

  // Ignore les répétitions automatiques sauf si la touche est déjà active
  if (event.repeat && activeKey === event.keyCode) 
    return;
  

  switch (event.keyCode || event.which) {
    case 38: // Flèche haut
      if (!event.repeat || activeKey === 38) {
        setActiveButton(forwardBtn, true);
        activeKey = 38;
        fetch('/UseTelecommande?val=FORWARD');
      }
      break;
    case 40: // Flèche bas
      if (!event.repeat || activeKey === 40) {
        setActiveButton(reverseBtn, true);
        activeKey = 40;
        fetch('/UseTelecommande?val=REVERSE');
      }
      break;
    case 37: // Flèche gauche
      if (!event.repeat || activeKey === 37) {
        setActiveButton(leftBtn, true);
        activeKey = 37;
        fetch('/UseTelecommande?val=LEFT');
      }
      break;
    case 39: // Flèche droite
      if (!event.repeat || activeKey === 39) {
        setActiveButton(rightBtn, true);
        activeKey = 39;
        fetch('/UseTelecommande?val=RIGHT');
      }
      break;
    case 32: // Espace
    if (!event.repeat || activeKey === 32) {
      setActiveButton(stopBtn, true);
      activeKey = 32;
      fetch('/stopMotors');
      }
      break;
  }
});

// Gestion du relâchement des touches
document.addEventListener('keyup', function(event) {
  // Vérifie si la touche relâchée correspond à la touche active
  if (activeKey === event.keyCode) {
    switch (event.keyCode || event.which) {
      case 37: // Flèche gauche
      case 38: // Flèche haut
      case 39: // Flèche droite
      case 40: // Flèche bas
      case 32: // barre espace
        setActiveButton(forwardBtn, false);
        setActiveButton(reverseBtn, false);
        setActiveButton(leftBtn, false);
        setActiveButton(rightBtn, false);
        setActiveButton(stopBtn, false);
        break;
    }
    fetch('/stopMotors');
    activeKey = null;
  }
});

// Gestion du menu navbar
document.querySelector('.navbar-toggle')?.addEventListener('click', function() {
  document.getElementById('myNavbar')?.classList.toggle('open');
});

)rawliteral";
#endif
