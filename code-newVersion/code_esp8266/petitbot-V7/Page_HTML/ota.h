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
  <style>
    .ota-container {
      max-width: 600px;
      margin: 20px auto;
      padding: 20px;
      background: white;
      border-radius: 10px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
    }
    .ota-form {
      display: flex;
      flex-direction: column;
      gap: 15px;
    }
    .ota-input {
      padding: 10px;
      border: 1px solid #ddd;
      border-radius: 5px;
    }
    .ota-button {
      background: #4CAF50;
      color: white;
      padding: 12px;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      font-size: 16px;
    }
    .ota-button:hover {
      background: #45a049;
    }
    .ota-progress {
      margin-top: 20px;
      height: 20px;
      background: #f0f0f0;
      border-radius: 5px;
      overflow: hidden;
    }
    .ota-progress-bar {
      height: 100%;
      background: #4CAF50;
      width: 0%;
      transition: width 0.3s;
    }
    .ota-status {
      margin-top: 10px;
      padding: 10px;
      border-radius: 5px;
      display: none;
    }
    .success { background: #dff0d8; color: #3c763d; }
    .error { background: #f2dede; color: #a94442; }
    .message-important {
      background: #ffcccc;
      color: #cc0000;
      padding: 8px;
      border-radius: 4px;
      margin: 10px 0;
      font-weight: bold;
      text-align: center;
    }
  </style>
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
    <div class="message-important">Pour bien utiliser cette fonctionnalitée vous devez être connecter sur un navigateur avec l'url "192.168.4.1/ota" </div>
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
