#ifndef CONNECT_LIMIT_STYLE_H
#define CONNECT_LIMIT_STYLE_H



const char CONNECT_LIMIT_CSS[] PROGMEM = R"rawliteral(
 body {
      padding: 20px;
      font-family: Arial, sans-serif;
      text-align: center;
    }
    .error-message {
      background: #ff6b6b;
      padding: 15px;
      border-radius: 5px;
      margin: 20px auto;
      max-width: 400px;
    }
    .restart-button {
      background: #4CAF50;
      color: white;
      padding: 10px 20px;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      font-size: 16px;
      margin: 20px auto;
      display: block;
      max-width: 200px;
    }
    .restart-button:hover {
      background: #45a049;
    }
)rawliteral";
#endif