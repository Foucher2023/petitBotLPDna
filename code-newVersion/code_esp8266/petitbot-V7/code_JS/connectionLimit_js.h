#ifndef CONNECTION_LIMIT_JS_H
#define CONNECTION_LIMIT_JS_H

const char CONNECT_LIMIT_JS[] PROGMEM = R"rawliteral(
// Variable globale pour l'intervalle de vérification des connexions 
let checkConnectionInterval = null;

// Initialise les écouteurs d'événements au chargement de la page 
document.addEventListener('DOMContentLoaded', () => {
  startConnectionCheck();

  document.addEventListener('click', (e) => {
    const actionElement = e.target.closest('[data-action="restart-wifi"]');
    if (actionElement) {
      e.preventDefault();
      restartWiFi();
    }
  });
});

// Redémarre le Wi-Fi et met à jour l'interface 
function restartWiFi() {
  const button = document.querySelector('.restart-button');
  if (!button) return;

  // Désactive le bouton et met à jour son texte
  button.disabled = true;
  button.textContent = "Redémarrage en cours...";

  fetch('/restart-wifi')
    .then(response => {
      if (!response.ok) {
        throw new Error(`Erreur HTTP : ${response.status}`);
      }
      return response.text();
    })
    .then(() => {
      button.textContent = "Redémarrage réussi !";
      // Relance la vérification des connexions après un redémarrage
      startConnectionCheck();
    })
    .catch(error => {
      console.error("Erreur :", error);
      button.textContent = "Erreur. Réessayer ?";
      button.disabled = false;
      // Relance la vérification des connexions même en cas d'erreur
      startConnectionCheck();
    });
}

// Démarre la vérification périodique du nombre de connexions 
function startConnectionCheck() {
  // Arrête l'intervalle existant (si présent)
  if (checkConnectionInterval) {
    clearInterval(checkConnectionInterval);
  }

  // Vérifie toutes les 1.5 secondes
  checkConnectionInterval = setInterval(checkConnections, 1500);
}

// Vérifie le nombre de connexions actives 
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
        updateUIBasedOnConnections(num);
      }
    })
    .catch(error => {
      console.error("Erreur lors de la vérification des connexions :", error);
    });
}

// Met à jour l'interface en fonction du nombre de connexions
function updateUIBasedOnConnections(numConnections) {
  const restartButton = document.getElementById("restart-button");
  const returnButtonContainer = document.getElementById("return-button-container");
  if (!restartButton || !returnButtonContainer) return;

  if (numConnections < 2) {
    // Désactive et masque le bouton de redémarrage
    restartButton.disabled = true;
    restartButton.style.display = 'none';

    // Affiche le bouton de retour
    returnButtonContainer.innerHTML = `
      <button
        class="redirect-button"
        onclick="window.location.href='/telecommande'"
      >Retour à la télécommande
      </button>
    `;
    returnButtonContainer.style.display = 'block';

  } else {
    // Si plus d'une connexion, assure que le bouton de redémarrage est visible et activé
    restartButton.style.display = 'block';
    restartButton.disabled = false;
    returnButtonContainer.style.display = 'none';
    returnButtonContainer.innerHTML = '';
  }
}
)rawliteral";
#endif