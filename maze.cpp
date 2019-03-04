#include <iostream>
#include <stack>
#include <ctime>
#include <fstream>

using namespace std;

struct Cell //initialize cell
{
	bool visited = false;
	bool wall[4] = { true, true, true, true }; //north, east, south, west
	bool pathvisited = false;
	char wallDisplay[4] = { '*', '*', '*', '*' };
	char cellDisplay = ' ';
};

//funcion prototype
int input(); //get input of the size of maze
void tracking(Cell cell[], int size); //check each cell and break connecting cell wall
void buildCell(Cell cell[], int length); //draw the cell
void displayMaze(Cell cell[], int size); //draw the maze

int index(int i, int j, int size); //convert the position from 2D array to 1D array
void findNeighbour(Cell cell[], int i, int j, int size, stack <int> usingMazeCell, stack <int> currentX, stack <int> currentY); //find a random cell near the current cell

void findPath(Cell cell[], int size); //find the path pass through the maze
void DFS(Cell cell[], int i, int j, int size, stack <int> usingMazeCell, stack <int> currentX, stack <int> currentY); //use DFS to search the route

void moving(Cell cellNo[], int mazeSize, char player); //conrol the character

//convert the position from 2D array to 1D array
int index(int i, int j, int size) {
	int index = size * i + j;
	return index;
}

//get input of the size of maze
int input() {
	int size;

	cout << "Enter the size (size must be > 1): ";
	do cin >> size;
	while (size <= 1);
	if (size % 2 == 0)
		size++;

	cout << "Maze size is: " << size << endl << endl;
	return size;
}

//check each cell and break connecting cell wall
void tracking(Cell cell[], int size) {
	int i = 0;
	int j = 0;
	cell[0].visited = true;
	stack <int> usingMazeCell;
	stack <int> currentX;
	stack <int> currentY;
	usingMazeCell.push(0);
	currentX.push(j);
	currentY.push(i);
	findNeighbour(cell, i, j, size, usingMazeCell, currentX, currentY);
}

//find a random cell near the current cell
void findNeighbour(Cell cell[], int i, int j, int size, stack <int> usingMazeCell, stack <int> currentX, stack <int> currentY) {
	int neighbour[4];
	int north, east, south, west;
	int newX, newY;
	int neighbourNo = -1;

	int coorX[4];
	int coorY[4];
	for (int a = 0; a < 4; a++) { //initialize the values
		coorX[a] = j;
		coorY[a] = i;
		neighbour[a] = -1;
	}

	if (i < 1) //cell on the north edge
		north = -1;
	else {
		coorY[0]--;
		north = index(coorY[0], coorX[0], size);
		if (cell[north].visited == false)
			neighbour[0] = north;
		else neighbour[0] = -1;
	}
	if (j > size - 2 && j < size) //cell on the east edge
		east = -1;
	else {
		coorX[1]++;
		east = index(coorY[1], coorX[1], size);
		if (cell[east].visited == false)
			neighbour[1] = east;
		else neighbour[1] = -1;
	}
	if (i > size - 2 && i < size) //cell on the south edge
		south = -1;
	else {
		coorY[2]++;
		south = index(coorY[2], coorX[2], size);
		if (cell[south].visited == false)
			neighbour[2] = south;
		else neighbour[2] = -1;
	}
	if (j < 1) //cell on the west edge
		west = -1;
	else {
		coorX[3]--;
		west = index(coorY[3], coorX[3], size);
		if (cell[west].visited == false)
			neighbour[3] = west;
		else neighbour[3] = -1;
	}

	if (neighbour[0] != -1 || neighbour[1] != -1 || neighbour[2] != -1 || neighbour[3] != -1) {
		do neighbourNo = rand() % 4;
		while (neighbour[neighbourNo] == -1); //random pick one neighbour
		usingMazeCell.push(neighbour[neighbourNo]); //push into stack
		currentX.push(coorX[neighbourNo]);
		currentY.push(coorY[neighbourNo]);
		newX = coorX[neighbourNo];
		newY = coorY[neighbourNo];

		if (neighbour[neighbourNo] == north) {
			cell[index(i, j, size)].wall[0] = false;
			cell[north].wall[2] = false;
			cell[north].visited = true;
		}
		else
			if (neighbour[neighbourNo] == east) {
				cell[index(i, j, size)].wall[1] = false;
				cell[east].wall[3] = false;
				cell[east].visited = true;
			}
			else
				if (neighbour[neighbourNo] == south) {
					cell[index(i, j, size)].wall[2] = false;
					cell[south].wall[0] = false;
					cell[south].visited = true;
				}
				else
					if (neighbour[neighbourNo] == west) {
						cell[index(i, j, size)].wall[3] = false;
						cell[west].wall[1] = false;
						cell[west].visited = true;
					}
		findNeighbour(cell, newY, newX, size, usingMazeCell, currentX, currentY);
	}
	else if (usingMazeCell.size() > 1)
	{
		usingMazeCell.pop();
		currentX.pop();
		currentY.pop();
		newX = currentX.top();
		newY = currentY.top();
		findNeighbour(cell, newY, newX, size, usingMazeCell, currentX, currentY);
	}

}

