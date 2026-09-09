// ==================== CONSTANTS ====================
const CONNECTION_CHECK_INTERVAL_MS = 500;
const SPINNER_DELAY_MS = 1000;
const CLOSE_TAB_DELAY_MS = 2000;
const FALLBACK_REDIRECT_DELAY_MS = 500;

// ==================== DOM CACHE ====================
const elements = {
  ssidText: document.getElementById("ssidText"),
  spinnerContainer: document.getElementById("spinnerContainer"),
  contentButton: document.getElementById("content-button"),
  warningMessageContainer: document.getElementById("WarningMessagecontainer"),
  okMessageContainer: document.getElementById("OKMessageContainer"),
  waitingMessageContainer: document.getElementById("WaitingMessageContainer"),
  restartButtonContainer: document.getElementById("restartButtonContainer"),
  returnButtonContainer: document.getElementById("returnButtonContainer"),
  restartButton: document.querySelector('.restart-button')
};

// ==================== STATE ====================
let checkConnectionInterval = null;
let isFetching = false;
let spinnerTimeout = null;

// ==================== UTILITIES ====================
/**
 * Delay execution for a specified time.
 * @param {number} ms - Milliseconds to delay.
 * @returns {Promise<void>}
 */
function delay(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

/**
 * Close the current tab or redirect to about:blank as fallback.
 */
function closeTab() {
  window.close();
  setTimeout(() => {
    window.location.href = 'about:blank';
  }, FALLBACK_REDIRECT_DELAY_MS);
}

// ==================== UI FUNCTIONS ====================
/**
 * Update the SSID text in the UI.
 */
function setTextSsid() {
  const ssid = sessionStorage.getItem('ssidName');
  if (!elements.ssidText) return;
  elements.ssidText.style.display = 'block';
  elements.ssidText.textContent = `nom de reseaux : ${ssid}`;
}

/**
 * Show spinner after a delay.
 */
function showSpinnerAfterDelay() {
  spinnerTimeout = setTimeout(() => {
    if (elements.spinnerContainer) elements.spinnerContainer.style.display = "flex";
    if (elements.contentButton) elements.contentButton.style.display = "none";
    if (elements.waitingMessageContainer) {
      elements.waitingMessageContainer.style.display = 'block';
      elements.waitingMessageContainer.innerHTML = '<p>il y a un changement</p>';
      elements.waitingMessageContainer.classList.add("waiting-message");
    }
    if (elements.warningMessageContainer) elements.warningMessageContainer.style.display = 'none';
    if (elements.okMessageContainer) elements.okMessageContainer.style.display = 'none';
  }, SPINNER_DELAY_MS);
}

/**
 * Hide the spinner.
 */
function hideSpinner() {
  clearTimeout(spinnerTimeout);
  if (elements.spinnerContainer) elements.spinnerContainer.style.display = "none";
  if (elements.contentButton) elements.contentButton.style.display = "block";
  if (elements.waitingMessageContainer) elements.waitingMessageContainer.style.display = 'none';
  if (elements.warningMessageContainer) elements.warningMessageContainer.style.display = 'block';
}

// ==================== CORE FUNCTIONS ====================
/**
 * Fetch SSID from EEPROM and update UI.
 */
function fetchAndSetSsid() {
  fetch('/get-values-EEPROM')
    .then(response => {
      if (!response.ok) throw new Error(`HTTP Error: ${response.status}`);
      return response.text();
    })
    .then(data => {
      const [,,,,, ssid] = data.split(',').map(String);
      sessionStorage.setItem('ssidName', ssid);
      setTextSsid();
    })
    .catch(error => {
      console.error("Erreur lors de la récupération des valeurs EEPROM:", error);
    });
}

/**
 * Restart WiFi and update UI.
 */
function restartWiFi() {
  if (!elements.restartButton) return;
  elements.restartButton.disabled = true;
  elements.restartButton.textContent = "Redémarrage en cours...";

  fetch('/restart-wifi')
    .then(response => {
      if (!response.ok) throw new Error(`HTTP Error: ${response.status}`);
      return delay(CLOSE_TAB_DELAY_MS);
    })
    .then(() => closeTab())
    .catch(error => {
      console.error("Erreur lors du redémarrage WiFi:", error);
      delay(CLOSE_TAB_DELAY_MS)
        .then(() => window.location.reload())
        .then(() => delay(1000))
        .then(() => closeTab());
    });
}

/**
 * Start periodic connection checking.
 */
function startConnectionCheck() {
  stopConnectionCheck();
  checkConnectionInterval = setInterval(checkConnections, CONNECTION_CHECK_INTERVAL_MS);
}

/**
 * Stop periodic connection checking.
 */
function stopConnectionCheck() {
  if (checkConnectionInterval) {
    clearInterval(checkConnectionInterval);
    checkConnectionInterval = null;
  }
  clearTimeout(spinnerTimeout);
}

/**
 * Check the number of active connections.
 */
function checkConnections() {
  if (isFetching) return;
  isFetching = true;
  showSpinnerAfterDelay();

  fetch('/getNumConnections')
    .then(response => {
      if (!response.ok) throw new Error(`Erreur HTTP : ${response.status}`);
      return response.text();
    })
    .then(text => {
      const num = parseInt(text, 10);
      if (!isNaN(num)) updateUIBasedOnConnections(num);
    })
    .catch(error => {
      console.error("Erreur lors de la vérification des connexions :", error);
      closeTab();
    })
    .finally(() => {
      isFetching = false;
      hideSpinner();
    });
}

/**
 * Update UI based on the number of connections.
 * @param {number} numConnections - Number of active connections.
 */
function updateUIBasedOnConnections(numConnections) {
  if (!elements.restartButtonContainer || !elements.returnButtonContainer) return;

  if (numConnections < 2) {
    if (elements.restartButton) {
      elements.restartButton.disabled = true;
      elements.restartButton.style.display = 'none';
    }
    if (elements.warningMessageContainer) elements.warningMessageContainer.style.display = 'none';
    if (elements.okMessageContainer) {
      elements.okMessageContainer.innerHTML = `
        <p>Vous êtes maintenant seul(e) sur le reseaux</p>
        <p>Vous pouvez retourner sur la télécommande avec le boutton en dessous</p>
        <p>et accèder au autre pages</p>
      `;
      elements.okMessageContainer.style.display = 'block';
      elements.okMessageContainer.classList.add("ok-message");
    }
    if (elements.returnButtonContainer) {
      elements.returnButtonContainer.innerHTML = `
        <button class="redirect-button" onclick="window.location.href='/telecommande'">
          Retour à la télécommande
        </button>
      `;
      elements.returnButtonContainer.style.display = 'block';
    }
  } else {
    if (elements.restartButton) {
      elements.restartButton.style.display = 'block';
      elements.restartButton.disabled = false;
    }
    if (elements.returnButtonContainer) {
      elements.returnButtonContainer.style.display = 'none';
      elements.returnButtonContainer.innerHTML = '';
    }
    if (elements.warningMessageContainer) elements.warningMessageContainer.style.display = 'block';
    if (elements.okMessageContainer) elements.okMessageContainer.style.display = 'none';
  }
}

// ==================== CLEANUP ====================
window.addEventListener('beforeunload', () => {
  stopConnectionCheck();
  clearTimeout(spinnerTimeout);
});

// ==================== INIT ====================
document.addEventListener('DOMContentLoaded', () => {
  startConnectionCheck();

  document.addEventListener('click', (e) => {
    const actionElement = e.target.closest('[data-action="restart-wifi"]');
    if (actionElement) {
      e.preventDefault();
      restartWiFi();
    }
  });

  if (!sessionStorage.getItem('ssidName')) {
    fetchAndSetSsid();
  } else {
    setTextSsid();
  }
});
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================

// ==================== DOM CACHE ====================
const elements = {
  ssidText: document.getElementById("ssidText"),
  spinnerContainer: document.getElementById("spinnerContainer"),
  contentButton: document.getElementById("content-button"),
  warningMessageContainer: document.getElementById("WarningMessagecontainer"),
  okMessageContainer: document.getElementById("OKMessageContainer"),
  waitingMessageContainer: document.getElementById("WaitingMessageContainer"),
  restartButtonContainer: document.getElementById("restartButtonContainer"),
  returnButtonContainer: document.getElementById("returnButtonContainer"),
  restartButton: document.querySelector('.restart-button')
};

// Variable globale pour l'intervalle de vérification des connexions 
let checkConnectionInterval = null;
let isFetching = false; // Drapeau pour suivre si une requête fetch est en cours
let spinnerTimeout;
const Timming = 500;

// Initialise les écouteurs d'événements au chargement de la page 
document.addEventListener('DOMContentLoaded', () => {
  startConnectionCheck(); // Démarre la vérification périodique des connexions

  // Écoute les clics sur les éléments avec l'attribut data-action="restart-wifi"
  document.addEventListener('click', (e) => {
    const actionElement = e.target.closest('[data-action="restart-wifi"]');
    if (actionElement) {
      e.preventDefault();
      restartWiFi();
    }
  });

  // Récupère le nom du SSID depuis l'EEPROM si non présent dans sessionStorage
  if(!sessionStorage.getItem('ssidName')){
    fetch('/get-values-EEPROM')
      .then(response => response.text())
      .then(data => {
        const [,,,,, ssid] = data.split(',').map(String);
        sessionStorage.setItem('ssidName', ssid); // Stocke le SSID dans sessionStorage
        setTextSsid(); // Met à jour l'interface avec le SSID
    })
    .catch(error => console.error("Erreur lors de la récupération des valeurs EEPROM:", error));
  } else {
    setTextSsid(); // Si le SSID est déjà en sessionStorage, met à jour l'interface
  }
});


//Définir le ssid dans le text de warning 
function setTextSsid(){
  const ssid = sessionStorage.getItem('ssidName');
  const textssid = document.getElementById("ssidText");
  if (textssid) 
  {
    textssid.style.display = 'block'; // Affiche l'élément
    textssid.textContent = "nom de reseaux : " + ssid; // Affiche le nom du réseau
  }
};

// Fonction pour afficher le spinner après 1 seconde
  function showSpinnerAfterDelay() {
    const WarningContainer = document.getElementById("WarningMessagecontainer");
    const OKContainer = document.getElementById("OKMessageContainer");
    const WaitingContainer = document.getElementById("WaitingMessageContainer");

    spinnerTimeout = setTimeout(() => {
      document.getElementById("spinnerContainer").style.display = "flex";
      document.getElementById("content-button").style.display = "none";
      WarningContainer.style.display = 'none';
      OKContainer.style.display = 'none';
      WaitingContainer.style.display = 'block' ;
        //changer le texte 
      WaitingContainer.innerHTML = `
        <p>il y a un changement</p> 
    `
    WaitingContainer.classList.add("waiting-message");

    }, 1000); // 1 seconde de délai
  }

function hideSpinner() {
      clearTimeout(spinnerTimeout); // Annule le timeout
      const WaitingContainer = document.getElementById("WaitingMessageContainer");
      const WarningContainer = document.getElementById("WarningMessagecontainer");

      document.getElementById("spinnerContainer").style.display = "none";
      document.getElementById("content-button").style.display = "block";

      WaitingContainer.style.display = 'none' ;
      WarningContainer.style.display = 'block' ;
    };

function closeTab() {
  // Essaye de fermer l'onglet (fonctionne si l'onglet a été ouvert via JavaScript)
  window.close();

  // Si window.close() ne fonctionne pas, redirige vers une page vide
  setTimeout(() => {
    window.location.href = 'about:blank';
  }, 500);
};

// Redémarre le Wi-Fi et met à jour l'interface 
function restartWiFi() {
  const button = document.querySelector('.restart-button');
  if (!button) return; // Si le bouton n'existe pas, on sort

  // Désactive le bouton et met à jour son texte
  button.disabled = true;
  button.textContent = "Redémarrage en cours...";

  // Envoie une requête pour redémarrer le Wi-Fi
  fetch('/restart-wifi')
  .then(response => response.text())
    .then(() => {
     setTimeout(() => {
      closeTab();
      }, 2000);
    })
    .catch(() => {
      // En cas d'erreur, recharge également la page après 2 secondes
      setTimeout(() => {
        window.location.reload();
        delay(1000);
        closeTab();
      }, 2000);
    });
}



// Démarre la vérification périodique du nombre de connexions 
function startConnectionCheck() {
  // Arrête l'intervalle existant (si présent)
  if (checkConnectionInterval) {
    clearInterval(checkConnectionInterval);
  }

  // Vérifie toutes les 1.5 secondes
  checkConnectionInterval = setInterval(checkConnections, Timming);
}

// Vérifie le nombre de connexions actives 
function checkConnections() {
  
// Si une requête fetch est déjà en cours, on ignore cet appel
  if (isFetching) {
    return;
  }

  // Définit le drapeau à true pour indiquer qu'une requête est en cours
  isFetching = true;

  showSpinnerAfterDelay();

  // Envoie une requête pour récupérer le nombre de connexions
  fetch('/getNumConnections')
    .then(response => {
      if (!response.ok) {
        throw new Error(`Erreur HTTP : ${response.status}`); // Lance une erreur si la réponse n'est pas OK
      }
      return response.text(); // Récupère le texte de la réponse
    })
    .then(numConnections => {
      const num = parseInt(numConnections, 10); // Convertit la réponse en nombre entier
      if (!isNaN(num)) {
        updateUIBasedOnConnections(num); // Met à jour l'interface en fonction du nombre de connexions
      }
    })
    .catch(error => {
      console.error("Erreur lors de la vérification des connexions :", error); // Affiche l'erreur dans la console
      isFetching = true;
      closeTab();
    })
    .finally(() => {
      // Réinitialise le drapeau à false lorsque la requête est terminée (succès ou erreur)
      isFetching = false;
      hideSpinner();
    });
}

// Met à jour l'interface en fonction du nombre de connexions
function updateUIBasedOnConnections(numConnections) {

  const restartButton = document.getElementById("restartButtonContainer");
  const returnButtonContainer = document.getElementById("returnButtonContainer");

  const warningContainer = document.getElementById("WarningMessagecontainer");
  const OKContainer = document.getElementById("OKMessageContainer");

  if (!restartButton || !returnButtonContainer) return; // Si les éléments n'existent pas, on sort

  if (numConnections < 2) {
    // Si moins de 2 connexions, on cache le bouton de redémarrage et l'avertissement
    restartButton.disabled = true;
    restartButton.style.display = 'none';
    warningContainer.style.display = 'none';

    //changer le texte 
    OKContainer.innerHTML = `
    <p>Vous êtes maintenant seul(e) sur le reseaux</p> 
    <p>Vous pouvez retourner sur la télécommande avec le boutton en dessous</p>
    <p>et accèder au autre pages </p>
    `
    OKContainer.style.display = 'block';
    OKContainer.classList.add("ok-message");

    // Affiche le bouton pour retourner à la télécommande
    returnButtonContainer.innerHTML = `
      <button
        class="redirect-button"
        onclick="window.location.href='/telecommande'"
      >Retour à la télécommande
      </button>
    `;
    returnButtonContainer.style.display = 'block';

  } else {
    // Si 2 connexions ou plus, on affiche le bouton de redémarrage et l'avertissement
    restartButton.style.display = 'block';
    restartButton.disabled = false;
    returnButtonContainer.style.display = 'none';
    returnButtonContainer.innerHTML = '';

    warningContainer.style.display = 'block';
    OKContainer.style.display = 'none';
  }
}