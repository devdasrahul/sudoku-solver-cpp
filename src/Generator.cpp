#include "Generator.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>


Generator::Generator() {}

void Generator::generate(SudokuBoard &board, Difficulty level) {
  // 1. Clear the board
  // We need a way to clear the board, assuming loadBoard with empty grid works
  // or simple reset. SudokuBoard doesn't have a public clear/reset exposed
  // directly other than loadBoard, but we can create an empty grid.
  std::vector<std::vector<int>> emptyGrid(9, std::vector<int>(9, 0));
  board.loadBoard(emptyGrid);

  // 2. Fill Diagonal Boxes (independent of each other)
  fillDiagonal(board);

  // 3. Solve the rest randomly
  Solver solver(board);
  // Suppress callbacks during generation if any were set?
  // Solver internal state is fresh.
  bool success = solver.solveRandomized();

  if (!success) {
    std::cerr << "Error: Failed to generate a valid board base!" << std::endl;
    return;
  }

  // At this point, board is fully filled and valid.
  // 4. Remove Digits
  int k = 0;
  switch (level) {
  case Difficulty::EASY:
    k = 30;
    break; // Remove 30 -> 51 clues (Very Easy)
  case Difficulty::MEDIUM:
    k = 45;
    break; // Remove 45 -> 36 clues
  case Difficulty::HARD:
    k = 54;
    break; // Remove 54 -> 27 clues
  case Difficulty::EXPERT:
    k = 60;
    break; // Remove 60 -> 21 clues
  default:
    k = 40;
  }

  removeDigits(board, k);

  // Now re-load the board into itself to fix the remaining cells?
  // Or does removeDigits just set them to 0?
  // Expected behavior: The remaining numbers should be "Fixed" (Clues), and the
  // removed ones are 0. But SudokuBoard.placeNumber/removeNumber updates grid.
  // We want the resulting state to treat the remaining numbers as FIXED.
  // Current SudokuBoard.loadBoard does exactly that: takes a grid and marks
  // non-zero as fixed.

  // Extract current grid
  std::vector<std::vector<int>> queryGrid(9, std::vector<int>(9));
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      queryGrid[i][j] = board.getValue(i, j);
    }
  }
  board.loadBoard(queryGrid);
}

void Generator::fillDiagonal(SudokuBoard &board) {
  for (int i = 0; i < 9; i = i + 3) {
    fillBox(board, i, i);
  }
}

void Generator::fillBox(SudokuBoard &board, int row, int col) {
  int num;
  std::random_device rd;
  std::mt19937 g(rd());

  // We simply need to fill the 3x3 box with 1-9 randomly.
  // Since it's diagonal, no row/col conflicts exist yet with other diagonal
  // boxes.
  std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::shuffle(nums.begin(), nums.end(), g);

  int idx = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      board.placeNumber(row + i, col + j, nums[idx++]);
    }
  }
}

void Generator::removeDigits(SudokuBoard &board, int count) {
  std::random_device rd;
  std::mt19937 g(rd());

  while (count > 0) {
    int cellId = std::uniform_int_distribution<>(0, 80)(g);
    int i = cellId / 9;
    int j = cellId % 9;

    if (board.getValue(i, j) != 0) {
      // Check if removing this cell leaves a unique solution?
      // "True" Sudoku generation requires checking uniqueness.
      // For this project, we might skip the rigorous uniqueness check for
      // performance and "playability" speed, or we can add a quick check. Let's
      // just remove for now to keep it responsive. If we want uniqueness, we'd
      // need to attempt solve for every removal.

      board.removeNumber(i, j);
      count--;
    }
  }
}