//draw the cell
void buildCell(Cell cell[], int size) {
	for (int i = 0; i < size * size; i++) {
		for (int j = 0; j < 4; j++)
			if (cell[i].wall[j] == false)
				cell[i].wallDisplay[j] = ' '; //no wall
			else cell[i].wallDisplay[j] = '*'; //have wall
	}
}

//draw the maze
void displayMaze(Cell cell[], int length) {
	int size = (length - 1) / 2; //number of cell in each row and col

	for (int i = 0; i < length; i++) //line 1
		cout << '*' << ' ';
	for (int i = 0; i < size - 1; i++) { //after line 1
		cout << endl;
		cout << '*' << ' ';
		for (int j = 0; j < size - 1; j++) { //horizontal
			cout << cell[index(i, j, size)].cellDisplay << ' ' << cell[index(i, j, size)].wallDisplay[1] << ' ';
		}
		cout << cell[index(i, size - 1, size)].cellDisplay << ' ' << '*' << endl; //last col of horizontal
		for (int j = 0; j < size; j++) //vertical
			cout << '*' << ' ' << cell[index(i, j, size)].wallDisplay[2] << ' ';
		cout << '*'; //last col of vertical
	}
	cout << endl;
	cout << '*' << ' '; //last row of horizontal
	for (int j = 0; j < size - 1; j++) {
		cout << cell[index(size - 1, j, size)].cellDisplay << ' ' << cell[index(size - 1, j, size)].wallDisplay[1] << ' ';
	}
	cout << cell[index(size - 1, size - 1, size)].cellDisplay << ' ' << '*' << endl;
	for (int j = 0; j < size; j++) //last row of vertical
		cout << '*' << ' ' << '*' << ' ';
	cout << '*';

	cout << endl;
}

//find the path pass through the maze
void findPath(Cell cell[], int size) {
	cout << "Display the route? Yes(Y)/No(N)" << endl;
	char ans;
	cin >> ans;
	if (ans == 'Y' || ans == 'y') {
		cell[0].pathvisited = true;
		cell[0].cellDisplay = '>';
		int i = 0;
		int j = 0;

		stack <int> usingMazeCell;
		stack <int> currentX;
		stack <int> currentY;
		usingMazeCell.push(0);
		currentX.push(j);
		currentY.push(i);

		DFS(cell, i, j, size, usingMazeCell, currentX, currentY);
	}
	else if (ans == 'N' || ans == 'n')
		return;
}

