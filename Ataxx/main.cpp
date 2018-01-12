//  
//  main.cpp
//  Ataxx
//  
//  Created by NikiHuang.
//  Copyright (C) Penguin_G. All rights reserved.
//  
//  Any question, plz contact 1500017836@pku.edu.cn
//  Thanks for using our products.
//  

#include <iostream>
#include <io.h>
#include <string>
#include <cstring>
#include <cmath>
#include <algorithm>

using namespace std;

//¡ñ:1
//¡ð:0

//people: 1
//computer: 0
//Blank: -1
int map[7][7];

struct node {
	bool valid;
	int around[7][7];	// -1 Free, 0 Can't go, 1 black, 2 white
	int choice[5][5];
	int bestscore;
	int control;
} stat[7][7];

// Function allocations and globe vars
void Calculate();
void Change(int i, int j, int C);
bool Continue(int C);
int Over();
void Game();
void Game2p();
void Save();
bool Load();
void Help();
bool PrintSavings(string path);
void PrintMap();
bool modified;
bool color;

int main() {
	printf("Created by NikiHuang.\n");
	printf("Copyright (C) Penguin_G. All rights reserved.\n\n");
	printf("Welcome to Ataxx! Enjoy your game.\n\n");
	printf("  N: New Game\n");
	printf("  S: Quick save\n");
	printf("  L: Load Game\n");
	printf("  Q: Quit\n\n");
	
	printf("[Menu] What do you want to do?\n\n");
	char operation;
	while (cin >> operation) {
		if (operation == 'N') {
			memset(map, 0xFF, sizeof(map));
			Game();
		}
		else if (operation == 'S') { 
			printf("\nYou are not in a game.\n");
			printf("Do you want to start a new game? (Y/N)\n\n");
			char choice;
			cin >> choice;
			if (choice == 'Y') {
				memset(map, 0xFF, sizeof(map));
				Game();
			}
		}
		else if (operation == 'L') {
			if (Load() == 0) {
				printf("Do you want to start a new game? (Y/N)\n\n");
				char choice;
				cin >> choice;
				if (choice == 'Y') {
					memset(map, 0xFF, sizeof(map));
					Game();
				}
			} else {
				printf("Do you want to start now? (Y/N)\n\n");
				char choice;
				cin >> choice;
				if (choice == 'Y')
					Game();
			}
		}
		else if (operation == 'Q') {
			break;
		}
		else Help();
		printf("\n[Menu] What do you want to do?\n\n");
	}
	
	return 0;
}

