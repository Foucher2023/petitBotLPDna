#ifndef INFO_H
#define INFO_H

const char INFO_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>PetitBot - OTA Update</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/global.css">
</head>
<body>
  <!-- ======== NAVBAR  ======== -->

  <div id="navbar-placeholder"></div>

    <h1 class="infoInBlack">Informations sur ce qu'est un petitbot</h1>
    <!-- ======== MESSAGE PRINCIPALE ======== -->

   
  <p class="infoInBlack">texte a modifier pour parler du projet des petit bot de Julien Rat et Corentin Foucher
    Ce projet est basé sur celui de Julien Rat des petits débrouillards https://github.com/julienrat/petitbot, 
    vous pouvez controller le robot en vous connectant par wifi via téléphone ou ordinateur.
    Vous pouvez le paramètrer, le programmer, et le mettre à jour directement via l'interface en ligne .
    Vous pouvez suivre le tutoriel pour réaliser cette version : https://www.wikidebrouillard.org/wiki/PetitBot-V5 
    </p>
    <!-- ======== Script JS ======== -->

  <script>
// ====================== Variables globales ====================

let checkConnectionInterval = null;

// ======================== Initialisation =======================
  window.onload = function() {
  // Récupère la navbar
  fetch('/navbar')
    .then(response => response.text())
    .then(html => {
      document.getElementById('navbar-placeholder').innerHTML = html;
      document.querySelector('.navbar-toggle')?.addEventListener('click', function() {
        document.getElementById('myNavbar').classList.toggle('open');
      });
      document.getElementById('button-info').classList.add('active');
    })
    .catch(error => console.error("Erreur lors du chargement de la navbar :", error));
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
  </script>
</body>
</html>
)rawliteral";
#endif
