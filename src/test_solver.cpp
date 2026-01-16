#include "Solver.hpp"
#include "SudokuBoard.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

void printHeader(const std::string &title) {
  std::cout << "\n========================================\n";
  std::cout << "  " << title << "\n";
  std::cout << "========================================\n";
}

void runTest(const std::string &name, const std::vector<std::vector<int>> &grid,
             Solver::Mode mode) {
  printHeader("TEST: " + name);

  SudokuBoard board;
  board.loadBoard(grid);
  Solver solver(board);
  solver.setMode(mode);

  // Callback to log steps
  solver.setStepCallback([](int r, int c, int n, std::string reason) {
    // Only print interesting logical steps or first few backtracks to avoid
    // spam
    if (reason.find("Backtrack") == std::string::npos || rand() % 20 == 0) {
      std::cout << "[STEP] (" << r << "," << c << ") -> " << n << " : "
                << reason << "\n";
    }
  });

  std::cout << "Difficulty Estimate: "
            << solver.difficultyToString(solver.estimateDifficulty()) << "\n";

  bool solved = solver.solve();

  if (solved) {
    std::cout << "\n[SUCCESS] Solved!\n";
  } else {
    std::cout << "\n[FAILURE] Could not solve.\n";
  }

  std::cout << "Stats:\n";
  std::cout << "  Logic Moves : " << solver.getLogicCount() << "\n";
  std::cout << "  Backtracks  : " << solver.getBacktrackCount() << "\n";
  std::cout << "  Recursions  : " << solver.getRecursionCount() << "\n";

  if (solved && !board.isComplete()) {
    std::cout << "ERROR: solver returned true but board is not complete!\n";
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  std::string mode = (argc > 1) ? argv[1] : "all";

  // 1. Easy Puzzle (Solvable by Logic)
  // From a known easy source
  std::vector<std::vector<int>> easyGrid = {
      {0, 0, 0, 2, 6, 0, 7, 0, 1}, {6, 8, 0, 0, 7, 0, 0, 9, 0},
      {1, 9, 0, 0, 0, 4, 5, 0, 0}, {8, 2, 0, 1, 0, 0, 0, 4, 0},
      {0, 0, 4, 6, 0, 2, 9, 0, 0}, {0, 5, 0, 0, 0, 3, 0, 2, 8},
      {0, 0, 9, 3, 0, 0, 0, 7, 4}, {0, 4, 0, 0, 5, 0, 0, 3, 6},
      {7, 0, 3, 0, 1, 8, 0, 0, 0}};

  // 2. Hard Puzzle (Requires Backtracking)
  // "AI Escargot" or similar is too hard, let's pick a generic "Hard" one
  std::vector<std::vector<int>> hardGrid = {
      {0, 2, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 6, 0, 0, 0, 0, 3},
      {0, 7, 4, 0, 8, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 3, 0, 0, 2},
      {0, 8, 0, 0, 4, 0, 0, 1, 0}, {6, 0, 0, 5, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 1, 0, 7, 8, 0}, {5, 0, 0, 0, 0, 9, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 4, 0}};

  if (mode == "easy" || mode == "all") {
    runTest("Easy (Logic Check)", easyGrid, Solver::Mode::HYBRID);
  }
  if (mode == "hard" || mode == "all") {
    runTest("Hard (Backtrack Check)", hardGrid, Solver::Mode::HYBRID);
  }

  return 0;
}
