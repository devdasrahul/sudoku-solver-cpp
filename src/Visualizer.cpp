#include "Visualizer.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

Visualizer::Visualizer(SudokuBoard &board, Solver &solver)
    : board(board), solver(solver) {
  if (!font.loadFromFile("assets/font.ttf")) {
    if (!font.loadFromFile("../assets/font.ttf")) {
      std::cerr << "Failed to load assets/font.ttf and ../assets/font.ttf. "
                   "Text may not appear.\n";
    }
  }
  initialBoard = board;
}

Visualizer::~Visualizer() {
  if (solverThread.joinable()) {
    solverThread.join();
  }
}

void Visualizer::run() {
  window.create(sf::VideoMode(1000, 700),
                "Sudoku Solver - Visualizer (Interview Grade)");
  window.setFramerateLimit(60);

  // Set callback on solver
  solver.setStepCallback([this](int r, int c, int n, std::string reason) {
    this->solverCallback(r, c, n, reason);
  });

  while (window.isOpen()) {
    processEvents();
    render();
  }
}

void Visualizer::processEvents() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      window.close();

    // Mouse Interaction
    if (event.type == sf::Event::MouseButtonPressed) {
      if (!solving && event.mouseButton.button == sf::Mouse::Left) {
        float cellSize = 60.0f;
        float startX = 50.0f;
        float startY = 50.0f;
        int c = (event.mouseButton.x - startX) / cellSize;
        int r = (event.mouseButton.y - startY) / cellSize;
        if (r >= 0 && r < 9 && c >= 0 && c < 9) {
          selectedRow = r;
          selectedCol = c;
        } else {
          selectedRow = -1;
          selectedCol = -1;
        }
      }
    }

    if (event.type == sf::Event::KeyPressed) {
      // Gameplay Controls (Only when not solving)
      if (!solving) {
        // Arrow Keys Navigation
        if (selectedRow != -1 && selectedCol != -1) {
          if (event.key.code == sf::Keyboard::Up)
            selectedRow = std::max(0, selectedRow - 1);
          if (event.key.code == sf::Keyboard::Down)
            selectedRow = std::min(8, selectedRow + 1);
          if (event.key.code == sf::Keyboard::Left)
            selectedCol = std::max(0, selectedCol - 1);
          if (event.key.code == sf::Keyboard::Right)
            selectedCol = std::min(8, selectedCol + 1);
        }

        // Number Input
        int numInput = -1;
        if (event.key.code >= sf::Keyboard::Num1 &&
            event.key.code <= sf::Keyboard::Num9)
          numInput = event.key.code - sf::Keyboard::Num1 + 1;
        else if (event.key.code >= sf::Keyboard::Numpad1 &&
                 event.key.code <= sf::Keyboard::Numpad9)
          numInput = event.key.code - sf::Keyboard::Numpad1 + 1;

        if (numInput != -1 && selectedRow != -1 && selectedCol != -1) {
          if (!board.isFixed(selectedRow, selectedCol)) {
            if (board.isValid(selectedRow, selectedCol, numInput)) {
              board.placeNumber(selectedRow, selectedCol, numInput);
              currentReason = "Placed " + std::to_string(numInput);
              checkWinCondition();
            } else {
              currentReason = "Invalid Move!";
            }
          } else {
            currentReason = "Cell is Fixed!";
          }
        }

        // Clear Cell
        if ((event.key.code == sf::Keyboard::BackSpace ||
             event.key.code == sf::Keyboard::Delete) &&
            selectedRow != -1) {
          if (!board.isFixed(selectedRow, selectedCol)) {
            board.removeNumber(selectedRow, selectedCol);
            currentReason = "Cleared cell";
          }
        }
      }

      if (event.key.code == sf::Keyboard::Space) {
        if (!solving) {
          if (board.isComplete()) {
            // Already solved, maybe reset?
            currentReason = "Board Complete. Press R to Reset.";
          } else {
            selectedRow = -1;
            selectedCol = -1; // Deselect
            startSolving();
          }
        } else {
          paused = !paused;
        }
      }
      if (event.key.code == sf::Keyboard::P) {
        if (solving)
          paused = !paused;
      }
      if (event.key.code == sf::Keyboard::N) {
        if (solving && paused)
          nextStep = true;
      }
      if (event.key.code == sf::Keyboard::R) {
        if (!solving) {
          board = initialBoard;
          // Reset visual state
          currentR = -1;
          currentC = -1;
          selectedRow = -1;
          selectedCol = -1;
          currentNum = 0;
          currentReason = "Reset Board";
          elapsed = sf::Time::Zero;
        }
      }
      if (event.key.code == sf::Keyboard::G) {
        if (!solving) {
          generator.generate(board, Generator::Difficulty::HARD);
          initialBoard = board;
          currentReason = "Generated New Hard Puzzle";
          currentR = -1;
          currentC = -1;
          selectedRow = -1;
          selectedCol = -1;
          currentNum = 0;
          elapsed = sf::Time::Zero;
        }
      }
      if (event.key.code == sf::Keyboard::Num1) {
        // Only change mode if shifted or control? No, just keep simple. But
        // conflict with input? Solved by: Number input only works if a cell is
        // SELECTED. Mode switching works if NO cell selected? Or use different
        // keys? Let's make Mode Selection require Control key or F1-F3 user
        // requirement didn't specify, but better UX. Actually, if selectedRow
        // == -1, allow mode switch.
        if (selectedRow == -1)
          solver.setMode(Solver::Mode::LOGIC_ONLY);
      }
      if (event.key.code == sf::Keyboard::Num2) {
        if (selectedRow == -1)
          solver.setMode(Solver::Mode::BACKTRACK_ONLY);
      }
      if (event.key.code == sf::Keyboard::Num3) {
        if (selectedRow == -1)
          solver.setMode(Solver::Mode::HYBRID);
      }
      if (event.key.code == sf::Keyboard::Add ||
          event.key.code == sf::Keyboard::Equal) {
        int s = speedDelayMs.load();
        if (s > 0)
          speedDelayMs.store(s - 10);
      }
      if (event.key.code == sf::Keyboard::Subtract ||
          event.key.code == sf::Keyboard::Hyphen) {
        int s = speedDelayMs.load();
        speedDelayMs.store(s + 10);
      }
      if (event.key.code == sf::Keyboard::Escape) {
        window.close();
      }
    }
  }
}

