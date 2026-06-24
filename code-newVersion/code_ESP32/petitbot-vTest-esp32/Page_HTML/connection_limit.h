#ifndef CONNECTION_LIMIT_H
#define CONNECTION_LIMIT_H

const char CONNECTION_LIMIT_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Connexion Refusée</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="/connectionLimit.css">

</head>
<body>
  <h1>Connexion Refusée</h1>
  <div class="error-message">
    <p>Un appareil est déjà connecté au PetitBot.</p>
    <p>Veuillez déconnecter cet appareil avant de vous connecter.</p>
  </div>

<button class="restart-button" onclick="restartWiFi(this)">Redémarrer le réseau</button>
        <script src="/connectionLimit.js"></script>
</body>
</html>
)rawliteral";

#endif