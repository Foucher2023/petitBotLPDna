#ifndef TELECOMMANDE_STYLE_H
#define TELECOMMANDE_STYLE_H

const char TELECOMMANDE_CSS[] PROGMEM = R"rawliteral(


body {
  -webkit-touch-callout: none;
  -webkit-user-select: none;
  -ms-user-select: none;
  user-select: none;
  -webkit-tap-highlight-color: transparent;
}

h1 {
  font-size: 6vw;
  margin-top:0;
}

.dpad {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 3vh;
}

.middle {
  display: flex;
  gap: 5vw;
}

.btnDir {
  touch-action: manipulation;
  width: 20vw;
  height: 20vw;
  max-width: 80px;
  max-height: 80px;
  border-radius: 15%;
  border: none;
  background: #00adb5;
  color: white;
  display: flex;
  align-items: center;
  justify-content: center;
}

/* Taille de police par défaut */
.btnDir .bigArrow {
  font-size: 3.5vw;
  transform: scale(1.2);
}

/* Taille de police pour les téléphones */
@media only screen and (max-width: 768px) {
  .btnDir .bigArrow {
    font-size: 8vw !important; /* Force la taille sur mobile */
  }
}

.button-active {
  background: #007c80;
  transform: scale(0.9);
}

.stop-container {
    text-align: center;
    margin-top: 20px;
}
.btnStop {
    touch-action: manipulation;
    background-color: #ff0000;
    color: white;
    border: none;
    border-radius: 50%;
    width: 100px;
    height: 100px;
    font-size: 24px;
    font-weight: bold;
    cursor: pointer;
    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
}
.btnStop-active {
    background-color: #a80000;
    transform: scale(0.9);

}
)rawliteral";
#endif