void Visualizer::startSolving() {
  solving = true;
  timer.restart();
  elapsed = sf::Time::Zero;
  solverThread = std::thread([this]() {
    solver.solve();
    solving = false;
  });
}

void Visualizer::solverCallback(int row, int col, int num, std::string reason) {
  currentR = row;
  currentC = col;
  currentNum = num;

  {
    std::lock_guard<std::mutex> lock(reasonMutex);
    currentReason = reason;
  }

  // Handling Pause/Next
  while (paused) {
    if (nextStep) {
      nextStep = false;
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(speedDelayMs.load()));
}

void Visualizer::render() {
  window.clear(sf::Color(20, 20, 30)); // Darker background

  if (solving && !paused) {
    elapsed += timer.restart();
  } else if (!solving) {
    timer.restart(); // keep resetting if not solving
  } else {
    timer.restart(); // Don't accumulate time while paused
  }

  drawGrid();
  drawNumbers();
  drawInfo();
  window.display();
}

void Visualizer::drawGrid() {
  float cellSize = 60.0f;
  float startX = 50.0f;
  float startY = 50.0f;

  for (int i = 0; i <= 9; ++i) {
    sf::RectangleShape line;
    if (i % 3 == 0)
      line.setSize(sf::Vector2f(9 * cellSize, 4));
    else
      line.setSize(sf::Vector2f(9 * cellSize, 1));

    line.setFillColor(sf::Color(150, 150, 150));
    line.setPosition(startX, startY + i * cellSize);
    window.draw(line);

    sf::RectangleShape vline;
    if (i % 3 == 0)
      vline.setSize(sf::Vector2f(4, 9 * cellSize));
    else
      vline.setSize(sf::Vector2f(1, 9 * cellSize));

    vline.setFillColor(sf::Color(150, 150, 150));
    vline.setPosition(startX + i * cellSize, startY);
    window.draw(vline);
  }
}

void Visualizer::drawNumbers() {
  float cellSize = 60.0f;
  float startX = 50.0f;
  float startY = 50.0f;

  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      int val = board.getValue(i, j);

      // Highlights
      sf::RectangleShape cell(sf::Vector2f(cellSize - 4, cellSize - 4));
      cell.setPosition(startX + j * cellSize + 2, startY + i * cellSize + 2);

      bool drawBg = false;

      // Active Cell (Blue)
      if (i == currentR && j == currentC) {
        if (currentNum == 0)
          cell.setFillColor(sf::Color(200, 50, 50, 150)); // Red (Backtrack)
        else
          cell.setFillColor(sf::Color(50, 50, 200, 150)); // Blue (Active)
        drawBg = true;
      }
      // Fixed Cell (White/Grey) is default background? No, let's highlight
      // fixed
      else if (board.isFixed(i, j)) {
        cell.setFillColor(sf::Color(50, 50, 50));
        drawBg = true;
      }

      if (drawBg)
        window.draw(cell);

      // Draw Selection Highlight
      if (!solving && i == selectedRow && j == selectedCol) {
        sf::RectangleShape sel(sf::Vector2f(cellSize - 4, cellSize - 4));
        sel.setPosition(startX + j * cellSize + 2, startY + i * cellSize + 2);
        sel.setFillColor(sf::Color::Transparent);
        sel.setOutlineColor(sf::Color::Cyan);
        sel.setOutlineThickness(3);
        window.draw(sel);
      }

      if (val != 0) {
        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(val));
        text.setCharacterSize(32);

        if (board.isFixed(i, j)) {
          text.setFillColor(sf::Color::White);
        } else if (i == currentR && j == currentC) {
          text.setFillColor(sf::Color::White); // Active
        } else {
          text.setFillColor(sf::Color::Yellow); // Filled
        }

        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition(startX + j * cellSize + (cellSize - bounds.width) / 2 -
                             bounds.left,
                         startY + i * cellSize +
                             (cellSize - bounds.height) / 2 - bounds.top);
        window.draw(text);
      }
    }
  }
}

