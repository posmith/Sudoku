/*
 * sudoku.cpp
 *
 *  Created on: May 7, 2020
 *      Author: Patrick
 */

#include <iostream>
#include <ctime>

using namespace std;

short dimension = 3; // Use to set size of Sudoku grid. Will include n^2 unique integer values
bool include_runtime = false; // Toggle to calculate and print runtime
bool include_messages = false; // Toggle to include debugging messages and backtracking calculations

short pow(short base, short exp) {
	if (exp == 0) {
		return 1;
	}
	if (exp == 1) {
		return base;
	}
	short x = base;
	for (short i = 0; i < exp - 1; i++) {
		x *= base;
	}
	return x;
}

short *idxGrid = (short*) calloc(pow(dimension, 4), sizeof(short));

void printChar(char ch, short times) {
	for (short i = 0; i < times; i++) {
		cout << ch;
	}
}

void printGenBorder() {
	printChar('#', 5 * pow(dimension, 2) + 1);
	cout << endl;
}

void print2Border() {
	cout << "#################" << endl;
}

void print3Border() {
	cout << "#####################################" << endl;
}

void printGenBlank() {
	cout << "#";
	for (short i = 0; i < dimension; i++) {
		for (short j = 0; j < dimension - 1; j++) {
			cout << "    |";
		}
		cout << "    #";
	}
	cout << endl;
}

void print2Blank() {
	cout << "#   |   #   |   #" << endl;
}

void print3Blank() {
	cout << "#   |   |   #   |   |   #   |   |   #" << endl;
}

void printGenBottom() {
	cout << "#";
	for (short i = 0; i < dimension; i++) {
		for (short j = 0; j < dimension - 1; j++) {
			cout << "____|";
		}
		cout << "____#";
	}
	cout << endl;
}

void print2Bottom() {
	cout << "#___|___#___|___#" << endl;
}

void print3Bottom() {
	cout << "#___|___|___#___|___|___#___|___|___#" << endl;
}

void printGenGrid(short *g) {
	for (short i = 0; i < dimension; i++) {
		printGenBorder();
		for (short j = 0; j < dimension; j++) {
			//printGenBlank();
			short count = 0;
			cout << "# ";
			for (short k = 0; k < dimension; k++) {
				for (short l = 0; l < dimension - 1; l++) {
					short val = g[(i * pow(dimension, 3)) + (j * pow(dimension, 2)) + count++];
					cout << val;
					if (val < 10) {
						cout << "  | ";
					} else {
						cout << " | ";
					}
				}
				short val = g[(i * pow(dimension, 3)) + (j * pow(dimension, 2)) + count++];
				cout << val;
				if (val < 10) {
					cout << "  # ";
				} else {
					cout <<" # ";
				}
			}
			cout << endl;
			printGenBottom();
		}
	}
	printGenBorder();
}

void print2Grid(short *g) {
	for (short i = 0; i < 2; i++) {
		print2Border();
		for (short j = 0; j < 2; j++) {
			// print2Blank();
			short count = 0;
			cout << "# " << g[(i * 8) + (j * 4) + count++] << " | ";
			cout << g[(i * 8) + (j * 4) + count++] << " # ";
			cout << g[(i * 8) + (j * 4) + count++] << " | ";
			cout << g[(i * 8) + (j * 4) + count++] << " # " << endl;
			print2Bottom();
		}
	}
	print2Border();
}

void print3Grid(short *g) {
	for (short i = 0; i < 3; i++) {
		print3Border();
		for (short j = 0; j < 3; j++) {
			// print3Blank();
			short count = 0;
			cout << "# " << g[(i * 27) + (j * 9) + count++] << " | ";
			cout << g[(i * 27) + (j * 9) + count++] << " | ";
			cout << g[(i * 27) + (j * 9) + count++] << " # ";
			cout << g[(i * 27) + (j * 9) + count++] << " | ";
			cout << g[(i * 27) + (j * 9) + count++] << " | ";
			cout << g[(i * 27) + (j * 9) + count++] << " # ";
			cout << g[(i * 27) + (j * 9) + count++] << " | ";
			cout << g[(i * 27) + (j * 9) + count++] << " | ";
			cout << g[(i * 27) + (j * 9) + count++] << " #" << endl;
			print3Bottom();
		}
	}
	print3Border();
}

