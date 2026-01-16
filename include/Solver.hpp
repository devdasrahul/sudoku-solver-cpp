#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "SudokuBoard.hpp"
#include <functional>
#include <random>
#include <utility>


class Solver {
public:
  // Constructor takes a reference to the board to solve
  explicit Solver(SudokuBoard &board);

  enum class Mode { LOGIC_ONLY, BACKTRACK_ONLY, HYBRID };
  enum class Difficulty { UNKNOWN, EASY, MEDIUM, HARD, EXPERT };

  void setMode(Mode mode);
  Mode getMode() const;

  // Solves the board using backtracking or logic based on Mode
  // Returns true if a solution is found
  bool solve();
  bool solveRandomized();

  // Try to solve using logical techniques (Human-like)
  // Returns true if fully solved using logic
  bool solveHumanistic();

  // Set a callback to observe steps (for visualization)
  // Callback params: row, col, number (0 implies backtracking/removal), reason
  void
  setStepCallback(std::function<void(int, int, int, std::string)> callback);

  long long getRecursionCount() const;
  long long getBacktrackCount() const;
  long long getLogicCount() const;
  Difficulty estimateDifficulty() const;
  std::string difficultyToString(Difficulty d) const;

private:
  SudokuBoard &board;
  std::function<void(int, int, int, std::string)> stepCallback;

  long long recursionCount = 0;
  long long backtrackCount = 0;
  long long logicCount = 0;
  Mode currentMode = Mode::HYBRID;

  bool solveRecursive();
  bool solveRecursiveRandomized(std::mt19937 &g);
  bool findBestCell(int &row, int &col); // MRV Heuristic

  // Human-like techniques
  bool applyNakedSingles();
  bool applyHiddenSingles();
  int getCandidates(int row, int col); // Returns bitmask of candidates
  int countSetBits(int n);
};

#endif // SOLVER_HPP
