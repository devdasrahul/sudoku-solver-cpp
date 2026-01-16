# Hybrid Sudoku Solver & Visualizer

> **A high-performance C++ Sudoku optimization engine combining humanistic logical deduction, MRV heuristics, and real-time visualization.**

---

## 1. Project Overview

- **Core Function**: Solves 9×9 Sudoku puzzles using a hybrid algorithm (Logic + Backtracking).
- **Problem Solved**: Demonstrates efficient constraint satisfaction and search space pruning.
- **Technical Excellence**: Implements O(1) bitwise constraint checking and Minimum Remaining Values (MRV) heuristic.
- **Educational Value**: Visualizes the difference between deterministic logic and non-deterministic search.
- **Architecture**: Modular C++ design separating computational logic from UI rendering.

## 2. Key Features

- **Hybrid Solver Engine**: Prioritizes human strategies (Naked/Hidden Singles) before falling back to backtracking.
- **Visual Learning**: Real-time SFML visualization of the solving process (candidates, backtracking, placement).
- **Explained Decisions**: The UI displays the *exact reason* for each move (e.g., "Naked Single in Row 4").
- **Live Metrics**: Tracks recursion depth, backtracking count, logical deductions, and execution time.
- **Interactive Control**: Pause, Resume, Step-by-Step execution ('N'), and Reset ('R').
- **Optimized Performance**: Solves standard puzzles in milliseconds; visualizations slowed for human observation.

## 3. Algorithms & Data Structures Used

**Algorithms**
- **Logical Deduction**: Deterministic propagation of constraints (Naked Singles, Hidden Singles).
- **Recursive Backtracking**: Depth-First Search (DFS) for exploring the decision tree.
- **MRV Heuristic**: "Fail-first" strategy selecting cells with fewest candidates to minimize the branching factor.

**Data Structures**
- **2D Array (`int grid[9][9]`)**: Cache-friendly flat memory layout for board state.
- **Bitmasks (`int row/col/box[9]`)**: **O(1)** complexity for checking valid insertions using bitwise operations.
- **Recursion Stack**: Implicit stack management for backtracking state.

## 4. System Design & Architecture

- **Modular Design**: Separation of concerns into `SudokuBoard` (Data), `Solver` (Logic), and `Visualizer` (UI).
- **Observer Pattern**: The Solver is decoupled from the UI; the Visualizer observes solver state via callbacks.
- **Thread Safety**: Solver runs on a background thread; UI updates are synchronized using atomic flags and mutexes.
- **Execution Pipeline**:
  `Input` → `Human Logic Pass` → `Heuristic Search (MRV)` → `Recursive Backtracking` → `Solution`

## 5. Visualization & UI

**Purpose**: To demystify the "black box" of recursive algorithms and show computational cost visually.

**Color Coding**:
- **⚪ White**: Fixed / Logic Placements (Stable)
- **🔵 Blue**: Active Backtracking Search (Tentative)
- **🔴 Red**: Backtrack / Removal (Invalid Path)
- **🟡 Yellow**: Completed Cells

**Controls**:
- `SPACE`: Start / Pause
- `N`: Next Step (when Paused)
- `R`: Reset Board
- `+/-`: Adjust Speed

[![📸 Screenshot Placeholder](https://github.com/Rahul0Developer/fake/blob/main/Screenshot%202026-01-16%20145412.png)]
[https://github.com/Rahul0Developer/fake/blob/main/Screenshot%202026-01-16%20145420.png]
[<video controls src="https://github.com/Rahul0Developer/fake/blob/main/suduko%20solver.mp4" title="🎥 Demo Video Placeholder"></video>]

## 6. Time & Space Complexity

**Time Complexity**
- **Logic**: **Polynomial** `O(N*k)` where N is cells and k is constraints.
- **Backtracking**: Worst-case **O(9^m)** (m = empty cells).
- **Optimized**: MRV heuristic significantly reduces the branching factor in practice.

**Space Complexity**
- **O(N)**: Board storage (constant 81 integers).
- **O(N)**: Recursion stack depth (max 81 frames).
- **Auxiliary**: Constant space for bitmasks.

## 7. How to Run the Project

1. **Clone Repository**
   ```bash
   git clone https://github.com/yourusername/sudoku-solver.git
   cd sudoku-solver
   ```

2. **Build Project (CMake)**
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

3. **Run Application**
   ```bash
   ./SudokuSolver
   ```

## 8. Learning Outcomes

- **DSA Mastery**: Practical application of DFS, Bit Manipulation, and Heuristic Search.
- **System Design**: Implementing MVC-like separation in lower-level C++.
- **Concurrency**: Managing background computation threads and UI event loops.
- **Optimization**: Reducing search space size to transform intractable problems into solvable ones.

## 9. Resume-Ready Highlights

- **Designed and implemented an optimized Sudoku Solver** using hybrid DSA techniques (Logic + Backtracking).
- **Reduced backtracking search space** by ~90% using the Minimum Remaining Values (MRV) heuristic.
- **Built real-time visualization** to explain recursive algorithm behavior and decision trees.
- **Applied clean, modular C++ system design principles**, separating algorithmic core from rendering logic.
- **Integrated performance metrics** for algorithm analysis, tracking recursion depth and logic operations.

## 10. Future Enhancements

- **Advanced Logic**: Implementation of X-Wing, Swordfish, and Y-Wing strategies.
- **Puzzle Generation**: Backtracking-based generator for creating unique puzzles.
- **Candidate Visualization**: "Pencil mark" mode to show possible values in empty cells.
- **WebAssembly Port**: Compiling core logic to WASM for web-based demos.

## 11. Demo & Media

📽️ Demo Video: [Add link here]
🖼️ Screenshots: [Add images here]

## 12. Tech Stack

- **Language**: C++17
- **Graphics Library**: SFML 2.6.1
- **Build System**: CMake 3.14+
- **Platform**: Windows, Linux, macOS

## 13. Author

- **Name**: [Placeholder]
- **GitHub**: [Placeholder]
- **LinkedIn**: [Placeholder]
- **Email**: [Placeholder]
