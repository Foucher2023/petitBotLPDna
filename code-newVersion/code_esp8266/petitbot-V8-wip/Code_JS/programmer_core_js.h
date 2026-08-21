#ifndef PROGRAMMER_CORE_JS_H
#define PROGRAMMER_CORE_JS_H

const char PROGRAMMER_CORE_JS[] PROGMEM = R"rawliteral(

// ====================== ÉTAT GLOBAL ======================
let program = [];
let isRunning = false;
let isAnimating = false;

// Mappage des noms de blocs
const blockNames = {
  FORWARD: "Avancer",
  REVERSE: "Reculer",
  LEFT: "Gauche",
  RIGHT: "Droite",
  STOP: "Stop",
  WAIT: "Attendre",
  LOOP: "Boucle"
};

// ====================== FONCTIONS UTILITAIRES ======================
// Compte le nombre de blocs d'un type donné dans le programme (y compris dans les boucles)
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

// Compte le nombre total de blocs dans le programme (y compris dans les boucles)
function countTotalBlocks(program) {
  let count = program.length;
  for (const step of program) {
    if (step.type === "LOOP" && step.children) {
      count += countTotalBlocks(step.children);
    }
  }
  return count;
}

// Supprime les références circulaires pour la sérialisation JSON
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

// ====================== GESTION DES BLOCS ======================
// Ajoute un bloc au programme
function addBlockToProgram(blockId, blockType, time, insertAt = -1, iterations = null, parentLoop = null) {
  if (isRunning || isAnimating) return;

  const blockData = {
    type: blockType,
    ...(blockType === 'WAIT' && { time: parseInt(time) || 1 }),
    ...(blockType === 'LOOP' && {
      iterations: parseInt(iterations) || 3,
      children: []
    })
  };

  if (parentLoop) {
    parentLoop.children.push(blockData);
    blockData.parent = parentLoop;
  } else if (insertAt >= 0) {
    program.splice(insertAt, 0, blockData);
  } else {
    program.push(blockData);
  }
}

// Met à jour le nombre d'itérations d'une boucle
function updateLoopIterations(input) {
  const blockElement = input.closest('.dropped-block');
  const blockIndex = parseInt(blockElement.dataset.stepIndex);
  if (blockIndex >= 0 && program[blockIndex]?.type === 'LOOP') {
    program[blockIndex].iterations = parseInt(input.value) || 1;
  }
}

// Ajoute un bloc directement dans une boucle
function placeBlockInLoop(blockType, loopIndex, childIndex = -1) {
  if (isRunning || isAnimating) return;

  if (loopIndex < 0 || !program[loopIndex] || program[loopIndex].type !== "LOOP") return;

  const newBlock = { type: blockType };
  if (blockType === "WAIT") newBlock.time = 1;

  if (!program[loopIndex].children) program[loopIndex].children = [];
  if (childIndex >= 0) {
    program[loopIndex].children.splice(childIndex, 0, newBlock);
  } else {
    program[loopIndex].children.push(newBlock);
  }
}

// ====================== SUPPRESSION DES BLOCS ======================
// Supprime un bloc du programme
function removeBlock(blockElement) {
  const loopBody = blockElement.closest('.loop-body');
  const programArea = document.getElementById("program-area");

  if (loopBody) {
    const loopBlock = loopBody.closest('.dropped-block');
    const loopIndex = parseInt(loopBlock.dataset.stepIndex);
    const childIndex = Array.from(loopBody.children).indexOf(blockElement);

    if (loopIndex >= 0 && program[loopIndex]?.type === "LOOP" && program[loopIndex].children) {
      program[loopIndex].children.splice(childIndex, 1);
    }
  } else {
    const blockIndex = parseInt(blockElement.dataset.stepIndex);
    if (blockIndex >= 0 && blockIndex < program.length) {
      program.splice(blockIndex, 1);
    }
  }
}

// ====================== CONTRÔLE DU PROGRAMME ======================
// Efface le programme actuel
function clearProgram() {
  if (isRunning) return;
  fetch('/stopMotors').catch(() => {});
  program = [];
}

