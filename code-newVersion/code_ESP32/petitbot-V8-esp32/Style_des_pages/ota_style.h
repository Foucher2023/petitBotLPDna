#ifndef OTA_STYLE_H
#define OTA_STYLE_H

const char OTA_CSS[] PROGMEM = R"rawliteral(

:root{
  --font-size-brand: 5vw;
}

/*navbar*/
.navbar-brand {
  color: var(--color-text);
  text-decoration: none;
  font-size: var(--font-size-brand);
  font-weight: bold;
}

.navbar-inverse {
    background-color: #222;
    transition: background-color 0.3s ease;
}

.navbar-inverse:hover {
    background-color: #3498db;
}
    
/* Conteneur principal pour la page OTA */
.ota-container {
  max-width: 600px;
  margin: 20px auto;
  padding: 20px;
  background: white;
  border-radius: 10px;
  box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
}

/* Style du formulaire OTA */
.ota-form {
  display: flex;
  flex-direction: column; /* Force l'affichage vertical */
  gap: 15px; /* Espace entre les éléments */
  align-items: center; /* Aligne les éléments à gauche */
}

/* Style des champs d'entrée */
.ota-input {
  padding: 10px 0;
  border: 1px solid #838383;
  border-radius: 5px;
  color: black;
  display: flex;
  width: 100%;
}

/* ========== Style des boutons ============*/
.ota-button {
  background: #4CAF50;
  color: white;
  padding: 12px;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  font-size: 16px;
  margin-top: 1vh;
  align-self: flex-start;
}

.ota-button:hover {
  background: #45a049;
}

/* ============== Barre de progression ==================*/
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

/* Messages de statut */
.ota-status {
  margin-top: 10px;
  padding: 10px;
  border-radius: 5px;
  display: none;
}

.success {
  background: #dff0d8;
  color: #3c763d;
}

.error {
  background: #f2dede;
  color: #a94442;
}

/* Message important */
.message-important {
  background: #ffcccc;
  color: #cc0000;
  padding: 8px;
  border-radius: 4px;
  margin: 10px 0;
  font-weight: bold;
  text-align: center;
}
)rawliteral";
#endif
