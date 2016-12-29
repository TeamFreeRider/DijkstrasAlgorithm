#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<conio.h>
#include<stdlib.h>
#include <stdbool.h>

//typedef enum { false, true } bool;
#define INT_MAX 2147483647
#define V 13 // number of vertex
#define ROTATIONDISTANCE 5
int coord[V][2] =
{ // 42 X 24
	{ 0, 0 },// startvertex
	{ 0, 0 },  // 0 // x, y
	{ 14, 0 },  // 1
	{ 28, 0 }, // 2
	{ 42, 0 }, // 3
	{ 0, 12 },  // 4
	{ 14, 12 },  // 5
	{ 28, 12 }, // 6
	{ 0, 24 }, // 7
	{ 14, 24 }, // 8
	{ 28, 24 },// 9
	{ 42, 24 },// 10
	{ 0,0 } // end vertex
};
// node number 설정 total 11개
// 7    8    9    10
// 4    5    6   
// 0    1    2    3

int graph[V][V] =
{ //  s  0  1  2  3  4  5  6  7  8  9 10, e
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0 },
	{ 0, 0, 5, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 }, // 0
	{ 0, 5, 0, 4, 0, 0, 5, 0, 0, 0, 0, 0, 0 }, // 1
	{ 0, 0, 4, 0, 7, 0, 0,12, 0, 0, 0, 0, 0 }, // 2
	{ 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,10, 0 },// 3
	{ 0, 2, 0, 0, 0, 0,10, 0, 7, 0, 0, 0, 0 }, // 4
	{ 0, 0, 5, 0, 0,10, 0, 5, 0,10, 0, 0, 0 },//5
	{ 0, 0, 0,12, 0, 0, 5, 0, 0, 0, 3, 0, 0 }, //6
	{ 0, 0, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0 }, // 7
	{ 0, 0, 0, 0, 0, 0,10, 0, 7, 0, 9, 0, 0 }, // 8
	{ 0, 0, 0, 0, 0, 0, 0, 3, 0, 9, 0, 4, 0 }, // 9
	{ 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 4, 0, 0 }, // 10
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // e 
};

int closenode(int x, int y) {

	int node = 0, i;
	double min = 99999.999;
	double distance[V];
	for (i = 0; i < V; i++) {
		distance[i] = sqrt(pow((double)(x - coord[i][0]), 2) + pow((double)(y - coord[i][1]), 2));
	}
	for (i = 1; i < V - 1; i++) {
		if (min > distance[i]) {
			min = distance[i];
			node = i;
		}
	}


	return node;
}
double distance(int x1, int y1, int x2, int y2) {
	return sqrt(pow((double)(x1 - x2), 2) + pow((double)(y1 - y2), 2));
}

int mindistance(double best[], int sptSet[])
{
	double min = 99999;
	int min_index;

	for (int v = 0; v < V; v++)
		if (sptSet[v] == 0 && best[v] <= min)
			min = best[v], min_index = v;

	return min_index;

}





//Queue Functions
#define MAX V*3
int intArray[V*3];
int rear = -1;
int front = -1;
int itemCount = 0;
/*
void initQ(int _size) {
	size = _size;
	int *queue = NULL;
	queue = (int*)malloc(sizeof(int)*size);
	head = 0; tail = 0;
}*/

int peek() {
	return intArray[front];
}

bool isEmpty() {
	return itemCount == 0;
}

bool isFull() {
	return itemCount == MAX;
}

int size() {
	return itemCount;
}

void insertQ(int data) {

	if (!isFull()) {

		if (rear == MAX - 1) {
			rear = -1;
		}

		intArray[++rear] = data;
		itemCount++;
	}
}

int delQ() {
	int data = intArray[front++];

	if (front == MAX) {
		front = 0;
	}

	itemCount--;
	return data;
}


//Adding instructions and coordinations of nodes
void addInstructions(int path[])
{
	char instruction;
	int prev[V][2] = { 0,0 }, curr[V][2] = { 0,0 }, next[V][2] = { 0,0 };
	//initQ(V * 3);

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

			// y decrease
			else if (prev[x][1] > curr[x][1]) {
				if (curr[x][0] > next[x][0])//x decrease
					instruction = 'L';
				else instruction = 'R';
			}
			// y increase
			else if (prev[x][1] < curr[x][1]) {
				if (curr[x][0] > next[x][0])//x decrease
					instruction = 'R';
				else instruction = 'L';
			}
			else instruction = 'F';
			printf("Instruction has been calculated\n");
		}

		
			insertQ(curr[x][0]);
			insertQ(curr[x][1]);
			insertQ(atoi((char*)instruction));
		
		printf("instruction %d : %s \n", x, instruction);
		getchar();

	}

	getchar();
}



