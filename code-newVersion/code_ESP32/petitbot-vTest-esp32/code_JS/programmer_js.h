#ifndef PROGRAMMER_JS_H
#define PROGRAMMER_JS_H

const char PROGRAMMER_JS[] PROGMEM = R"rawliteral(
let program = [];
let isRunning = false;
let isAnimating = false; // Pour éviter les animations simultanées

// Fonction pour animer le déplacement d'un bloc
function animateBlockMove(blockElement, direction) {
  if (direction === 'up') {
    blockElement.classList.add('moving-up');
  } else if (direction === 'down') {
    blockElement.classList.add('moving-down');
  }

  // Retire la classe d'animation après la fin de l'animation
  setTimeout(() => {
    blockElement.classList.remove('moving-up', 'moving-down');
  }, 500);
}

// Permet le drop
window.allowDrop = function(ev) {
  ev.preventDefault();
};

// Glisser un bloc
window.drag = function(ev) {
  ev.dataTransfer.setData("text", ev.target.id);
  ev.dataTransfer.setData("type", ev.target.querySelector('span') ? ev.target.querySelector('span').textContent : 'WAIT');
  if (ev.target.id === 'wait-block') {
    const time = document.getElementById('wait-time').value;
    ev.dataTransfer.setData("time", time);
  }
};

// Déposer un bloc
window.drop = function(ev) {
  ev.preventDefault();
  const blockId = ev.dataTransfer.getData("text");
  const blockType = ev.dataTransfer.getData("type");
  const time = ev.dataTransfer.getData("time");

  addBlockToProgram(blockId, blockType, time);
};

// Fonction pour ajouter un bloc au programme
function addBlockToProgram(blockId, blockType, time, insertAt = -1) {
  const blockElement = document.createElement("div");
  blockElement.className = "dropped-block";

  let blockContent = "";
  if (blockType === 'WAIT') {
    blockContent = `<strong>Attendre</strong> <span>${time} seconde(s)</span>`;
  } else {
    const blockName = document.getElementById(blockId).querySelector('strong').textContent;
    blockContent = `<strong>${blockName}</strong>`;
  }

  blockElement.innerHTML = blockContent + `
    <div class="block-controls">
      <button class="move-btn up" onclick="moveBlockUp(this)" title="Monter" ${insertAt === 0 ? 'disabled' : ''}>▲</button>
      <button class="move-btn down" onclick="moveBlockDown(this)" title="Descendre">▼</button>
    </div>
    <button class="remove-btn" onclick="removeBlock(this)">×</button>
  `;

  // Si on insère à une position spécifique
  if (insertAt >= 0) {
    const programArea = document.getElementById("program-area");
    if (insertAt < programArea.children.length) {
      programArea.insertBefore(blockElement, programArea.children[insertAt]);
    } else {
      programArea.appendChild(blockElement);
    }
  } else {
    document.getElementById("program-area").appendChild(blockElement);
  }

  // Crée l'objet du bloc pour le programme
  const blockData = blockType === 'WAIT'
    ? { type: blockType, time: parseInt(time) }
    : { type: blockType };

  // Si on insère à une position spécifique
  if (insertAt >= 0) {
    program.splice(insertAt, 0, blockData);
  } else {
    program.push(blockData);
  }

  // Met à jour les boutons de déplacement
  updateMoveButtons();
};

// Monter un bloc
window.moveBlockUp = function(button) {
  if (isAnimating) return; // Empêche les animations simultanées

  const blockElement = button.closest('.dropped-block');
  const programArea = document.getElementById("program-area");
  const blocks = Array.from(programArea.children);
  const index = blocks.indexOf(blockElement);

  // Ne peut pas monter le premier bloc
  if (index <= 0) return;

  isAnimating = true;

  // Récupère le bloc précédent
  const prevBlock = blocks[index - 1];

  // Anime les deux blocs
  animateBlockMove(blockElement, 'up');
  animateBlockMove(prevBlock, 'down');

  // Échange les positions dans le DOM après un petit délai pour laisser le temps à l'animation
  setTimeout(() => {
    programArea.insertBefore(blockElement, prevBlock);

    // Échange les positions dans le tableau program
    [program[index], program[index - 1]] = [program[index - 1], program[index]];

    // Met à jour les boutons de déplacement
    updateMoveButtons();
    isAnimating = false;
  }, 600);
};

