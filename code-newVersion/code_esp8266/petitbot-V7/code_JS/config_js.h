#ifndef CONFIG_JS_H
#define CONFIG_JS_H

const char CONFIG_JS[] PROGMEM = R"rawliteral(
/* ===== Variables globales ===== */
const pinMap = {
  "": "",
  "D0": 16,
  "D1": 5,
  "D2": 4,
  "D3": 0,
  "D4": 2,
  "D5": 14,
  "D6": 12,
  "D7": 13,
  "D8": 15
};

const gpioToNameMap = {
  "16": "D0",
  "5": "D1",
  "4": "D2",
  "0": "D3",
  "2": "D4",
  "14": "D5",
  "12": "D6",
  "13": "D7",
  "15": "D8"
};

/* ===== Initialisation ===== */
document.addEventListener('DOMContentLoaded', function() {
  // get navbar
 fetch('/navbar')
    .then(response => response.text())
    .then(html => {
      document.getElementById('navbar-placeholder').innerHTML = html;
      document.querySelector('.navbar-toggle')?.addEventListener('click', function() {
        document.getElementById('myNavbar').classList.toggle('open');
      });
      document.getElementById('button-config').classList.add('active');
    })
    .catch(error => console.error("Erreur lors du chargement de la navbar :", error));
    
  // Récupère l'état depuis l'ESP
  fetch('/get-values-EEPROM')
    .then(response => response.text())
    .then(data => {
      const [led, motorBF, motorLR, pinMotor1, pinMotor2, ssid] = data.split(',').map(String);

      // Stocke dans sessionStorage
      sessionStorage.setItem('ledState', led);
      sessionStorage.setItem('motorBF', motorBF);
      sessionStorage.setItem('motorLR', motorLR);
      sessionStorage.setItem('pinMotor1', pinMotor1);
      sessionStorage.setItem('pinMotor2', pinMotor2);
      sessionStorage.setItem('ssidName', ssid);

      // Met à jour l'interface
      updateStatusDisplay();
      restoreUIState();
    })
    .catch(error => console.error("Erreur :", error));

  // Écouteurs pour les sélecteurs de broches
  document.getElementById("pinMotor1")?.addEventListener("change", checkButtonState);
  document.getElementById("pinMotor2")?.addEventListener("change", checkButtonState);

  // Écouteur UNIQUE pour les boutons radio (via change)
  document.querySelectorAll('input[type="radio"]').forEach(radio => {
    radio.addEventListener('change', (e) => {
      const label = e.target.closest('label[data-action]');
      if (label) {
        const action = label.dataset.action;
        const value = label.dataset.value;
        handleAction(action, value);
      }
    });
  });

  // Écouteur UNIQUE pour les boutons classiques (via click)
  document.querySelectorAll('button[data-action]').forEach(button => {
    button.addEventListener('click', (e) => {
      e.stopPropagation();
      const action = button.dataset.action;
      handleAction(action);
    });
  });

  // --- Gestion de l'affichage du bouton OTA ---
  const hostname = window.location.hostname;
  const theParam = "BASE_URL";

  // Fonction pour vérifier et afficher le bouton
  function checkAndShowOTAButton() {
    if (hostname === "192.168.4.1" || hostname === sessionStorage.getItem('BASE_URL')) {
      document.getElementById('otaButtonContainer')?.style.setProperty('display', 'block');
    }
  }

  // Si BASE_URL n'est pas encore dans sessionStorage, on le récupère
  if (!sessionStorage.getItem('BASE_URL')) {
    fetch(`/get-static-value?param=${encodeURIComponent(theParam)}`)
      .then(response => response.text())
      .then(data => {
        const url_host = data + ".local";
        sessionStorage.setItem('BASE_URL', url_host);
        // vérifie APRÈS avoir stocké BASE_URL
        checkAndShowOTAButton();
      })
      .catch(error => console.error("Erreur :", error));
  } else {
    // Si BASE_URL est déjà dans sessionStorage, on vérifie directement
    checkAndShowOTAButton();
  }
});

/* ===== Mise à jour de l'interface ===== */
/**
 * Met à jour l'affichage du SSID et des broches
 */
function updateStatusDisplay() {
  // Afficher le SSID
  const ssidState = sessionStorage.getItem('ssidName');
  const statusSSIDElement = document.getElementById("statusSSID");
  if (statusSSIDElement && ssidState) {
    statusSSIDElement.textContent = `Connecté au PetitBot : ${ssidState}`;
  }

  // Afficher les broches des moteurs
  const pinMotor1State = sessionStorage.getItem('pinMotor1');
  const pinMotor2State = sessionStorage.getItem('pinMotor2');
  const statusPinElement = document.getElementById("statusPinMotorSet");

  if (statusPinElement) {
    const pin1Display = convertGPIOToName(pinMotor1State);
    const pin2Display = convertGPIOToName(pinMotor2State);
    statusPinElement.textContent = `Moteurs branchés sur : Moteur 1 (${pin1Display}), Moteur 2 (${pin2Display})`;
  }
}

