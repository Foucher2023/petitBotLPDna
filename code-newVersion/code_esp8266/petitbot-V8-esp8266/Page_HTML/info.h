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

   
    <p class="infoInBlack">texte a modifier pour parler du projet des petit bot je julien rat et de corentin foucher</p>

    <!-- ======== Script JS ======== -->

  <script>
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
};

  </script>
</body>
</html>
)rawliteral";
#endif
