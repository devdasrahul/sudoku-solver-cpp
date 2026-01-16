#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include "Generator.hpp"
#include "Solver.hpp"
#include "SudokuBoard.hpp"
#include <SFML/Graphics.hpp>
#include <atomic>
#include <mutex>
#include <thread>


class Visualizer {
public:
  Visualizer(SudokuBoard &board, Solver &solver);
  ~Visualizer();
  void run();

private:
  SudokuBoard &board;
  Solver &solver;
  Generator generator;
  sf::RenderWindow window;
  sf::Font font;
  SudokuBoard initialBoard;

  std::thread solverThread;
  std::atomic<bool> solving{false};
  // We use mutex potentially, but for simple visualization we might skip
  // rigorous locking if we just read ints. But let's keep it safe.

  std::atomic<int> currentR{-1};
  std::atomic<int> currentC{-1};
  std::atomic<int> currentNum{0};

  // Selection state for Play Mode
  int selectedRow{-1};
  int selectedCol{-1};

  // Controls
  std::atomic<int> speedDelayMs{50};
  std::atomic<bool> paused{false};
  std::atomic<bool> nextStep{false};
  std::string currentReason;
  std::mutex reasonMutex;
  sf::Clock timer;
  sf::Time elapsed;

  void processEvents();
  void render();
  void drawGrid();
  void drawNumbers();
  void drawInfo();
  void checkWinCondition(); // Check if user has won

  void startSolving();
  void solverCallback(int row, int col, int num, std::string reason);
};

#endif // VISUALIZER_HPP
