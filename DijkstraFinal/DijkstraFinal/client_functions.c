#include "Freerider.h"


int path[V], inversepath[V], prevnode[V];
double best[V];

int size, head, tail;
struct inst* instructionset;

struct car_info cararray[50];
int length = 0;
int maxnode;
int altpath[V];
int altgraph[V][V];
int interruptedNode;
int u, v, j;

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

void initQ(int _size) {
	size = _size;
	instructionset = (struct inst*)malloc(sizeof(struct inst)*size);
	head = 0; tail = 0;;
}

void insertQ(struct inst data) {
	instructionset[tail] = data;
	tail = (tail + 1);
	printf(" insert is finished successfully, posx : %d, posy : %d, inst : %c\n", data.pos_x, data.pos_y, data.instruction);
}

struct inst delQ() {
	struct inst temp;
	if (head == tail) {
		temp.instruction = 'Z';
		temp.pos_x = '-1';
		temp.pos_y = '-1';
	}
	else {
		temp = instructionset[head];
		head = (head + 1);
	}
	return temp;
}


//Adding instructions and coordinations of nodes
void addInstructions(int path[])
{
	struct inst tempinst;

	int prev[V][2] = { 0,0 }, curr[V][2] = { 0,0 }, next[V][2] = { 0,0 };
	initQ(V);

	for (int x = 0; x < V; x++) {
		//printf("\nfor starts");
		printf("\nfor starts\n");
		if (path[x] == 0)
		{
			tempinst.instruction = 'F';
			tempinst.pos_x = coord[path[x]][0];
			tempinst.pos_y = coord[path[x]][1];
		}
		else if (path[x + 1] == -1) {
			tempinst.instruction = 'S';
			tempinst.pos_x = coord[path[x]][0];
			tempinst.pos_y = coord[path[x]][1];
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
				tempinst.instruction = 'F';
			}
			//prev x > curr x   
			else if (prev[x][0] > curr[x][0]) {
				if (curr[x][1] > next[x][1])
					tempinst.instruction = 'L';
				else tempinst.instruction = 'R';
			}
			// prev x < curr x
			else if (prev[x][0] < curr[x][0]) {
				if (curr[x][1] > next[x][1])
					tempinst.instruction = 'R';
				else tempinst.instruction = 'L';
			}

			// prev y> curr y
			else if (prev[x][1] > curr[x][1]) {
				if (curr[x][0] > next[x][0])
					tempinst.instruction = 'R';
				else tempinst.instruction = 'L';
			}
			// prev y< curr y
			else if (prev[x][1] < curr[x][1]) {
				if (curr[x][0] > next[x][0])
					tempinst.instruction = 'L';
				else tempinst.instruction = 'R';
			}
			else tempinst.instruction = 'F';

			tempinst.pos_x = curr[x][0];
			tempinst.pos_y = curr[x][1];
		}
		printf("%d -th instruction : posx : %d, posy : %d, inst : %c\n", x, tempinst.pos_x, tempinst.pos_y, tempinst.instruction);
		insertQ(tempinst);
		getchar();
		if (path[x + 1] == -1)
			break;
	}
	getchar();
}