/**
 * Restaure l'état de l'UI depuis sessionStorage
 */
function restoreUIState() {
  // Restaurer l'état de la LED
  const ledState = sessionStorage.getItem('ledState');
  if (ledState !== null) {
    const radio = document.querySelector(`input[name="stateLed"][value="${ledState === '0' ? 'ON' : 'OFF'}"]`);
    if (radio) radio.checked = true;
    updateCheckedState(radio?.closest('label'));
  }

  // Restaurer l'état des moteurs avant/arrière
  const motorBFState = sessionStorage.getItem('motorBF');
  if (motorBFState !== null) {
    const radio = document.querySelector(`input[name="stateMotor"][value="${motorBFState === '1' ? 'YES' : 'NO'}"]`);
    if (radio) radio.checked = true;
    updateCheckedState(radio?.closest('label'));
  }

  // Restaurer l'état des moteurs gauche/droite
  const motorLRState = sessionStorage.getItem('motorLR');
  if (motorLRState !== null) {
    const radio = document.querySelector(`input[name="stateMotorLeftRight"][value="${motorLRState === '1' ? 'YES' : 'NO'}"]`);
    if (radio) radio.checked = true;
    updateCheckedState(radio?.closest('label'));
  }
}

/**
 * Met à jour l'état visuel du bouton (ajout/suppression de button-checked)
 * @param {HTMLElement} label - Le label du bouton radio
 */
function updateCheckedState(label) {
  if (!label) return;
  // Supprime button-checked de tous les labels du même fieldset
  const fieldset = label.closest('fieldset');
  fieldset?.querySelectorAll('label.button').forEach(l => {
    l.classList.remove('button-checked');
  });
  // Ajoute button-checked au label cliqué
  label.classList.add('button-checked');
}

/* ===== Gestion des actions ===== */
/**
 * Gère les actions en fonction de data-action
 * @param {string} action - L'action à effectuer
 * @param {string} value - La valeur associée (optionnelle)
 */
function handleAction(action, value) {
  switch (action) {
    case 'led':
      controlLed(value);
      break;
    case 'motorBF':
      motorInvertedFrontBack(value);
      break;
    case 'motorLR':
      motorInvertedLeftRight(value);
      break;
    case 'pin-select':
      // Géré par checkButtonState
      break;
    case 'update-pins':
      updatePinMotor();
      break;
    case 'update-ssid':
      updateSSID();
      break;
    case 'reset':
      resetConfig();
      break;
  }
}

/* ===== Contrôle des paramètres ===== */
/**
 * Met à jour un paramètre sur l'ESP
 * @param {string} param - Le paramètre à mettre à jour
 * @param {string|number} value - La valeur à appliquer
 */
function updateState(param, value) {
  const query = `/update-state?param=${encodeURIComponent(param)}&value=${encodeURIComponent(value)}`;
  fetch(query)
    .then(response => {
      if (!response.ok) throw new Error("Erreur lors de la mise à jour.");
      sessionStorage.setItem(param, value);
    })
    .catch(error => console.error("Erreur :", error));
}

/**
 * Contrôle la LED
 * @param {string} state - ON ou OFF
 */
function controlLed(state) {
  const boolState = state === 'ON' ? '0' : '1';
  updateState('ledState', boolState);
  sessionStorage.setItem('ledState', boolState);
  updateCheckedState(document.querySelector(`label[data-action="led"][data-value="${state}"]`));
}

/**
 * Inverse les moteurs avant/arrière
 * @param {string} state - YES ou NO
 */
function motorInvertedFrontBack(state) {
  const boolState = state === 'YES' ? '1' : '0';
  updateState('motorBF', boolState);
  sessionStorage.setItem('motorBF', boolState);
  updateCheckedState(document.querySelector(`label[data-action="motorBF"][data-value="${state}"]`));
}

/**
 * Inverse les moteurs gauche/droite
 * @param {string} state - YES ou NO
 */
function motorInvertedLeftRight(state) {
  const boolState = state === 'YES' ? '1' : '0';
  updateState('motorLR', boolState);
  sessionStorage.setItem('motorLR', boolState);
  updateCheckedState(document.querySelector(`label[data-action="motorLR"][data-value="${state}"]`));
}

/* ===== Gestion des broches ===== */
/**
 * Vérifie si le bouton de mise à jour des broches doit être désactivé
 */
