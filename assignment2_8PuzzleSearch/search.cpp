#include <iostream>
#include <stdio.h>
#include <deque>
#include <iterator>
#include <algorithm>
#include <time.h>

#include "puzzle.h"

using namespace std;

deque<Node> open_list;
deque<Node> closed_list;
deque<BestNode> open_lb;
deque<BestNode> closed_lb;

unsigned int open_num = 0;    // open_list의 누적 node 수
unsigned int closed_num = 0;  // closed_list의 누적 node 수

void breadth_first_search(Node* root);
void depth_first_search(Node* root, int bound);
void best_first_search(BestNode* root);
void printPath(Node* term);
void printPathB(BestNode* term);
void printState(int puzzle[3][3]);
bool compare(const BestNode& node1, const BestNode& node2);
void randomize(int random[3][3]);
int getInvCount(int arr[]);
bool isSolvable(int puzzle[3][3]);

bool found;

void main() {
	int random[3][3];

	srand(time(NULL));
	randomize(random);

	printf("초기 상태\n\n");
	printState(random);
	if (isSolvable(random))      // 풀 수 있는 퍼즐인지 확인
	{
		printf("solvable\n\n");

		Puzzle* ex_puzzle = new Puzzle(random);
		Node* root = new Node(ex_puzzle, 0);

		breadth_first_search(root);
		printf("== Breadth First Search ==\n");
		printf("Open List의 누적 node 수 : %d\n", open_num);
		printf("Closed List의 누적 node 수 : %d\n", closed_num);
		printf("최종 Solution에 대한 Path length : %d\n\n", closed_list.front().depth);

		open_list.clear();
		closed_list.clear();
		open_num = 0;
		closed_num = 0;

		found = false;
		depth_first_search(root, 25);
		printf("== depth First Search ==\n");
		printf("Open List의 누적 node 수 : %d\n", open_num);
		printf("Closed List의 누적 node 수 : %d\n", closed_num);
		printf("최종 Solution에 대한 Path length : %d\n\n", closed_list.front().depth);
		if (found)
			printf("sol found\n\n");
		else
			printf("not found\n\n");

		open_list.clear();
		closed_list.clear();
		open_num = 0;
		closed_num = 0;

		BestNode* root_best = new BestNode(ex_puzzle, 0);
		best_first_search(root_best);
		printf("== Best First Search ==\n");
		printf("Open List의 누적 node 수 : %d\n", open_num);
		printf("Closed List의 누적 node 수 : %d\n", closed_num);
		printf("최종 Solution에 대한 Path length: %d\n\n", closed_lb.back().depth);
	}
	else
		printf("not solvable\n\n");
}

void breadth_first_search(Node* root) {
	Node* temp = root;
	open_list.push_back(*root);
	open_num++;

	while ((temp->puzzle.isSolution() == false) && (!open_list.empty())) {
		closed_num++;
		closed_list.push_front(*temp);     // temp가 solution인지 여부를 확인하고나서 closed_list에 삽입
		open_list.pop_front();

		char dir[4];
		int possible = temp->getPossibleMove(dir); // 생성 가능한 자식 node의 개수와 움직이는 방향을 얻어온다
		closed_list.front().next = new Node*[possible];
		for (int i = 0; i < possible; i++) {
			temp->puzzle.do_move(dir[i]);
			Node* child = new Node(&temp->puzzle ,temp->depth + 1); // 현재 node의 자식 node를 생성하여 open_list에 삽입
			temp->puzzle.undo_move();
			child->parent = &closed_list.front();
			closed_list.front().next[i] = child;

			open_list.push_back(*child);
			open_num++;
			
		}
		temp = &open_list.front();                // temp에 open_list 가장 앞에 있는 원소를 대입
	}
	if (temp->puzzle.isSolution() == true) {
		closed_list.push_front(*temp);
		closed_num++;
	}
}