bool interruption(int carloc[], int interruptloc[]) {
	struct inst temp;

	int car_next; // ���� ������ ������ �� ���
	int car_prev; // ���� ������ ���� ���
	int car_nextorder, car_prevorder; // next or prev ����� path array������ ����

	int inter_next1, inter_next2; // interruption ���� ��� 2��. ������� x
	int i = 0;
	temp = instructionset[head]; // ������ ������ �� ����� ������ ����ִ� instructionset
	for (car_next = 0; car_next < V; car_next++) {
		if ((temp.pos_x == coord[car_next][0]) && temp.pos_y == coord[car_next][1]) { // car_next�� ���ϴ� for ��
			break;
		}
	}
	car_prev = prevnode[car_next]; // �翬�� carnext�� ���� ���

	inter_next1 = closenode(interruptloc[0], interruptloc[1]); // interrupt�� ����� ����� �ϳ�. closenode�� ���� ���
	for (inter_next2 = 0; inter_next2 < V; inter_next2++) { // interrupt ���� �� �ٸ� �ϳ��� ����. graph matrix�� ���� ������� �� 0�̾ƴ� ��忡 ���ؼ� �˻�.
		if (!graph[inter_next1][inter_next2]) {
			if (coord[inter_next2][1] == coord[inter_next1][1]) {//�� ��尡 y��ǥ�� ���� ���
				if ((coord[inter_next2][0] - interruptloc[0])*(coord[inter_next1][0] - interruptloc[0]) < 0) { //�� ��尡x�࿡�� interruptloc[]�� ���ΰ� �ִٸ� break;
					break;
				}
			}
			else if (coord[inter_next2][0] == coord[inter_next1][0]) { // �� ��尡 x��ǥ�� ���� ���
				if ((coord[inter_next2][1] - interruptloc[1])*(coord[inter_next1][1] - interruptloc[1]) < 0) { // �� ��尡 y �࿡�� interruptloc�� ���ΰ� �ִٸ� break/
					break;
				}
			}
		}
	}
	for (i = 0; i < length; i++) {// car nextorder ���.
		if (path[i] == car_next) {
			break;
		}
	}
	car_nextorder = i;
	car_prevorder = car_nextorder - 1; // �翬

	for (i = 0; i < length - 1; i++) {
		if ((path[i] == inter_next1 && path[i + 1] == inter_next2) || (path[i] == inter_next2 && path[i + 1] == inter_next1)) { // ��� ���� interrupt�� �ִ� ���. �� �� path[i]�� interrupt��������� ����, path[i+1]�� ��
			if (car_nextorder <= path[i]) { // car�� ���� �� interrupt�� �ִ� edge�� ������ ���� ���. return true.
				interruptedNode = i;
				return true;
			}
			else if (car_prevorder >= path[i + 1]) { // car �̹� �� interrupt edge�� ���� ���. interrupt�� �־ �Ű�Ⱦ� return false.
				return false;
			}
			else if (car_prevorder == path[i] && car_nextorder == path[i + 1]) { // car�� interrupt edge�� �ִ� ���. path[i+1]���� distance�� ����Ͽ� car�� �� �ָ��ִٸ� ���� interrupt�����������Ƿ� return true.
				if (distance(coord[path[i + 1]][0], coord[path[i + 1]][1], carloc[0], carloc[1]) > distance(coord[path[i + 1]][0], coord[path[i + 1]][1], interruptloc[0], interruptloc[1])) {
					interruptedNode = i;
					return true;
				}
				else return false;
			}
		}
		else return false;
	}

}
void makeNewGraph(bool interruption , int graph[][V], int path[]) { 
			if (interruption == true) {
				for (u = 0; u < V; u++) {
					for (v = 0; v < V; v++) {
						altgraph[u][v] = graph[u][v];
					}
				}

				altgraph[interruptedNode][interruptedNode + 1] = 0;
				altgraph[interruptedNode + 1][interruptedNode] = 0;

				dijkstra(altgraph, altpath);
				for (j = 0; j < sizeof(path)/sizeof(int); j++) {
					if (altpath[j] == path[j])
						maxnode = j;
					else
						break;
				}
			}
}
char Sendinstruction(int location[], int location2[], int path[], struct inst* instruction) { // location : car location. location2 : interruption location.( == other car location)
	while (1) {
		char sendinstruction;
		struct inst temp;

		if (temp.instruction == 'Z') {
			break;
		}
		else {
			if ((coord[maxnode][0] == temp.pos_x && coord[maxnode][1] == temp.pos_y)&&(interruption(location, location2) == true) ){ // path�� altpath�� ��ġ�� ���κп� �������� interruption�� �����Ѵٸ�
					path = altpath; // path�� altpath�� �����.
					for (int u = 0; u < V; u++) {
						for (int v = 0; v < V; v++) {
							graph[u][v] = altgraph[u][v]; // ���� graph�� altgraph�� �����.
						}
					}
					addInstructions(path); // instructionset �籸��
					break;
			}

			while (location[0] != temp.pos_x || location[1] != temp.pos_y) { // car�� ��ġ�� ���� ��忡 �������� �ʾ��� ��	
				if (distance(location[0], location[1], temp.pos_x, temp.pos_y) < ROTATIONDISTANCE)
					sendinstruction = 'F';
				else {
					temp = delQ(); // instruction�� �ʿ��� �� delQ
					sendinstruction = temp.instruction;
				}
				break;
				
			}
			//printf("%c at %d, %d \n", temp.instruction, temp.pos_x, temp.pos_y);
		}
		return sendinstruction;
	}
}