void Calculate() {
	if (Over() != -1) return;
	if (Continue(0) == 0) return;
	memset(stat, 0, sizeof(stat));
	
	int flag = 0;
	for (int i = 0; i < 7; ++ i)
		for (int j = 0; j < 7; ++ j)
			if (map[i][j] == 0) {
				stat[i][j].valid = 1;
				for (int m = 0; m < 7; ++ m)
					for (int n = 0; n < 7; ++ n)
						if (abs(m - i) <= 3 && abs(n - j) <= 3) {
							stat[i][j].around[m - i + 3][n - j + 3] = map[m][n];
							if (map[m][n] == 0)
								stat[i][j].around[m - i + 3][n - j + 3] = 2;
							if (map[m][n] == 1)
								flag = 1;
						}
			}
		
	int cx, cy;
	int MAX = 0;
	for (int i = 0; i < 7; ++ i)
		for (int j = 0; j < 7; ++ j)
			if (stat[i][j].valid == 1) {
				for (int m = 1; m < 6; ++ m)
					for (int n = 1; n < 6; ++ n) {
						if ((m == 3 && n == 3) || stat[i][j].around[m][n] != -1)
							stat[i][j].choice[m - 1][n - 1] = -9999;
						else {
							int tmp = 0;
							if (stat[i][j].around[m + 1][n] == 1)
								tmp ++;
							if (stat[i][j].around[m][n + 1] == 1)
								tmp ++;
							if (stat[i][j].around[m - 1][n] == 1)
								tmp ++;
							if (stat[i][j].around[m][n - 1] == 1)
								tmp ++;
							if (stat[i][j].around[m + 1][n - 1] == 1)
								tmp ++;
							if (stat[i][j].around[m + 1][n + 1] == 1)
								tmp ++;
							if (stat[i][j].around[m - 1][n - 1] == 1)
								tmp ++;
							if (stat[i][j].around[m - 1][n + 1] == 1)
								tmp ++;
							stat[i][j].choice[m - 1][n - 1] += tmp * 20;
						}
					}
					
				for (int m = 2; m <= 4; ++ m)
					for (int n = 2; n <= 4; ++ n)
						if (stat[i][j].around[m][n] > 0)
							stat[i][j].control ++;
				
				for (int m = 0; m < 5; ++ m)
					for (int n = 0; n < 5; ++ n)
						if (m == 0 || m == 4 || n == 0 || n == 4)
							stat[i][j].choice[m][n] -= 15 * stat[i][j].control;
				
				for (int m = 0; m < 5; ++ m)
					for (int n = 0; n < 5; ++ n)
						stat[i][j].bestscore = max(stat[i][j].bestscore, stat[i][j].choice[m][n]);
				
				MAX = max(stat[i][j].bestscore, MAX);
			}
	
	if (flag == 0) {
		for (int i = 0; i < 7; ++ i)
			for (int j = 0; j < 7; ++ j)
				if (stat[i][j].valid == 1) {
					cx = i; cy = j;
					goto L0;
				}
L0:
		for (int i = 2; i <= 4; ++ i)
			for (int j = 2; j <= 4; ++ j)
				if(stat[cx][cy].around[i][j] == -1) {
					Change(cx + i - 3, cy + j - 3, 0);
					printf("\nCPU choose :\n");
					printf("%d %d\n\n",cx + i - 3, cy + j - 3);
					return;
				}
	}
	
	int oi, oj;
	bool attack = 0;
	for (int i = 0; i < 7; ++ i)
		for (int j = 0; j < 7; ++ j)
			if (stat[i][j].valid && stat[i][j].bestscore == MAX)
				for (int m = 0; m < 5; ++ m)
					for (int n = 0; n < 5; ++ n)
						if (stat[i][j].choice[m][n] == MAX) {
							cx = i + m - 2; cy = j + n - 2;
							oi = i; oj = j;
							if (m == 0 || m == 4 || n == 0 || n == 4)
								attack = 1;
							goto L1;
						}
L1:	
	Change(cx, cy, 0);
	printf("\nCPU choose:\n");
	printf("%d %d\n\n", cx, cy);
	
	if (attack == 1)
		map[oi][oj] = -1;
	return;
}

void Change(int i, int j, int C) {
	map[i][j] = C;
	if (i > 0 && map[i - 1][j] == !C) map[i - 1][j] = C;
	if (j > 0 && map[i][j - 1] == !C) map[i][j - 1] = C;
	if (i < 6 && map[i + 1][j] == !C) map[i + 1][j] = C;
	if (j < 6 && map[i][j + 1] == !C) map[i][j + 1] = C;
	if (i > 0 && j > 0 && map[i - 1][j - 1] == !C) map[i - 1][j - 1] = C;
	if (i > 0 && j < 6 && map[i - 1][j + 1] == !C) map[i - 1][j + 1] = C;
	if (i < 6 && j > 0 && map[i + 1][j - 1] == !C) map[i + 1][j - 1] = C;
	if (i < 6 && j < 6 && map[i + 1][j + 1] == !C) map[i + 1][j + 1] = C;
	return;
}

bool Continue(int C) {
	for (int i = 0; i < 7; ++ i)
		for (int j = 0; j < 7; ++ j)
			if (map[i][j] == C)
				for (int m = 0; m < 7; ++ m)
					for (int n = 0; n < 7; ++ n)
						if (m != i || n != j)
							if (map[m][n] == -1 && abs(m - i) <= 2 && abs(n - j) <= 2)
								return 1;
	return 0;
}

