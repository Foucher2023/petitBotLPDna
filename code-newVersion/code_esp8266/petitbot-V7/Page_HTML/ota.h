#ifndef OTA_H
#define OTA_H

const char OTA_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>PetitBot - OTA Update</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/global.css">
  <link rel="stylesheet" href="/navbar.css">
  <link rel="stylesheet" href="/ota.css">
</head>
<body>
  <nav class="navbar navbar-inverse">
    <div class="container-fluid">
      <div class="navbar-header">
        <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#myNavbar">
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>
        <a class="navbar-brand" href="/">PetitBot</a>
      </div>
      <div class="collapse navbar-collapse" id="myNavbar">
        <ul class="nav navbar-nav">
          <li><a href="/config">Configuration</a></li>
          <li><a href="/telecommande">Télécommande</a></li>
          <li><a href="/programmer">Programmer</a></li>
          <li class="active"><a href="/ota">OTA Update</a></li>
        </ul>
      </div>
    </div>
  </nav>

  <div class="ota-container">
    <h1 class="infoInBlack">Mise à jour via OTA</h1>
    
    <div class="message-important" id="message-ota-important">Pour bien utiliser cette fonctionnalitée vous devez être connecter sur un navigateur avec l'url "192.168.4.1/ota" </div>
   
    <p class="infoInBlack">Sélectionnez un fichier .bin pour mettre à jour le PetitBot.</p>
  
    <form class="ota-form" id="otaForm">
      <input type="file" id="fileInput" class="ota-input" accept=".bin">
      <button type="button" class="ota-button" onclick="startOTA()">Démarrer la mise à jour</button>
    </form>

    <div class="ota-progress">
      <div class="ota-progress-bar" id="progressBar"></div>
    </div>

    <div class="ota-status" id="statusMessage"></div>
  </div>

  <script>
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
    }

    function showStatus(message, type) {
      const statusElement = document.getElementById('statusMessage');
      statusElement.textContent = message;
      statusElement.className = 'ota-status ' + type;
      statusElement.style.display = 'block';
    }
  </script>
</body>
</html>
)rawliteral";
#endif
