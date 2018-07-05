#pragma once

const int solution[3][3] = { { 1,2,3 },
                             { 8,0,4 },
                             { 7,6,5 } };

class Position {
public:
	int row;  // 행
	int col;  // 열
};

class Puzzle {
public:
	int info[3][3];       // 퍼즐에 존재하는 숫자 저장, 숫자 0은 빈 칸을 의미
	Position empty_idx;    // 퍼즐에서 빈 칸의 num배열에서의 index 저장
	bool didMove;
	char preDir;

public:
	Puzzle() {}
	Puzzle(int number[][3]) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				info[i][j] = number[i][j];
		getEmptyIdx();
		didMove = false;
		preDir = '\0';
	}
	bool isSolution() {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (solution[i][j] != info[i][j])
					return false;
		return true;
	}
	bool isSame(Puzzle* compare) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (compare->info[i][j] != info[i][j])
					return false;
		return true;
	}
	Position getEmptyIdx() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (info[i][j] == 0) {
					empty_idx.row = i;
					empty_idx.col = j;
				}
			}
		}
		return empty_idx;
	}
	void do_move(char dir) {
		switch (dir)
		{
		case 'l':
			move_left();
			break;
		case 'u':
			move_up();
			break;
		case 'd':
			move_down();
			break;
		case 'r':
			move_right();
			break;
		default:
			break;
		}
	}
	void move_left() {
		if (empty_idx.col - 1 < 0) {
			didMove = false;
			preDir = '\0';
		} else {
			info[empty_idx.row][empty_idx.col] = info[empty_idx.row][empty_idx.col - 1];
			empty_idx.col--;
			info[empty_idx.row][empty_idx.col] = 0;
			didMove = true;
			preDir = 'l';
		}
	}
	void move_right() {
		if (empty_idx.col + 1 > 2) {
			didMove = false;
			preDir = '\0';
		} else {
			info[empty_idx.row][empty_idx.col] = info[empty_idx.row][empty_idx.col + 1];
			empty_idx.col++;
			info[empty_idx.row][empty_idx.col] = 0;
			didMove = true;
			preDir = 'r';
		}
	}
	void move_up() {
		if (empty_idx.row - 1 < 0) {
			didMove = false;
			preDir = '\0';
		} else {
			info[empty_idx.row][empty_idx.col] = info[empty_idx.row-1][empty_idx.col];
			empty_idx.row--;
			info[empty_idx.row][empty_idx.col] = 0;
			didMove = true;
			preDir = 'u';
		}
	}
	void move_down() {
		if (empty_idx.row + 1 > 2) {
			didMove = false;
			preDir = '\0';
		} else {
			info[empty_idx.row][empty_idx.col] = info[empty_idx.row + 1][empty_idx.col];
			empty_idx.row++;
			info[empty_idx.row][empty_idx.col] = 0;
			didMove = true;
			preDir = 'd';
		}
	}
	void undo_move() {
		if (didMove == true) {
			switch (preDir)
			{
			case 'l':
				move_right();
				didMove = false;
				preDir = '\0';
				break;
			case 'r':
				move_left();
				didMove = false;
				preDir = '\0';
				break;
			case 'u':
				move_down();
				didMove = false;
				preDir = '\0';
				break;
			case 'd':
				move_up();
				didMove = false;
				preDir = '\0';
				break;
			default:
				break;
			}
		}
	}
	int getPossibleMove(char dir[4]) {
		int possible = 0;
		
		move_left();
		if (didMove == true) {
			dir[possible] = 'l';
			possible++;
		}
		undo_move();

		move_up();
		if (didMove == true) {
			dir[possible] = 'u';
			possible++;
		}
		undo_move();

		move_right();
		if (didMove == true) {
			dir[possible] = 'r';
			possible++;
		}
		undo_move();

		move_down();
		if (didMove == true) {
			dir[possible] = 'd';
			possible++;
		}
		undo_move();

		return possible;
	}
};

