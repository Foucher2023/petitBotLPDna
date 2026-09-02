#ifndef PROGRAMMER_CSS_H
#define PROGRAMMER_CSS_H

const char PROGRAMMER_CSS[] PROGMEM = R"rawliteral(
/* ===== Layout général ===== */
main.container {
  display: flex;
  flex-direction: column;
  gap: 20px;
  padding: 10px;
  margin: 0 auto;
  max-width: 1200px;
}

/* ===== Panneaux (blocs disponibles et programme) ===== */
.blocks-panel, .program-panel {
  background: white;
  border-radius: 10px;
  padding: 15px;
  box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
}

.blocks-panel {
  display: flex;
  flex-direction: column;
  gap: 10px;
}

.program-panel {
  display: flex;
  flex-direction: column;
  gap: 10px;
}

/* ===== Grille des blocs disponibles ===== */
.blocks-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
  gap: 10px;
}

.block-container {
  display: flex;
  align-items: center;
  gap: 10px;
  margin-bottom: 10px;
}

/* ===== Style des blocs ===== */
.block {
  background: #00adb5;
  color: white;
  padding: 10px;
  border-radius: 5px;
  cursor: grab;
  display: flex;
  justify-content: space-between;
  align-items: center;
  user-select: none;
  flex-grow: 1;
  min-width: 120px;
}

.block:hover {
  background: #008aa0;
}

/* Style pour les inputs dans les blocs (WAIT, LOOP) */
.inputBox {
  width: 50px;
  padding: 2px;
  text-align: center;
  margin: 0 5px;
  border: 1px solid #ccc;
  border-radius: 3px;
}

/* ===== Boutons "Placer" ===== */
.place-btn {
  background: #4CAF50;
  color: white;
  border: none;
  border-radius: 5px;
  padding: 8px 12px;
  cursor: pointer;
  white-space: nowrap;
  transition: background 0.2s ease;
}

.place-btn:hover {
  background: #45a049;
}

/* ===== Zone de programme ===== */
#program-area {
  min-height: 300px;
  background: #f9f9f9;
  border: 2px dashed #ccc;
  border-radius: 5px;
  padding: 10px;
  margin-bottom: 15px;
}

/* ===== Blocs déposés dans le programme ===== */
.dropped-block {
  background: #007c80;
  margin-bottom: 10px;
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 10px;
  border-radius: 5px;
  color: white;
  position: relative;
  transition: transform 0.2s ease, background 0.2s ease;
  will-change: transform;
}

.dropped-block:hover {
  background: #006b70;
}

/* Style pour les sous-blocs (dans LOOP) */
.loop-steps {
  display: flex;
  flex-direction: column;
  gap: 5px;
  margin-left: 20px;
  padding: 5px;
  background: rgba(255, 255, 255, 0.1);
  border-radius: 3px;
}

.nested-block {
  background: rgba(255, 255, 255, 0.2);
  padding: 5px;
  border-radius: 3px;
  font-size: 0.9em;
}

/* Contrôles des blocs (boutons ▲, ▼, ×) */
.dropped-block .block-controls {
  display: flex;
  flex-direction: column;
  margin-left: auto;
  margin-right: 10px;
}

.dropped-block .move-btn {
  background: #2196F3;
  color: white;
  border: none;
  width: 24px;
  height: 24px;
  border-radius: 3px;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 12px;
  margin: 2px 0;
  transition: all 0.2s ease;
}

.dropped-block .move-btn:hover:not(:disabled) {
  background: #1976D2;
  transform: scale(1.05);
}

.dropped-block .move-btn:disabled {
  background: #cccccc;
  cursor: not-allowed;
  opacity: 0.7;
}

.dropped-block .remove-btn {
  background: #ff6b6b;
  color: white;
  border: none;
  border-radius: 3px;
  padding: 2px 5px;
  cursor: pointer;
  margin-left: 5px;
  transition: all 0.2s ease;
}

.dropped-block .remove-btn:hover {
  background: #ff5252;
  transform: scale(1.05);
}

/* ===== Boutons de contrôle (Exécuter, Stop, etc.) ===== */
.controls {
  display: flex;
  flex-wrap: wrap;
  gap: 10px;
  justify-content: center;
}

.controls .main-buttons,
.controls .secondary-buttons,
.controls .tertiary-buttons {
  display: flex;
  gap: 10px;
  width: 100%;
  justify-content: center;
  margin-bottom: 10px;
}

#run-btn, #stop-btn {
  padding: 12px 24px;
  border: none;
  border-radius: 5px;
  color: white;
  cursor: pointer;
  font-size: 18px;
  font-weight: bold;
  transition: all 0.2s ease;
}

#run-btn {
  background: #4CAF50;
}

#stop-btn {
  background: #ff1100;
}

#run-btn:disabled, #stop-btn:disabled {
  opacity: 0.6;
  cursor: not-allowed;
}

/* Boutons secondaires et tertiaires */
.controls button:not(#run-btn):not(#stop-btn) {
  padding: 8px 15px;
  border: none;
  border-radius: 5px;
  background: #00adb5;
  color: white;
  cursor: pointer;
  transition: background 0.2s ease;
}

.controls button:not(#run-btn):not(#stop-btn):hover {
  background: #007c80;
}

/* ===== Titres ===== */
.program-title {
  color: black;
  margin-bottom: 15px;
  font-size: 1.2em;
}

/* ===== Animations (simplifiées) ===== */
.dropped-block.moving-up {
  transform: translateY(-5px);
}

.dropped-block.moving-down {
  transform: translateY(5px);
}

/* ===== Info en noir (compteurs) ===== */
.infoInBlack {
  color: black;
  font-size: 0.9em;
  margin: 0;
  text-align: center;
}
)rawliteral";
#endif
