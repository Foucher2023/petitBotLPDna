#ifndef CONNECTION_LIMIT_JS_H
#define CONNECTION_LIMIT_JS_H



const char CONNECT_LIMIT_JS[] PROGMEM = R"rawliteral(
    function restartWiFi(button) {
    // Disable the button immediately
    button.disabled = true;
    button.textContent = "Redémarrage en cours...";

    fetch('/restart-wifi')
        .then(response => response.text())
        .then(data => {
            alert("Le réseau va redémarrer. Veuillez vous reconnecter.");
            window.location.href = "/loading"; // Redirect to loading page
        })
        .catch(error => {
            alert("Erreur lors du redémarrage du réseau.");
            // Re-enable the button if there's an error
            button.disabled = false;
            button.textContent = "Redémarrer le réseau";
        });
}

)rawliteral";
#endif