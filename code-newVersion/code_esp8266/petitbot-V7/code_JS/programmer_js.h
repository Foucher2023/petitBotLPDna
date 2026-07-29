#ifndef PROGRAMMER_JS_H
#define PROGRAMMER_JS_H

const char PROGRAMMER_JS[] PROGMEM = R"rawliteral(
window.onload = function() {
fetch('/navbar')
    .then(response => response.text())
    .then(html => {
      document.getElementById('navbar-placeholder').innerHTML = html;
      document.querySelector('.navbar-toggle')?.addEventListener('click', function() {
        document.getElementById('myNavbar').classList.toggle('open');
      });
      document.getElementById('button-programmation').classList.add('active');
    })
    .catch(error => console.error("Erreur lors du chargement de la navbar :", error));
};

/*****************************************************************
 * PetitBot - Block Programming Interface
 * --------------------------------------------
 * A visual programming environment for controlling the PetitBot robot.
 * Supports drag-and-drop block programming with loops and nested structures.
 *****************************************************************/

// ====================== GLOBAL STATE ======================
let program = [];
let isRunning = false;
let isAnimating = false;

// Mappage des noms de blocs pour éviter les répétitions
const blockNames = {
  FORWARD: "Avancer",
  REVERSE: "Reculer",
  LEFT: "Gauche",
  RIGHT: "Droite",
  STOP: "Stop",
  WAIT: "Attendre",
  LOOP: "Boucle"
};

// ====================== UTILITY FUNCTIONS ======================
/**
 * Compte le nombre de blocs d'un type donné dans le programme (y compris dans les boucles)
 * @param {Array} program - Le programme ou sous-programme (pour les boucles)
 * @param {string} type - Le type de bloc à compter
 * @returns {number} - Le nombre de blocs du type spécifié
 */
function countBlocksByType(program, type) {
  let count = 0;
  for (const step of program) {
    if (step.type === type) count++;
    if (step.type === "LOOP" && step.children) {
      count += countBlocksByType(step.children, type);
    }
  }
  return count;
}

/**
 * Compte le nombre total de blocs dans le programme (y compris dans les boucles)
 * @param {Array} program - Le programme ou sous-programme
 * @returns {number} - Le nombre total de blocs
 */
function countTotalBlocks(program) {
  let count = program.length;
  for (const step of program) {
    if (step.type === "LOOP" && step.children) {
      count += countTotalBlocks(step.children);
    }
  }
  return count;
}

/**
 * Met à jour les compteurs de blocs dans l'interface
 */
function updateBlockCounters() {
  document.getElementById("NumbForward").textContent = `Nb : ${countBlocksByType(program, "FORWARD")}`;
  document.getElementById("NumbReverse").textContent = `Nb : ${countBlocksByType(program, "REVERSE")}`;
  document.getElementById("NumbLeft").textContent = `Nb : ${countBlocksByType(program, "LEFT")}`;
  document.getElementById("NumbRight").textContent = `Nb : ${countBlocksByType(program, "RIGHT")}`;
  document.getElementById("NumbStop").textContent = `Nb : ${countBlocksByType(program, "STOP")}`;
  document.getElementById("NumbWait").textContent = `Nb : ${countBlocksByType(program, "WAIT")}`;
  document.getElementById("NumbLoop").textContent = `Nb : ${countBlocksByType(program, "LOOP")}`;
  document.getElementById("NumbBlock").textContent = `Nombre de blocs : ${countTotalBlocks(program)}`;
}

/**
 * Supprime les références circulaires pour la sérialisation JSON
 * @param {Object} obj - L'objet à nettoyer
 * @returns {Object} - L'objet sans références circulaires
 */
function removeCircularReferences(obj) {
  const seen = new WeakSet();
  return JSON.parse(JSON.stringify(obj, (key, value) => {
    if (typeof value === "object" && value !== null) {
      if (seen.has(value)) return;
      seen.add(value);
    }
    if (key === "element" || key === "parent") return;
    return value;
  }));
}

// ====================== ANIMATION UTILITIES ======================
/**
 * Anime le déplacement d'un bloc
 * @param {HTMLElement} blockElement - L'élément bloc à animer
 * @param {string} direction - 'up' ou 'down'
 */
