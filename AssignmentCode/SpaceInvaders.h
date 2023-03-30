#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <time.h>
#include <vector>
#include <windows.h>
#define SIDEBAR 20

using namespace std;

extern int score;
extern int lives;
extern vector<vector<int>> enemyPositions;

using namespace std;
class GameObject;
class Player;
class Enemy;
class Bullet;

void setTimeInterval(int);
void cursorAtCenter(int);
void bringCursorToSide();
vector<int> returnRandomCursorPos(char);
void takeAndSetCursor(int, int);
void updateScore();
void addEnemy();
bool isEnemyAtThatPos(int);

HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPosition;
CONSOLE_SCREEN_BUFFER_INFO csbInfo;

class GameObject {
private:
  int posX;
  int posY;
  char symbol;

public:
  GameObject();
  GameObject(int, int);
  int getPosX();
  int getPosY();
  char getSymbol();
  void setPosX(int);
  void setPosY(int);
  void setSymbol(char);
};
GameObject::GameObject() {
  posX = 0;
  posY = 0;
}
GameObject::GameObject(int x, int y) {
  posX = x;
  posY = y;
}
int GameObject::getPosX() {
  return posX;
}
int GameObject::getPosY() {
  return posY;
}
char GameObject::getSymbol() {
  return symbol;
}
void GameObject::setPosX(int x) {
  posX = x;
}
void GameObject::setPosY(int y) {
  posY = y;
}
void GameObject::setSymbol(char symbol) {
  this->symbol = symbol;
}

class Player : public GameObject {
private:
  int score;
  int lives;

public:
  int getScore();
  int getLives();
  void setScore(int);
  void setLives(int);
};
int Player::getScore() {
  return score;
}
int Player::getLives() {
  return lives;
}
void Player::setScore(int score) {
  this->score = score;
}
void Player::setLives(int lives) {
  this->lives = lives;
}

class Enemy : public GameObject {
};

class Bullet : public GameObject {
public:
  void traverse(char);
};

void Bullet::traverse(char id) {
  if (id == 'P') {
    while (true) {
      if (this->getPosY() == 0) {
        takeAndSetCursor(this->getPosX(), this->getPosY());
        cout << " ";
        bringCursorToSide();
        break;
      }
      takeAndSetCursor(this->getPosX(), this->getPosY());
      cout << " ";
      this->setPosY(this->getPosY() - 1);
      takeAndSetCursor(this->getPosX(), this->getPosY());
      cout << this->getSymbol();
      bringCursorToSide();
      for (int i = 0; i < enemyPositions.size(); i++) {
        if (enemyPositions[i][0] == this->getPosX() && enemyPositions[i][1] == this->getPosY()) {
          for (int j = i; j < enemyPositions.size() - 1; j++) {
            enemyPositions[j] = enemyPositions[j + 1];
          }
          enemyPositions.pop_back();
          ++score;
          updateScore();
        }
      }
      setTimeInterval(25);
    }
  } else if (id == 'E') {
    while (true) {
      if (this->getPosY() == csbInfo.srWindow.Bottom) {
        takeAndSetCursor(this->getPosX(), this->getPosY());
        cout << " ";
        bringCursorToSide();
        break;
      }
      takeAndSetCursor(this->getPosX(), this->getPosY());
      cout << " ";
      bringCursorToSide();
      this->setPosY(this->getPosY() + 1);
      takeAndSetCursor(this->getPosX(), this->getPosY());
      cout << this->getSymbol();
      bringCursorToSide();
      setTimeInterval(25);
    }
  }
}

// Extra Functions
void setTimeInterval(int ms) {
  chrono::milliseconds interval(ms);
  auto start = chrono::high_resolution_clock::now();
  while (true) {
    auto current = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(current - start);
    if (elapsed >= interval) {
      break;
    }
  }
}
void cursorAtCenter(int size) {
  GetConsoleScreenBufferInfo(consoleHandle, &csbInfo);
  cursorPosition.X = (csbInfo.srWindow.Right / 2) - (size / 2);
  cursorPosition.Y = (csbInfo.srWindow.Bottom) / 2;
  SetConsoleCursorPosition(consoleHandle, cursorPosition);
}
vector<int> returnRandomCursorPos(char ch) {
  GetConsoleScreenBufferInfo(consoleHandle, &csbInfo);
  vector<int> vec;
  if (ch == 'P') {
    vec.push_back((csbInfo.srWindow.Right - SIDEBAR) / 2);
    vec.push_back(csbInfo.srWindow.Bottom - 2);
  } else if (ch == 'E') {
    vec.push_back(rand() % csbInfo.srWindow.Right - SIDEBAR);
    vec.push_back(2);
  }
  cursorPosition.X = vec[0];
  cursorPosition.Y = vec[1];
  return vec;
}

void bringCursorToSide() {
  GetConsoleScreenBufferInfo(consoleHandle, &csbInfo);
  cursorPosition.X = csbInfo.srWindow.Right;
  cursorPosition.Y = csbInfo.srWindow.Bottom - 1;
  SetConsoleCursorPosition(consoleHandle, cursorPosition);
}

void divideScreen() {
  GetConsoleScreenBufferInfo(consoleHandle, &csbInfo);
  int pos = 0;
  while (pos <= csbInfo.srWindow.Bottom) {
    cursorPosition.X = csbInfo.srWindow.Right - SIDEBAR;
    cursorPosition.Y = pos;
    SetConsoleCursorPosition(consoleHandle, cursorPosition);
    cout << "|";
    pos++;
  }
}

void takeAndSetCursor(int x, int y) {
  GetConsoleScreenBufferInfo(consoleHandle, &csbInfo);
  cursorPosition.X = x;
  cursorPosition.Y = y;
  SetConsoleCursorPosition(consoleHandle, cursorPosition);
}
void updateScore() {
  takeAndSetCursor(csbInfo.srWindow.Right - SIDEBAR + 10, 3);
  cout << score;
  setTimeInterval(50);
  bringCursorToSide();
  addEnemy();
}
void addEnemy() {
  vector<int> currentPos = returnRandomCursorPos('E');
  while(isEnemyAtThatPos(currentPos[0])) {
    currentPos = returnRandomCursorPos('E');
  }
  takeAndSetCursor(currentPos[0], currentPos[1]);
  Enemy e;
  e.setSymbol('Y');
  enemyPositions.push_back(currentPos);
  e.setPosX(currentPos[0]);
  e.setPosY(currentPos[1]);
  cout << e.getSymbol();
}
bool isEnemyAtThatPos(int posX) {
  for(vector<int> pos: enemyPositions) {
    if(pos[0] == posX) {
      return true;
    }
  }
  return false;
}