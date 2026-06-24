#ifndef GLOBAL_STYLE_H
#define GLOBAL_STYLE_H

const char GLOBAL_CSS[] PROGMEM = R"rawliteral(
/* ===== Variables CSS ===== */
:root {
  --color-bg: #111;
  --color-text: #fff;
  --color-title: #fff;
  --color-text-black: #000000;
  --font-family: Arial, sans-serif;
  --font-size-h1: 6vw;
}

/* ===== Base ===== */
body {
  font-family: var(--font-family);
  text-align: center;
  background: var(--color-bg);
  color: var(--color-text);
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

/* ===== Titres ===== */
h1 {
  font-size: var(--font-size-h1);
  margin: 0;
  padding: 0;
  color: var(--color-title);
}

/* ===== Inputs radio ===== */
input[type="radio"] {
  display: none;
}

/* ===== Texte en noir ===== */
.infoInBlack {
  color: var(--color-text-black);
}
)rawliteral";
#endif
