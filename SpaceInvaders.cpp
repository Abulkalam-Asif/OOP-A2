#include "SpaceInvaders.h"
#include <chrono>
#include <conio.h>
#include <iostream>
#include <thread>
#include <vector>
#include <windows.h>
using namespace std;
#define SIDEBAR 20

int score = 0;
int lives = 3;
vector<vector<int>> enemyPositions;

int main() {
  string welcome = "Welcome to SPACE INVADERS";
  string loading = "Loading ... ";
  cursorAtCenter(welcome.size());
  cout << welcome;

  setTimeInterval(500);
  system("cls");
  cursorAtCenter(loading.size());
  cout << loading;

  setTimeInterval(500);
  cout << "3";
  setTimeInterval(500);
  cout << "\b2";
  setTimeInterval(500);
  cout << "\b1";

  setTimeInterval(500);
  system("cls");
  cursorAtCenter(24);
  cout << "* * press s to START * *";
  getch();
  setTimeInterval(500);
  system("cls");

  divideScreen();
  takeAndSetCursor(csbInfo.srWindow.Right - SIDEBAR + 3, 3);
  cout << "Score: 0";
  takeAndSetCursor(csbInfo.srWindow.Right - SIDEBAR + 3, 5);
  cout << "Score 5 to win.";
  takeAndSetCursor(csbInfo.srWindow.Right - SIDEBAR + 3, 7);
  cout << "Lives: 3";
  Player p;
  p.setSymbol('A');
  vector<int> currentPos = returnRandomCursorPos('P');
  takeAndSetCursor(currentPos[0], currentPos[1]);
  p.setPosX(currentPos[0]);
  p.setPosY(currentPos[1]);
  cout << p.getSymbol();
  srand(time(NULL));
  bringCursorToSide();
  for (int i = 0; i < 5; i++) {
    setTimeInterval(100);
    currentPos.clear();
    currentPos = returnRandomCursorPos('E');
    if (isEnemyAtThatPos(currentPos[0])) {
      --i;
      continue;
    }
    takeAndSetCursor(currentPos[0], currentPos[1]);
    Enemy e;
    e.setSymbol('Y');
    enemyPositions.push_back(currentPos);
    e.setPosX(currentPos[0]);
    e.setPosY(currentPos[1]);
    cout << e.getSymbol();
    // Bullet eb;
    // eb.setPosX(e.getPosX());
    // eb.setPosY(e.getPosY() + 1);
    // eb.setSymbol('.');
    bringCursorToSide();
  }

  while (score != 5) {
    char movement = _getch();
    if (movement == 75 && p.getPosX() > csbInfo.srWindow.Left) {
      takeAndSetCursor(p.getPosX(), p.getPosY());
      cout << " ";
      p.setPosX(p.getPosX() - 1);
      takeAndSetCursor(p.getPosX(), p.getPosY());
      cout << p.getSymbol();
      bringCursorToSide();
    } else if (movement == 77 && p.getPosX() < csbInfo.srWindow.Right - SIDEBAR - 1) {
      takeAndSetCursor(p.getPosX(), p.getPosY());
      cout << " ";
      p.setPosX(p.getPosX() + 1);
      takeAndSetCursor(p.getPosX(), p.getPosY());
      cout << p.getSymbol();
      bringCursorToSide();
    } else if (movement == 32) {
      Bullet pb;
      pb.setPosX(p.getPosX());
      pb.setPosY(p.getPosY() - 1);
      pb.setSymbol('|');
      pb.traverse('P');
    }
  }

  system("cls");
  cursorAtCenter(7);
  cout << "You won!";

  bringCursorToSide();

  return 0;
}