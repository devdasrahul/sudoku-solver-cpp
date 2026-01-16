#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "Solver.hpp"
#include "SudokuBoard.hpp"
#include <random>


class Generator {
public:
  enum class Difficulty { EASY, MEDIUM, HARD, EXPERT };

  Generator();

  // Generates a new puzzle on the provided board
  void generate(SudokuBoard &board, Difficulty level);

private:
  void fillDiagonal(SudokuBoard &board);
  void fillBox(SudokuBoard &board, int row, int col);
  void removeDigits(SudokuBoard &board, int count);

  // Helper to check if a value is safe in a box (without full board overhead if
  // needed, though board.isValid handles it)
  bool unUsedInBox(SudokuBoard &board, int rowStart, int colStart, int num);
};

#endif // GENERATOR_HPP
