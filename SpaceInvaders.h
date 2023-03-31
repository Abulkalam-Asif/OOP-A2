#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <thread>
#include <time.h>
#include <vector>
#include <windows.h>
#define SIDEBAR 30
#define BOUNDARY 10

using namespace std;

class GameObject;
class Player;
class Enemy;
class Bullet;

extern int score;
extern int lives;
extern vector<Enemy> enemyVec;
extern int scoreToWin;


void setTimeInterval(int);
void cursorAtCenter(int);
void bringCursorToSide();
vector<int> returnRandomCursorPos(char);
void takeAndSetCursor(int, int);
void updateScore();
void updateLives();
void addEnemy();
bool isEnemyAtThatPos(int, int);
void playerActions(Player);
void moveEnemy();
void showInfo();

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
	void traverse();
};

void Bullet::traverse() {
	bool destroyed = false;
	while (!destroyed) {
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
		for (int i = 0; i < enemyVec.size(); i++) {
			if (enemyVec[i].getPosX() == this->getPosX() && enemyVec[i].getPosY() == this->getPosY()) {
				for (int j = i; j < enemyVec.size() - 1; j++) {
					enemyVec[j] = enemyVec[j + 1];
				}
				takeAndSetCursor(this->getPosX(), this->getPosY());
				cout << " ";
				destroyed = true;
				enemyVec.pop_back();
				++score;
				addEnemy();
				updateScore();
			}
		}
		setTimeInterval(25);
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
		vec.push_back(rand() % (csbInfo.srWindow.Right - SIDEBAR));
		vec.push_back(1);
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
	pos = csbInfo.srWindow.Right - SIDEBAR + 1;
	while (pos < csbInfo.srWindow.Right) {
		cursorPosition.X = pos;
		cursorPosition.Y = csbInfo.srWindow.Bottom - BOUNDARY;
		SetConsoleCursorPosition(consoleHandle, cursorPosition);
		cout << "_";
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
}
void updateLives() {
	takeAndSetCursor(csbInfo.srWindow.Right - SIDEBAR + 10, 7);
	cout << lives;
	setTimeInterval(50);
	bringCursorToSide();
}
void addEnemy() {
	vector<int> currentPos = returnRandomCursorPos('E');
	while (isEnemyAtThatPos(currentPos[0], currentPos[1])) {
		currentPos = returnRandomCursorPos('E');
	}
	takeAndSetCursor(currentPos[0], currentPos[1]);
	Enemy e;
	e.setSymbol('Y');
	e.setPosX(currentPos[0]);
	e.setPosY(currentPos[1]);
	enemyVec.push_back(e);
	cout << e.getSymbol();
}
bool isEnemyAtThatPos(int posX, int posY) {
	for (auto enemy : enemyVec) {
		if (enemy.getPosX() == posX && enemy.getPosY() == posY) {
			return true;
		}
	}
	return false;
}

void playerActions(Player p) {
	while (score != scoreToWin && lives > 0) {
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
			pb.traverse();
		}
	}
}

void moveEnemy() {
	while (score != scoreToWin && lives > 0) {
		setTimeInterval(1000);
		for (Enemy& enemy : enemyVec) {
			takeAndSetCursor(enemy.getPosX(), enemy.getPosY());
			cout << " ";
			enemy.setPosY(enemy.getPosY() + 1);
			takeAndSetCursor(enemy.getPosX(), enemy.getPosY());
			cout << enemy.getSymbol();
		}
		bringCursorToSide();
		for (int i = 0; i < enemyVec.size(); i++) {
			if (enemyVec[i].getPosY() == csbInfo.srWindow.Bottom - BOUNDARY) {
				takeAndSetCursor(enemyVec[i].getPosX(), enemyVec[i].getPosY());
				cout << " ";
				bringCursorToSide();
				if (enemyVec.size() > 1 && i != enemyVec.size() - 1) {
					for (int j = i; j < enemyVec.size() - 1; j++) {
						enemyVec[j] = enemyVec[j + 1];
					}
				}
				enemyVec.pop_back();
				--lives;
				--i;
				addEnemy();
				updateLives();
			}
		}
	}
	system("cls");
	cursorAtCenter(7);
	if (score >= scoreToWin)
		cout << "You won!";
	else if (lives < 1)
		cout << "You lost!";

	bringCursorToSide();
}

void showInfo() {
	takeAndSetCursor(csbInfo.srWindow.Right - SIDEBAR + 3, csbInfo.srWindow.Bottom - 12);
	cout << "If enemy touches this line";
	takeAndSetCursor(csbInfo.srWindow.Right - SIDEBAR + 3, csbInfo.srWindow.Bottom - 11);
	cout << "you lose a life";
	takeAndSetCursor(csbInfo.srWindow.Right - SIDEBAR + 3, 3);
	cout << "Score: 0";
	takeAndSetCursor(csbInfo.srWindow.Right - SIDEBAR + 3, 5);
	cout << "Score " << scoreToWin << " to win.";
	takeAndSetCursor(csbInfo.srWindow.Right - SIDEBAR + 3, 7);
	cout << "Lives: " << lives;
	divideScreen();
}