function animateBlockMove(blockElement, direction) {
  if (direction === 'up') {
    blockElement.classList.add('moving-up');
    blockElement.classList.remove('moving-down');
  } else if (direction === 'down') {
    blockElement.classList.add('moving-down');
    blockElement.classList.remove('moving-up');
  }
  setTimeout(() => {
    blockElement.classList.remove('moving-up', 'moving-down');
  }, 300);
}

// ====================== DRAG AND DROP HANDLERS ======================
/**
 * Permet le drop sur les zones cibles
 * @param {DragEvent} ev - L'événement de drop
 */
window.allowDrop = function(ev) {
  ev.preventDefault();
};

/**
 * Gère le début du drag pour les blocs
 * @param {DragEvent} ev - L'événement de drag
 */
window.drag = function(ev) {
  if (!ev.target.classList.contains('block')) return;
  ev.dataTransfer.setData("text/plain", ev.target.id);
  ev.dataTransfer.setData("block-type", ev.target.dataset.blockType);

  if (ev.target.id === 'wait-block') {
    const time = document.getElementById('wait-time').value;
    ev.dataTransfer.setData("time", time);
  } else if (ev.target.id === 'loop-block') {
    const iterations = document.getElementById('loop-iterations').value;
    ev.dataTransfer.setData("iterations", iterations);
  }
};

/**
 * Gère le drop des blocs
 * @param {DragEvent} ev - L'événement de drop
 */
window.drop = function(ev) {
  ev.preventDefault();
  ev.stopPropagation();

  const blockId = ev.dataTransfer.getData("text/plain");
  const blockType = ev.dataTransfer.getData("block-type");
  const time = ev.dataTransfer.getData("time");
  const iterations = ev.dataTransfer.getData("iterations");

  // Vérifie si on drop dans une boucle
  const loopBody = ev.target.closest('.loop-body');
  if (loopBody) {
    const loopBlock = loopBody.closest('.dropped-block');
    const loopIndex = parseInt(loopBlock.dataset.stepIndex);
    if (loopIndex >= 0 && program[loopIndex]?.type === 'LOOP') {
      addBlockToProgram(blockId, blockType, time, -1, iterations, program[loopIndex]);
      return;
    }
  }

  // Sinon, ajoute au programme principal
  addBlockToProgram(blockId, blockType, time, -1, iterations);
};

// ====================== BLOCK MANAGEMENT ======================
/**
 * Ajoute un bloc au programme
 * @param {string} blockId - ID du bloc
 * @param {string} blockType - Type de bloc (FORWARD, REVERSE, etc.)
 * @param {number|null} time - Temps pour les blocs WAIT
 * @param {number} insertAt - Index d'insertion (-1 pour ajouter à la fin)
 * @param {number|null} iterations - Nombre d'itérations pour les boucles
 * @param {Object|null} parentLoop - Boucle parente si on ajoute dans une boucle
 */
function addBlockToProgram(blockId, blockType, time, insertAt = -1, iterations = null, parentLoop = null) {
  if (isRunning || isAnimating) return;

  // Crée l'objet de données du bloc
  const blockData = {
    type: blockType,
    ...(blockType === 'WAIT' && { time: parseInt(time) || 1 }),
    ...(blockType === 'LOOP' && {
      iterations: parseInt(iterations) || 3,
      children: []
    })
  };

  // Ajoute au programme ou à la boucle parente
  if (parentLoop) {
    parentLoop.children.push(blockData);
    blockData.parent = parentLoop;
  } else if (insertAt >= 0) {
    program.splice(insertAt, 0, blockData);
  } else {
    program.push(blockData);
  }

  // Met à jour l'interface
  renderProgram();
}

/**
 * Met à jour le nombre d'itérations d'une boucle
 * @param {HTMLElement} input - L'élément input qui a changé
 */
window.updateLoopIterations = function(input) {
  const blockElement = input.closest('.dropped-block');
  const blockIndex = parseInt(blockElement.dataset.stepIndex);
  if (blockIndex >= 0 && program[blockIndex]?.type === 'LOOP') {
    program[blockIndex].iterations = parseInt(input.value) || 1;
  }
};

