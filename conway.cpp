#include <chrono>
#include <ctime>
#include <curses.h>
#include <iostream>
#include <stdlib.h>
#include <thread>
using namespace std;

const int ROWS = 24;   // Smaller for easier terminal view
const int COLUMNS = 80;

bool universe[ROWS][COLUMNS];
bool next_universe[ROWS][COLUMNS];

// Initialize curses
void InitializeCurses() {
  initscr();
  cbreak();
  noecho();
  clear();
}

void PrintExitInstructions() {
  mvaddstr(ROWS + 1, 0, "Press ctrl+c to quit ");
  refresh();
}

void InitializeUniverse() {
  srand(time(0));
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      universe[i][j] = rand() % 4 == 0; // ~25% chance alive
    }
  }
}

int CountNeighbors(int row, int col) {
  int count = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (i == 0 && j == 0) continue; // Skip self
      int ni = row + i;
      int nj = col + j;
      if (ni >= 0 && ni < ROWS && nj >= 0 && nj < COLUMNS) {
        if (universe[ni][nj]) count++;
      }
    }
  }
  return count;
}

void UpdateUniverse() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      int neighbors = CountNeighbors(i, j);
      if (universe[i][j]) {
        next_universe[i][j] = (neighbors == 2 || neighbors == 3);
      } else {
        next_universe[i][j] = (neighbors == 3);
      }
    }
  }
  // Copy back to main universe
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      universe[i][j] = next_universe[i][j];
    }
  }
}

void PrintUniverse() {
  for (int i = 0; i < ROWS; i++) {
    move(i, 0);
    for (int j = 0; j < COLUMNS; j++) {
      addch(universe[i][j] ? '*' : ' ');
    }
  }
  refresh();
}

int main() {
  InitializeCurses();
  PrintExitInstructions();
  InitializeUniverse();

  while (true) {
    PrintUniverse();
    UpdateUniverse();
    this_thread::sleep_for(chrono::milliseconds(200));
  }

  endwin();
  return 0;
}
