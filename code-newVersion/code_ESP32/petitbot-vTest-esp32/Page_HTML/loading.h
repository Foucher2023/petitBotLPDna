#ifndef LOADING_H
#define LOADING_H

const char LOADING_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Chargement en cours</title>
  <style>
    body {
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
      background: #111;
      flex-direction: column;
    }

    .loader {
      border: 20px solid #f3f3f3;
      border-top: 20px solid #3498db;
      border-radius: 50%;
      width: 200px;
      height: 200px;
      animation: spin 2s linear infinite;
      margin-bottom: 30px;
    }

    @keyframes spin {
      0% { transform: rotate(0deg); }
      100% { transform: rotate(360deg); }
    }

    .message {
      color: white;
      font-family: Arial, sans-serif;
      font-size: 20px;
      text-align: center;
    }
  </style>
</head>
<body>
  <div class="loader"></div>
  <div class="message" id="message">Connexion en cours...</div>

  <script>
    let attempts = 0;
    const maxAttempts = 10; // Nombre maximum de tentatives
    const retryDelay = 5000; // Délai entre les tentatives (5 secondes)

    function tryRedirect() {
      attempts++;
      const messageElement = document.getElementById('message');

      // Met à jour le message
      if (attempts <= maxAttempts) {
        messageElement.textContent = `Tentative ${attempts}/${maxAttempts}... Redirection vers la télécommande.`;
      } else {
        messageElement.textContent = "Impossible de se connecter. Veuillez vérifier votre connexion.";
        return;
      }

      // Tente la redirection
      fetch('/telecommande')
        .then(response => {
          if (response.ok) {
            window.location.href = '/telecommande';
          } else {
            // Si la page n'est pas accessible, réessaye après un délai
            setTimeout(tryRedirect, retryDelay);
          }
        })
        .catch(error => {
          // En cas d'erreur, réessaye après un délai
          setTimeout(tryRedirect, retryDelay);
        });
    }

    // Démarre la première tentative après un court délai
    setTimeout(tryRedirect, 1000);
  </script>
</body>
</html>
)rawliteral";

#endif