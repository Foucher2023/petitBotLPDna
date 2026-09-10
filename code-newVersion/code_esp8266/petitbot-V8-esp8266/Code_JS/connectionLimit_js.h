#ifndef CONNECTION_LIMIT_JS_H
#define CONNECTION_LIMIT_JS_H

const char CONNECT_LIMIT_JS[] PROGMEM = R"rawliteral(

// ==================== DOM CACHE ====================
const elements = {
  ssidText: document.getElementById("ssidText"),
  spinnerContainer: document.getElementById("spinnerContainer"),
  contentButton: document.getElementById("contentButton"),
  warningMessageContainer: document.getElementById("WarningMessageContainer"),
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
let GlobalNumConnection = 0;

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
  if (elements.ssidText) 
  {
    elements.ssidText.style.display = 'block'; // Affiche l'élément
    elements.ssidText.textContent = "nom de reseaux : " + ssid; // Affiche le nom du réseau
  }
};

// Fonction pour afficher le spinner après 1 seconde
  function showSpinnerAfterDelay() {

    spinnerTimeout = setTimeout(() => {
      elements.spinnerContainer.style.display = "flex";
      elements.contentButton.style.display = "none";

      elements.warningMessageContainer.style.display = 'none';
      elements.okMessageContainer.style.display = 'none';
      elements.waitingMessageContainer.style.display = 'block' ;
        //changer le texte 
      elements.waitingMessageContainer.innerHTML = `
        <p>il y a un changement</p> 
    `
    elements.waitingMessageContainer.classList.add("waiting-message");

    }, 1000); // 1 seconde de délai
  }

function hideSpinner() {
      clearTimeout(spinnerTimeout); // Annule le timeout

      elements.spinnerContainer.style.display = "none";
      elements.contentButton.style.display = "block";

      elements.waitingMessageContainer.style.display = 'none' ;
      elements.okMessageContainer.style.display = 'none';
      elements.warningMessageContainer.style.display = 'block' ;
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
  if (!elements.restartButton) return; // Si le bouton n'existe pas, on sort

  // Désactive le bouton et met à jour son texte
  elements.restartButton.disabled = true;
  elements.restartButton.textContent = "Redémarrage en cours...";

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
    .then(valueConnect => {
      if (GlobalNumConnection != parseInt(valueConnect, 10)){
        GlobalNumConnection = parseInt(valueConnect, 10);// Convertit la réponse en nombre entier
        updateUIBasedOnConnections(GlobalNumConnection); // Met à jour l'interface en fonction du nombre de connexions
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
      updateUIBasedOnConnections(GlobalNumConnection); // Met à jour l'interface en fonction du nombre de connexions

    });
}

// Met à jour l'interface en fonction du nombre de connexions
function updateUIBasedOnConnections(valueConnect) {


  if (!elements.restartButtonContainer || !elements.returnButtonContainer) return; // Si les éléments n'existent pas, on sort

  if (valueConnect < 2) {
    // Si moins de 2 connexions, on cache le bouton de redémarrage et l'avertissement
    elements.restartButtonContainer.disabled = true;
    elements.restartButtonContainer.style.display = 'none';
    elements.warningMessageContainer.style.display = 'none';

    //changer le texte 
    elements.okMessageContainer.innerHTML = `
    <p>Vous êtes maintenant seul(e) sur le reseaux</p> 
    <p>Vous pouvez retourner sur la télécommande avec le boutton en dessous</p>
    <p>et accèder au autre pages </p>
    `
    elements.okMessageContainer.style.display = 'block';
    elements.okMessageContainer.classList.add("ok-message");

    // Affiche le bouton pour retourner à la télécommande
    elements.returnButtonContainer.innerHTML = `
      <button
        class="redirect-button"
        onclick="window.location.href='/telecommande'"
      >Retour à la télécommande
      </button>
    `;
    elements.returnButtonContainer.style.display = 'block';

  } else {
    // Si 2 connexions ou plus, on affiche le bouton de redémarrage et l'avertissement
    elements.restartButton.style.display = 'block';
    elements.restartButton.disabled = false;
    elements.returnButtonContainer.style.display = 'none';
    elements.returnButtonContainer.innerHTML = '';

    elements.warningMessageContainer.style.display = 'block';
    elements.okMessageContainer.style.display = 'none';
  }
}
)rawliteral";
#endif