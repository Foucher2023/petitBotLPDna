#ifndef CONNECT_LIMIT_STYLE_H
#define CONNECT_LIMIT_STYLE_H

const char CONNECT_LIMIT_CSS[] PROGMEM = R"rawliteral(
/* ===== Variables CSS ===== */
:root {
  --color-error: #ff6b6b;
  --color-primary: #4CAF50;
  --color-primary-hover: #45a049;
  --color-text: #333;
  --font-family: Arial, sans-serif;
  --font-size: 16px;
  --padding: 20px;
  --padding-error: 15px;
  --border-radius: 5px;
  --max-width: 400px;
  --max-width-button: 200px;
  --transition: all 0.3s ease;
}

/* ===== Base ===== */
body {
  padding: var(--padding);
  font-family: var(--font-family);
  text-align: center;
  background: #f9f9f9;
  color: var(--color-text);
  margin: 0;
}

/* ===== Conteneur d'erreur ===== */
.error-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  margin: 0 auto;
  max-width: var(--max-width);
}

/* ===== Message d'erreur ===== */
.error-message {
  background: var(--color-error);
  padding: var(--padding-error);
  border-radius: var(--border-radius);
  margin: var(--padding) auto;
  max-width: var(--max-width);
  color: white;
  text-align: center;
}

/* ===== Bouton de redémarrage ===== */
.restart-button {
  background: var(--color-primary);
  color: white;
  padding: 10px 20px;
  border: none;
  border-radius: var(--border-radius);
  cursor: pointer;
  font-size: var(--font-size);
  margin: var(--padding) auto;
  display: block;
  max-width: var(--max-width-button);
  transition: var(--transition);
  font-weight: bold;
}

.restart-button:hover:not(:disabled) {
  background: var(--color-primary-hover);
}

.restart-button:disabled {
  background: #cccccc;
  color: #666666;
  cursor: not-allowed;
  opacity: 0.7;
}
/* ===== button redirect ===== */
.redirect-button {
  background: #00adb5;
  color: white;
  padding: 10px 20px;
  border: none;
  border-radius: var(--border-radius);
  cursor: pointer;
  font-size: var(--font-size);
  margin: var(--padding) auto;
  display: block;
  max-width: var(--max-width-button);
  transition: var(--transition);
  font-weight: bold;
}

.redirect-button:hover {
  background: #007c80;
}
/* ===== Titre ===== */
h1 {
  margin-top: 0;
  color: var(--color-text);
  font-size: 1.5em;
}
)rawliteral";
#endif
