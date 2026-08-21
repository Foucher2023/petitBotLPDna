#ifndef PROGRAMMER_UI_JS_H
#define PROGRAMMER_UI_JS_H

const char PROGRAMMER_UI_JS[] PROGMEM = R"rawliteral(
// ====================== INITIALISATION ======================
document.addEventListener('DOMContentLoaded', function() {
  const theParam = "BASE_URL";
  if (!sessionStorage.getItem('BASE_URL')) {
    fetch(`/get-static-value?param=${encodeURIComponent(theParam)}`)
      .then(response => response.text())
      .then(data => {
        let url_host = data + ".local";
        sessionStorage.setItem('BASE_URL', url_host);
      })
      .then(() => { initializeExportButton(); })
      .catch(error => console.error("Erreur :", error));
  }
  updateUI();
});

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

// ====================== ANIMATIONS ======================
// Anime le déplacement d'un bloc
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

// ====================== GESTION DU DRAG AND DROP ======================
// Permet le drop sur les zones cibles
window.allowDrop = function(ev) {
  ev.preventDefault();
};

document.addEventListener("drop", (e) => {
  if (e.target.closest("[data-action='drop-zone']") || e.target.closest(".loop-body")) {
    drop(e);
  }
});

// Gère le début du drag pour les blocs
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

// Gère le drop des blocs
window.drop = function(ev) {
  ev.preventDefault();
  const blockId = ev.dataTransfer.getData("text/plain");
  const blockType = ev.dataTransfer.getData("block-type");
  const time = ev.dataTransfer.getData("time");
  const iterations = ev.dataTransfer.getData("iterations");

  const loopBody = ev.target.closest('.loop-body');
  if (loopBody) {
    const loopBlock = loopBody.closest('.dropped-block');
    const loopIndex = parseInt(loopBlock.dataset.stepIndex);
    if (loopIndex >= 0 && Core.program[loopIndex]?.type === 'LOOP') {
      Core.addBlockToProgram(blockId, blockType, time, -1, iterations, Core.program[loopIndex]);
      renderProgram();
      return;
    }
  }
  Core.addBlockToProgram(blockId, blockType, time, -1, iterations);
  renderProgram();
};

// ====================== PLACEMENT DES BLOCS ======================
// Place un bloc standard via son bouton
window.placeBlock = function(blockId) {
  const block = document.getElementById(blockId);
  const blockType = block.dataset.blockType;
  Core.addBlockToProgram(blockId, blockType, 1);
  renderProgram();
};

// Place un bloc LOOP via son bouton
window.placeLoopBlock = function() {
  const iterations = document.getElementById('loop-iterations').value;
  Core.addBlockToProgram('loop-block', 'LOOP', null, -1, iterations);
  renderProgram();
};

// Place un bloc WAIT via son bouton
window.placeWaitBlock = function() {
  const time = document.getElementById('wait-time').value;
  Core.addBlockToProgram('wait-block', 'WAIT', time);
  renderProgram();
};

// ====================== DÉPLACEMENT DES BLOCS ======================
// Déplace un bloc vers le haut
window.moveBlockUp = function(button) {
  if (Core.isAnimating) return;

  const blockElement = button.closest('.dropped-block');
  const loopBody = blockElement.closest('.loop-body');
  const parent = loopBody || document.getElementById("program-area");
  const blocks = Array.from(parent.children);
  const index = blocks.indexOf(blockElement);

  if (index <= 0) return;

  Core.setAnimating(true);
  const prevBlock = blocks[index - 1];

  animateBlockMove(blockElement, 'up');
  animateBlockMove(prevBlock, 'down');

  setTimeout(() => {
    parent.insertBefore(blockElement, prevBlock);

    if (loopBody) {
      const loopBlock = loopBody.closest('.dropped-block');
      const loopIndex = parseInt(loopBlock.dataset.stepIndex);
      if (loopIndex >= 0 && Core.program[loopIndex]?.type === "LOOP") {
        [Core.program[loopIndex].children[index], Core.program[loopIndex].children[index - 1]] =
          [Core.program[loopIndex].children[index - 1], Core.program[loopIndex].children[index]];
      }
    } else {
      [Core.program[index], Core.program[index - 1]] = [Core.program[index - 1], Core.program[index]];
    }

    updateMoveButtons();
    Core.setAnimating(false);
  }, 300);
};