/**
 * Ajoute un bloc directement dans une boucle
 * @param {string} blockType - Type de bloc à ajouter
 * @param {Event} event - L'événement clic
 */
window.placeBlockInLoop = function(blockType, event) {
  if (isRunning || isAnimating) return;

  const loopBlock = event.target.closest('.dropped-block');
  if (!loopBlock) return;

  const loopIndex = parseInt(loopBlock.dataset.stepIndex);
  if (loopIndex < 0 || !program[loopIndex] || program[loopIndex].type !== "LOOP") return;

  // Crée le nouveau bloc
  const newBlock = { type: blockType };
  if (blockType === "WAIT") newBlock.time = 1;

  // Ajoute à la boucle
  if (!program[loopIndex].children) program[loopIndex].children = [];
  program[loopIndex].children.push(newBlock);

  // Met à jour l'interface
  renderProgram();
};

// ====================== BLOCK PLACEMENT ======================
/**
 * Place un bloc standard via son bouton
 * @param {string} blockId - ID du bloc à placer
 */
window.placeBlock = function(blockId) {
  const block = document.getElementById(blockId);
  const blockType = block.dataset.blockType;
  addBlockToProgram(blockId, blockType, 1);
};

/**
 * Place un bloc LOOP via son bouton
 */
window.placeLoopBlock = function() {
  const iterations = document.getElementById('loop-iterations').value;
  addBlockToProgram('loop-block', 'LOOP', null, -1, iterations);
};

/**
 * Place un bloc WAIT via son bouton
 */
window.placeWaitBlock = function() {
  const time = document.getElementById('wait-time').value;
  addBlockToProgram('wait-block', 'WAIT', time);
};

// ====================== BLOCK MOVEMENT ======================
/**
 * Déplace un bloc vers le haut
 * @param {HTMLElement} button - Le bouton "▲" cliqué
 */
window.moveBlockUp = function(button) {
  if (isAnimating) return;

  const blockElement = button.closest('.dropped-block');
  const loopBody = blockElement.closest('.loop-body');
  const parent = loopBody || document.getElementById("program-area");
  const blocks = Array.from(parent.children);
  const index = blocks.indexOf(blockElement);

  if (index <= 0) return;

  isAnimating = true;
  const prevBlock = blocks[index - 1];

  // Anime les blocs
  animateBlockMove(blockElement, 'up');
  animateBlockMove(prevBlock, 'down');

  // Déplace dans le DOM après l'animation
  setTimeout(() => {
    parent.insertBefore(blockElement, prevBlock);

    // Met à jour le tableau program ou children
    if (loopBody) {
      const loopBlock = loopBody.closest('.dropped-block');
      const loopIndex = parseInt(loopBlock.dataset.stepIndex);
      if (loopIndex >= 0 && program[loopIndex]?.type === "LOOP") {
        [program[loopIndex].children[index], program[loopIndex].children[index - 1]] =
          [program[loopIndex].children[index - 1], program[loopIndex].children[index]];
      }
    } else {
      [program[index], program[index - 1]] = [program[index - 1], program[index]];
    }

    updateMoveButtons();
    isAnimating = false;
  }, 300);
};

/**
 * Déplace un bloc vers le bas
 * @param {HTMLElement} button - Le bouton "▼" cliqué
 */
window.moveBlockDown = function(button) {
  if (isAnimating) return;

  const blockElement = button.closest('.dropped-block');
  const loopBody = blockElement.closest('.loop-body');
  const parent = loopBody || document.getElementById("program-area");
  const blocks = Array.from(parent.children);
  const index = blocks.indexOf(blockElement);

  if (index >= blocks.length - 1) return;

  isAnimating = true;
  const nextBlock = blocks[index + 1];

  // Anime les blocs
  animateBlockMove(blockElement, 'down');
  animateBlockMove(nextBlock, 'up');

  // Déplace dans le DOM après l'animation
  setTimeout(() => {
    parent.insertBefore(nextBlock, blockElement);

    // Met à jour le tableau program ou children
    if (loopBody) {
      const loopBlock = loopBody.closest('.dropped-block');
      const loopIndex = parseInt(loopBlock.dataset.stepIndex);
      if (loopIndex >= 0 && program[loopIndex]?.type === "LOOP") {
        [program[loopIndex].children[index], program[loopIndex].children[index + 1]] =
          [program[loopIndex].children[index + 1], program[loopIndex].children[index]];
      }
    } else {
      [program[index], program[index + 1]] = [program[index + 1], program[index]];
    }

    updateMoveButtons();
    isAnimating = false;
  }, 300);
};