void main()
{
	
	int nextnode[V][2];
	char sendInstruction;
	int location[1][2];
	int prevnode[V], i, sptSet[V];
	double  best[V];

	int inversepath[V], path[V], j = 0;
	int startV, endV; // source or destination 좌표에 가장 인접한 vertex.
					  // sptSet[i] will true if vertex i is included in shortest
					  // path tree or shortest distance from src to i is finalized                    

	scanf_s("%d %d", &coord[0][0], &coord[0][1]); // start
	scanf_s("%d %d", &coord[12][0], &coord[12][1]);   //end 

	startV = closenode(coord[0][0], coord[0][1]);
	endV = closenode(coord[12][0], coord[12][1]);
	printf("%d %d\n", startV, endV);
	graph[0][startV] = 1; // source와 그 인접노드 간 weight
	graph[endV][12] = 1; //destination과 인접노드간 weight
						 // starting vertex. 0에서 startV로 무조건 이동
						 // 12번 node(destination)에서 ending vertex으로 무조건 이동
	for (i = 0; i < V; i++)
		best[i] = 99999, sptSet[i] = 0, inversepath[i] = -1, path[i] = -1;


	best[0] = 0;
	prevnode[0] = -1;

	for (i = 0; i < V; i++) {
		for (int j = 0; j < V; j++) {
			printf("%2d  ", graph[i][j]);
		}
		printf("\n");
	}

	for (int count = 0; count < V - 1; count++)
	{
		int u = mindistance(best, sptSet);
		sptSet[u] = 1;


		for (int v = 0; v < V; v++) {
			if (!sptSet[v] && graph[u][v] && best[u] + graph[u][v] < best[v]) {
				best[v] = best[u] + graph[u][v];
				prevnode[v] = u;
			}
		}
	}

	// print the constructed distance array
	printf("Vertex Distance from Source\n");
	for (int i = 0; i < V; i++)
		printf("%d \t\t %.2f\n", i, best[i]);

	inversepath[j] = 12;

	while (1) {
		if (prevnode[inversepath[j]] == -1 && inversepath[j] != 12) {
			j++;
			break;
		}
		inversepath[j + 1] = prevnode[inversepath[j]];
		j++;
	} // j : length of path.
	for (i = 0; i < j; i++) {
		path[i] = inversepath[j - i - 1];

	}
	//if soruce node is in the middle of the StartV and the second node of the path 
	if ((coord[path[1]][0] - coord[0][0])*(coord[path[2]][0] - coord[0][0])<0 || (coord[path[1]][1] - coord[0][1])*(coord[path[2]][1] - coord[0][1])<0) {
		for (i = 1; i < j - 1; i++) {
			path[i] = path[i + 1];
		}
		j--;
		path[j] = -1;
	}

	//if destination node is in the middle of EndV and the last to second node of the path
	if ((coord[prevnode[12]][0] - coord[12][0])*(coord[prevnode[prevnode[12]]][0] - coord[12][0])<0 || (coord[prevnode[12]][1] - coord[12][1])*(coord[prevnode[prevnode[12]]][1] - coord[12][1])<0) {
		path[j - 2] = path[j - 1];
		path[j - 1] = -1;
		j--;
	}



	for (i = 0; i < j; i++) {
		if (i == j - 1)
			printf("%d\n", path[i]);
		else printf("%d->", path[i]);
	}


	printf("dijkstra is fine");

	addInstructions(path);

	printf("addInsturcution is fine \n");
	while (isEmpty != 0) {
		for (i = 0; i < sizeof(path); i++) {
			if (isEmpty = 0) {
				break;
			}
			nextnode[i][0] = delQ();
			nextnode[i][1] = delQ();
			printf("next node x: %d y: %d", nextnode[i][0], nextnode[i][1]);
			sendInstruction = delQ();
			printf("%dth sendInstruction is : %c", i, sendInstruction);
		}
		/*while ((location[0][0] != nextnode[0][0]) || (location[0][1] != nextnode[0][1])) {

			if (distance(location[0][0], location[0][1], nextnode[0][0], nextnode[0][1])<ROTATIONDISTANCE) {
				sendInstruction = 'F';
			}
			else
				_itoa_s(delQ(), sendInstruction, sizeof(sendInstruction), 10); // the final instruction that is sent to the car real-time
			printf("sendInstruction is : %c", sendInstruction);
		}*/

		//_itoa_s(delQ(), sendInstruction, 1000, 10); // the final instruction that is sent to the car real-time
		
	}
	//printf("del Q is %d", delQ());
	printf("send Instruction finished\n");
	getchar();
	getchar();
}
