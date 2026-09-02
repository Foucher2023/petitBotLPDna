#ifndef TELECOMMANDE_JS_H
#define TELECOMMANDE_JS_H

const char TELECOMMANDE_JS[] PROGMEM = R"rawliteral(
// ====================== Variables globales ====================
let activeKey = null;
let checkConnectionInterval = null;
// ====================== Initialisation =====================
window.onload = function() {
  // Récupère la navbar
  fetch('/navbar')
    .then(response => response.text())
    .then(html => {
      document.getElementById('navbar-placeholder').innerHTML = html;
      document.querySelector('.navbar-toggle')?.addEventListener('click', function() {
        document.getElementById('myNavbar').classList.toggle('open');
      });
      document.getElementById('button-remote').classList.add('active');
    })
    .catch(error => console.error("Erreur lors du chargement de la navbar :", error));
    
  // Récupère l'état depuis l'ESP et met à jour le titre
  fetch('/get-values-EEPROM')
    .then(response => response.text())
    .then(data => {
      const [,,,,, ssid] = data.split(',').map(String);
      sessionStorage.setItem('ssidName', ssid);
      setTitleRemote();
    })
    .catch(error => console.error("Erreur lors de la récupération des valeurs EEPROM:", error));

    startConnectionCheck();
};

// ============================== FONCTION CHECK-CONNEXION ======================

function startConnectionCheck() {
  // Arrête l'intervalle existant (si présent)
  if (checkConnectionInterval) {
    clearInterval(checkConnectionInterval);
  }

  // Vérifie toutes les 2 secondes
  checkConnectionInterval = setInterval(checkConnections, 2000);
}

function checkConnections() {
  fetch('/getNumConnections')
    .then(response => {
      if (!response.ok) {
        throw new Error(`Erreur HTTP : ${response.status}`);
      }
      return response.text();
    })
    .then(numConnections => {

      const num = parseInt(numConnections, 10);
      if (!isNaN(num)) {
        if(num>1){window.location.href = '/connection-limit';}
      }
    })
    .catch(error => {
      console.error("Erreur lors de la vérification des connexions :", error);
    });
}

// ====================== Mise à jour du titre ==============================
function setTitleRemote() {
  const ssidState = sessionStorage.getItem('ssidName');
  if (ssidState && ssidState.length > 2) {
    const titleElement = document.getElementById("titleRemote");
    if (titleElement) {
      titleElement.textContent = `Télécommande PetitBot : ${ssidState}`;
    }
  }
}

// ============================= Contrôle des moteurs ======================

function controlMotor(direction) {
  fetch(`/UseTelecommande?val=${direction}`)
    .catch(error => console.error("Erreur:", error));
}

function stopMotor() {
  fetch('/stopMotors')
    .catch(error => console.error("Erreur:", error));
}

// ======================= Gestion des boutons =============================
// Délégation d'événements pour les boutons de direction
document.addEventListener('mousedown', function(e) {
  const button = e.target.closest('[data-action]');
  if (button) {
    e.preventDefault();
    const action = button.dataset.action;
    if (action === 'STOP') {
      stopMotor();
      button.classList.add('btnStop-active');
    } else {
      controlMotor(action);
      button.classList.add('button-active');
    }
  }
});

document.addEventListener('mouseup', function(e) {
  const button = e.target.closest('[data-action]');
  if (button) {
    e.preventDefault();
    button.classList.remove('button-active', 'btnStop-active');
    stopMotor();
  }
});

// Gestion des événements tactiles
document.addEventListener('touchstart', function(e) {
  const button = e.target.closest('[data-action]');
  if (button) {
    e.preventDefault();
    const action = button.dataset.action;
    if (action === 'STOP') {
      stopMotor();
      button.classList.add('btnStop-active');
    } else {
      controlMotor(action);
      button.classList.add('button-active');
    }
  }
});

document.addEventListener('touchend', function(e) {
  const button = e.target.closest('[data-action]');
  if (button) {
    e.preventDefault();
    button.classList.remove('button-active', 'btnStop-active');
    stopMotor();
  }
});

// ================================ Gestion des touches clavier ==============================
// Mappage des touches clavier aux actions
const keyMap = {
  38: { action: 'FORWARD', button: 'forwardBtn' },  // Flèche haut
  40: { action: 'REVERSE', button: 'reverseBtn' }, // Flèche bas
  37: { action: 'LEFT', button: 'leftBtn' },      // Flèche gauche
  39: { action: 'RIGHT', button: 'rightBtn' },    // Flèche droite
  32: { action: 'STOP', button: 'stopBtn' }       // Espace
};

// Gestion des touches enfoncées
document.addEventListener('keydown', function(e) {
  const keyCode = e.keyCode || e.which;
  const keyInfo = keyMap[keyCode];

  // Empêche le défilement pour les touches directionnelles et espace
  if (keyInfo) {
    e.preventDefault();
  }

  // Ignore les répétitions automatiques sauf si la touche est déjà active
  if (e.repeat && activeKey === keyCode) {
    return;
  }

  if (keyInfo) {
    const button = document.getElementById(keyInfo.button);
    if (button) {
      button.classList.add(keyInfo.action === 'STOP' ? 'btnStop-active' : 'button-active');
    }
    activeKey = keyCode;
    if (keyInfo.action === 'STOP') {
      stopMotor();
    } else {
      controlMotor(keyInfo.action);
    }
  }
});

// Gestion des touches relâchées
document.addEventListener('keyup', function(e) {
  const keyCode = e.keyCode || e.which;
  if (activeKey === keyCode) {
    const keyInfo = keyMap[keyCode];
    if (keyInfo) {
      const button = document.getElementById(keyInfo.button);
      if (button) {
        button.classList.remove('button-active', 'btnStop-active');
      }
      stopMotor();
      activeKey = null;
    }
  }
});
)rawliteral";
#endif
