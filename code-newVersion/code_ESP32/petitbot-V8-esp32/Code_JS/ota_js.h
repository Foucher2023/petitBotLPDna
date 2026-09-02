#ifndef OTA_JS_H
#define OTA_JS_H

const char OTA_JS[] PROGMEM = R"rawliteral(

// Initialise la page et vérifie l'URL pour afficher/masquer les éléments OTA 
document.addEventListener('DOMContentLoaded', function() {
  const hostname = window.location.hostname;
  const theParam = "BASE_URL";

  if (!sessionStorage.getItem('BASE_URL')){
  fetch(`/get-static-value?param=${encodeURIComponent(theParam)}`)
  .then(response => response.text())
  .then(data => {
        const url_host = data + ".local";
        sessionStorage.setItem('BASE_URL', url_host);
    })
    .catch(error => console.error("Erreur :", error));
  };
  
  // Masque le message important si l'URL est 192.168.4.1 ou BASE_URL
  if (hostname === "192.168.4.1" || hostname === sessionStorage.getItem('BASE_URL')) {
    document.getElementById('message-ota-important').style.display = 'none';
  }else {
    document.getElementById('form-ota-working-block').style.display = 'none';
  }
  startConnectionCheck();
});

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
// ===================== FONCTIONS POUR L'OTA =============================

// Démarre le processus OTA avec un fichier .bin 
function startOTA() {
  const fileInput = document.getElementById('fileInput');
  const file = fileInput.files[0];

  if (!file) {
    showStatus('Veuillez sélectionner un fichier .bin', 'error');
    return;
  }

  if (file.name.split('.').pop().toLowerCase() !== 'bin') {
    showStatus('Veuillez sélectionner un fichier .bin valide', 'error');
    return;
  }

  const formData = new FormData();
  formData.append('file', file);

  const xhr = new XMLHttpRequest();
  xhr.open('POST', '/update', true);

  xhr.upload.onprogress = function(e) {
    if (e.lengthComputable) {
      const percent = Math.round((e.loaded / e.total) * 100);
      document.getElementById('progressBar').style.width = percent + '%';
    }
  };

  xhr.onload = function() {
    if (xhr.status === 200) {
      showStatus('Mise à jour réussie! Redémarrage...', 'success');
      setTimeout(() => window.location.reload(), 3000);
    } else {
      showStatus('Erreur lors de la mise à jour : ' + xhr.responseText, 'error');
    }
  };

  xhr.onerror = function() {
    showStatus('Erreur de connexion', 'error');
  };

  showStatus('Mise à jour en cours...', 'success');
  xhr.send(formData);
};

// Affiche un message de statut avec un type (succès/erreur)
function showStatus(message, type) {
  const statusElement = document.getElementById('statusMessage');
  statusElement.textContent = message;
  statusElement.className = 'ota-status ' + type;
  statusElement.style.display = 'block';
};
)rawliteral";
#endif