//use DFS to search the route
void DFS(Cell cell[], int i, int j, int size, stack <int> usingMazeCell,stack <int> currentX, stack <int> currentY) {
	int neighbour[4];
	int north, east, south, west;
	int newX, newY;
	int neighbourNo = -1;

	int coorX[4];
	int coorY[4];
	for (int a = 0; a < 4; a++) { //initialize the values
		coorX[a] = j;
		coorY[a] = i;
		neighbour[a] = -1;
	}

	if (i < 1) //cell on the north edge
		north = -1;
	else {
		coorY[0]--;
		north = index(coorY[0], coorX[0], size);
		if (cell[north].pathvisited == false && cell[index(i, j, size)].wall[0] == false && cell[north].wall[2] == false)
			neighbour[0] = north;
		else neighbour[0] = -1;
	}
	if (j > size - 2 && j < size) //cell on the east edge
		east = -1;
	else {
		coorX[1]++;
		east = index(coorY[1], coorX[1], size);
		if (cell[east].pathvisited == false && cell[index(i, j, size)].wall[1] == false && cell[east].wall[3] == false)
			neighbour[1] = east;
		else neighbour[1] = -1;
	}
	if (i > size - 2 && i < size) //cell on the south edge
		south = -1;
	else {
		coorY[2]++;
		south = index(coorY[2], coorX[2], size);
		if (cell[south].pathvisited == false && cell[index(i, j, size)].wall[2] == false && cell[south].wall[0] == false)
			neighbour[2] = south;
		else neighbour[2] = -1;
	}
	if (j < 1) //cell on the west edge
		west = -1;
	else {
		coorX[3]--;
		west = index(coorY[3], coorX[3], size);
		if (cell[west].pathvisited == false && cell[index(i, j, size)].wall[3] == false && cell[west].wall[1] == false)
			neighbour[3] = west;
		else neighbour[3] = -1;
	}

	if (usingMazeCell.top() == (size * size - 1)) { //display '>' on the route
		while (!usingMazeCell.empty()) {
			cell[usingMazeCell.top()].cellDisplay = '-';
			for (int i = 0; i < 4; i++)
				if (cell[usingMazeCell.top()].wall[i] == false)
					cell[usingMazeCell.top()].wallDisplay[i] = '-';
			usingMazeCell.pop();
		}
	
		system("CLS");
		cout << "Solution" << endl;
		displayMaze(cell, size * 2 + 1);
	} else if (neighbour[0] != -1 || neighbour[1] != -1 || neighbour[2] != -1 || neighbour[3] != -1) 
	{
		do neighbourNo = rand() % 4;
		while (neighbour[neighbourNo] == -1); //random pick one neighbour
		usingMazeCell.push(neighbour[neighbourNo]); //push into stack
		currentX.push(coorX[neighbourNo]);
		currentY.push(coorY[neighbourNo]);
		newX = coorX[neighbourNo];
		newY = coorY[neighbourNo];

		if (neighbour[neighbourNo] == north) {
			cell[north].pathvisited = true;
		}
		else
			if (neighbour[neighbourNo] == east) {
				cell[east].pathvisited = true;
			}
			else
				if (neighbour[neighbourNo] == south) {
					cell[south].pathvisited = true;
				}
				else
					if (neighbour[neighbourNo] == west) {
						cell[west].pathvisited = true;
					}
		DFS(cell, newY, newX, size, usingMazeCell, currentX, currentY);
	}
	else if (usingMazeCell.size() > 1)
	{
		usingMazeCell.pop();
		currentX.pop();
		currentY.pop();
		newX = currentX.top();
		newY = currentY.top();
		DFS(cell, newY, newX, size, usingMazeCell, currentX, currentY);
	}
}

//Control the character
void moving(Cell cell[], int size, char player) {
	int current = 0;
	cell[current].cellDisplay = player;
	int x = 0;
	int y = 0;
	char direction;
	do {
		cout << "moving by W A S D ";
		cin >> direction;
		if ((direction == 'W' || direction == 'w') && cell[current].wall[0] == false) { //Move up
			cell[current].cellDisplay = ' ';
			y--;
			current = index(y, x, size);
			cell[current].cellDisplay = player;
			system("CLS");
			displayMaze(cell, size * 2 + 1);
		}
		if ((direction == 'D' || direction == 'd') && cell[current].wall[1] == false) { //Move left
			cell[current].cellDisplay = ' ';
			x++;
			current = index(y, x, size);
			cell[current].cellDisplay = player;
			system("CLS");
			displayMaze(cell, size * 2 + 1);
		}
		if ((direction == 'S' || direction == 's') && cell[current].wall[2] == false) { //Move down
			cell[current].cellDisplay = ' ';
			y++;
			current = index(y, x, size);
			cell[current].cellDisplay = player;
			system("CLS");
			displayMaze(cell, size * 2 + 1);
		}
		if ((direction == 'A' || direction == 'a') && cell[current].wall[3] == false) { //Move left
			cell[current].cellDisplay = ' ';
			x--;
			current = index(y, x, size);
			cell[current].cellDisplay = player;
			system("CLS");
			displayMaze(cell, size * 2 + 1);
		}
	} while (current != size * size - 1);
	cout << "You find the exit!" << endl;
}

void main() {
	srand((unsigned)time(NULL));
	char player = 'O';
	int mazeSize = input(); //get input of the size of maze

	int cellLength = mazeSize * 2 + 1; //characters on one line
	Cell cellNo[1000];
	cellNo[0].cellDisplay = player;
	cellNo[mazeSize * mazeSize - 1].cellDisplay = 'E';

	tracking(cellNo, mazeSize); //check each cell and break connecting cell wall
	buildCell(cellNo, mazeSize); //draw the cell
	displayMaze(cellNo, cellLength); //draw the maze
	moving(cellNo, mazeSize, player); //conrol the character
	findPath(cellNo, mazeSize); //find the path pass through the maze

	system("pause");
}