void depth_first_search(Node* root, int bound) {
	Node* temp = root;
	open_list.push_front(*root);
	open_num++;

	while (((found = temp->puzzle.isSolution()) == false) && (!open_list.empty())) {
		closed_num++;
		closed_list.push_front(*temp);     // temp가 solution인지 여부를 확인하고나서 closed_list에 삽입
		open_list.pop_front();

		char dir[4];
		int possible = temp->getPossibleMove(dir); // 생성 가능한 자식 node의 개수와 움직이는 방향을 얻어온다
		closed_list.front().next = new Node*[possible];
		if (closed_list.front().depth < bound) {
			for (int i = possible - 1; i >= 0; i--) {
				closed_list.front().puzzle.do_move(dir[i]);
				Node* child = new Node(&closed_list.front().puzzle, closed_list.front().depth+1); // 현재 node의 자식 node를 생성하여 open_list에 삽입
				closed_list.front().puzzle.undo_move();
				child->parent = &closed_list.front();
				closed_list.front().next[possible - i - 1] = child;

				open_list.push_front(*child);
				open_num++;

			}
		}
		if(!open_list.empty())
			temp = &open_list.front();    // temp에 open_list 가장 앞에 있는 원소를 대입
	}
	if (temp->puzzle.isSolution() == true) {
		closed_list.push_front(*temp);
		closed_num++;
	}
}

void best_first_search(BestNode* root) {
	BestNode* temp = root;
	open_lb.push_back(*root);
	open_num++;

	while ((temp->puzzle.isSolution() == false) && (!open_lb.empty())) {
		closed_num++;
		closed_lb.push_back(*temp);     // temp가 solution인지 여부를 확인하고나서 closed_list에 삽입
		open_lb.pop_front();

		char dir[4];
		int possible = temp->getPossibleMove(dir); // 생성 가능한 자식 node의 개수와 움직이는 방향을 얻어온다
		closed_lb.front().next = new BestNode*[possible];
		for (int i = 0; i < possible; i++) {
			temp->puzzle.do_move(dir[i]);
			BestNode* child = new BestNode(&temp->puzzle, temp->depth + 1); // 현재 node의 자식 node를 생성하여 open list에 삽입
			temp->puzzle.undo_move();
			child->parent = &closed_lb.back();
			closed_lb.front().next[i] = child;

			open_lb.push_back(*child);
			open_num++;
		}

		sort(open_lb.begin(), open_lb.end(), compare);  // open list 를 오름차순으로 정렬
		
		temp = &open_lb.front();                // temp에 open list 가장 앞에 있는 원소를 대입
	}
	if (temp->puzzle.isSolution() == true) {
		closed_lb.push_back(*temp);
		closed_num++;
	}
}

void printPath(Node* term) {
	int tmp = term->depth;
	term->mark = true;
	
	for (int i = 0; i < tmp; i++) {
		term = term->parent;
		term->mark = true;
	}

	deque<Node>::iterator it;
	it = closed_list.end();
	for (it = closed_list.end()-1; it >= closed_list.begin(); it--) {
		if (it->mark == true) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					printf("%d", it->puzzle.info[i][j]);
				}
				printf("\n");
			}
			printf("\n");
		}
		if (it == closed_list.begin())
			break;
	}
}

void printPathB(BestNode* term) {
	int tmp = term->depth;
	term->mark = true;

	for (int i = 0; i < tmp; i++) {
		term = term->parent;
		term->mark = true;
	}

	deque<BestNode>::iterator it;
	for (it = closed_lb.begin(); it < closed_lb.end(); it++) {
		if (it->mark == true) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					printf("%d", it->puzzle.info[i][j]);
				}
				printf("\n");
			}
			printf("\n");
		}
	}
}

void printState(int puzzle[3][3])
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%d", puzzle[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

bool compare(const BestNode& node1, const BestNode& node2) {
	return node1.f < node2.f;
}

void randomize(int random[3][3]) {
	int num[9];
	srand(time(NULL));

	for (int i = 0; i < 9; i++) {
		num[i] = rand() % 9;
		for (int j = 0; j <= i; j++) {
			for (int k = 0; k <= j; k++) {
				if (j != k) {
					if (num[j] == num[k]) {
						i -= 1;
						j -= 1;
						k -= 1;
						num[i] = rand() % 9;
					}
				}
			}
		}
	}
	int k = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			random[i][j] = num[k++];
		}
	}
}

int getInvCount(int arr[])
{
	int inv_count = 0;
	for (int i = 0; i < 9 - 1; i++)
		for (int j = i + 1; j < 9; j++)
			// Value 0 is used for empty space
			if (arr[j] && arr[i] && arr[i] > arr[j])
				inv_count++;
	return inv_count;
}

// This function returns true if given 8 puzzle is solvable.
bool isSolvable(int puzzle[3][3])
{
	// Count inversions in given 8 puzzle
	int invCount = getInvCount((int *)puzzle);

	// return true if inversion count is even.
	return (invCount % 2 == 1);
}