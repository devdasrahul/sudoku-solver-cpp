#ifndef SUDOKU_BOARD_HPP
#define SUDOKU_BOARD_HPP

#include <iostream>
#include <vector>


class SudokuBoard {
public:
  SudokuBoard();

  // Load a 9x9 board. 0 represents empty cells.
  void loadBoard(const std::vector<std::vector<int>> &grid);

  // Check if placing number at (row, col) is valid
  bool isValid(int row, int col, int number) const;

  // Place a number at (row, col)
  void placeNumber(int row, int col, int number);

  // Remove a number from (row, col)
  void removeNumber(int row, int col);

  // Check if the board is completely filled and valid
  bool isComplete() const;

  // Check if a cell is fixed (initially given)
  bool isFixed(int row, int col) const;

  // Print the board to console
  void printBoard() const;

  // Get value at (row, col)
  int getValue(int row, int col) const;

private:
  int grid[9][9];
  bool fixed[9][9];

  // Bitmasks for O(1) checking
  // bits 1-9 are used. 0th bit unused (or ignored).
  int rowMask[9];
  int colMask[9];
  int boxMask[3][3];

  void reset();
};

#endif // SUDOKU_BOARD_HPP