/**
 * Met à jour l'état des boutons de déplacement (▲ et ▼)
 */
function updateMoveButtons() {
  const programArea = document.getElementById("program-area");
  const allBlocks = programArea.querySelectorAll('.dropped-block, .loop-body .dropped-block');

  allBlocks.forEach((block) => {
    const parent = block.closest('.loop-body') || programArea;
    const blocks = Array.from(parent.children);
    const index = blocks.indexOf(block);
    const upButton = block.querySelector('.move-btn.up');
    const downButton = block.querySelector('.move-btn.down');

    if (upButton) upButton.disabled = index === 0;
    if (downButton) downButton.disabled = index === blocks.length - 1;
  });
}

// ====================== BLOCK REMOVAL ======================
/**
 * Supprime un bloc du programme
 * @param {HTMLElement} button - Le bouton "×" cliqué
 */
window.removeBlock = function(button) {
  if (isRunning || isAnimating) return;

  const blockElement = button.closest('.dropped-block');
  const loopBody = blockElement.closest('.loop-body');
  const programArea = document.getElementById("program-area");

  // Si le bloc est dans une boucle
  if (loopBody) {
    const loopBlock = loopBody.closest('.dropped-block');
    const loopIndex = parseInt(loopBlock.dataset.stepIndex);
    const childIndex = Array.from(loopBody.children).indexOf(blockElement);

    if (loopIndex >= 0 && program[loopIndex]?.type === "LOOP" && program[loopIndex].children) {
      program[loopIndex].children.splice(childIndex, 1);
    }
  }
  // Si le bloc est dans le programme principal
  else {
    const blockIndex = parseInt(blockElement.dataset.stepIndex);
    if (blockIndex >= 0 && blockIndex < program.length) {
      program.splice(blockIndex, 1);
    }
  }

  // Supprime du DOM
  blockElement.remove();
  updateMoveButtons();
  updateBlockCounters();
};

// ====================== PROGRAM CONTROL ======================
/**
 * Efface le programme actuel
 */
window.clearProgram = function() {
  if (isRunning) return;
  fetch('/stopMotors').catch(() => {});
  program = [];
  renderProgram();
};

/**
 * Efface le programme sauvegardé dans sessionStorage
 */
window.clearSessionStorage = function() {
  if (sessionStorage.getItem("petitbotProgram")) {
    sessionStorage.removeItem("petitbotProgram");
    alert("✅ Programme supprimé de la mémoire du navigateur");
  }
};

/**
 * Démarre l'exécution du programme
 */
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
  document.getElementById("export-btn").disabled = true;

  isRunning = true;
  executeStep(0);
};

/**
 * Arrête l'exécution du programme
 */
window.stopProgram = function() {
  isRunning = false;
  const stopMotors = () => fetch('/stopMotors').catch(() => {});
  stopMotors();
  setTimeout(stopMotors, 100);
  setTimeout(stopMotors, 200);

  // Réactive les boutons
  document.getElementById("run-btn").disabled = false;
  document.getElementById("stop-btn").disabled = true;
  document.getElementById("clear-btn").disabled = false;
  document.getElementById("save-btn").disabled = false;
  document.getElementById("load-btn").disabled = false;
  document.getElementById("export-btn").disabled = false;
};

// ====================== PROGRAM EXECUTION ======================
/**
 * Exécute une étape du programme
 * @param {number} index - Index de l'étape à exécuter
 */
