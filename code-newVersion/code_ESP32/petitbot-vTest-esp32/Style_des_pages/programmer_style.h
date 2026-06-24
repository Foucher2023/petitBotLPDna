#ifndef PROGRAMMER_CSS_H
#define PROGRAMMER_CSS_H

const char PROGRAMMER_CSS[] PROGMEM = R"rawliteral(
.container {
  display: flex;
  flex-direction: column;
  gap: 20px;
  padding: 10px;
}

.blocks-panel, .program-panel {
  background: white;
  border-radius: 10px;
  padding: 15px;
  box-shadow: 0 2px 5px rgba(0,0,0,0.1);
}

.blocks-panel {
  display: flex;
  flex-direction: column;
  gap: 10px;
}

.block-container {
  display: flex;
  align-items: center;
  gap: 10px;
  margin-bottom: 10px;
}

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
}

.place-btn {
  background: #4CAF50;
  color: white;
  border: none;
  border-radius: 5px;
  padding: 10px;
  cursor: pointer;
  white-space: nowrap;
}

.block:hover {
  background: #008aa0;
}

#program-area {
  min-height: 300px;
  background: #f9f9f9;
  border: 2px dashed #ccc;
  border-radius: 5px;
  padding: 10px;
  margin-bottom: 15px;
}

.controls {
  display: flex;
  flex-wrap: wrap;
  gap: 10px;
  justify-content: center;
}

.controls .main-buttons {
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

.hide {
  display: none;
}

.controls .secondary-buttons {
  display: flex;
  gap: 10px;
  width: 100%;
  justify-content: center;
}

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
  transition: all 0.3s ease;
  will-change: transform; /* Optimisation pour les animations */
}

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

.program-title {
  color: black;
  margin-bottom: 15px;
}

/* Animation pour le déplacement des blocs */
@keyframes moveUp {
  0%, 100% { transform: translateY(0); }
  50% { transform: translateY(-10px); }
}

@keyframes moveDown {
  0%, 100% { transform: translateY(0); }
  50% { transform: translateY(10px); }
}

.moving-up {
  animation: moveUp 0.3s ease-in-out;
}

.moving-down {
  animation: moveDown 0.3s ease-in-out;
}


/* Assure que les blocs ont la propriété transform */
.dropped-block {
  will-change: transform; /* Optimisation pour les animations */
}
)rawliteral";
#endif