// Déplace un bloc vers le bas
window.moveBlockDown = function(button) {
  if (Core.isAnimating) return;

  const blockElement = button.closest('.dropped-block');
  const loopBody = blockElement.closest('.loop-body');
  const parent = loopBody || document.getElementById("program-area");
  const blocks = Array.from(parent.children);
  const index = blocks.indexOf(blockElement);

  if (index >= blocks.length - 1) return;

  Core.setAnimating(true);
  const nextBlock = blocks[index + 1];

  animateBlockMove(blockElement, 'down');
  animateBlockMove(nextBlock, 'up');

  setTimeout(() => {
    parent.insertBefore(nextBlock, blockElement);

    if (loopBody) {
      const loopBlock = loopBody.closest('.dropped-block');
      const loopIndex = parseInt(loopBlock.dataset.stepIndex);
      if (loopIndex >= 0 && Core.program[loopIndex]?.type === "LOOP") {
        [Core.program[loopIndex].children[index], Core.program[loopIndex].children[index + 1]] =
          [Core.program[loopIndex].children[index + 1], Core.program[loopIndex].children[index]];
      }
    } else {
      [Core.program[index], Core.program[index + 1]] = [Core.program[index + 1], Core.program[index]];
    }

    updateMoveButtons();
    Core.setAnimating(false);
  }, 300);
};

// Met à jour l'état des boutons de déplacement (▲ et ▼)
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
};