// Descendre un bloc
window.moveBlockDown = function(button) {
  if (isAnimating) return; // Empêche les animations simultanées

  const blockElement = button.closest('.dropped-block');
  const programArea = document.getElementById("program-area");
  const blocks = Array.from(programArea.children);
  const index = blocks.indexOf(blockElement);

  // Ne peut pas descendre le dernier bloc
  if (index >= blocks.length - 1) return;

  isAnimating = true;

  // Récupère le bloc suivant
  const nextBlock = blocks[index + 1];

  // Anime les deux blocs
  animateBlockMove(blockElement, 'down');
  animateBlockMove(nextBlock, 'up');

  // Échange les positions dans le DOM après un petit délai pour laisser le temps à l'animation
  setTimeout(() => {
    // Solution corrigée pour échanger les positions
    const temp = document.createElement('div');
    programArea.insertBefore(temp, blockElement);
    programArea.insertBefore(nextBlock, blockElement);
    programArea.insertBefore(blockElement, nextBlock.nextSibling);
    programArea.removeChild(temp);

    // Échange les positions dans le tableau program
    [program[index], program[index + 1]] = [program[index + 1], program[index]];

    // Met à jour les boutons de déplacement
    updateMoveButtons();
    isAnimating = false;
  }, 600);
};

// Mise à jour des boutons de déplacement
function updateMoveButtons() {
  const programArea = document.getElementById("program-area");
  const blocks = programArea.querySelectorAll('.dropped-block');

  blocks.forEach((block, index) => {
    const upButton = block.querySelector('.move-btn.up');
    const downButton = block.querySelector('.move-btn.down');

    // Désactive le bouton "Monter" pour le premier bloc
    if (upButton) upButton.disabled = index === 0;

    // Désactive le bouton "Descendre" pour le dernier bloc
    if (downButton) downButton.disabled = index === blocks.length - 1;
  });
}

// Placer un bloc avec le bouton "Placer"
window.placeBlock = function(blockId) {
  const block = document.getElementById(blockId);
  const blockType = block.querySelector('span').textContent;
  addBlockToProgram(blockId, blockType, 1);
};

// Placer un bloc "Attendre" avec le bouton "Placer"
window.placeWaitBlock = function() {
  const time = document.getElementById('wait-time').value;
  addBlockToProgram('wait-block', 'WAIT', time);
};

// Supprimer un bloc
window.removeBlock = function(button) {
  const blockElement = button.parentElement;
  const programArea = document.getElementById("program-area");
  const blocks = Array.from(programArea.children);
  const index = blocks.indexOf(blockElement);

  blockElement.remove();
  program.splice(index, 1);

  // Met à jour les boutons de déplacement
  updateMoveButtons();
};

// Effacer le programme
window.clearProgram = function() {
  fetch('/stopMotors');
  document.getElementById("program-area").innerHTML = "";
  program = [];
  isRunning = false;
  document.getElementById("run-btn").disabled = false;
  document.getElementById("stop-btn").disabled = true;
};

// Exécuter le programme
window.runProgram = function() {
  if (program.length === 0) {
    alert("Aucun bloc dans le programme !");
    return;
  }

  // Désactive les boutons pendant l'exécution
  document.getElementById("run-btn").disabled = true;
  document.getElementById("stop-btn").disabled = false;
  document.getElementById("clear-btn").disabled = true;
  document.getElementById("save-btn").disabled = true;
  document.getElementById("load-btn").disabled = true;
  isRunning = true;

  executeStep(0); // Commence l'exécution
};

