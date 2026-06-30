#ifndef OTA_STYLE_H
#define OTA_STYLE_H

const char OTA_CSS[] PROGMEM = R"rawliteral(
 .ota-container {
      max-width: 600px;
      margin: 20px auto;
      padding: 20px;
      background: white;
      border-radius: 10px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
    }
    .ota-form {
      display: flex;
      flex-direction: column;
      gap: 15px;
    }
    .ota-input {
      padding: 10px;
      border: 1px solid #ddd;
      border-radius: 5px;
    }
    .ota-button {
      background: #4CAF50;
      color: white;
      padding: 12px;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      font-size: 16px;
    }
    .ota-button:hover {
      background: #45a049;
    }
    .ota-progress {
      margin-top: 20px;
      height: 20px;
      background: #f0f0f0;
      border-radius: 5px;
      overflow: hidden;
    }
    .ota-progress-bar {
      height: 100%;
      background: #4CAF50;
      width: 0%;
      transition: width 0.3s;
    }
    .ota-status {
      margin-top: 10px;
      padding: 10px;
      border-radius: 5px;
      display: none;
    }
    .success { background: #dff0d8; color: #3c763d; }
    .error { background: #f2dede; color: #a94442; }
    .message-important {
      background: #ffcccc;
      color: #cc0000;
      padding: 8px;
      border-radius: 4px;
      margin: 10px 0;
      font-weight: bold;
      text-align: center;
    }
)rawliteral";
#endif