int Over() {
	bool find_b = 0;
	bool find_w = 0;
	for (int i = 0; i < 7; ++ i)
		for (int j = 0; j < 7; ++ j) {
			if (map[i][j] == 1) find_b = 1;
			if (map[i][j] == 0) find_w = 1;
			if (find_b && find_w) return -1;
		}
	if (find_b == 0) return 0;
	if (find_w == 0) return 1;
}

void Game() {
	printf("\nPlay with CPU or people? (C/P)\n\n");
	char choice;
	cin >> choice;
	if (choice == 'P') {
		Game2p();
		return;
	}
	
	modified = 0;
	printf("\nChoose your color. (B/W)\n\n");
	char c; cin >> c;
	color = ((c == 'B') ? 1 : 0);
	map[0][0] = map[6][6] = 1;
	map[0][6] = map[6][0] = 0;
	PrintMap();
	printf("Choose yours, or enter other operations :\n");
	printf("(row and col separated by a single blank)\n\n");
	
	char operation;
	int people_continue = 1, CPU_continue = 1;
	
	while (cin >> operation) {
		if (people_continue == 0) {
			printf("\nYou can't go. Let CPU go.\n\n");
			Calculate();
			goto L1;
		}
		
		if (operation >= '0' && operation <= '6') {
			int x = operation - '0';
			int y; cin >> y;
			
			if (map[x][y] != 1)
				printf("\nIt's not yours. Please choose another position.\n\n");
			
			else {
				printf("\nWhere do you want to move? \n");
				printf("(row and col separated by a single blank)\n\n");
				int new_x, new_y;
				cin >> new_x >> new_y;
				
				if (new_x < 0 || new_x > 6 || new_y < 0 || new_y > 6)
					printf("\nWrong position. Please try again.\n\n");
				else if (map[new_x][new_y] != -1)
					printf("\nWrong position. Please try again.\n\n");
				else if (abs(new_x - x) <= 1 && abs(new_y - y) <= 1) {
					modified = 1;
					Change(new_x, new_y, 1);
					if (CPU_continue == 0) {
						printf("\nCPU can't go. You go.\n\n");
					}
					else Calculate();
				}
				
				else if (abs(new_x - x) <= 2 && abs(new_y - y) <= 2) {
					modified = 1;
					Change(new_x, new_y, 1);
					map[x][y] = -1;
					if (CPU_continue == 0) {
						printf("\nCPU can't go. You go.\n\n");
					}
					else Calculate();
				}
				
				else printf("\nWrong position. Please try again.\n\n");
			}
		}
		
		else if (operation == 'M') {
			if (modified) {
				printf("\nDo you want to save? (Y/N)\n\n");
				cin >> choice;
				if (choice == 'Y')
					Save();
			}
			return;
		}
		
		else if (operation == 'S')
			Save();
		
		else if (operation == 'L') {
			if (modified) {
				printf("\nDo you want to save? (Y/N)\n\n");
				cin >> choice;
				if (choice == 'Y')
					Save();
			}
			
			if (Load() == 0) {
				printf("\nDo you want to start a new game? (Y/N)\n\n");
				cin >> choice;
				if (choice == 'Y') {
					memset(map, 0xFF, sizeof(map));
					modified = 0;
					printf("\nChoose your color. (B/W)\n\n");
					cin >> c;
					color = ((c == 'B') ? 1 : -1);
					map[0][0] = map[6][6] = 1;
					map[0][6] = map[6][0] = 0;
				}
			}
			
			else {
				printf("\nDo you want to start now? (Y/N)\n\n");
				cin >> choice;
				if (choice == 'N')
					return;
			}
		}
		
		else if (operation == 'N') {
			if (modified) {
				printf("\nDo you want to save? (Y/N)\n\n");
				cin >> choice;
				if (choice == 'Y')
					Save();
			}
			memset(map, 0xFF, sizeof(map));
			modified = 0;
			printf("\nChoose your color. (B/W)\n\n");
			cin >> c;
			color = ((c == 'B') ? 1 : -1);
			map[0][0] = map[6][6] = 1;
			map[0][6] = map[6][0] = 0;
		}
		
L1:		PrintMap();
		people_continue = Continue(1);
		CPU_continue = Continue(0);
		if ((people_continue || CPU_continue) == 0) {
			printf("Game over.\n");
			int win = 0;
			for (int i = 0; i < 7; ++ i)
				for (int j = 0; j < 7; ++ j)
					if(map[i][j] == 1)
						win ++;
			if (win > 24) printf("You win.\n\n");
			else printf("CPU win.\n\n");
			break;
		}
		
		int stop;
		if ((stop = Over()) != -1) {
			printf("Game over.\n");
			if (stop == 1) printf("You win.\n\n");
			else printf("CPU win.\n\n");
			break;
		}
		
		else {
			printf("Choose yours, or enter other operations :\n");
			printf("(row and col separated by a single blank)\n\n");
		}
	}
	return;
}

