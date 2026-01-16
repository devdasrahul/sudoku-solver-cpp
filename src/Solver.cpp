#include "Solver.hpp"
#include <algorithm>
#include <climits>
#include <iostream>
#include <random>
#include <string>
#include <vector>

Solver::Solver(SudokuBoard &board) : board(board) {}

void Solver::setStepCallback(
    std::function<void(int, int, int, std::string)> callback) {
  stepCallback = callback;
}

void Solver::setMode(Mode mode) { currentMode = mode; }
Solver::Mode Solver::getMode() const { return currentMode; }

long long Solver::getLogicCount() const { return logicCount; }
long long Solver::getRecursionCount() const { return recursionCount; }
long long Solver::getBacktrackCount() const { return backtrackCount; }

Solver::Difficulty Solver::estimateDifficulty() const {
  // Simple heuristic based on empty cells and backtrack count
  // Note: This is an estimation. Real difficulty depends on techniques
  // required.
  int emptyCount = 0;
  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      if (board.getValue(r, c) == 0)
        emptyCount++;
    }
  }

  // If we have already solved or tried to solve, use backtrack stats
  if (backtrackCount > 500)
    return Difficulty::EXPERT;
  if (backtrackCount > 50)
    return Difficulty::HARD;

  // Initial estimation based on givens
  if (emptyCount < 30)
    return Difficulty::EASY;
  if (emptyCount < 45)
    return Difficulty::MEDIUM;
  if (emptyCount < 55)
    return Difficulty::HARD;
  return Difficulty::EXPERT;
}

std::string Solver::difficultyToString(Difficulty d) const {
  switch (d) {
  case Difficulty::EASY:
    return "Easy";
  case Difficulty::MEDIUM:
    return "Medium";
  case Difficulty::HARD:
    return "Hard";
  case Difficulty::EXPERT:
    return "Expert";
  default:
    return "Unknown";
  }
}

bool Solver::solve() {
  recursionCount = 0;
  backtrackCount = 0;
  logicCount = 0;

  if (currentMode == Mode::LOGIC_ONLY || currentMode == Mode::HYBRID) {
    solveHumanistic();
    if (currentMode == Mode::LOGIC_ONLY)
      return board.isComplete();
  }

  if (board.isComplete())
    return true;

  if (currentMode == Mode::BACKTRACK_ONLY || currentMode == Mode::HYBRID) {
    return solveRecursive();
  }
  return false;
}

bool Solver::solveRandomized() {
  recursionCount = 0;
  backtrackCount = 0;
  // We need a random engine.
  // Since we don't want to re-seed every recursive call, pass it down?
  // Or just construct it here.
  std::random_device rd;
  std::mt19937 g(rd());
  return solveRecursiveRandomized(g);
}

int Solver::countSetBits(int n) {
  int count = 0;
  while (n > 0) {
    n &= (n - 1);
    count++;
  }
  return count;
}

bool Solver::findBestCell(int &row, int &col) {
  // MRV Heuristic: Find cell with minimum remaining candidates
  int minCandidates = 10;
  row = -1;
  col = -1;

  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      if (board.getValue(r, c) == 0) {
        int mask = getCandidates(r, c);
        int count = countSetBits(mask);
        if (count < minCandidates) {
          minCandidates = count;
          row = r;
          col = c;
          if (minCandidates == 1)
            return true; // Can't beat 1
        }
      }
    }
  }
  return (row != -1);
}

int Solver::getCandidates(int row, int col) {
  int mask = 0;
  for (int n = 1; n <= 9; ++n) {
    if (board.isValid(row, col, n)) {
      mask |= (1 << n);
    }
  }
  return mask;
}

bool Solver::solveHumanistic() {
  bool progress = true;
  while (progress) {
    progress = false;
    if (applyNakedSingles()) {
      progress = true;
      continue;
    }
    if (applyHiddenSingles()) {
      progress = true;
      continue;
    }
  }
  return board.isComplete();
}

