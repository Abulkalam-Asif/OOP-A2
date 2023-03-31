#include "SpaceInvaders.h"
#include <chrono>
#include <conio.h>
#include <iostream>
#include <thread>
#include <vector>
#include <windows.h>
using namespace std;
#define SIDEBAR 30

int score = 0;
int lives = 3;
vector<Enemy> enemyVec;
int scoreToWin = 10;

int main() {
  srand(time(NULL));

  string welcome = "Welcome to SPACE INVADERS";
  string loading = "Loading ... ";
  cursorAtCenter(welcome.size());
  cout << welcome;

  setTimeInterval(1000);
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
  showInfo();
  _getch();
  setTimeInterval(500);
  system("cls");
  showInfo();

  Player p;
  p.setSymbol('A');
  vector<int> currentPos = returnRandomCursorPos('P');
  takeAndSetCursor(currentPos[0], currentPos[1]);
  p.setPosX(currentPos[0]);
  p.setPosY(currentPos[1]);
  cout << p.getSymbol();
  bringCursorToSide();
  for (int i = 0; i < 4; i++) {
    setTimeInterval(100);
    currentPos.clear();
    currentPos = returnRandomCursorPos('E');
    if (isEnemyAtThatPos(currentPos[0], currentPos[1])) {
      --i;
      continue;
    }
    takeAndSetCursor(currentPos[0], currentPos[1]);
    Enemy e;
    e.setSymbol('Y');
    cout << e.getSymbol();
    e.setPosX(currentPos[0]);
    e.setPosY(currentPos[1]);
    enemyVec.push_back(e);
    bringCursorToSide();
  }

  thread t2(moveEnemy);
  thread t(playerActions, p);

  t.join();
  t2.join();

  return 0;
}