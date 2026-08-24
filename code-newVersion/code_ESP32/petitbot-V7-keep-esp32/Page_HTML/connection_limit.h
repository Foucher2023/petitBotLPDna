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
      <div class="error-message">
        <p>Un appareil est déjà connecté au PetitBot.</p>
        <p>Veuillez déconnecter cet appareil avant de vous connecter.</p>
      </div>
    </section>

    <!-- ======== BOUTTON POUR REDEMARRER LA WIFI DU ROBOT ======== -->

    <div id="restart-button">
    <button class="restart-button" data-action="restart-wifi" aria-label="Redémarrer le réseau Wi-Fi">
      Redémarrer le réseau
    </button>
    </div>
        <!-- ======== Conteneur pour le bouton de retour (affiché dynamiquement) ======== -->

    <div id="return-button-container" style="display: none; text-align: center; margin-top: 20px;"></div>
  </main>

  <script src="/connectionLimit.js"></script>
</body>
</html>
)rawliteral";
#endif
