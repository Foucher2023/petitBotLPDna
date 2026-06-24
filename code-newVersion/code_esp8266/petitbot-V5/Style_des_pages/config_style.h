#ifndef CONFIG_STYLE_H
#define CONFIG_STYLE_H

const char CONFIG_CSS[] PROGMEM = R"rawliteral(

.h1{
font-size: 4vw;
}
input[type="text"] {
  background: #333;
  color: #fff;
  border: 1px solid #555;
  padding: 8px;
  margin: 5px;
  border-radius: 4px;
}

.button {
  font-size: 110%;
  font-weight: bold;
  padding: 1% 1.6%;
  margin: 10px;
  border: none;
  border-radius: 10px;
  background: #00adb5;
  color: white;
  scale: 100%;
}

.button-checked {
  background: #1a8000;
  padding: 0.9% 1.5%;
}

#updateButton:disabled {
  background-color: #cccccc;
  color: #666666;
  cursor: not-allowed;
}

)rawliteral";
#endif
