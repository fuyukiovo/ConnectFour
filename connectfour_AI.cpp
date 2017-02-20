#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace std;

void printboard(vector<vector<int> > gameBoard)
{
	cout << "| 1 | 2 | 3 | 4 | 5 | 6 | 7 |" << endl;
	for(int i = 5; i >= 0; i--)	{
		cout << "|";
		for(int j = 0; j < 7; j++) {
			switch(gameBoard[i][j]) {
				case -1: cout << "   |"; break;
				case 0: cout << " O |"; break;
				case 1: cout << " X |"; break;
				default: break;
			}
		}
		cout << endl;
	}
	
}

bool boardFull(vector<vector<int> > gameBoard)
{
	for(int i = 0; i < 6; i++)
		for(int j = 0; j < 7; j++)
			if (gameBoard[i][j] == -1) return 0;
	return 1;
}

int checkWin(vector<vector<int> > gameBoard)
{
	// check horizontal
	for(int i = 0; i < 6; i++) {
		for(int j = 0; j < 4; j++) {
			if(gameBoard[i][j] == -1) continue;
			int first = gameBoard[i][j];
			int k;
			for(k = 1; k < 4; k++) {
				if((gameBoard[i][j+k] == -1) || (gameBoard[i][j+k] != first)) break;
			}
			if (k == 4) return first;
		}
	}
	// check vertical
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 7; j++) {
			if(gameBoard[i][j] == -1) continue;
			int first = gameBoard[i][j];
			int k;
			for(k = 1; k < 4; k++) {
				if((gameBoard[i+k][j] == -1) || (gameBoard[i+k][j] != first)) break;
			}
			if (k == 4) return first;
		}
	}
	// check right up
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 4; j++) {
			if(gameBoard[i][j] == -1) continue;
			int first = gameBoard[i][j];
			int k;
			for(k = 1; k < 4; k++) {
				if((gameBoard[i+k][j+k] == -1) || (gameBoard[i+k][j+k] != first)) break;
			}
			if (k == 4) return first;
		}
	}
	// check right down
	for(int i = 3; i < 6; i++) {
		for(int j = 0; j < 4; j++) {
			if(gameBoard[i][j] == -1) continue;
			int first = gameBoard[i][j];
			int k;
			for(k = 1; k < 4; k++) {
				if((gameBoard[i-k][j+k] == -1) || (gameBoard[i-k][j+k] != first)) break;
			}
			if (k == 4) return first;
		}
	}
	return -1;
}

bool validPlay(vector<vector<int> > gameBoard, int play)
{
	if((play < 0) || (play > 6)) return 0;
	if(gameBoard[5][play] != -1) return 0;
	return 1;
}

vector<int> score(vector<vector<int> > gameBoard, int turn, int depth, int difficulty)
{
	int won = checkWin(gameBoard);
	if(depth >= 6) {
		vector<int> endNode;
		endNode.resize(2);
		endNode[0] = -5;	// -5 = end search
		endNode[1] = 0;
		return endNode;
	}
	if((won == -1) && (boardFull(gameBoard))) {
		vector<int> endNode;
		endNode.resize(2);
		endNode[0] = -2;	// -2 = end game
		endNode[1] = 0;
		return endNode;
	}
	if(won != -1) {
		vector<int> endNode;
		endNode.resize(2);
		endNode[0] = -2;	// -2 = end game
		if (won == 1) endNode[1] = 41 - depth;
		else if (won == 0) endNode[1] = depth - 41;
		return endNode;
	}
	else {
		vector<vector<int> > potentialPlay;
		potentialPlay.resize(7);
		for(int play = 0; play < 7; play++) {
			if(validPlay(gameBoard,play)) {
				// Make Play
				int playedi, playedj;
				for(int i = 0; i < 6; i++) {
					if(gameBoard[i][play] == -1) {
						gameBoard[i][play] = turn;
						playedi = i;
						playedj = play;
						break;
					}
				}
				potentialPlay[play] = score(gameBoard,1-turn,depth+1,difficulty);
				potentialPlay[play][0] = play;
				gameBoard[playedi][playedj] = -1;
			}
			else {
				potentialPlay[play].resize(2);
				potentialPlay[play][0] = play;
				potentialPlay[play][1] = -2000;
			}
		}
		vector<int> bestplay;
		bestplay.resize(0);
		if(turn == 1) { // Maximise on computer turn
			int maxscore = -999;
			for(int play = 0; play < 7; play++) {
				if (potentialPlay[play][1] == -2000) continue;
				// Allow random sampling among degenerate (same score) play
				if (potentialPlay[play][1] > maxscore) {
					maxscore = potentialPlay[play][1];
					bestplay.resize(0);
					bestplay.push_back(play);
				}
				else if (potentialPlay[play][1] == maxscore) {
					bestplay.push_back(play);
				}
			}
		}
		else if (turn == 0) { // Minimise on Player turn
			int minscore = 999;
			for(int play = 0; play < 7; play++) {
				if (potentialPlay[play][1] == -2000) continue;
				if (potentialPlay[play][1] < minscore) {
					minscore = potentialPlay[play][1];
					bestplay.resize(0);
					bestplay.push_back(play);
				}
			}
		}
		if(depth == 0) {
			for(int play = 0; play < 7; play++) {
				cout << potentialPlay[play][1] << "  ";
			}
			cout << endl;
		}
		int nohomo = bestplay.size();
		if (nohomo > 1) {
			int which = rand() % nohomo;
			return potentialPlay[bestplay[which]];
		}
		else if (nohomo == 1) {
			return potentialPlay[bestplay[0]];
		}
		vector<int> error;
		error.assign(2,-3000);
		return error;
	}
}

int main (int argc, char *argv[])
{
	srand(time(NULL));
	vector<vector<int> > gameBoard;
	gameBoard.resize(6);
	for(int i = 0; i < 6; i++)
		gameBoard[i].assign(7,-1);
	// -1 = empty, 0 = player 1, 1 = player 2;
	int turn = 0; // turn 0 = player 1, turn 1 = player 2
	int nturn = 0;
	int difficulty = 5;
	printboard(gameBoard);
	cout << "Select difficulty (1 to 8): ";
	cin >> difficulty;
	while(nturn < 42) {
		int play;
		if(turn == 0) {
			cout << "Enter play: ";
			if(nturn == 0) cout << "(enter 8 to play second) ";
			cin >> play;
			if((nturn == 0) && (play == 8)) {
				cout << "You play second." << endl;
			}
			else {
				play--;
				while(!validPlay(gameBoard,play)) {
					cout << "Invalid Play! Enter again: ";
					cin >> play;
					play--;
				}
			}
		}
		else if (turn == 1) {
			vector<int> aiplay = score(gameBoard,turn,0,difficulty);
			play = aiplay[0];
		}
		// Make Play
		for(int i = 0; i < 6; i++) {
			if(gameBoard[i][play] == -1) {
				gameBoard[i][play] = turn;
				break;
			}  
		}
		printboard(gameBoard);
		int won = checkWin(gameBoard);
		if(won != -1) {
			if (won == 0) {
				cout << "Player 1 win!\n";
				break;
			}
			else if (won == 1) {
				cout << "Player 2 win!\n";
				break;
			}
		}
		turn = 1 - turn;
		nturn++;
	}
	return 0;
}
