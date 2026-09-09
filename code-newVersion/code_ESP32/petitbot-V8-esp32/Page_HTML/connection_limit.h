#ifndef CONNECTION_LIMIT_H
#define CONNECTION_LIMIT_H

const char CONNECTION_LIMIT_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <title>Connexion Refusée - PetitBot</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/connectionLimit.css">
</head>
<body>
  <main>
    <h1>Connexion Refusée</h1>
    
    <!-- ======== MESSAGE PRINCIPALE ======== -->
    <section class="error-container" role="alert" aria-live="polite">

      <div id="WarningMessageContainer"class="error-message">
        <p>Un appareil est déjà connecté au PetitBot.</p>
        <p id="ssidText" style="display:none;"></p>
        <p>Veuillez déconnecter cet appareil avant de pouvoir utiliser le robot.</p>
        <p>Vous pouvais aussi redémarrer entièrement le réseaux pour déconnecter tout le monde d'un seul coup ! </p>
      </div>
      <!-- ======== Conteneur pour le message OK (affiché dynamiquement) ======== -->
    <div id="OKMessageContainer" style="display: none;"></div>
    <div id="WaitingMessageContainer" style="display: none;"></div>
    
    </section>

    <div class="spinner-container" id="spinnerContainer">
    <div class="spinner"></div>
  </div>

  <div class="content" id="contentButton">
    <!-- ======== BOUTTON POUR REDEMARRER LA WIFI DU ROBOT ======== -->

    <div id="restartButtonContainer">
      <button class="restart-button" data-action="restart-wifi" aria-label="Redémarrer le réseau Wi-Fi">Redémarrer le réseau</button>
    </div>
      <!-- ======== Conteneur pour le bouton de retour (affiché dynamiquement) ======== -->
    <div id="returnButtonContainer" style="display: none; text-align: center; margin-top: 20px;"></div>

  </div>  

</main>

  <script src="/connectionLimit.js"></script>
</body>
</html>
)rawliteral";
#endif