function executeStep(index) {
  if (index >= program.length || !isRunning) {
    stopProgram();
    return;
  }

  const step = program[index];

  if (step.type === 'LOOP') {
    executeLoop(step, 0, () => executeStep(index + 1));
  }
  else if (step.type === 'WAIT') {
    setTimeout(() => isRunning && executeStep(index + 1), step.time * 1000);
  }
  else if (step.type === 'STOP') {
    fetch('/stopMotors')
      .then(() => isRunning && setTimeout(() => executeStep(index + 1), 500))
      .catch(() => stopProgram());
  }
  else {
    fetch(`/UseTelecommande?val=${step.type}`)
      .then(() => isRunning && setTimeout(() => executeStep(index + 1), 500))
      .catch(() => stopProgram());
  }
}

/**
 * Exécute une boucle
 * @param {Object} loop - Données de la boucle
 * @param {number} iteration - Itération actuelle (0-based)
 * @param {Function} callback - Fonction à appeler à la fin de la boucle
 */
function executeLoop(loop, iteration, callback) {
  if (iteration >= loop.iterations || !isRunning) {
    callback();
    return;
  }

  executeChildren(loop.children, 0, () => {
    executeLoop(loop, iteration + 1, callback);
  });
}

/**
 * Exécute les enfants d'une boucle
 * @param {Array} children - Tableau des enfants
 * @param {number} childIndex - Index de l'enfant actuel
 * @param {Function} callback - Fonction à appeler à la fin
 */
function executeChildren(children, childIndex, callback) {
  if (childIndex >= children.length || !isRunning) {
    callback();
    return;
  }

  const child = children[childIndex];

  if (child.type === 'WAIT') {
    setTimeout(() => isRunning && executeChildren(children, childIndex + 1, callback), child.time * 1000);
  }
  else if (child.type === 'LOOP') {
    executeLoop(child, 0, () => isRunning && executeChildren(children, childIndex + 1, callback));
  }
  else if (child.type === 'STOP') {
    fetch('/stopMotors')
      .then(() => isRunning && setTimeout(() => executeChildren(children, childIndex + 1, callback), 500))
      .catch(() => stopProgram());
  }
  else {
    fetch(`/UseTelecommande?val=${child.type}`)
      .then(() => isRunning && setTimeout(() => executeChildren(children, childIndex + 1, callback), 500))
      .catch(() => stopProgram());
  }
}

// ====================== PROGRAM SAVING/LOADING ======================
/**
 * Sauvegarde le programme dans sessionStorage
 */
window.saveProgramSession = function() {
  const cleanProgram = removeCircularReferences(program);
  sessionStorage.setItem("petitbotProgram", JSON.stringify(cleanProgram));
  alert("✅ Programme sauvegardé dans le navigateur");
};

/**
 * Exporte le programme sous forme de fichier JSON
 */
window.saveProgramLocal = function() {
  if (isValidUrl()) {
    const cleanProgram = removeCircularReferences(program);
    const data = JSON.stringify(cleanProgram, null, 2);
    const blob = new Blob([data], { type: 'application/json' });
    const url = URL.createObjectURL(blob);

    const a = document.createElement('a');
    a.href = url;
    a.download = `petitbot_${new Date().toLocaleDateString()}.json`;
    document.body.appendChild(a);
    a.click();

    setTimeout(() => {
      document.body.removeChild(a);
      URL.revokeObjectURL(url);
    }, 100);
  }
};

/**
 * Charge un programme depuis sessionStorage ou un fichier
 */