class Node {
public:
	Puzzle puzzle;
	int depth;
	Node* parent;
	Node** next;
	static unsigned int nodeNum;
	bool mark;
public:
	Node() {}
	Node(Puzzle* p, int depth) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				puzzle.info[i][j] = p->info[i][j];
		puzzle.didMove = p->didMove;
		puzzle.empty_idx = p->empty_idx;
		puzzle.preDir = p->preDir;
		parent = NULL;
		next = NULL;
		this->depth = depth;
		mark = false;
		nodeNum++;
	}
	int getPossibleMove(char dir[4]) {
		int possible = 0;
		if (parent == NULL) {
			puzzle.move_left();
			if (puzzle.didMove == true) {
				dir[possible] = 'l';
				possible++;
			}
			puzzle.undo_move();

			puzzle.move_up();
			if (puzzle.didMove == true) {
				dir[possible] = 'u';
				possible++;
			}
			puzzle.undo_move();

			puzzle.move_right();
			if (puzzle.didMove == true) {
				dir[possible] = 'r';
				possible++;
			}
			puzzle.undo_move();

			puzzle.move_down();
			if (puzzle.didMove == true) {
				dir[possible] = 'd';
				possible++;
			}
			puzzle.undo_move();

		}
		else {
			puzzle.move_left();
			if (!puzzle.isSame(&parent->puzzle)) {
				if (puzzle.didMove == true) {
					dir[possible] = 'l';
					possible++;
				}
			}
			puzzle.undo_move();

			puzzle.move_up();
			if (!puzzle.isSame(&parent->puzzle)) {
				if (puzzle.didMove == true) {
					dir[possible] = 'u';
					possible++;
				}
			}
			puzzle.undo_move();

			puzzle.move_right();
			if (!puzzle.isSame(&parent->puzzle)) {
				if (puzzle.didMove == true) {
					dir[possible] = 'r';
					possible++;
				}
			}
			puzzle.undo_move();

			puzzle.move_down();
			if (!puzzle.isSame(&parent->puzzle)) {
				if (puzzle.didMove == true) {
					dir[possible] = 'd';
					possible++;
				}
			}
			puzzle.undo_move();

		}

		return possible;
	}
};
unsigned int Node::nodeNum = 0;

class BestNode {
public:
	Puzzle puzzle;
	int depth;
	BestNode* parent;
	BestNode** next;
	static unsigned int nodeNum;
	bool mark;
	int f, g, h;                   // f(n), g(n), h(n) 을 저장하는 변수
public:
	BestNode() {}
	BestNode(Puzzle* p, int depth) {
		h = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				puzzle.info[i][j] = p->info[i][j];
				if (!(i == 1 && j == 1)) {
					if (puzzle.info[i][j] != solution[i][j])  // h(n) == solution에서 틀려있는 타일 개수
						h++;
				}
			}
		}
		puzzle.didMove = p->didMove;
		puzzle.empty_idx = p->empty_idx;
		puzzle.preDir = p->preDir;
		parent = NULL;
		next = NULL;
		this->depth = depth;
		g = depth;            // g(n) == depth
		f = g + h;            // f(n) = g(n) + h(n)
		mark = false;
		nodeNum++;
	}

	int getPossibleMove(char dir[4]) {
		int possible = 0;
		if (parent == NULL) {
			puzzle.move_left();
			if (puzzle.didMove == true) {
				dir[possible] = 'l';
				possible++;
			}
			puzzle.undo_move();

			puzzle.move_up();
			if (puzzle.didMove == true) {
				dir[possible] = 'u';
				possible++;
			}
			puzzle.undo_move();

			puzzle.move_right();
			if (puzzle.didMove == true) {
				dir[possible] = 'r';
				possible++;
			}
			puzzle.undo_move();

			puzzle.move_down();
			if (puzzle.didMove == true) {
				dir[possible] = 'd';
				possible++;
			}
			puzzle.undo_move();

		}
		else {
			puzzle.move_left();
			if (!puzzle.isSame(&parent->puzzle)) {
				if (puzzle.didMove == true) {
					dir[possible] = 'l';
					possible++;
				}
			}
			puzzle.undo_move();

			puzzle.move_up();
			if (!puzzle.isSame(&parent->puzzle)) {
				if (puzzle.didMove == true) {
					dir[possible] = 'u';
					possible++;
				}
			}
			puzzle.undo_move();

			puzzle.move_right();
			if (!puzzle.isSame(&parent->puzzle)) {
				if (puzzle.didMove == true) {
					dir[possible] = 'r';
					possible++;
				}
			}
			puzzle.undo_move();

			puzzle.move_down();
			if (!puzzle.isSame(&parent->puzzle)) {
				if (puzzle.didMove == true) {
					dir[possible] = 'd';
					possible++;
				}
			}
			puzzle.undo_move();

		}

		return possible;
	}
};
unsigned int BestNode::nodeNum = 0;