bool fitsGenRow(short *g, short pos, short val) {
	short init = pos / pow(dimension, 2) * pow(dimension, 2);
	short until = init + pow(dimension, 2) - 1;
	for (short i = init; i <= until; i++) {
		if (i == pos) {
			continue;
		}
		if (g[i] == val) {
			return false;
		}
	}
	return true;
}
bool fits2Row(short *g, short pos, short val) {
	for (short i = (pos / 4 * 4); i <= (pos / 4 * 4 + 3); i++) {
		if (i == pos) {
			continue;
		}
		if (g[i] == val) {
			return false;
		}
	}
	return true;
}

bool fits3Row(short *g, short pos, short val) {
	for (short i = (pos / 9 * 9); i <= (pos / 9 * 9 + 8); i++) {
		if (i == pos) {
			continue;
		}
		if (g[i] == val) {
			return false;
		}
	}
	return true;
}

bool fitsGenColumn(short *g, short pos, short val) {
	short init = pos % pow(dimension, 2);
	short until = pow(dimension, 4);
	short inc = pow(dimension, 2);
	for (short i = init; i < until; i += inc) {
		if (i == pos) {
			continue;
		}
		if (g[i] == val) {
			return false;
		}
	}
	return true;
}

bool fits2Column(short *g, short pos, short val) {
	for (short i = pos % 4; i < 16; i += 4) {
		if (i == pos) {
			continue;
		}
		if (g[i] == val) {
			return false;
		}
	}
	return true;
}

bool fits3Column(short *g, short pos, short val) {
	for (short i = pos % 9; i < 81; i += 9) {
		if (i == pos) {
			continue;
		}
		if (g[i] == val) {
			return false;
		}
	}
	return true;
}

bool fits2Square(short *g, short pos, short val) {
	for (int i = 0; i <= 2; i += 2) {
		if (pos == 0 + i || pos == 1 + i || pos == 4 + i || pos == 5 + i) {
			if ((pos != 0 + i && val == g[0 + i])
					|| (pos != 1 + i && val == g[1 + i])
					|| (pos != 4 + i && val == g[4 + i])
					|| (pos != 5 + i && val == g[5 + i])) {
				return false;
			}
		}
	}
	for (int i = 8; i <= 10; i += 2) {
		if (pos == 0 + i || pos == 1 + i || pos == 4 + i || pos == 5 + i) {
			if ((pos != 0 + i && val == g[0 + i])
					|| (pos != 1 + i && val == g[1 + i])
					|| (pos != 4 + i && val == g[4 + i])
					|| (pos != 5 + i && val == g[5 + i])) {
				return false;
			}
		}
	}
	return true;
}

bool fits3Square(short *g, short pos, short val) {
	for (int i = 0; i <= 6; i += 3) {
		if (pos == 0 + i || pos == 1 + i || pos == 2 + i || pos == 9 + i
				|| pos == 10 + i || pos == 11 + i || pos == 18 + i
				|| pos == 19 + i || pos == 20 + i) {
			if ((pos != 0 + i && val == g[0 + i])
					|| (pos != 1 + i && val == g[1 + i])
					|| (pos != 2 + i && val == g[2 + i])
					|| (pos != 9 + i && val == g[9 + i])
					|| (pos != 10 + i && val == g[10 + i])
					|| (pos != 11 + i && val == g[11 + i])
					|| (pos != 18 + i && val == g[18 + i])
					|| (pos != 19 + i && val == g[19 + i])
					|| (pos != 20 + i && val == g[20 + i])) {
				return false;
			}
		}
	}
	for (int i = 27; i <= 33; i += 3) {
		if (pos == 0 + i || pos == 1 + i || pos == 2 + i || pos == 9 + i
				|| pos == 10 + i || pos == 11 + i || pos == 18 + i
				|| pos == 19 + i || pos == 20 + i) {
			if ((pos != 0 + i && val == g[0 + i])
					|| (pos != 1 + i && val == g[1 + i])
					|| (pos != 2 + i && val == g[2 + i])
					|| (pos != 9 + i && val == g[9 + i])
					|| (pos != 10 + i && val == g[10 + i])
					|| (pos != 11 + i && val == g[11 + i])
					|| (pos != 18 + i && val == g[18 + i])
					|| (pos != 19 + i && val == g[19 + i])
					|| (pos != 20 + i && val == g[20 + i])) {
				return false;
			}
		}
	}
	for (int i = 54; i <= 60; i += 3) {
		if (pos == 0 + i || pos == 1 + i || pos == 2 + i || pos == 9 + i
				|| pos == 10 + i || pos == 11 + i || pos == 18 + i
				|| pos == 19 + i || pos == 20 + i) {
			if ((pos != 0 + i && val == g[0 + i])
					|| (pos != 1 + i && val == g[1 + i])
					|| (pos != 2 + i && val == g[2 + i])
					|| (pos != 9 + i && val == g[9 + i])
					|| (pos != 10 + i && val == g[10 + i])
					|| (pos != 11 + i && val == g[11 + i])
					|| (pos != 18 + i && val == g[18 + i])
					|| (pos != 19 + i && val == g[19 + i])
					|| (pos != 20 + i && val == g[20 + i])) {
				return false;
			}
		}
	}
	return true;
}