// ====================== AFFICHAGE DU PROGRAMME ======================
// Rend le programme dans le DOM
window.renderProgram = function() {
  const programArea = document.getElementById("program-area");
  programArea.innerHTML = "";

  Core.program.forEach((step, index) => {
    const blockElement = document.createElement("div");
    blockElement.className = "dropped-block";
    blockElement.dataset.stepIndex = index;

    if (step.type === 'LOOP') {
      blockElement.classList.add('loop-block');
      blockElement.innerHTML = `
        <strong>${Core.blockNames.LOOP}</strong>
        <span>
          <input type="number" value="${step.iterations || 3}" min="1" class="inputBox inProgInput" onchange="Core.updateLoopIterations(this)">fois
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
      const blockName = Core.blockNames[step.type] || step.type;
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

    if (step.type === 'LOOP' && step.children) {
      renderLoopChildren(step.children, blockElement.querySelector('.loop-body'));
    }
  });

  updateUI();
};

// Rend les enfants d'une boucle
function renderLoopChildren(children, loopBody) {
  loopBody.innerHTML = "";
  children.forEach((child, childIndex) => {
    const childElement = document.createElement("div");
    childElement.className = "dropped-block loop-child";
    childElement.dataset.childIndex = childIndex;

    let childContent = `<strong>${Core.blockNames[child.type] || child.type}</strong>`;
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

    if (child.type === 'LOOP' && child.children) {
      const childLoopBody = document.createElement("div");
      childLoopBody.className = "loop-body";
      childElement.appendChild(childLoopBody);
      renderLoopChildren(child.children, childLoopBody);
    }
  });
}

// ====================== DÉLÉGATION D'ÉVÉNEMENTS ======================
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
      const loopBlock = e.target.closest('.dropped-block');
      const loopIndex = parseInt(loopBlock.dataset.stepIndex);
      Core.placeBlockInLoop(blockType, loopIndex);
      renderProgram();
    }
  });

  // Gestion des boutons de contrôle (Exécuter, Stop, etc.)
  document.addEventListener("click", (e) => {
    const action = e.target.closest("[data-action]")?.dataset.action;
    if (action === "run") {
      Core.setRunning(true);
      Core.executeStep(0);
    } else if (action === "stop") {
      Core.setRunning(false);
      stopProgram();
    } else if (action === "clear") {
      Core.clearProgram();
      renderProgram();
    } else if (action === "save-session") {
      Core.saveProgramSession();
      const clearSaveButton = document.getElementById('clear-save-btn');
      enableButton(clearSaveButton);
    } else if (action === "clear-session") {
      Core.clearSessionStorage();
    } else if (action === "export") {
      Core.saveProgramLocal();
    } else if (action === "load") {
      if (Core.loadProgram()) {
        renderProgram();
      }
    }
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
      const blockElement = e.target.closest(".remove-btn").closest('.dropped-block');
      Core.removeBlock(blockElement);
      blockElement.remove();
      updateUI();
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
      e.preventDefault();
    }
  });
});

// ====================== CONTRÔLE DU PROGRAMME ======================
// Arrête l'exécution du programme
function stopProgram() {
  Core.setRunning(false);
  const stopMotors = () => fetch('/stopMotors').catch(() => {});
  stopMotors();
  setTimeout(stopMotors, 100);
  setTimeout(stopMotors, 200);

  document.getElementById("run-btn").disabled = false;
  document.getElementById("stop-btn").disabled = true;
  document.getElementById("clear-zone-btn").disabled = false;
  document.getElementById("save-btn").disabled = false;
  document.getElementById("load-btn").disabled = false;
  document.getElementById("export-btn").disabled = false;
}

// ====================== FONCTIONS Utilisabilité des boutons ======================
// Active un bouton
function enableButton(button) {
  button.style.opacity = '1';
  button.style.cursor = 'pointer';
  button.style.backgroundColor = '';
  if (button.id === 'export-btn') {
    button.textContent = 'Exporter le programme';
  } else if (button.id === 'save-btn') {
    button.textContent = 'Sauvegarder dans le navigateur';
  } else if (button.id === 'run-btn') {
    button.textContent = 'Exécuter';
  } else if (button.id === 'clear-zone-btn') {
    button.textContent = 'Effacer la zone de programme';
  }
}

// Désactive un bouton
function disableButton(button) {
  button.style.opacity = '0.5';
  button.style.cursor = 'not-allowed';
  button.style.backgroundColor = '';
  if (button.id === 'export-btn') {
    button.textContent = 'Pas d\'export';
  } else if (button.id === 'save-btn') {
    button.textContent = 'Pas de sauvegarde';
  } else if (button.id === 'run-btn') {
    button.textContent = 'Pas d\'exécution';
  } else if (button.id === 'clear-zone-btn') {
    button.textContent = 'Zone vide';
  }
}

// Gère le clic sur le portail captif
function handleCaptivePortalClick(button) {
  button.textContent = 'Quitter le portail captif';
  button.style.backgroundColor = 'red';
  button.style.opacity = '1';

  setTimeout(() => {
    disableButton(button);
  }, 3000);
}

// Initialise le bouton d'export
function initializeExportButton() {
  const exportButton = document.getElementById('export-btn');
  if (!exportButton) return;

  if (Core.isValidUrl()) {
    enableButton(exportButton);
  } else {
    disableButton(exportButton);
    exportButton.addEventListener('click', () => handleCaptivePortalClick(exportButton), { once: true });
  }
}

// Met à jour l'état du bouton de sauvegarde de session
function setStateButton() {
  const runButton = document.getElementById('run-btn');
  const saveButton = document.getElementById('save-btn');
  const exportButton = document.getElementById('export-btn');
  const clearZoneButton = document.getElementById('clear-zone-btn');
  const clearSaveButton = document.getElementById('clear-save-btn');

  if (Core.countTotalBlocks(Core.program) > 0) {
    enableButton(runButton);
    enableButton(saveButton);
    enableButton(exportButton);
    enableButton(clearZoneButton);
  } else {
    disableButton(runButton);
    disableButton(saveButton);
    disableButton(exportButton);
    disableButton(clearZoneButton);
  }

  if (!sessionStorage.getItem('petitbotProgram')) {
    disableButton(clearSaveButton);
  } else {
    enableButton(clearSaveButton);
  }
};

// Met à jour l'interface utilisateur
function updateUI() {
  updateMoveButtons();
  updateBlockCounters();
  setStateButton();
}

// Met à jour les compteurs de blocs dans l'interface
function updateBlockCounters() {
  document.getElementById("NumbForward").textContent = `Nb : ${Core.countBlocksByType(Core.program, "FORWARD")}`;
  document.getElementById("NumbReverse").textContent = `Nb : ${Core.countBlocksByType(Core.program, "REVERSE")}`;
  document.getElementById("NumbLeft").textContent = `Nb : ${Core.countBlocksByType(Core.program, "LEFT")}`;
  document.getElementById("NumbRight").textContent = `Nb : ${Core.countBlocksByType(Core.program, "RIGHT")}`;
  document.getElementById("NumbStop").textContent = `Nb : ${Core.countBlocksByType(Core.program, "STOP")}`;
  document.getElementById("NumbWait").textContent = `Nb : ${Core.countBlocksByType(Core.program, "WAIT")}`;
  document.getElementById("NumbLoop").textContent = `Nb : ${Core.countBlocksByType(Core.program, "LOOP")}`;
  document.getElementById("NumbBlock").textContent = `Nombre de blocs : ${Core.countTotalBlocks(Core.program)}`;
}

)rawliteral";
#endif