void Game2p() {
	color = 1;
	printf("\nFight with your friends.\n");
	printf("Black goes first.\n\n");
	modified = 0;
	map[0][0] = map[6][6] = 1;
	map[0][6] = map[6][0] = 0;
	PrintMap();
	printf("Enter your operation :\n");
	printf("(row and col separated by a single blank)\n\n");
	
	char operation, choice;
	int p0_continue = 1, p1_continue = 1;
	bool cur = 1;
	
	while (cin >> operation) {
		if (p0_continue == 0) {
			printf("\nWhite can't go. Black can go.\n\n");
			cur = 1;
		}
		
		if (p1_continue == 0) {
			printf("\nBlack can't go. White can go.\n\n");
			cur = 0;
		}
		
		if (operation >= '0' && operation <= '6') {
			int x = operation - '0';
			int y; cin >> y;
			
			if (map[x][y] != cur)
				printf("\nIt's not yours. Please choose another position.\n\n");
			
			else {
				printf("\nWhere do you want to move? \n");
				printf("(row and col separated by a single blank)\n\n");
				int new_x, new_y;
				cin >> new_x >> new_y;
				
				if (new_x < 0 || new_x > 6 || new_y < 0 || new_y > 6)
					printf("\nWrong position. Please try again.\n\n");
				else if (map[new_x][new_y] != -1)
					printf("\nWrong position. Please try again.\n\n");
				else if (abs(new_x - x) <= 1 && abs(new_y - y) <= 1) {
					modified = 1;
					Change(new_x, new_y, cur);
					cur = !cur;
				}
				
				else if (abs(new_x - x) <= 2 && abs(new_y - y) <= 2) {
					modified = 1;
					Change(new_x, new_y, cur);
					map[x][y] = -1;
					cur = !cur;
				}
				
				else printf("\nWrong position. Please try again.\n\n");
			}
		}
		
		else if (operation == 'M') {
			if (modified) {
				printf("\nDo you want to save? (Y/N)\n\n");
				cin >> choice;
				if (choice == 'Y')
					Save();
			}
			return;
		}
		
		else if (operation == 'S')
			Save();
		
		else if (operation == 'L') {
			if (modified) {
				printf("\nDo you want to save? (Y/N)\n\n");
				cin >> choice;
				if (choice == 'Y')
					Save();
			}
			
			if (Load() == 0) {
				printf("\nDo you want to start a new game? (Y/N)\n\n");
				cin >> choice;
				if (choice == 'Y') {
					memset(map, 0xFF, sizeof(map));
					modified = 0;
					map[0][0] = map[6][6] = 1;
					map[0][6] = map[6][0] = 0;
				}
			}
			
			else {
				printf("\nDo you want to start now? (Y/N)\n\n");
				cin >> choice;
				if (choice == 'N')
					return;
			}
		}
		
		else if (operation == 'N') {
			if (modified) {
				printf("\nDo you want to save? (Y/N)\n\n");
				cin >> choice;
				if (choice == 'Y')
					Save();
			}
			memset(map, 0xFF, sizeof(map));
			modified = 0;
			map[0][0] = map[6][6] = 1;
			map[0][6] = map[6][0] = 0;
		}
		
		PrintMap();
		p1_continue = Continue(1);
		p0_continue = Continue(0);
		if ((p0_continue || p1_continue) == 0) {
			printf("Game over.\n");
			int win = 0;
			for (int i = 0; i < 7; ++ i)
				for (int j = 0; j < 7; ++ j)
					if(map[i][j] == 1)
						win ++;
			if (win > 24) printf("Black win.\n\n");
			else printf("White win.\n\n");
			break;
		}
		
		int stop;
		if ((stop = Over()) != -1) {
			printf("Game over.\n");
			if (stop == 1) printf("Black win.\n\n");
			else printf("White win.\n\n");
			break;
		}
		
		else {
			printf("Enter your operation :\n");
			printf("(row and col separated by a single blank)\n\n");
		}
	}
	return;
}