//int graph[V][V];
//int altpath[V]; �̷��� �־����� ��( graph�� path�� ���������� ����)
//dijkstra(altgraph, altpath);// �Ͱ��� �����ϸ� �� or dijkstra(graph, path);


void dijkstra(int editgraph[][V], int* editpath) {

	int  i, sptSet[V], j = 0;
	//struct inst* instructionset;
	//instructionset = (struct inst*)malloc(sizeof(struct inst)*V);
	int startV, endV; // source or destination ��ǥ�� ���� ������ vertex.
					  // sptSet[i] will true if vertex i is included in shortest
					  // path tree or shortest distance from src to i is finalized                    

					  //  scanf_s("%d %d", &coord[0][0], &coord[0][1]); // start
					  //  scanf_s("%d %d", &coord[12][0], &coord[12][1]);   //end 

	startV = closenode(coord[0][0], coord[0][1]);
	endV = closenode(coord[12][0], coord[12][1]);
	printf("%d %d\n", startV, endV);
	editgraph[0][startV] = 1; // source�� �� ������� �� weight
	editgraph[endV][12] = 1; //destination�� ������尣 weight
							 // starting vertex. 0���� startV�� ������ �̵�
							 // 12�� node(destination)���� ending vertex���� ������ �̵�
	for (i = 0; i < V; i++)
		best[i] = 99999, sptSet[i] = 0, inversepath[i] = -1, editpath[i] = -1;


	best[0] = 0;
	prevnode[0] = -1;

	for (i = 0; i < V; i++) {
		for (int j = 0; j < V; j++) {
			printf("%2d  ", editgraph[i][j]);
		}
		printf("\n");
	}

	for (int count = 0; count < V - 1; count++)
	{
		int u = mindistance(best, sptSet);
		sptSet[u] = 1;


		for (int v = 0; v < V; v++) {
			if (!sptSet[v] && editgraph[u][v] && best[u] + editgraph[u][v] < best[v]) {
				best[v] = best[u] + editgraph[u][v];
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
	length = j;
	for (i = 0; i < length; i++) {
		editpath[i] = inversepath[length - i - 1];

	}
	//if soruce node is in the middle of the StartV and the second node of the path 
	if ((coord[editpath[1]][0] - coord[0][0])*(coord[editpath[2]][0] - coord[0][0])<0 || (coord[editpath[1]][1] - coord[0][1])*(coord[editpath[2]][1] - coord[0][1])<0) {
		for (i = 1; i < length - 1; i++) {
			editpath[i] = editpath[i + 1];
		}
		length--;
		editpath[length] = -1;
	}

	//if destination node is in the middle of EndV and the last to second node of the path
	if ((coord[prevnode[12]][0] - coord[12][0])*(coord[prevnode[prevnode[12]]][0] - coord[12][0])<0 || (coord[prevnode[12]][1] - coord[12][1])*(coord[prevnode[prevnode[12]]][1] - coord[12][1])<0) {
		editpath[length - 2] = editpath[length - 1];
		editpath[length - 1] = -1;
		length--;
	}

	for (i = 0; i < length; i++) {
		if (i == length - 1)
			printf("%d\n", editpath[i]);
		else printf("%d->", editpath[i]);
	}
	printf("dijkstra is fine\n");

	//addInstructions(editpath);
}







char FindNearestDijkstra(struct car_info cararray[], int user[]) {
	cararray[0].pos_x = 14;
	cararray[0].pos_x = 0;
	int distance[50];
	coord[12][0] = user[0];
	coord[12][1] = user[1];
	int min = 99999;
	char minserial;
	for (int i = 0; i<numofcar; i++) {
		coord[0][0] = cararray[i].pos_x;
		coord[0][1] = cararray[i].pos_y;
		dijkstra(graph,path);
		distance[i] = best[12];
		if (distance[i] < min) {
			min = distance[i];
			minserial = cararray[i].serial;
		}
	}
	return minserial;
}
