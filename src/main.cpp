#include "Solver.hpp"
#include "SudokuBoard.hpp"
#include "Visualizer.hpp"
#include <iostream>
#include <vector>


int main() {
  std::cout << "Starting Sudoku Visualizer...\n";

  SudokuBoard board;
  // Sample board
  std::vector<std::vector<int>> sampleGrid = {
      {5, 3, 0, 0, 7, 0, 0, 0, 0}, {6, 0, 0, 1, 9, 5, 0, 0, 0},
      {0, 9, 8, 0, 0, 0, 0, 6, 0}, {8, 0, 0, 0, 6, 0, 0, 0, 3},
      {4, 0, 0, 8, 0, 3, 0, 0, 1}, {7, 0, 0, 0, 2, 0, 0, 0, 6},
      {0, 6, 0, 0, 0, 0, 2, 8, 0}, {0, 0, 0, 4, 1, 9, 0, 0, 5},
      {0, 0, 0, 0, 8, 0, 0, 7, 9}};

  board.loadBoard(sampleGrid);

  Solver solver(board);
  Visualizer visualizer(board, solver);

  std::cout << "Launching Visualizer... Press SPACE to Solve, +/- for speed.\n";
  visualizer.run();

  return 0;
}
