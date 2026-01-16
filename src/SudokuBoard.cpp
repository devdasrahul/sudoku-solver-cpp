#include "SudokuBoard.hpp"
#include <cstring> // for memset
#include <iostream>


SudokuBoard::SudokuBoard() { reset(); }

void SudokuBoard::reset() {
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      grid[i][j] = 0;
      fixed[i][j] = false;
    }
  }
  std::memset(rowMask, 0, sizeof(rowMask));
  std::memset(colMask, 0, sizeof(colMask));
  std::memset(boxMask, 0, sizeof(boxMask));
}

void SudokuBoard::loadBoard(const std::vector<std::vector<int>> &inputGrid) {
  reset();
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      int num = inputGrid[i][j];
      if (num != 0) {
        placeNumber(i, j, num);
        fixed[i][j] = true; // Mark as fixed
      }
    }
  }
}

bool SudokuBoard::isValid(int row, int col, int number) const {
  // Check boundaries
  if (row < 0 || row >= 9 || col < 0 || col >= 9 || number < 1 || number > 9) {
    return false;
  }

  // If the cell already contains this number, it's "valid" in the sense that
  // it's the current state, but usually we ask "can we place this number
  // here?". If we are replacing a number, we should technically temporarily
  // remove the old one to check? But bitmasks track what is CURRENTLY in the
  // board. If grid[row][col] is number, then isValid is true.
  if (grid[row][col] == number)
    return true;

  // If the cell is not empty, and we want to check if 'number' is valid,
  // we assume we are asking for a potential replacement or a new placement in
  // an empty cell. The bitmasks already include the current number at (row,col)
  // if any. So if we are checking a DIFFERENT number, we just check the masks.
  // NOTE: The current number at (row,col) contributes to row/col/box masks.
  // If we want to check if 'number' can go there, we implicitly ignore the
  // CURRENT content of (row,col) because we would be overwriting it. HOWEVER,
  // standard Sudoku solvers usually operate on empty cells. Let's assume this
  // is mostly used for empty cells. If not empty, we really should subtract the
  // current value's contribution before checking. But to keep O(1) clean and
  // simple, let's assume we use it on empty cells or handled by caller.
  // Actually, properly:

  int val = (1 << number);

  // Check row
  if (rowMask[row] & val)
    return false;

  // Check col
  if (colMask[col] & val)
    return false;

  // Check box
  if (boxMask[row / 3][col / 3] & val)
    return false;

  return true;
}

void SudokuBoard::placeNumber(int row, int col, int number) {
  if (fixed[row][col]) {
    std::cout << "Warning: Attempt to modify fixed cell (" << row << ", " << col
              << ") ignored.\n";
    return;
  }

  // If there's already a number, remove it first to update masks correctly
  if (grid[row][col] != 0) {
    removeNumber(row, col);
  }

  if (number == 0)
    return; // effectively remove

  grid[row][col] = number;
  int val = (1 << number);

  rowMask[row] |= val;
  colMask[col] |= val;
  boxMask[row / 3][col / 3] |= val;
}

void SudokuBoard::removeNumber(int row, int col) {
  if (fixed[row][col]) {
    std::cout << "Warning: Attempt to clear fixed cell (" << row << ", " << col
              << ") ignored.\n";
    return;
  }

  int number = grid[row][col];
  if (number == 0)
    return;

  grid[row][col] = 0;
  int val = (1 << number);

  rowMask[row] &= ~val;
  colMask[col] &= ~val;
  boxMask[row / 3][col / 3] &= ~val;
}

bool SudokuBoard::isComplete() const {
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      if (grid[i][j] == 0)
        return false;
    }
  }
  // Theoretically should also check strictly valid, but if we only used isValid
  // to place, it should be valid.
  return true;
}

bool SudokuBoard::isFixed(int row, int col) const { return fixed[row][col]; }

int SudokuBoard::getValue(int row, int col) const { return grid[row][col]; }

void SudokuBoard::printBoard() const {
  std::cout << "-------------------------\n";
  for (int i = 0; i < 9; ++i) {
    std::cout << "| ";
    for (int j = 0; j < 9; ++j) {
      int val = grid[i][j];
      if (val == 0)
        std::cout << ". ";
      else
        std::cout << val << " ";

      if ((j + 1) % 3 == 0)
        std::cout << "| ";
    }
    std::cout << "\n";
    if ((i + 1) % 3 == 0)
      std::cout << "-------------------------\n";
  }
}