// Arrêter le programme
window.stopProgram = function() {
  fetch('/stopMotors');
  isRunning = false;

  // Réactive les boutons
  document.getElementById("run-btn").disabled = false;
  document.getElementById("stop-btn").disabled = true;
  document.getElementById("clear-btn").disabled = false;
  document.getElementById("save-btn").disabled = false;
  document.getElementById("load-btn").disabled = false;
};

// Exécute une étape du programme
function executeStep(index) {
  if (index >= program.length || !isRunning) {
    // Fin du programme ou arrêt manuel
    document.getElementById("run-btn").disabled = false;
    document.getElementById("stop-btn").disabled = true;
    document.getElementById("clear-btn").disabled = false;
    document.getElementById("save-btn").disabled = false;
    document.getElementById("load-btn").disabled = false;
    isRunning = false;
    return;
  }

  const step = program[index];

  if (step.type === 'WAIT') {
    // Attendre X secondes
    setTimeout(() => {
      if (isRunning) executeStep(index + 1);
    }, step.time * 1000);
  } else if (step.type === 'STOP') {
    fetch('/stopMotors')
      .then(() => {
        if (isRunning) setTimeout(() => executeStep(index + 1), 500);
      })
      .catch(error => {
        console.error("Erreur:", error);
        document.getElementById("run-btn").disabled = false;
        document.getElementById("stop-btn").disabled = true;
        document.getElementById("clear-btn").disabled = false;
        document.getElementById("save-btn").disabled = false;
        document.getElementById("load-btn").disabled = false;
        isRunning = false;
      });
  } else {
    // Envoyer la commande au robot
    fetch(`/UseTelecommande?val=${step.type}`)
      .then(() => {
        if (isRunning) setTimeout(() => executeStep(index + 1), 500);
      })
      .catch(error => {
        console.error("Erreur:", error);
        document.getElementById("run-btn").disabled = false;
        document.getElementById("stop-btn").disabled = true;
        document.getElementById("clear-btn").disabled = false;
        document.getElementById("save-btn").disabled = false;
        document.getElementById("load-btn").disabled = false;
        isRunning = false;
      });
  }
}

// Sauvegarder le programme (dans le sessionStorage)
window.saveProgram = function() {
  sessionStorage.setItem("petitbotProgram", JSON.stringify(program));
  alert("Programme sauvegardé !");
};

// Charger un programme
window.loadProgram = function() {
  const savedProgram = sessionStorage.getItem("petitbotProgram");
  if (savedProgram) {
    program = JSON.parse(savedProgram);
    renderProgram();
    alert("Programme chargé !");
  } else {
    alert("Aucun programme sauvegardé.");
  }
};

// Affiche le programme chargé
window.renderProgram = function() {
  const programArea = document.getElementById("program-area");
  programArea.innerHTML = "";

  program.forEach((step, index) => {
    const blockElement = document.createElement("div");
    blockElement.className = "dropped-block";

    let blockContent = "";
    if (step.type === 'WAIT') {
      blockContent = `<strong>Attendre</strong> <span>${step.time} seconde(s)</span>`;
    } else {
      let blockName = "";
      switch (step.type) {
        case "FORWARD": blockName = "Avancer"; break;
        case "REVERSE": blockName = "Reculer"; break;
        case "LEFT": blockName = "Gauche"; break;
        case "RIGHT": blockName = "Droite"; break;
        case "STOP": blockName = "Stop"; break;
      }
      blockContent = `<strong>${blockName}</strong>`;
    }

    blockElement.innerHTML = blockContent + `
      <div class="block-controls">
        <button class="move-btn up" onclick="moveBlockUp(this)" title="Monter">▲</button>
        <button class="move-btn down" onclick="moveBlockDown(this)" title="Descendre">▼</button>
      </div>
      <button class="remove-btn" onclick="removeBlock(this)">×</button>
    `;

    programArea.appendChild(blockElement);
  });

  // Met à jour les boutons de déplacement
  updateMoveButtons();
};

// Gestion du menu navbar
document.querySelector('.navbar-toggle')?.addEventListener('click', function() {
  document.getElementById('myNavbar')?.classList.toggle('open');
});
)rawliteral";
#endif
