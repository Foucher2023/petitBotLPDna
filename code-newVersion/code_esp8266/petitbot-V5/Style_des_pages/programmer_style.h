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

.block:hover{
  box-shadow: 10px 10px 5px lightblue;
}

/* Loop styling */
.loop-block-content {
  display: flex;
  flex-direction: column;  /* Stack children vertically */
  gap: 5px;               /* Space between quick-add and body */
}

.loop-block {
  background: rgb(255, 215, 0); /* Gold */
  border: 2px dashed #ff8c00;
  margin: 5px 0;
  padding: 8px;
}

/* Loop quick-add buttons */
.loop-quick-add {
  display: flex;
  flex-wrap: wrap;
  gap: 5px;
  margin: 5px 0;
  padding: 5px;
  border-radius: 3px;
}

.loop-quick-add button {
  padding: 3px 6px;
  font-size: 12px;
  background: #4CAF50;
  color: white;
  border: none;
  border-radius: 3px;
  cursor: pointer;
  transition: background 0.2s;
}

.loop-quick-add button:hover {
  background: #45a049;
}

.loop-body {
  min-width: 70%;
  border: 1px dashed #ff8c00;
  margin: 5px 0;
  padding: 3%;
  background: rgba(255, 215, 0, 0.7);
}

.loop-child {
  margin: 3px;
  background: rgba(255, 255, 255, 0.5);
  border: 1px solid #ccc;
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

.place-btn:hover {
  background: #437e45;
}

.place-btn:active{
  transform: scale(0.8);

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

.controls .tertiary-buttons {
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

/* ===== OPTIMIZED INPUT STYLES ===== */
.inputBox{
  min-width: 6%;
  width: 8%;          /* Compact width */
  min-height: 4%;
  height: 6%;         /* Fixed height */
  padding: 0 2px;       /* Minimal padding */
  font-size: 12px;      /* Smaller text */
  text-align: center;
  margin: 0 3px;
  border: 1px solid #ccc;
  border-radius: 3px;
  background: white;
}

.inputBox.inProgInput{
  width: 20%;          /* Compact width */
  height: 12%;         /* Fixed height */
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
  will-change: transform;
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
  transform: scale(1.45);
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
)rawliteral";
#endif