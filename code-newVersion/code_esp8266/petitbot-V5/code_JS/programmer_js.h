  #ifndef PROGRAMMER_JS_H
  #define PROGRAMMER_JS_H

  const char PROGRAMMER_JS[] PROGMEM = R"rawliteral(
  /*****************************************************************
  * PetitBot - Block Programming Interface
  * --------------------------------------------
  * A visual programming environment for controlling the PetitBot robot.
  * Supports drag-and-drop block programming with loops and nested structures.
  *****************************************************************/

  // ====================== GLOBAL STATE ======================
  /**
  * Program state variables
  * @type {Array} program - Array of block objects representing the user's program
  * @type {boolean} isRunning - Flag indicating if program is currently executing
  * @type {boolean} isAnimating - Flag to prevent simultaneous block animations
  */
  let program = [];
  let isRunning = false;
  let isAnimating = false;

  // ====================== ANIMATION UTILITIES ======================
  /**
  * Animates a block's movement up or down
  * @param {HTMLElement} blockElement - The block element to animate
  * @param {string} direction - 'up' or 'down'
  */
  function animateBlockMove(blockElement, direction) {
    if (direction === 'up') {
      blockElement.classList.add('moving-up');
    } else if (direction === 'down') {
      blockElement.classList.add('moving-down');
    }

    // Remove animation class after animation completes
    setTimeout(() => {
      blockElement.classList.remove('moving-up', 'moving-down');
    }, 500);
  }

  // ====================== DRAG AND DROP HANDLERS ======================
  /**
  * Allows drop on target elements
  * @param {DragEvent} ev - The drag event
  */
  window.allowDrop = function(ev) {
    ev.preventDefault();
  };

  /**
  * Handles drag start for blocks
  * Sets drag data for different block types (including LOOP with iterations)
  * @param {DragEvent} ev - The drag event
  */
  window.drag = function(ev) {
    ev.dataTransfer.setData("text", ev.target.id);

    // Get block type from hidden span or default to WAIT
    const blockType = ev.target.querySelector('.hide') ? ev.target.querySelector('.hide').textContent : 'WAIT';
    ev.dataTransfer.setData("type", blockType);

    // Handle special block types
    if (ev.target.id === 'wait-block') {
      const time = document.getElementById('wait-time').value;
      ev.dataTransfer.setData("time", time);
    } else if (ev.target.id === 'loop-block') {
      const iterations = document.getElementById('loop-iterations').value;
      ev.dataTransfer.setData("iterations", iterations);
    }
  };

  /**
  * Handles drop events for blocks
  * Supports dropping into loop bodies for nested structures
  * @param {DragEvent} ev - The drop event
  */
  window.drop = function(ev) {
    ev.preventDefault();
    ev.stopPropagation(); 

    const blockId = ev.dataTransfer.getData("text");
    const blockType = ev.dataTransfer.getData("type");
    const time = ev.dataTransfer.getData("time");
    const iterations = ev.dataTransfer.getData("iterations");

    // Check if dropping into a loop body
    const loopBody = ev.target.closest('.loop-body');
    if (loopBody) {
      const loopBlock = loopBody.closest('.dropped-block');
      const loopIndex = Array.from(document.getElementById("program-area").children).indexOf(loopBlock);
      if (loopIndex >= 0 && program[loopIndex]?.type === 'LOOP') {
        addBlockToProgram(blockId, blockType, time, -1, iterations, program[loopIndex]);
        return;
      }
    }else {
    // Default: add to main program
      addBlockToProgram(blockId, blockType, time, -1, iterations);
    }

  };

  // ====================== BLOCK MANAGEMENT ======================
  /**
  * Adds a block to the program
  * @param {string} blockId - ID of the block being added
  * @param {string} blockType - Type of block (FORWARD, REVERSE, LOOP, etc.)
  * @param {number|null} time - Time parameter for WAIT blocks
  * @param {number} insertAt - Index to insert at (-1 for append)
  * @param {number|null} iterations - Iterations for LOOP blocks
  * @param {Object|null} parentLoop - Parent loop object if adding to a loop
  */
  function addBlockToProgram(blockId, blockType, time, insertAt = -1, iterations = null, parentLoop = null) {
    // Create block DOM element
    const blockElement = document.createElement("div");
    blockElement.className = "dropped-block";
    if (blockType === 'LOOP') blockElement.classList.add('loop-block');

    // Set block content based on type
    let blockContent = "";
    if (blockType === 'WAIT') {
      blockContent = `<strong>Attendre</strong> <span>${time} seconde(s)</span>`;
    } else if (blockType === 'LOOP') {
      blockContent = `
        <strong>Boucle X </strong>
        <span><input type="number" value="${iterations || 3}" min="1"
                      class="inputBox inProgInput" onchange="updateLoopIterations(this)"></span>
                      <div class="loop-block-content">
                      <div class="loop-quick-add">
        <button onclick="placeBlockInLoop('FORWARD', event)" title="Ajouter AVANCER">AVANCER</button>
        <button onclick="placeBlockInLoop('LEFT', event)" title="Ajouter GAUCHE">GAUCHE</button>
        <button onclick="placeBlockInLoop('RIGHT', event)" title="Ajouter DROITE">DROITE</button>
        <button onclick="placeBlockInLoop('REVERSE', event)" title="Ajouter RECULER">RECULER</button>
        <button onclick="placeBlockInLoop('STOP', event)" title="Ajouter STOP">STOP</button>
        <button onclick="placeBlockInLoop('WAIT', event)" title="Ajouter ATTENDRE">ATTENDRE</button>
      </div>
        <div class="loop-body" ondrop="drop(event)" ondragover="allowDrop(event)"></div>
        </div>
      `;
    } else {
      const blockName = document.getElementById(blockId).querySelector('strong').textContent;
      blockContent = `<strong>${blockName}</strong>`;
    }

    // Add control buttons
    blockElement.innerHTML = blockContent + `
      <div class="block-controls">
        <button class="move-btn up" onclick="moveBlockUp(this)" title="Monter" ${insertAt === 0 ? 'disabled' : ''}>▲</button>
        <button class="move-btn down" onclick="moveBlockDown(this)" title="Descendre">▼</button>
      </div>
      <button class="remove-btn" onclick="removeBlock(this)">×</button>
    `;

    // Insert into DOM
    if (parentLoop) {
      // Add to loop body
      const loopBody = parentLoop.element.querySelector('.loop-body');
      loopBody.appendChild(blockElement);
      blockElement.dataset.parentIndex = program.indexOf(parentLoop);
      blockElement.dataset.childIndex = parentLoop.children.length;
    } else if (insertAt >= 0) {
      // Insert at specific position
      const programArea = document.getElementById("program-area");
      programArea.insertBefore(blockElement, programArea.children[insertAt]);
      blockElement.dataset.stepIndex = insertAt;
    } else {
      // Append to end
      document.getElementById("program-area").appendChild(blockElement);
      blockElement.dataset.stepIndex = program.length;
    }

    // Create block data object
    const blockData = blockType === 'WAIT'
      ? { type: blockType, time: parseInt(time) }
      : blockType === 'LOOP'
        ? { type: blockType, iterations: parseInt(iterations) || 3, children: [], element: blockElement }
        : { type: blockType };

    // Add to program data structure
    if (parentLoop) {
      parentLoop.children.push(blockData);
      blockData.parent = parentLoop;
    } else if (insertAt >= 0) {
      program.splice(insertAt, 0, blockData);
    } else {
      program.push(blockData);
    WriteStateNumbBlock (program);
    }

    // Store reference to program data on DOM element
    blockElement.dataset.blockData = program.indexOf(blockData);
    blockData.element = blockElement;

    updateMoveButtons();
  }

  // function do modify the paragraph for the numb of block
  function WriteStateNumbBlock(program) {
  const statusBlockNumb1 = document.getElementById("NumbForward");
  let ForwardNumb = getNumbBlock(program, "FORWARD");
  statusBlockNumb1.textContent = `Nb : ${ForwardNumb}`;

  const statusBlockNumb2 = document.getElementById("NumbReverse");
  let ReverseNumb = getNumbBlock(program, "REVERSE");
  statusBlockNumb2.textContent = `Nb : ${ReverseNumb}`;

  const statusBlockNumb3 = document.getElementById("NumbLeft");
  let LeftNumb = getNumbBlock(program, "LEFT");
  statusBlockNumb3.textContent = `Nb : ${LeftNumb}`;

  const statusBlockNumb4 = document.getElementById("NumbRight");
  let RightNumb = getNumbBlock(program, "RIGHT");
  statusBlockNumb4.textContent = `Nb : ${RightNumb}`;

  const statusBlockNumb5 = document.getElementById("NumbStop");
  let StopNumb = getNumbBlock(program, "STOP");
  statusBlockNumb5.textContent = `Nb : ${StopNumb}`;

  const statusBlockNumb6 = document.getElementById("NumbWait");
  let WaitNumb = getNumbBlock(program, "WAIT");
  statusBlockNumb6.textContent = `Nb : ${WaitNumb}`;

  const statusBlockNumb7 = document.getElementById("NumbLoop");
  let LoopNumb = getNumbBlock(program, "LOOP");
  statusBlockNumb7.textContent = `Nb : ${LoopNumb}`;

  const statusBlockNumbTotal = document.getElementById("NumbBlock");
  let TotalNumb = countTotalBlocks(program); // Use a helper function to count all blocks
  statusBlockNumbTotal.textContent = `Nombre de blocs : ${TotalNumb}`;
};