void Visualizer::drawInfo() {
  sf::Text text;
  text.setFont(font);
  text.setCharacterSize(20);
  text.setFillColor(sf::Color::White);
  text.setPosition(650, 50);

  std::string modeStr = "Hybrid";
  if (solver.getMode() == Solver::Mode::LOGIC_ONLY)
    modeStr = "Logic Only";
  if (solver.getMode() == Solver::Mode::BACKTRACK_ONLY)
    modeStr = "Backtrack Only";

  std::string reasonCopy;
  {
    std::lock_guard<std::mutex> lock(reasonMutex);
    reasonCopy = currentReason;
  }

  std::stringstream ss;
  ss << "--- DASHBOARD ---\n\n";
  ss << "Mode: " << modeStr << "\n";
  ss << "Difficulty: " << solver.difficultyToString(solver.estimateDifficulty())
     << "\n";
  ss << "Time: " << std::fixed << std::setprecision(2) << elapsed.asSeconds()
     << "s\n";
  ss << "\n--- STATS ---\n";
  ss << "Recursions: " << solver.getRecursionCount() << "\n";
  ss << "Backtracks: " << solver.getBacktrackCount() << "\n";
  ss << "Logic Moves: " << solver.getLogicCount() << "\n";

  ss << "\n--- STATUS ---\n";
  if (paused)
    ss << "[ PAUSED ]\n";
  else if (solving)
    ss << "[ SOLVING ]\n";
  else
    ss << "[ IDLE ]\n";

  ss << "\n--- REASON ---\n";
  // Wrap reason text
  std::string wrapped = reasonCopy;
  if (wrapped.length() > 30) {
    size_t p = wrapped.find_last_of(" ", 30);
    if (p != std::string::npos)
      wrapped.insert(p, "\n");
  }
  ss << (wrapped.empty() ? "-" : wrapped) << "\n";

  ss << "\n--- CONTROLS ---\n";
  ss << "Space : Start / Pause\n";
  ss << "N     : Next Step (in Pause)\n";
  ss << "R     : Reset Board\n";
  ss << "G     : Generate New (Hard)\n";
  ss << "1-3   : Mode Select\n";
  ss << "+/-   : Speed (" << speedDelayMs.load() << "ms)\n";
  ss << "ESC   : Exit";

  text.setString(ss.str());
  window.draw(text);
}

void Visualizer::checkWinCondition() {
  if (board.isComplete()) {
    std::lock_guard<std::mutex> lock(reasonMutex);
    currentReason = "CONGRATULATIONS! Board Solved!";
  }
}
