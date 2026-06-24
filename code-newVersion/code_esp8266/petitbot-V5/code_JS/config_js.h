  #ifndef CONFIG_JS_H
  #define CONFIG_JS_H



  const char CONFIG_JS[] PROGMEM = R"rawliteral(

    // changement d'état de la navbar sur mobile
    document.querySelector('.navbar-toggle').onclick = function() {
      document.getElementById('myNavbar').classList.toggle('open');
    };

  // Fonction unique pour le chargement de la page
  window.onload = function() {

    // Récupérer l'état depuis l'ESP
    fetch('/get-values-EEPROM')
      .then(response => response.text())
      .then(data => {
        // Séparer les valeurs par la virgule
        const [led, motorBF, motorLR,pinMotor1,pinMotor2,ssid] = data.split(',').map(String);

        // Stocker dans sessionStorage
        sessionStorage.setItem('ledState', led);
        sessionStorage.setItem('motorBF', motorBF);
        sessionStorage.setItem('motorLR', motorLR);
        sessionStorage.setItem('pinMotor1', pinMotor1);
        sessionStorage.setItem('pinMotor2', pinMotor2);
        sessionStorage.setItem('ssidName', ssid);

  // Fonction pour afficher le ssid
  function afficherSSID() {
      const statusElement1 = document.getElementById("statusSSID");
      const ssidState = sessionStorage.getItem('ssidName');
      statusElement1.textContent =
          `Actuellement, vous êtes connecter au PetitBot : ${ssidState}`;
  }

  afficherSSID();

      // Récupérer les valeurs actuelles des pins moteur
  const pinMotor1State = sessionStorage.getItem('pinMotor1');
  const pinMotor2State = sessionStorage.getItem('pinMotor2');

  function convertirPinEnNom(pin) {
      const mappingPins = {
          "16": "D0",
          "5":  "D1",
          "4":  "D2",
          "0":  "D3",
          "2":  "D4",
          "14": "D5",
          "12": "D6",
          "13": "D7",
          "15": "D8"
      };
      return mappingPins[pin] ? `${pin} = ${mappingPins[pin]}` : `Pin ${pin} (non mappé)`;
  }

  // Fonction pour afficher le statut des pins
  function afficherStatutPins() {
      const statusElement = document.getElementById("statusPinMotorSet");
      const moteur1 = convertirPinEnNom(pinMotor1State);
      const moteur2 = convertirPinEnNom(pinMotor2State);

      statusElement.textContent =
          `Actuellement, les moteurs sont branchés sur :
          Moteur 1 pin ${moteur1},
          Moteur 2 pin ${moteur2}`;
  }

  // Appel de la fonction pour afficher le statut
  afficherStatutPins();


        // Restaurer les états sauvegardés
        const ledState = sessionStorage.getItem('ledState');
        if (ledState) {
          const onBtn = document.getElementById('btnLed-on');
          const offBtn = document.getElementById('btnLed-off');
          onBtn.classList.remove('button-checked');
          offBtn.classList.remove('button-checked');
          if (ledState == 0) onBtn.classList.add('button-checked');
          else offBtn.classList.add('button-checked');
        }

        // Restaurer l'état des moteurs avant/arrière
        const motorStateFrontBack = sessionStorage.getItem('motorBF');
        if (motorStateFrontBack) {
          const yesBtn = document.getElementById('btnMotor-invert-yes');
          const noBtn = document.getElementById('btnMotor-invert-no');
          yesBtn.classList.remove('button-checked');
          noBtn.classList.remove('button-checked');
          if (motorStateFrontBack == 1) yesBtn.classList.add('button-checked');
          else noBtn.classList.add('button-checked');
        }

        // Restaurer l'état des moteurs gauche/droite
        const motorStateLeftRight = sessionStorage.getItem('motorLR');
        if (motorStateLeftRight) {
          const yesBtn = document.getElementById('btnMotor-invert-left-yes');
          const noBtn = document.getElementById('btnMotor-invert-left-no');
          yesBtn.classList.remove('button-checked');
          noBtn.classList.remove('button-checked');
          if (motorStateLeftRight == 1) yesBtn.classList.add('button-checked');
          else noBtn.classList.add('button-checked');
        }
      })
      .catch(error => console.error("Erreur :", error));

      //récupérer l'état de input de selection des gpio 
          checkButtonState();
      document.getElementById("pinMotor1").addEventListener("change", checkButtonState);
      document.getElementById("pinMotor2").addEventListener("change", checkButtonState);
  };

  // Fonction générique pour mettre à jour un paramètre
  function updateState({ param, value }) {
    const query = `/update-state?param=${encodeURIComponent(param)}&value=${encodeURIComponent(value)}`;

    fetch(query, { method: 'GET' })
      .then(response => response.text())
      .then(result => {
        // Met à jour le sessionStorage
        sessionStorage.setItem(param, value);
        // Met à jour l'UI
        updateUI(param, value);
      })
      .catch(error => console.error("Erreur :", error));
  }

  // Fonction pour mettre à jour l'UI en fonction du paramètre modifié
  function updateUI(param, value) {
    if (param === 'ledState') {
      const onBtn = document.getElementById('btnLed-on');
      const offBtn = document.getElementById('btnLed-off');
      onBtn.classList.remove('button-checked');
      offBtn.classList.remove('button-checked');
      if (value == 0) onBtn.classList.add('button-checked');
      else offBtn.classList.add('button-checked');
    }
    else if (param === 'motorBF') {
      const yesBtn = document.getElementById('btnMotor-invert-yes');
      const noBtn = document.getElementById('btnMotor-invert-no');
      yesBtn.classList.remove('button-checked');
      noBtn.classList.remove('button-checked');
      if (value == 1) yesBtn.classList.add('button-checked');
      else noBtn.classList.add('button-checked');
    }
    else if (param === 'motorLR') {
      const yesBtn = document.getElementById('btnMotor-invert-left-yes');
      const noBtn = document.getElementById('btnMotor-invert-left-no');
      yesBtn.classList.remove('button-checked');
      noBtn.classList.remove('button-checked');
      if (value == 1) yesBtn.classList.add('button-checked');
      else noBtn.classList.add('button-checked');
    }
  }

  // Fonctions pour les boutons (utilisent updateState)
  function controlLed(state) {
    let boolState ;
    if (state == 'ON'){boolState = 0;updateState({ param: 'ledState', value: boolState });}else if (state == 'OFF'){boolState = 1;updateState({ param: 'ledState', value: boolState });}else{updateState({ param: 'notgood', value: 1 });}
  }

  function motorInvertedFrontBack(state) {
      let boolState ;
      if (state === 'YES'){boolState = 1;updateState({ param: 'motorBF', value: boolState });}else if (state === 'NO'){boolState = 0;updateState({ param: 'motorBF', value: boolState });}else{updateState({ param: 'notgood', value: 2 });}
  }

  function motorInvertedLeftRight(state) {
    let boolState ;
      if (state == 'YES'){boolState = 1;updateState({ param: 'motorLR', value: boolState });}else if (state == 'NO'){boolState = 0;updateState({ param: 'motorLR', value: boolState });}else{updateState({ param: 'notgood', value: 3 });}
  }

  // Désactive le bouton si les deux sélections sont vides
  function checkButtonState() {
      const pinMotor1 = document.getElementById("pinMotor1").value;
      const pinMotor2 = document.getElementById("pinMotor2").value;
      const updateButton = document.getElementById("updateButton");

      // Désactive le bouton si les deux sélections sont "pas de modification"
      if (pinMotor1 === "" && pinMotor2 === "") {
          updateButton.disabled = true;
      } else {
          updateButton.disabled = false;
      }
  }

  // Convertit un nom de broche (ex: "D1") en numéro de GPIO
  function convertPinNameToGPIO(pinName) {
      const pinMap = {
          "" : "",
          "D0": 16,
          "D1": 5,
          "D2": 4,
          "D3": 0,
          "D4": 2,
          "D5": 14,
          "D6": 12,
          "D7": 13,
          "D8": 15
      };
      return pinMap[pinName] !== undefined ? pinMap[pinName] : -1; // Retourne -1 si invalide
  }

  // Exemple d'utilisation dans updatePinMotor
    function updatePinMotor() {
      const pinName1 = document.getElementById("pinMotor1").value; // Ex: "D1"
      const pinName2 = document.getElementById("pinMotor2").value; // Ex: "D2"
      const statusElement = document.getElementById("statusGPIO");
      if (pinName1 != "" && pinName2 != "" ){
      if (pinName1 === pinName2) {
          statusElement.textContent = "Erreur : Les deux broches ne peuvent pas être identiques !";
          statusElement.style.color = "red";
          return;
        }
      }

      const gpio1 = convertPinNameToGPIO(pinName1);
      const gpio2 = convertPinNameToGPIO(pinName2);


      if (gpio1 === -1 || gpio2 === -1) {
          alert("Nom de broche invalide. Utilisez D0 à D8.");
          return;
      }
  if(gpio1 != "" || gpio2 != ""){
  // Envoie les deux broches en tant qu'arguments séparés
      fetch(`/update-config?pinMotor1=${encodeURIComponent(gpio1)}&pinMotor2=${encodeURIComponent(gpio2)}`)
          .then(response => {
              if (response.ok) {
                      statusElement.textContent = `Broches mises à jour : Moteur 1 = GPIO${gpio1}, Moteur 2 = GPIO${gpio2}`;
              } else {
                  throw new Error("Erreur lors de la mise à jour.");
              }
          })
          .catch(error => {
              console.error("Erreur :", error);
              alert("Erreur : " + error.message);
          });
  }
      
  }


    // Mettre à jour le SSID
    function updateSSID() {
      const newSSID = document.getElementById("ssidInput").value;
      const statusElement = document.getElementById("statusSSID");

      if (!newSSID) {
        statusElement.textContent = "Veuillez saisir un SSID!";
        return;
      }

      fetch('/update-config?ssid=' + encodeURIComponent(newSSID))
        .then(response => {
          if (response.ok) {
            statusElement.textContent = "SSID mis à jour ! Redémarrage du Wi-Fi...";
            setTimeout(() => window.location.reload(), 2500);
          } else {
            statusElement.textContent = "Échec de la mise à jour du SSID.";
          }
        })
        .catch(error => {
          statusElement.textContent = "Erreur : " + error.message;
        });
    }

    // Réinitialiser la configuration
    function resetConfig() {
      fetch('/update-config?reset=all');
    }


  )rawliteral";
  #endif