window.loadProgram = function() {
  // Essaye de charger depuis sessionStorage
  const savedProgram = sessionStorage.getItem("petitbotProgram");
  if (savedProgram) {
    try {
      program = JSON.parse(savedProgram);
      renderProgram();
      alert("✅ Programme chargé depuis le navigateur !");
      return;
    } catch (e) {
      console.error("Erreur sessionStorage:", e);
    }
  }

  // Sinon, utilise un sélecteur de fichier
  const input = document.createElement('input');
  input.type = 'file';
  input.accept = '.json,application/json';
  input.style.display = 'none';

  input.onchange = function(e) {
    document.body.removeChild(input);
    const file = e.target.files[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = function(e) {
      try {
        program = JSON.parse(e.target.result);
        sessionStorage.setItem("petitbotProgram", e.target.result);
        renderProgram();
        alert("✅ Programme chargé depuis le fichier !");
      } catch (e) {
        alert("❌ Erreur: fichier JSON invalide.");
      }
    };
    reader.onerror = function() {
      alert("❌ Erreur de lecture du fichier.");
    };
    reader.readAsText(file);
  };

  document.body.appendChild(input);
  input.click();
};

// ====================== PROGRAM RENDERING ======================
/**
 * Rend le programme dans le DOM
 */
window.renderProgram = function() {
  const programArea = document.getElementById("program-area");
  programArea.innerHTML = "";

  program.forEach((step, index) => {
    const blockElement = document.createElement("div");
    blockElement.className = "dropped-block";
    blockElement.dataset.stepIndex = index;

    if (step.type === 'LOOP') {
      blockElement.classList.add('loop-block');
      blockElement.innerHTML = `
        <strong>${blockNames.LOOP}</strong>
        <span>
          <input type="number" value="${step.iterations || 3}" min="1"
                 class="inputBox inProgInput" onchange="updateLoopIterations(this)">
          fois
        </span>
        <div class="loop-block-content">
          <div class="loop-quick-add">
            <button data-action="place-in-loop" data-block-type="FORWARD" title="Ajouter AVANCER">AVANCER</button>
            <button data-action="place-in-loop" data-block-type="LEFT" title="Ajouter GAUCHE">GAUCHE</button>
            <button data-action="place-in-loop" data-block-type="RIGHT" title="Ajouter DROITE">DROITE</button>
            <button data-action="place-in-loop" data-block-type="REVERSE" title="Ajouter RECULER">RECULER</button>
            <button data-action="place-in-loop" data-block-type="STOP" title="Ajouter STOP">STOP</button>
            <button data-action="place-in-loop" data-block-type="WAIT" title="Ajouter ATTENDRE">ATTENDRE</button>
          </div>
          <div class="loop-body" data-action="drop-zone"></div>
        </div>
        <div class="block-controls">
          <button class="move-btn up" data-action="move-up" title="Monter" ${index === 0 ? 'disabled' : ''}>▲</button>
          <button class="move-btn down" data-action="move-down" title="Descendre">▼</button>
        </div>
        <button class="remove-btn" data-action="remove" title="Supprimer">×</button>
      `;
    } else {
      const blockName = blockNames[step.type] || step.type;
      let content = `<strong>${blockName}</strong>`;
      if (step.type === 'WAIT') {
        content += ` <span>${step.time} seconde(s)</span>`;
      }
      blockElement.innerHTML = `
        ${content}
        <div class="block-controls">
          <button class="move-btn up" data-action="move-up" title="Monter" ${index === 0 ? 'disabled' : ''}>▲</button>
          <button class="move-btn down" data-action="move-down" title="Descendre">▼</button>
        </div>
        <button class="remove-btn" data-action="remove" title="Supprimer">×</button>
      `;
    }

    programArea.appendChild(blockElement);
    step.element = blockElement;

    // Rend les enfants si c'est une boucle
    if (step.type === 'LOOP' && step.children) {
      renderLoopChildren(step.children, blockElement.querySelector('.loop-body'));
    }
  });

  updateMoveButtons();
  updateBlockCounters();
};

/**
 * Rend les enfants d'une boucle
 * @param {Array} children - Tableau des enfants
 * @param {HTMLElement} loopBody - Élément DOM où rendre les enfants
 */
function renderLoopChildren(children, loopBody) {
  loopBody.innerHTML = "";
  children.forEach((child, childIndex) => {
    const childElement = document.createElement("div");
    childElement.className = "dropped-block loop-child";
    childElement.dataset.childIndex = childIndex;

    let childContent = `<strong>${blockNames[child.type] || child.type}</strong>`;
    if (child.type === 'WAIT') {
      childContent += ` <span>${child.time} seconde(s)</span>`;
    }

    childElement.innerHTML = `
      ${childContent}
      <div class="block-controls">
        <button class="move-btn up" data-action="move-up" title="Monter">▲</button>
        <button class="move-btn down" data-action="move-down" title="Descendre">▼</button>
      </div>
      <button class="remove-btn" data-action="remove" title="Supprimer">×</button>
    `;

    loopBody.appendChild(childElement);
    child.element = childElement;

    // Rend les enfants récursivement si c'est une boucle
    if (child.type === 'LOOP' && child.children) {
      const childLoopBody = document.createElement("div");
      childLoopBody.className = "loop-body";
      childElement.appendChild(childLoopBody);
      renderLoopChildren(child.children, childLoopBody);
    }
  });
}

// ====================== EVENT DELEGATION ======================
// Gestion centralisée des événements via délégation
document.addEventListener("DOMContentLoaded", () => {
  // Gestion des boutons "Placer"
  document.addEventListener("click", (e) => {
    const action = e.target.closest("[data-action]")?.dataset.action;
    const blockType = e.target.closest("[data-block-type]")?.dataset.blockType;
    const blockId = e.target.closest("[data-block-id]")?.dataset.blockId;

    if (action === "place" && blockId) {
      placeBlock(blockId);
    } else if (action === "place-loop") {
      placeLoopBlock();
    } else if (action === "place-wait") {
      placeWaitBlock();
    } else if (action === "place-in-loop" && blockType) {
      placeBlockInLoop(blockType, e);
    }
  });

  // Gestion des boutons de contrôle (Exécuter, Stop, etc.)
  document.addEventListener("click", (e) => {
    const action = e.target.closest("[data-action]")?.dataset.action;
    if (action === "run") runProgram();
    else if (action === "stop") stopProgram();
    else if (action === "clear") clearProgram();
    else if (action === "save-session") saveProgramSession();
    else if (action === "clear-session") clearSessionStorage();
    else if (action === "export") saveProgramLocal();
    else if (action === "load") loadProgram();
  });

  // Gestion des boutons de mouvement (▲ et ▼)
  document.addEventListener("click", (e) => {
    const action = e.target.closest("[data-action]")?.dataset.action;
    if (action === "move-up") moveBlockUp(e.target.closest(".move-btn"));
    else if (action === "move-down") moveBlockDown(e.target.closest(".move-btn"));
  });

  // Gestion des boutons de suppression (×)
  document.addEventListener("click", (e) => {
    if (e.target.closest("[data-action='remove']")) {
      removeBlock(e.target.closest(".remove-btn"));
    }
  });

  // Gestion du drag & drop
  document.addEventListener("dragstart", (e) => {
    if (e.target.classList.contains("block")) {
      drag(e);
    }
  });

  // Gestion des zones de drop
  document.addEventListener("dragover", (e) => {
    if (e.target.closest("[data-action='drop-zone']") || e.target.closest(".loop-body")) {
      allowDrop(e);
    }
  });

  // Charger le programme depuis sessionStorage au démarrage
  loadProgram();
});

//==============================================================================================
// Fonction pour vérifier l'URL et mettre à jour le/les boutons
function isValidUrl() {
//todo baseURL
  return window.location.hostname === '192.168.4.1';
}

function enableButton(button) {
  button.style.opacity = '1';
  button.style.cursor = 'pointer';
  button.style.backgroundColor = '';
  button.textContent = 'Exporter le programme';
}

function disableButton(button) {
  button.style.opacity = '0.5';
  button.style.cursor = 'not-allowed';
  button.style.backgroundColor = '';
  button.textContent = 'Exporter le programme';
}

function handleCaptivePortalClick(button) {
  button.textContent = 'Quitter le portail captif';
  button.style.backgroundColor = 'red';
  button.style.opacity = '1';

  setTimeout(() => {
    disableButton(button);
  }, 2000);
}

function initializeExportButton() {
  const exportButton = document.getElementById('export-btn');
  if (!exportButton) return;

  if (isValidUrl()) {
    enableButton(exportButton);
  } else {
    disableButton(exportButton);
    exportButton.addEventListener('click', () => handleCaptivePortalClick(exportButton), { once: true });
  }
}

window.addEventListener('DOMContentLoaded', initializeExportButton);
)rawliteral";
#endif