function checkButtonState() {
  const pinMotor1 = document.getElementById("pinMotor1")?.value;
  const pinMotor2 = document.getElementById("pinMotor2")?.value;
  const updateButton = document.getElementById("updateButton");
  if (updateButton) {
    updateButton.disabled = (pinMotor1 === "" && pinMotor2 === "");
  }
}

/**
 * Convertit un nom de broche (ex: "D1") en numéro de GPIO
 * @param {string} pinName - Le nom de la broche (ex: "D1")
 * @returns {number} - Le numéro de GPIO ou -1 si invalide
 */
function convertPinNameToGPIO(pinName) {
  return pinMap[pinName] !== undefined ? pinMap[pinName] : -1;
}

/**
 * Convertit un numéro de GPIO en nom de broche (ex: 5 -> "D1")
 * @param {string} gpio - Le numéro de GPIO
 * @returns {string} - Le nom de la broche ou le GPIO si non mappé
 */
function convertGPIOToName(gpio) {
  return gpioToNameMap[gpio] ? `${gpioToNameMap[gpio]}` : `GPIO${gpio}`;
}

/**
 * Met à jour les broches des moteurs
 */
function updatePinMotor() {
  const pinName1 = document.getElementById("pinMotor1")?.value;
  const pinName2 = document.getElementById("pinMotor2")?.value;
  const statusElement = document.getElementById("statusGPIO");

  if (!statusElement) return;

  // Vérifie que les broches ne sont pas identiques
  if (pinName1 !== "" && pinName2 !== "" && pinName1 === pinName2) {
    statusElement.textContent = "Erreur : Les deux broches ne peuvent pas être identiques !";
    statusElement.style.color = "red";
    return;
  }

  const gpio1 = convertPinNameToGPIO(pinName1);
  const gpio2 = convertPinNameToGPIO(pinName2);

  // Vérifie que les broches sont valides
  if (gpio1 === -1 || gpio2 === -1) {
    statusElement.textContent = "Erreur : Broche invalide. Utilisez D0 à D8.";
    statusElement.style.color = "red";
    return;
  }

  // Envoie la requête de mise à jour
  fetch(`/update-config?pinMotor1=${encodeURIComponent(gpio1)}&pinMotor2=${encodeURIComponent(gpio2)}`)
    .then(response => {
      if (response.ok) {
        statusElement.textContent = `Broches mises à jour : Moteur 1 = ${pinName1}, Moteur 2 = ${pinName2}`;
        statusElement.style.color = "green";
        sessionStorage.setItem('pinMotor1', gpio1);
        sessionStorage.setItem('pinMotor2', gpio2);
        updateStatusDisplay();
      } else {
        throw new Error("Échec de la mise à jour des broches.");
      }
    })
    .catch(error => {
      console.error("Erreur :", error);
      statusElement.textContent = `Erreur : ${error.message}`;
      statusElement.style.color = "red";
    });
}

/* ===== Gestion du SSID ===== */
/**
 * Met à jour le SSID
 */
function updateSSID() {
  const newSSID = document.getElementById("ssidInput")?.value.trim();
  const statusElement = document.getElementById("statusSSID");

  if (!statusElement) return;

  if (!newSSID) {
    statusElement.textContent = "Veuillez saisir un SSID !";
    statusElement.style.color = "red";
    return;
  }
  sessionStorage.setItem('ssidName', newSSID);
  fetch(`/update-config?ssid=${encodeURIComponent(newSSID)}`)
    .then(response => {
      if (response.ok) {
        statusElement.textContent = "SSID mis à jour ! Redémarrage du Wi-Fi...";
        statusElement.style.color = "green";
        sessionStorage.setItem('ssidName', newSSID);
        setTimeout(() => window.location.reload(), 2500);
      } else {
        throw new Error("Échec de la mise à jour du SSID.");
      }
    })
    .catch(error => {
      statusElement.textContent = `Erreur : ${error.message}`;
      statusElement.style.color = "red";
    });
};

/* ===== Réinitialisation ===== */
/**
 * Réinitialise la configuration
 */
function resetConfig() {
  if (confirm("Êtes-vous sûr de vouloir réinitialiser la configuration ?")) {
    fetch('/update-config?reset=all')
      .then(response => {
        if (response.ok) {
          sessionStorage.clear();
          alert("Configuration réinitialisée. Redémarrage...");
          setTimeout(() => window.location.reload(), 1000);
        } else {
          throw new Error("Échec de la réinitialisation.");
        }
      })
      .catch(error => {
        console.error("Erreur :", error);
        alert(`Erreur : ${error.message}`);
      });
  }
};
)rawliteral";
#endif