// Efface le programme sauvegardé dans sessionStorage
function clearSessionStorage() {
  if (sessionStorage.getItem("petitbotProgram")) {
    sessionStorage.removeItem("petitbotProgram");
    setStateButton();
    alert("✅ Programme supprimé de la mémoire du navigateur");
  }
}

// ====================== EXÉCUTION DU PROGRAMME ======================
// Exécute une étape du programme
function executeStep(index) {
  if (index >= program.length || !isRunning) {
    stopProgram();
    return;
  }

  const step = program[index];

  if (step.type === 'LOOP') {
    executeLoop(step, 0, () => executeStep(index + 1));
  } else if (step.type === 'WAIT') {
    setTimeout(() => isRunning && executeStep(index + 1), step.time * 1000);
  } else if (step.type === 'STOP') {
    fetch('/stopMotors')
      .then(() => isRunning && setTimeout(() => executeStep(index + 1), 500))
      .catch(() => stopProgram());
  } else {
    fetch(`/UseTelecommande?val=${step.type}`)
      .then(() => isRunning && setTimeout(() => executeStep(index + 1), 500))
      .catch(() => stopProgram());
  }
}

// Exécute une boucle
function executeLoop(loop, iteration, callback) {
  if (iteration >= loop.iterations || !isRunning) {
    callback();
    return;
  }

  executeChildren(loop.children, 0, () => {
    executeLoop(loop, iteration + 1, callback);
  });
}

// Exécute les enfants d'une boucle
function executeChildren(children, childIndex, callback) {
  if (childIndex >= children.length || !isRunning) {
    callback();
    return;
  }

  const child = children[childIndex];

  if (child.type === 'WAIT') {
    setTimeout(() => isRunning && executeChildren(children, childIndex + 1, callback), child.time * 1000);
  } else if (child.type === 'LOOP') {
    executeLoop(child, 0, () => isRunning && executeChildren(children, childIndex + 1, callback));
  } else if (child.type === 'STOP') {
    fetch('/stopMotors')
      .then(() => isRunning && setTimeout(() => executeChildren(children, childIndex + 1, callback), 500))
      .catch(() => stopProgram());
  } else {
    fetch(`/UseTelecommande?val=${child.type}`)
      .then(() => isRunning && setTimeout(() => executeChildren(children, childIndex + 1, callback), 500))
      .catch(() => stopProgram());
  }
}

// ====================== SAUVEGARDE/CHARGEMENT ======================
// Sauvegarde le programme dans sessionStorage
function saveProgramSession() {
  const cleanProgram = removeCircularReferences(program);
  sessionStorage.setItem("petitbotProgram", JSON.stringify(cleanProgram));
  alert("✅ Programme sauvegardé dans le navigateur");
}

// Exporte le programme sous forme de fichier JSON
function saveProgramLocal() {
  if (!isValidUrl()) return;
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

// Charge un programme depuis sessionStorage ou un fichier
function loadProgram() {
  const savedProgram = sessionStorage.getItem("petitbotProgram");
  if (savedProgram) {
    try {
      program = JSON.parse(savedProgram);
      return true;
    } catch (e) {
      console.error("Erreur sessionStorage:", e);
    }
  }

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
  return false;
}

// ====================== FONCTIONS UTILITAIRES POUR L'URL ======================
// Vérifie si l'URL est valide
function isValidUrl() {
  const hostname = window.location.hostname;
  return hostname === "192.168.4.1" || hostname === sessionStorage.getItem('BASE_URL');
}

// ====================== EXPORTS ======================
// Exporter les fonctions et variables nécessaires pour l'UI
window.Core = {
  program,
  isRunning,
  isAnimating,
  blockNames,
  countBlocksByType,
  countTotalBlocks,
  addBlockToProgram,
  updateLoopIterations,
  placeBlockInLoop,
  removeBlock,
  clearProgram,
  executeStep,
  executeLoop,
  executeChildren,
  saveProgramSession,
  clearSessionStorage,
  saveProgramLocal,
  loadProgram,
  removeCircularReferences,
  isValidUrl,
  setRunning: (value) => { isRunning = value; },
  setAnimating: (value) => { isAnimating = value; },
};

)rawliteral";
#endif