bool fitsGenSquare(short *g, short pos, short val) {
	short until = pow(dimension, 4);
	short bxSz = pow(dimension, 2);
	for (short i = 0; i < until; i++) {
		if (pos % bxSz / dimension == i % bxSz / dimension
				&& pos / bxSz / dimension == i / bxSz / dimension
				&& val == g[i]) {
			return false;
		}
	}
	return true;
}

bool fits(short *g, short pos, short val) {
	return fitsGenRow(g, pos, val) && fitsGenColumn(g, pos, val)
			&& fitsGenSquare(g, pos, val);
}

void setValue(short *g, short idx, short val) {
	if (val != 0) {
		if (idx < 0 || idx >= pow(dimension, 4) || val < 0
				|| val > pow(dimension, 2) || !fits(g, idx, val)) {
			cout << "Could not set value";
			exit(-1);
		} else {
			g[idx] = val;
			idxGrid[idx] = 1;
			cout << "Value of g[" << idx << "] set to " << val << endl;
		}
	}
}

void solve(short *g) {
	short big = pow(dimension, 4);
	short small = pow(dimension, 2);
	long int c = 0; // For Debugging
	for (short i = 0; i < big; i++) {
		if (idxGrid[i] == 0) { // Tests preset values
			for (short j = 1; j <= small; j++) {
				if (fits(g, i, j)) {
					cout << ++c << ":\tChanged g[" << i << "] from " << g[i]
							<< " to " << j << endl; // Debugging
					g[i] = j;
					break;
				} else { // Debugging
					cout << ++c << ":\tValue of " << j
							<< " will not work for g[" << i
							<< "]\tIncrementing value..." << endl; // Debugging
				}
			}
			// Backtracking
			while (g[i] == 0) {
				i--;
				cout << ++c << ":\tBacktracking... " << "\tNow working on g["
						<< i << "]..." << endl; // Debugging
				while (idxGrid[i] != 0) { // Tests preset values
					i--;
					cout << ++c << ":\t\t\t\tWorking on g[" << i << "]..."
							<< endl; // Debugging
				}
				cout << ++c << ":\tChanged g[" << i << "] from " << g[i]
						<< " to ";
				if (g[i] + 1 > small) {
					g[i] = 0;
					cout << g[i] << endl;
				} else {
					for (short k = g[i] + 1; k <= small; k++) {
						if (fits(g, i, k)) {
							g[i] = k;
							goto point;
						}
					}
					g[i] = 0;
					point: cout << g[i] << endl;
				}
			}
		} else { // Debugging
			cout << ++c << ":\tCannot change g[" << i << "]'s value of " << g[i]
					<< endl; // Debugging
		}
	}
}

void solve2(short *g) {
	for (short i = 0; i < 16; i++) {
		for (short j = 1; j <= 4; j++) {
			if (fits2Row(g, i, j) && fits2Column(g, i, j)
					&& fits2Square(g, i, j)) {
				g[i] = j;
				break;
			}
		}
	}
}

