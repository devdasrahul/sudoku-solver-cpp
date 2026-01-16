# How to Push Your Sudoku Project to GitHub

This guide will help you upload your C++ Sudoku Solver to GitHub.

## 1. Prerequisites

- You must have **Git** installed on your computer.
- You must have a **GitHub account**.

## 2. Prepare Your Project

I have already created a `.gitignore` file for you. This file tells Git which files to **IGNORE** (not upload).

**What gets pushed?**
- `src/` (Source code: `.cpp` files)
- `include/` (Header files: `.hpp` files)
- `assets/` (Fonts, images)
- `CMakeLists.txt` (Build configuration)
- `README.md` (Project description)
- `.gitignore` (Configuration for ignored files)
- `GITHUB_SETUP.md` (This guide)

**What is IGNORED (Not pushed)?**
- `build/` folder (Generated build files)
- `.exe` files (Executables don't belong in source control)
- `.vscode/` (Your local editor settings)
- `_deps/` (Downloaded dependencies)

## 3. Create a Repository on GitHub

1.  Log in to [GitHub.com](https://github.com/).
2.  Click the **+** icon in the top-right corner and select **New repository**.
3.  Name it: `sudoku-solver-cpp` (or anything you like).
4.  Description: "A Hybrid Sudoku Solver & Visualizer using C++ and SFML".
5.  **Do NOT** initialize with README, .gitignore, or License (we have them local).
6.  Click **Create repository**.
7.  Copy the URL provided (e.g., `https://github.com/Start-0000/sudoku-solver-cpp.git`).

## 4. Initialize Git Locally

Open your terminal in VS Code (ensure you are in `d:\rahul\Cpp-Mastery\Suduko c++`) and run these commands one by one:

```powershell
# 1. Initialize a new git repository
git init

# 2. Add all files to the staging area
git add .

# 3. Commit the files (Save the snapshot)
git commit -m "Initial commit: Sudoku Solver with SFML Visualizer"

# 4. Rename the default branch to 'main'
git branch -M main

# 5. Link your local repo to GitHub (Replace URL with YOUR repo URL from Step 3)
git remote add origin https://github.com/YOUR_USERNAME/sudoku-solver-cpp.git

# 6. Push the code to GitHub
git push -u origin main
```

## 5. Verify

Go back to your GitHub repository page and refresh. You should see all your files (`src`, `include`, `README.md`) listed there!
