#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<malloc.h>
#include<conio.h>

define V 
void addInstructions(int path[])
{
	char instruction;
	int prev[V][2] = { 0,0 }, curr[V][2] = { 0,0 }, next[V][2] = { 0,0 };
	initQ(V * 3);

	for (int x = 0; x<(V * 3); x++) {
		//printf("\nfor starts");
		printf("\nfor starts\n");
		if (path[x] == 0 || path[x + 1] == -1)
		{
			curr[x][0] = coord[path[x]][0];
			curr[x][1] = coord[path[x]][1];
			instruction = 'F';
			printf("\npath is either starting node or last node\n");
			break;
		}

		else {
			curr[x][0] = coord[path[x]][0];
			curr[x][1] = coord[path[x]][1];
			prev[x][0] = coord[path[x - 1]][0];
			prev[x][1] = coord[path[x - 1]][1];
			next[x][0] = coord[path[x + 1]][0];
			next[x][1] = coord[path[x + 1]][1];
			printf("coord has been inserted\n");


			if (prev[x][0] == next[x][0] || prev[x][1] == next[x][1]) {
				instruction = 'F';
			}
			//prev x > curr x   
			else if (prev[x][0] > curr[x][0]) {
				if (curr[x][1] > next[x][1])
					instruction = 'L';
				else instruction = 'R';
			}
			// prev x < curr x
			else if (prev[x][0] < curr[x][0]) {
				if (curr[x][1] > next[x][1])
					instruction = 'R';
				else instruction = 'L';
			}

			// prev y> curr y
			else if (prev[x][1] > curr[x][1]) {
				if (curr[x][0] > next[x][0])
					instruction = 'L';
				else instruction = 'R';
			}
			// prev y< curr y
			else if (prev[x][1] < curr[x][1]) {
				if (curr[x][0] > next[x][0])
					instruction = 'R';
				else instruction = 'L';
			}
			else instruction = 'F';
			printf("Instruction has been calculated\n");
		}
		if (insertQ(curr[x][0]) && insertQ(curr[x][1]) == 0)
			printf("queue is full\n");

		else if (insertQ(atoi(instruction)) == 0)
			printf("queue is full. Can't insert instruction\n");
		else
		{
			insertQ(curr[x][0]);
			insertQ(curr[x][1]);
			insertQ(atoi(instruction));
		}

		printf("instruction %d : %s \n", x, instruction);
		getchar();

	}