void solve3(short *g) {
	for (short i = 0; i < 81; i++) {
		for (short j = 1; j <= 9; j++) {
			if (fits3Row(g, i, j) && fits3Column(g, i, j)
					&& fits3Square(g, i, j)) {
				g[i] = j;
				break;
			}
		}
	}
}

void setValues(short *genGrid) {
	// For dimension = 2
	setValue(genGrid, 0, 0);
	setValue(genGrid, 1, 0);
	setValue(genGrid, 2, 0);

	setValue(genGrid, 3, 0);
	setValue(genGrid, 4, 0);
	setValue(genGrid, 5, 0);

	setValue(genGrid, 6, 0);
	setValue(genGrid, 7, 0);
	setValue(genGrid, 8, 0);

	setValue(genGrid, 9, 0);
	setValue(genGrid, 10, 0);
	setValue(genGrid, 11, 0);

	setValue(genGrid, 12, 0);
	setValue(genGrid, 13, 0);
	setValue(genGrid, 14, 3);

	setValue(genGrid, 15, 0);
	// For dimension = 3
	setValue(genGrid, 16, 8);
	setValue(genGrid, 17, 5);

	setValue(genGrid, 18, 0);
	setValue(genGrid, 19, 0);
	setValue(genGrid, 20, 1);

	setValue(genGrid, 21, 0);
	setValue(genGrid, 22, 2);
	setValue(genGrid, 23, 0);

	setValue(genGrid, 24, 0);
	setValue(genGrid, 25, 0);
	setValue(genGrid, 26, 0);

	setValue(genGrid, 27, 0);
	setValue(genGrid, 28, 0);
	setValue(genGrid, 29, 0);

	setValue(genGrid, 30, 5);
	setValue(genGrid, 31, 0);
	setValue(genGrid, 32, 7);

	setValue(genGrid, 33, 0);
	setValue(genGrid, 34, 0);
	setValue(genGrid, 35, 0);

	setValue(genGrid, 36, 0);
	setValue(genGrid, 37, 0);
	setValue(genGrid, 38, 4);

	setValue(genGrid, 39, 0);
	setValue(genGrid, 40, 0);
	setValue(genGrid, 41, 0);

	setValue(genGrid, 42, 1);
	setValue(genGrid, 43, 0);
	setValue(genGrid, 44, 0);

	setValue(genGrid, 45, 0);
	setValue(genGrid, 46, 9);
	setValue(genGrid, 47, 0);

	setValue(genGrid, 48, 0);
	setValue(genGrid, 49, 0);
	setValue(genGrid, 50, 0);

	setValue(genGrid, 51, 0);
	setValue(genGrid, 52, 0);
	setValue(genGrid, 53, 0);

	setValue(genGrid, 54, 5);
	setValue(genGrid, 55, 0);
	setValue(genGrid, 56, 0);

	setValue(genGrid, 57, 0);
	setValue(genGrid, 58, 0);
	setValue(genGrid, 59, 0);

	setValue(genGrid, 60, 0);
	setValue(genGrid, 61, 7);
	setValue(genGrid, 62, 3);

	setValue(genGrid, 63, 0);
	setValue(genGrid, 64, 0);
	setValue(genGrid, 65, 2);

	setValue(genGrid, 66, 0);
	setValue(genGrid, 67, 1);
	setValue(genGrid, 68, 0);

	setValue(genGrid, 69, 0);
	setValue(genGrid, 70, 0);
	setValue(genGrid, 71, 0);

	setValue(genGrid, 72, 0);
	setValue(genGrid, 73, 0);
	setValue(genGrid, 74, 0);

	setValue(genGrid, 75, 0);
	setValue(genGrid, 76, 4);
	setValue(genGrid, 77, 0);

	setValue(genGrid, 78, 0);
	setValue(genGrid, 79, 0);
	setValue(genGrid, 80, 9);
}

int main() {

	short *genGrid = (short*) calloc(pow(dimension, 4), sizeof(short));
	setValues(genGrid);

	clock_t begin = clock();
	solve(genGrid);
	clock_t end = clock();

	printGenGrid(genGrid);
	cout << "Time to solve: " << (end - begin) / CLOCKS_PER_SEC;

	free(genGrid);
	return 0;
}