bool Solver::applyNakedSingles() {
  bool change = false;
  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      if (board.getValue(r, c) != 0)
        continue;

      int mask = getCandidates(r, c);
      int count = countSetBits(mask);

      if (count == 1) {
        // Find the bit
        int val = 0;
        for (int n = 1; n <= 9; ++n) {
          if (mask & (1 << n)) {
            val = n;
            break;
          }
        }
        board.placeNumber(r, c, val);
        logicCount++;
        if (stepCallback)
          stepCallback(r, c, val,
                       "Naked Single at (" + std::to_string(r) + "," +
                           std::to_string(c) + ")");
        change = true;
        // Correctness: Placing a number changes constraints for others.
        // We should return true to restart the scan to be perfectly safe,
        // though finding all independent naked singles is an optimization.
        return true;
      }
    }
  }
  return change;
}

bool Solver::applyHiddenSingles() {
  // Rows
  for (int r = 0; r < 9; ++r) {
    int counts[10] = {0};
    int lastPos[10] = {0};

    for (int c = 0; c < 9; ++c) {
      if (board.getValue(r, c) == 0) {
        int mask = getCandidates(r, c);
        for (int n = 1; n <= 9; ++n) {
          if (mask & (1 << n)) {
            counts[n]++;
            lastPos[n] = c;
          }
        }
      }
    }
    for (int n = 1; n <= 9; ++n) {
      if (counts[n] == 1) {
        board.placeNumber(r, lastPos[n], n);
        logicCount++;
        if (stepCallback)
          stepCallback(r, lastPos[n], n,
                       "Hidden Single in Row " + std::to_string(r));
        return true;
      }
    }
  }

  // Cols
  for (int c = 0; c < 9; ++c) {
    int counts[10] = {0};
    int lastPos[10] = {0};

    for (int r = 0; r < 9; ++r) {
      if (board.getValue(r, c) == 0) {
        int mask = getCandidates(r, c);
        for (int n = 1; n <= 9; ++n) {
          if (mask & (1 << n)) {
            counts[n]++;
            lastPos[n] = r;
          }
        }
      }
    }
    for (int n = 1; n <= 9; ++n) {
      if (counts[n] == 1) {
        board.placeNumber(lastPos[n], c, n);
        logicCount++;
        if (stepCallback)
          stepCallback(lastPos[n], c, n,
                       "Hidden Single in Col " + std::to_string(c));
        return true;
      }
    }
  }

  // Boxes
  for (int br = 0; br < 3; ++br) {
    for (int bc = 0; bc < 3; ++bc) {
      int counts[10] = {0};
      int lastPosR[10] = {0};
      int lastPosC[10] = {0};

      for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
          int r = br * 3 + i;
          int c = bc * 3 + j;
          if (board.getValue(r, c) == 0) {
            int mask = getCandidates(r, c);
            for (int n = 1; n <= 9; ++n) {
              if (mask & (1 << n)) {
                counts[n]++;
                lastPosR[n] = r;
                lastPosC[n] = c;
              }
            }
          }
        }
      }

      for (int n = 1; n <= 9; ++n) {
        if (counts[n] == 1) {
          board.placeNumber(lastPosR[n], lastPosC[n], n);
          logicCount++;
          if (stepCallback)
            stepCallback(lastPosR[n], lastPosC[n], n,
                         "Hidden Single in Box (" + std::to_string(br) + "," +
                             std::to_string(bc) + ")");
          return true;
        }
      }
    }
  }

  return false;
}

bool Solver::solveRecursive() {
  recursionCount++;
  int row, col;

  // Use MRV to find best cell
  if (!findBestCell(row, col))
    return true; // No empty cells left

  for (int num = 1; num <= 9; num++) {
    if (board.isValid(row, col, num)) {
      board.placeNumber(row, col, num);
      if (stepCallback)
        stepCallback(row, col, num, "Backtrack Try: " + std::to_string(num));

      if (solveRecursive())
        return true;

      // Backtrack
      board.removeNumber(row, col);
      if (stepCallback)
        stepCallback(row, col, 0, "Backtracking");
      backtrackCount++;
    }
  }
  return false;
}

bool Solver::solveRecursiveRandomized(std::mt19937 &g) {
  recursionCount++;
  int row, col;

  if (!findBestCell(row, col))
    return true;

  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::shuffle(numbers.begin(), numbers.end(), g);

  for (int num : numbers) {
    if (board.isValid(row, col, num)) {
      board.placeNumber(row, col, num);
      if (solveRecursiveRandomized(g))
        return true;
      board.removeNumber(row, col);
      backtrackCount++;
    }
  }
  return false;
}