function getNumbBlock(program, typeWanted) {
  let number = 0;
  for (let i = 0; i < program.length; i++) {
    if (program[i].type === typeWanted) {
      number++;
    }
    // If the block is a loop, recursively count its children
    if (program[i].type === "LOOP" && program[i].children) {
      number += getNumbBlock(program[i].children, typeWanted);
    }
  }
  return number;
};

function countTotalBlocks(program) {
  let total = 0;
  for (let i = 0; i < program.length; i++) {
    total++; // Count the current block
    // If the block is a loop, recursively count its children
    if (program[i].type === "LOOP" && program[i].children) {
      total += countTotalBlocks(program[i].children);
    }
  }
  return total;
};


  /**
  * Updates loop iterations when input changes
  * @param {HTMLElement} input - The input element that changed
  */
  window.updateLoopIterations = function(input) {
    const blockElement = input.closest('.dropped-block');
    const blockIndex = parseInt(blockElement.dataset.blockData);
    if (blockIndex >= 0 && program[blockIndex]?.type === 'LOOP') {
      program[blockIndex].iterations = parseInt(input.value) || 1;
    }
  };

  /**
  * Adds a block directly to the loop body where the button was clicked.
  * @param {string} blockType - The type of block to add (e.g., 'FORWARD', 'LEFT', 'WAIT').
  * @param {Event} event - The click event (to find the loop body).
  */
  window.placeBlockInLoop = function(blockType, event) {
    if (isRunning || isAnimating) return;

    // Find the loop block (parent of the clicked button)
    const loopBlock = event.target.closest('.dropped-block.loop-block');
    if (!loopBlock) {
      alert("Impossible de trouver la boucle associée à ce bouton.");
      return;
    }

    // Find the loop body inside the loop block
    const loopBody = loopBlock.querySelector('.loop-body');
    if (!loopBody) {
      alert("Impossible de trouver le corps de la boucle.");
      return;
    }

    // Find the index of the loop block in the program
    const programArea = document.getElementById("program-area");
    const loopIndex = Array.from(programArea.children).indexOf(loopBlock);
    if (loopIndex < 0 || !program[loopIndex] || program[loopIndex].type !== "LOOP") {
      alert("Impossible de trouver la boucle dans le programme.");
      return;
    }

    // Create the new block
    const newBlock = { type: blockType };
    if (blockType === "WAIT") {
      newBlock.time = 1; // Default wait time
    }

    // Initialize children array if it doesn't exist
    if (!program[loopIndex].children) {
      program[loopIndex].children = [];
    }

    // Add the new block to the loop's children
    program[loopIndex].children.push(newBlock);

    // Re-render the program to update the UI
    renderProgram();
  };

  // ====================== BLOCK MOVEMENT ======================
  /**
  * Moves a block up in the program
  * @param {HTMLElement} button - The up button that was clicked
  */
  window.moveBlockUp = function(button) {
    if (isAnimating) return;
    const blockElement = button.closest('.dropped-block');
    const loopBody = blockElement.closest('.loop-body');
    const programArea = document.getElementById("program-area");

    // If inside a loop, use the loop body as the parent
    const parent = loopBody || programArea;
    const blocks = Array.from(parent.children);
    const index = blocks.indexOf(blockElement);

    if (index <= 0) return;

    isAnimating = true;
    const prevBlock = blocks[index - 1];

    animateBlockMove(blockElement, 'up');
    animateBlockMove(prevBlock, 'down');

    setTimeout(() => {
      parent.insertBefore(blockElement, prevBlock);
      // Update the program array (handle loops if needed)
      if (loopBody) {
        const loopBlock = loopBody.closest('.dropped-block');
        const loopIndex = Array.from(programArea.children).indexOf(loopBlock);
        if (loopIndex >= 0 && program[loopIndex]?.type === "LOOP") {
          [program[loopIndex].children[index], program[loopIndex].children[index - 1]] =
            [program[loopIndex].children[index - 1], program[loopIndex].children[index]];
        }
      } else {
        [program[index], program[index - 1]] = [program[index - 1], program[index]];
      }
      updateMoveButtons();
      isAnimating = false;
    }, 600);
  };

  /**
  * Moves a block down in the program
  * @param {HTMLElement} button - The down button that was clicked
  */
  window.moveBlockDown = function(button) 
  {
    if (isAnimating) return;
    const blockElement = button.closest('.dropped-block');
    const loopBody = blockElement.closest('.loop-body');
    const programArea = document.getElementById("program-area");

    // If inside a loop, use the loop body as the parent
    const parent = loopBody || programArea;
    const blocks = Array.from(parent.children);
    const index = blocks.indexOf(blockElement);

    // Cannot move down if it's the last block
    if (index >= blocks.length - 1) return;

    isAnimating = true;
    const nextBlock = blocks[index + 1];

    // Animate both blocks
    animateBlockMove(blockElement, 'down');
    animateBlockMove(nextBlock, 'up');

    setTimeout(() => {
      // Insert the nextBlock before the current block, then move the current block after it
      parent.insertBefore(nextBlock, blockElement);

      // Update the program array (handle loops if needed)
      if (loopBody) {
        const loopBlock = loopBody.closest('.dropped-block');
        const loopIndex = Array.from(programArea.children).indexOf(loopBlock);
        if (loopIndex >= 0 && program[loopIndex]?.type === "LOOP") {
          [program[loopIndex].children[index], program[loopIndex].children[index + 1]] =
            [program[loopIndex].children[index + 1], program[loopIndex].children[index]];
        }
      } else {
        [program[index], program[index + 1]] = [program[index + 1], program[index]];
      }

      updateMoveButtons();
      isAnimating = false;
    }, 600);
  };

  /**
  * Updates the enabled/disabled state of move buttons
  * Disables up button for first block, down button for last block
  */
  function updateMoveButtons() {
    const programArea = document.getElementById("program-area");
    const blocks = programArea.querySelectorAll('.dropped-block');

    blocks.forEach((block, index) => {
      const upButton = block.querySelector('.move-btn.up');
      const downButton = block.querySelector('.move-btn.down');

      if (upButton) upButton.disabled = index === 0;
      if (downButton) downButton.disabled = index === blocks.length - 1;
    });
  }

  // ====================== BLOCK PLACEMENT ======================
  /**
  * Places a standard block via its button
  * @param {string} blockId - ID of the block to place
  */
  window.placeBlock = function(blockId) {
    const block = document.getElementById(blockId);
    const blockType = block.querySelector('span').textContent;
    addBlockToProgram(blockId, blockType, 1);
  };

  /**
  * Places a LOOP block via its button
  */
  window.placeLoopBlock = function() {
    const iterations = document.getElementById('loop-iterations').value;
    addBlockToProgram('loop-block', 'LOOP', null, -1, iterations);
  };

  /**
  * Places a WAIT block via its button
  */
  window.placeWaitBlock = function() {
    const time = document.getElementById('wait-time').value;
    addBlockToProgram('wait-block', 'WAIT', time);
  };

  // ====================== BLOCK REMOVAL ======================
  /**
  * Removes a block from the program
  * Handles both top-level blocks and blocks inside loops
  * @param {HTMLElement} button - The remove button that was clicked
  */
