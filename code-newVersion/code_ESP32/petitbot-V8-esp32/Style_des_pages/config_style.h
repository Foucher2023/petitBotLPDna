#ifndef CONFIG_STYLE_H
#define CONFIG_STYLE_H

const char CONFIG_CSS[] PROGMEM = R"rawliteral(
/* ===== Variables CSS ===== */
:root {
  --color-primary: #00adb5;
  --color-checked: #1a8000;
  --color-disabled: #cccccc;
  --color-disabled-text: #666666;
  --color-input-bg: #333;
  --color-input-text: #fff;
  --color-input-border: #555;
  --font-size: 4vw;
  --font-size-button: 1.1em;
  --padding: 8px;
  --padding-button: 1% 1.6%;
  --padding-checked: 0.9% 1.5%;
  --margin: 5px;
  --margin-button: 10px;
  --border-radius: 4px;
  --border-radius-button: 10px;
  --transition: all 0.2s ease;
}

/* ===== Base ===== */
h1 {
  font-size: var(--font-size);
  margin-top: 0;
}

/* ===== Champs de texte ===== */
input[type="text"] {
  background: var(--color-input-bg);
  color: var(--color-input-text);
  border: 1px solid var(--color-input-border);
  padding: var(--padding);
  margin: var(--margin);
  border-radius: var(--border-radius);
  width: 100%;
  max-width: 300px;
  box-sizing: border-box;
}

/* ===== Boutons ===== */
.button {
  font-size: var(--font-size-button);
  font-weight: bold;
  padding: var(--padding-button);
  margin: var(--margin-button);
  border: none;
  border-radius: var(--border-radius-button);
  background: var(--color-primary);
  color: white;
  cursor: pointer;
  transition: var(--transition);
  display: inline-block;
  text-align: center;
}

/* État coché des boutons */
.button-checked {
  background: var(--color-checked);
  padding: var(--padding-checked);
}

/* Bouton désactivé */
#updateButton:disabled {
  background-color: var(--color-disabled);
  color: var(--color-disabled-text);
  cursor: not-allowed;
}

/* ===== Sélecteurs ===== */
select {
  background: var(--color-input-bg);
  color: var(--color-input-text);
  border: 1px solid var(--color-input-border);
  padding: var(--padding);
  margin: var(--margin);
  border-radius: var(--border-radius);
  width: 100%;
  max-width: 300px;
  box-sizing: border-box;
  cursor: pointer;
}

/* ===== Statuts ===== */
p[id^="status"] {
  margin: 5px 0;
  font-size: 0.9em;
  color: #666;
}

/* ===== Sections de configuration ===== */
.config-section {
  margin-bottom: 20px;
  padding: 15px;
  background: white;
  border-radius: 8px;
  box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
}

.config-section h2 {
  margin-top: 0;
  color: #333;
  font-size: 1.2em;
}

/* ===== Formulaires ===== */
.config-form {
  margin: 10px 0;
  text-align: center; /* Centre les éléments enfants */
}

.config-form fieldset {
  border: none;
  padding: 0;
  margin: 0;
  display: flex;
  gap: 10px;
  justify-content: center; /* Centre les boutons horizontalement */
  flex-wrap: wrap; /* Permet aux boutons de passer à la ligne si nécessaire */
}

/* Style pour les labels dans les fieldset */
.config-form fieldset label.button {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  margin: var(--margin);
}

/* ===== Groupes de sélecteurs ===== */
.pin-selectors {
  display: flex;
  flex-direction: column;
  gap: 10px;
  margin: 10px 0;
  align-items: center; /* Centre les sélecteurs horizontalement */
}

.pin-selector {
  display: flex;
  flex-direction: column;
  gap: 5px;
  width: 100%;
  max-width: 300px; /* Limite la largeur pour éviter que les sélecteurs ne soient trop larges */
  align-items: center; /* Centre les labels et sélecteurs */
}

.pin-selector label {
  font-weight: bold;
  color: #333;
  width: 100%;
  text-align: center; /* Centre le texte du label */
}

.pin-selector select {
  width: 100%;
  max-width: 300px;
}

/* Centre le bouton "Changer les broches" */
#updateButton {
  display: block;
  margin: 10px auto; /* Centre le bouton horizontalement */
}

.ota-update-button-container {
    margin: 20px 0;
    text-align: center;
  }

  .ota-update-button {
    display: inline-block;
    padding: 10px 20px;
    background: #4cc207;
    color: white;
    text-decoration: none;
    border-radius: 5px;
    font-weight: bold;
    transition: background 0.3s;
  }

  .ota-update-button:hover {
    background: #67fd10;
  }
)rawliteral";
#endif