void Save() {
	if (PrintSavings("Savings") == 0) {
		printf("Enter your name of the saving.\n\n");
	} else {
		printf("Enter your name of the saving.\n");
		printf("You can overwrite the savings.\n\n");
	}
	
	char NAME[20];
	cin >> NAME;
	string path = "Savings\\";
	path += NAME;
	
	freopen(path.c_str(), "w", stdout);
	for (int i = 0; i < 7; ++ i) {
		for (int j = 0; j < 7; ++ j)
			cout << map[i][j] << " ";
		cout << endl;
	}
	fclose(stdout);
	freopen("CON", "w", stdout);
	printf("\nSaving success.\n\n");
	return;
}

bool Load() {
	if (PrintSavings("Savings") == 0) {
		return 0;
	} else {
		printf("Please enter your saving:\n\n");
		string Saving;
		cin >> Saving;
		string path = "Savings\\";
		path += Saving;
		
		freopen(path.c_str(), "r", stdin);
		for (int i = 0; i < 7; ++ i)
			for (int j = 0; j < 7; ++ j)
				cin >> map[i][j];
		fclose(stdin);
		freopen("CON", "r", stdin);
		printf("\nLoading success.\n");
		return 1;
	}
}

void Help() {
	printf("Wrong input!\n");
	printf("  N: New Game\n");
	printf("  S: Quick save\n");
	printf("  L: Load Game\n");
	printf("  Q: Quit\n\n");
	return ;
}

bool PrintSavings(string path) {
	long hFile = 0;
	struct _finddata_t fileInfo;
	string FILENAME[50];
	int k = 0;
	string pathName, exdName;
	
	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1)
		return 0;
	
	// Read file names
	do {
		FILENAME[k ++] = fileInfo.name;
	} while (_findnext(hFile, &fileInfo) == 0);
	
	// No files
	if (k < 3) {
		printf("\nNo savings now!\n");
		return 0;
	}
	
	// Print files
	printf("\nHere are the savings:\n");
	for (int i = 0; i < k; ++ i) {
		if (FILENAME[i] == "." || FILENAME[i] == "..") ;
		else cout << "    " << FILENAME[i] << endl;
	}
	cout << endl;
	
	_findclose(hFile);
	return 1;
}

void PrintMap() {
	printf("     0   1   2   3   4   5   6\n\n");
	for (int i = 0; i < 7; ++ i) {
		printf("%d    ", i);
		for (int j = 0; j < 7; ++ j) {
			if (map[i][j] == -1) {
				if (j == 6) printf("+");
				else printf("+-");
			}
			else if (map[i][j] ^ color) printf("¡ñ");
			else if (!(map[i][j] ^ color)) printf("¡ð");
			if (j < 6) printf("--");
		}
		if (i < 6) printf("\n     |   |   |   |   |   |   |\n");
		else printf("\n\n");
	}
	printf("N: New game  S: Save  L: Load  M: Menu\n\n");
	return;
}
