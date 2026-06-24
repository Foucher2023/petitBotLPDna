#ifndef GLOBAL_STYLE_H
#define GLOBAL_STYLE_H

const char GLOBAL_CSS[] PROGMEM = R"rawliteral(
body {
  font-family: Arial;
  text-align: center;
  background: #111;
  color: #fff;
  margin: 0;
}

h1 {
  font-size: 6vw;
  margin-top:0;
}

input[type="radio"] {
  display: none;
}

.infoInBlack{
color: #000000;
}

)rawliteral";
#endif
