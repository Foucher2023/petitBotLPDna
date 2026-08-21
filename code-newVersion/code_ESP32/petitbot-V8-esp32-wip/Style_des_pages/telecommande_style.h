#ifndef TELECOMMANDE_STYLE_H
#define TELECOMMANDE_STYLE_H

const char TELECOMMANDE_CSS[] PROGMEM = R"rawliteral(
/* ===== Variables CSS ===== */
:root {
  --color-primary: #00adb5;
  --color-primary-active: #007c80;
  --color-stop: #ff0000;
  --color-stop-active: #a80000;
  --btn-size: 20vw;
  --btn-size-max: 80px;
  --arrow-size: 3.5vw;
  --arrow-size-mobile: 8vw;
  --stop-btn-size: 100px;
  --shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
  --transition: all 0.2s ease;
}

/* ===== Réinitialisation et base ===== */
body {
  -webkit-touch-callout: none;
  -webkit-user-select: none;
  -ms-user-select: none;
  user-select: none;
  -webkit-tap-highlight-color: transparent;
}

h1 {
  margin-top: 0;
  text-align: center;
}

/* ===== D-Pad (Directional Pad) ===== */
.dpad {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 3vh;
  margin: 20px auto;
  max-width: 300px;
}

.middle {
  display: flex;
  gap: 5vw;
}

/* ===== Boutons directionnels ===== */
.btnDir {
  touch-action: manipulation;
  width: var(--btn-size);
  height: var(--btn-size);
  max-width: var(--btn-size-max);
  max-height: var(--btn-size-max);
  border-radius: 15%;
  border: none;
  background: var(--color-primary);
  color: white;
  display: flex;
  align-items: center;
  justify-content: center;
  cursor: pointer;
  transition: var(--transition);
}

.btnDir .bigArrow {
  font-size: var(--arrow-size);
  transform: scale(1.2);
}

/* État actif des boutons directionnels */
.button-active {
  background: var(--color-primary-active) !important;
  transform: scale(0.9);
}

/* ===== Bouton STOP ===== */
.stop-container {
  text-align: center;
  margin-top: 20px;
}

.btnStop {
  touch-action: manipulation;
  background-color: var(--color-stop);
  color: white;
  border: none;
  border-radius: 50%;
  width: var(--stop-btn-size);
  height: var(--stop-btn-size);
  font-size: 24px;
  font-weight: bold;
  cursor: pointer;
  box-shadow: var(--shadow);
  transition: var(--transition);
}

/* État actif du bouton STOP */
.btnStop-active {
  background-color: var(--color-stop-active) !important;
  transform: scale(0.9);
}

/* ===== Adaptation mobile ===== */
@media only screen and (max-width: 768px) {
  .btnDir .bigArrow {
    font-size: var(--arrow-size-mobile);
  }

  .dpad {
    gap: 2vh;
  }

  .middle {
    gap: 3vw;
  }
}
)rawliteral";
#endif
