# Console Sudoku Project

## General Description
Project made for a school assignment

## Game Screenshots

<div style="display: flex; justify-content: space-between;">
  <img src="images/menu.png" alt="Main menu" width="225">
  <img src="images/9x9.png" alt="9x9 Board" width="225" style="margin-right: 10px;">
  <img src="images/6x6solved.png" alt="6x6 Solved Board" width="225" style="margin-right: 10px;">
  <img src="images/3x3.png" alt="3x3 Board" width="225">
</div>

## How to Build the project
1. Clone the repository in Visual Studio:
   - `git clone https://github.com/Mushkrem/sudoku.git`
2. Build the solution inside Visual Studio (Release x64):
3. The built executable should be inside `.\x64\Release\sudoku.exe`

## Running the latest released executable
1. Download the latest executable from [Releases](https://github.com/Mushkrem/sudoku/releases/latest)
2. Simplu run it on any Windows operating system

The project should be compatible with legacy consoles, ensuring correct rendering. However it was written for the latest [Windows Terminal](https://apps.microsoft.com/detail/9n0dx20hk701?hl=en-US&gl=US) application.

Additionally, all Sudoku-related coding tasks, including solving boards, generating new ones, and ensuring unique solutions, are handled within the `sudoku/game.c` file
