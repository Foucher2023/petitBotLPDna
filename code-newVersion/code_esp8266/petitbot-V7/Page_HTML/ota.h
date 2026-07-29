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
  <link rel="stylesheet" href="/ota.css">
</head>
<body>
  <nav class="navbar navbar-inverse">
    <div class="container-fluid">
      <div class="navbar-header">
        <a class="navbar-brand" href="/">PetitBot</a>
      </div>
    </div>
  </nav>

  <div class="ota-container">
    <h1 class="infoInBlack">Mise à jour via OTA</h1>
    
    <div style="display: block; class="message-important" id="message-ota-important">Pour bien utiliser cette fonctionnalitée vous devez être connecter sur un navigateur avec l'url "192.168.4.1/ota" </div>
   
    <p class="infoInBlack">Sélectionnez un fichier .bin pour mettre à jour le PetitBot.</p>
  
    <div id="form-ota-working-block" style="display: block;">
      <div class="ota-form">
        <form id="otaForm">
          <input type="file" id="fileInput" class="ota-input" accept=".bin">
          <button type="button" class="ota-button" onclick="startOTA()">Démarrer la mise à jour</button>
        </form>
      </div>
    </div>

    <div class="ota-progress">
      <div class="ota-progress-bar" id="progressBar"></div>
    </div>

    <div class="ota-status" id="statusMessage"></div>
  </div>

  <script src="/ota.js"></script>
</body>
</html>
)rawliteral";
#endif
