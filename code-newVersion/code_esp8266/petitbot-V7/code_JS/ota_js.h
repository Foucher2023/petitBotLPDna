#ifndef OTA_JS_H
#define OTA_JS_H

const char OTA_JS[] PROGMEM = R"rawliteral(
document.addEventListener('DOMContentLoaded', function() {
  const hostname = window.location.hostname;
  const pathname = window.location.pathname;

  const theParam = "BASE_URL";
  if (!sessionStorage.getItem('BASE_URL')){
  fetch(`/get-static-value?param=${encodeURIComponent(theParam)}`)
  .then(response => response.text())
  .then(data => {
    let url_host = data + ".local";
    sessionStorage.setItem('Base_URL', url_host);
    })
    .catch(error => console.error("Erreur :", error));
  };
  
  // cacher le message important UNIQUEMENT si l'url est 192.168.4.1 ou le base_URL
  if (hostname === "192.168.4.1" || hostname === sessionStorage.getItem('BASE_URL')) {
  //todo check not working properly 
    document.getElementById('message-ota-important').style.display = 'none';
  }else {
    document.getElementById('form-ota-working-block').style.display = 'none';
  }
});

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
      showStatus('Erreur lors de la mise à jour: ' + xhr.responseText, 'error');
    }
  };

  xhr.onerror = function() {
    showStatus('Erreur de connexion', 'error');
  };

  showStatus('Mise à jour en cours...', 'success');
  xhr.send(formData);
};

function showStatus(message, type) {
  const statusElement = document.getElementById('statusMessage');
  statusElement.textContent = message;
  statusElement.className = 'ota-status ' + type;
  statusElement.style.display = 'block';
};
)rawliteral";
#endif