window.removeBlock = function(button) {
    const blockElement = button.parentElement;
    const parentIndex = blockElement.dataset.parentIndex;
    const childIndex = blockElement.dataset.childIndex;
    const blockIndex = blockElement.dataset.blockData;

    // Supprimer du tableau children de la boucle parente
    if (parentIndex !== undefined && childIndex !== undefined) {
      const loop = program[parentIndex];
      if (loop && loop.type === "LOOP" && loop.children) {
        if (loop.children.length === 1) {
          loop.children = [];
        } else {
          loop.children.splice(childIndex, 1);
          // Re-indexer les enfants restants dans le DOM
          const loopBody = blockElement.parentElement;
          Array.from(loopBody.children).forEach((child, idx) => {
            child.dataset.childIndex = idx;
          });
        }
      }
    }
// Supprimer du tableau principal program
else if (blockIndex !== undefined && blockIndex >= 0 && blockIndex <= program.length) {
      if (program.length === 1) {
        program = [];
      } else {
        program.splice(blockIndex, 1);
        // Re-indexer TOUS les blocs restants dans le DOM
        const programArea = document.getElementById("program-area");
        Array.from(programArea.children).forEach((child, idx) => {
          child.dataset.stepIndex = idx;
          child.dataset.blockData = idx;
        });
      }
    }

    // Supprimer le bloc du DOM
    blockElement.remove();
    updateMoveButtons();
    WriteStateNumbBlock(program);
};

  // ====================== PROGRAM CONTROL ======================
  /**
  * Clears the current program
  */
  window.clearProgram = function() {
    fetch('/stopMotors');
    document.getElementById("program-area").innerHTML = "";
    program = [];
    isRunning = false;
    document.getElementById("run-btn").disabled = false;
    document.getElementById("stop-btn").disabled = false;
    WriteStateNumbBlock(program);
  };

  /**
  * Clears the saved program from sessionStorage
  */
  window.clearSessionStorage = function() {
    if (sessionStorage.getItem("petitbotProgram")) {
      sessionStorage.removeItem("petitbotProgram");
      alert("✅ Programme supprimé de la mémoire du navigateur");
    }
  };

  /**
  * Starts program execution
  */
  window.runProgram = function() {
    if (program.length === 0) {
      alert("Aucun bloc dans le programme !");
      return;
    }

    // Disable buttons during execution
    document.getElementById("run-btn").disabled = true;
    document.getElementById("stop-btn").disabled = false;
    document.getElementById("clear-btn").disabled = true;
    document.getElementById("save-btn").disabled = true;
    document.getElementById("load-btn").disabled = true;
    isRunning = true;

    executeStep(0); // Start execution from first block
  };

  /**
  * Stops program execution
  */
  window.stopProgram = function() {
    isRunning = false;
    const stopMotors = () => fetch('/stopMotors').catch(() => {});
    stopMotors();
    setTimeout(stopMotors, 100);  // Retry after 100ms
    setTimeout(stopMotors, 200);  // Retry after 200ms

    // Re-enable buttons
    document.getElementById("run-btn").disabled = false;
    document.getElementById("stop-btn").disabled = false;
    document.getElementById("clear-btn").disabled = false;
    document.getElementById("save-btn").disabled = false;
    document.getElementById("load-btn").disabled = false;
  };

  // ====================== PROGRAM EXECUTION ======================
  /**
  * Executes a single step of the program
  * @param {number} index - Current step index
  */
  function executeStep(index) {
    // Termination conditions
    if (index >= program.length || !isRunning) {
      document.getElementById("run-btn").disabled = false;
      document.getElementById("stop-btn").disabled = false;
      document.getElementById("clear-btn").disabled = false;
      document.getElementById("save-btn").disabled = false;
      document.getElementById("load-btn").disabled = false;
      isRunning = false;
      return;
    }

    const step = program[index];

    // Handle LOOP blocks
    if (step.type === 'LOOP') {
      executeLoop(step, 0, () => executeStep(index + 1));
    }
    // Handle WAIT blocks
    else if (step.type === 'WAIT') {
      setTimeout(() => isRunning && executeStep(index + 1), step.time * 1000);
    }else if(step.type === 'STOP') {
    fetch(`/stopMotors`)
    .then(() => isRunning && setTimeout(() => executeStep(index + 1), 500))
        .catch(error => {
          console.error("Error:", error);
          document.getElementById("run-btn").disabled = false;
          document.getElementById("stop-btn").disabled = false;
          document.getElementById("clear-btn").disabled = false;
          document.getElementById("save-btn").disabled = false;
          document.getElementById("load-btn").disabled = false;
          isRunning = false;
        });
    }
    // Handle all other blocks (movement commands)
    else {
      fetch(`/UseTelecommande?val=${step.type}`)
        .then(() => isRunning && setTimeout(() => executeStep(index + 1), 500))
        .catch(error => {
          console.error("Error:", error);
          document.getElementById("run-btn").disabled = false;
          document.getElementById("stop-btn").disabled = false;
          document.getElementById("clear-btn").disabled = false;
          document.getElementById("save-btn").disabled = false;
          document.getElementById("load-btn").disabled = false;
          isRunning = false;
        });
    }
  }

  /**
  * Executes a loop block's iterations
  * @param {Object} loop - The loop block data
  * @param {number} iteration - Current iteration (0-based)
  * @param {Function} callback - Function to call when loop completes
  */
  function executeLoop(loop, iteration, callback) {
    if (iteration >= loop.iterations || !isRunning) {
      callback();
      return;
    }

    // Execute all children in the loop body
    executeChildren(loop.children, 0, () => {
      // After all children complete, do next iteration
      executeLoop(loop, iteration + 1, callback);
    });
  }

  /**
  * Executes children of a loop sequentially
  * @param {Array} children - Array of child blocks
  * @param {number} childIndex - Current child index
  * @param {Function} callback - Function to call when all children complete
  */
  function executeChildren(children, childIndex, callback) {
    if (childIndex >= children.length || !isRunning) {
      callback();
      return;
    }

    const child = children[childIndex];

    // Handle WAIT children
    if (child.type === 'WAIT') {
      setTimeout(() => isRunning && executeChildren(children, childIndex + 1, callback), child.time * 1000);
    }
    // Handle nested LOOP children
    else if (child.type === 'LOOP') {
      executeLoop(child, 0, () => {
        isRunning && executeChildren(children, childIndex + 1, callback);
      });
    }else if(child.type === 'STOP') {
    fetch(`/stopMotors`)
    .then(() => isRunning && setTimeout(() => executeChildren(children, childIndex + 1, callback), 500))
        .catch(error => {
          console.error("Error:", error);
          document.getElementById("run-btn").disabled = false;
          document.getElementById("stop-btn").disabled = false;
          document.getElementById("clear-btn").disabled = false;
          document.getElementById("save-btn").disabled = false;
          document.getElementById("load-btn").disabled = false;
          isRunning = false;
        });
    }
    // Handle movement command children
    else {
      fetch(`/UseTelecommande?val=${child.type}`)
        .then(() => isRunning && setTimeout(() => executeChildren(children, childIndex + 1, callback), 500))
        .catch(error => {
          console.error("Error:", error);
          document.getElementById("run-btn").disabled = false;
          document.getElementById("stop-btn").disabled = false;
          document.getElementById("clear-btn").disabled = false;
          document.getElementById("save-btn").disabled = false;
          document.getElementById("load-btn").disabled = false;
          isRunning = false;
        });
    }
  }

  // ====================== PROGRAM SAVING/LOADING ======================
  /**
  * Saves the current program to sessionStorage
  */
  window.saveProgramSession = function() {
    const cleanProgram = removeCircularReferences(program);
    sessionStorage.setItem("petitbotProgram", JSON.stringify(cleanProgram));
    alert("✅ Programme sauvegardé dans le navigateur");
  };

  function removeCircularReferences(obj) {
    const seen = new WeakSet();
    return JSON.parse(JSON.stringify(obj, (key, value) => {
      if (typeof value === "object" && value !== null) {
        if (seen.has(value)) {
          return; // Skip circular references
        }
        seen.add(value);
      }
      // Remove parent property (or any other circular property)
      if (key === "parent") {
        return;
      }
      return value;
    }));
  };

  /**
  * Exports the current program as a downloadable JSON file
  */
  window.saveProgramLocal = function() {
    const data = JSON.stringify(program, null, 2);
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
  };

  /**
  * Loads a program from sessionStorage or a JSON file
  * Tries sessionStorage first, falls back to file picker
  */
  window.loadProgram = function() {
    // Try loading from sessionStorage first
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

    // Fall back to file picker
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
          const content = e.target.result;
          program = JSON.parse(content);
          sessionStorage.setItem("petitbotProgram", content); // Cache for next time
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
  * Renders the entire program to the DOM
  * Handles nested loops recursively
  */
  window.renderProgram = function() {
    const programArea = document.getElementById("program-area");
    programArea.innerHTML = "";

    program.forEach((step, index) => {
      // Create block element
      const blockElement = document.createElement("div");
      blockElement.className = "dropped-block";
      blockElement.dataset.stepIndex = index; 

      if (step.type === 'LOOP') blockElement.classList.add('loop-block');
      blockElement.dataset.stepIndex = index;
      blockElement.dataset.blockData = index;

      // Set block content based on type
      let blockContent = "";
      if (step.type === 'WAIT') {
        blockContent = `<strong>Attendre</strong> <span>${step.time} seconde(s)</span>`;
      } else if (step.type === 'LOOP') {
        blockContent = `
          <strong>Boucle X </strong>
          <span><input type="number" value="${step.iterations || 3}" min="1"
                        class="inputBox inProgInput" onchange="updateLoopIterations(this)"></span>
                        <div class="loop-block-content">
                                  <div class="loop-quick-add">
        <button onclick="placeBlockInLoop('FORWARD', event)" title="Ajouter AVANCER">AVANCER</button>
        <button onclick="placeBlockInLoop('LEFT', event)" title="Ajouter GAUCHE">GAUCHE</button>
        <button onclick="placeBlockInLoop('RIGHT', event)" title="Ajouter DROITE">DROITE</button>
        <button onclick="placeBlockInLoop('REVERSE', event)" title="Ajouter RECULER">RECULER</button>
        <button onclick="placeBlockInLoop('STOP', event)" title="Ajouter STOP">STOP</button>
        <button onclick="placeBlockInLoop('WAIT', event)" title="Ajouter ATTENDRE">ATTENDRE</button>
      </div>
          <div class="loop-body" ondrop="drop(event)" ondragover="allowDrop(event)"></div>
          </div>
        `;
      } else {
        // Get block name from palette or use type directly
        const blockName = document.getElementById(`${step.type.toLowerCase()}-block`)
          ? document.getElementById(`${step.type.toLowerCase()}-block`).querySelector('strong').textContent
          : step.type;
        blockContent = `<strong>${blockName}</strong>`;
      }

      // Add control buttons
      blockElement.innerHTML = blockContent + `
        <div class="block-controls">
          <button class="move-btn up" onclick="moveBlockUp(this)" title="Monter" ${index === 0 ? 'disabled' : ''}>▲</button>
          <button class="move-btn down" onclick="moveBlockDown(this)" title="Descendre">▼</button>
        </div>
        <button class="remove-btn" onclick="removeBlock(this)">×</button>
      `;

      programArea.appendChild(blockElement);
      step.element = blockElement; // Store DOM reference

      // Render loop children if this is a loop
      if (step.type === 'LOOP' && step.children) {
        renderLoopChildren(step.children, blockElement.querySelector('.loop-body'));
      }
    });

    updateMoveButtons();
    WriteStateNumbBlock(program);

  };

  /**
  * Renders children of a loop block recursively
  * @param {Array} children - Array of child blocks
  * @param {HTMLElement} loopBody - The DOM element to render children into
  */
  function renderLoopChildren(children, loopBody) {
    children.forEach((child, childIndex) => {
      const childElement = document.createElement("div");
      childElement.className = "dropped-block loop-child";
      childElement.dataset.parentIndex = loopBody.closest('.dropped-block').dataset.stepIndex;
      childElement.dataset.childIndex = childIndex;
      childElement.dataset.blockData = children.indexOf(child);

      // Set child content based on type
      let childContent = "";
      if (child.type === 'WAIT') {
        childContent = `<strong>Attendre</strong> <span>${child.time} seconde(s)</span>`;
      } else {
        const childName = document.getElementById(`${child.type.toLowerCase()}-block`)
          ? document.getElementById(`${child.type.toLowerCase()}-block`).querySelector('strong').textContent
          : child.type;
        childContent = `<strong>${childName}</strong>`;
      }

      // Add control buttons
      childElement.innerHTML = childContent + `
        <div class="block-controls">
          <button class="move-btn up" onclick="moveBlockUp(this)" title="Monter">▲</button>
          <button class="move-btn down" onclick="moveBlockDown(this)" title="Descendre">▼</button>
        </div>
        <button class="remove-btn" onclick="removeBlock(this)">×</button>
      `;

      loopBody.appendChild(childElement);
      child.element = childElement;

      // Recursively render nested loop children
      if (child.type === 'LOOP' && child.children) {
        renderLoopChildren(child.children, childElement.querySelector('.loop-body'));
      }
    });
  }

  // ====================== INITIALIZATION ======================
  // Navbar toggle for mobile
  document.querySelector('.navbar-toggle')?.addEventListener('click', function() {
    document.getElementById('myNavbar')?.classList.toggle('open');
  });

  )rawliteral";
  #endif