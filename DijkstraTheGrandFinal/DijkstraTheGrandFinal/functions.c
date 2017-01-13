#include "Freerider.h"

int interruptrecovery[V][3];
int recoveryweight1[3]; // editweight function���� graph �� source/destination node ���� ��� ���� ���� weight ������ ����
int recoveryweight2[3];
int altrecoveryweight1[3];// editweight function���� altgraph �� source node ���� ��� ���� ���� weight ������ ����
int path[V], inversepath[V], prevnode[V];
int best[V];
int start = 0;
int size, head, tail; // instructionset
int altsize, althead, alttail; // altinstructionset
struct inst* instructionset;
struct inst* altinstructionset; // altpath�� ���� instruction. ���� maxnode���� �����ϸ� instructionset���� ����

int uturn_x;
int uturn_y;
int interruptatnode = 0;
int whatinterruptnode;
int altassign = 0;
int sendUturn;
//struct car_info carlist[50];
int continueUturn = 0;
int length = 0, altlength = 0;
int maxnode = 99;
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
{ //  s  1  2  3  4  5  6  7  8  9  10 11, e
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0 },
	{ 0, 0, 5, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 }, // 1
	{ 0, 5, 0, 4, 0, 0, 5, 0, 0, 0, 0, 0, 0 }, // 2
	{ 0, 0, 4, 0, 7, 0, 0,12, 0, 0, 0, 0, 0 }, // 3
	{ 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,10, 0 }, // 4
	{ 0, 2, 0, 0, 0, 0,10, 0, 7, 0, 0, 0, 0 }, // 5
	{ 0, 0, 5, 0, 0,10, 0, 5, 0,10, 0, 0, 0 }, // 6
	{ 0, 0, 0,12, 0, 0, 5, 0, 0, 0, 3, 0, 0 }, // 7
	{ 0, 0, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0 }, // 8
	{ 0, 0, 0, 0, 0, 0,10, 0, 7, 0, 9, 0, 0 }, // 9
	{ 0, 0, 0, 0, 0, 0, 0, 3, 0, 9, 0, 4, 0 }, // 10
	{ 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 4, 0, 0 }, // 11
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // e 
};


int closenode(int x, int y) {
	int nextnearnode = 0;
	int node = 0, i;
	double min = 99999.999;
	double distance[V];
	for (i = 0; i < V; i++) {
		distance[i] = sqrt(pow((double)(x - coord[i][0]), 2) + pow((double)(y - coord[i][1]), 2));
	}
	for (i = 1; i < V - 1; i++) {
		if (min > distance[i]) {
			min = distance[i];
			nextnearnode = node;
			node = i;
		}
	}
	return node;
}

double distance(int x1, int y1, int x2, int y2) {
	return sqrt(pow((double)(x1 - x2), 2) + pow((double)(y1 - y2), 2));
}

int mindistance(int best[], int sptSet[])
{
	double min = 99999;
	int min_index;

	for (int v = 0; v < V; v++)
		if (sptSet[v] == 0 && best[v] <= min)
			min = best[v], min_index = v;

	return min_index;
}

//Queue Functions

void initQ(int _size, int altchk) {
	if (altchk == 0) {
		size = _size;
		instructionset = (struct inst*)malloc(sizeof(struct inst)*size);
		head = 0; tail = 0;
	}
	else { // altchk = 1 --> altinstructionset
		altsize = _size;
		altinstructionset = (struct inst*)malloc(sizeof(struct inst)*altsize);
		althead = 0; alttail = 0;
	}
}

void insertQ(struct inst data, int altchk) {
	if (altchk == 0) {
		instructionset[tail] = data;
		tail++;
//		printf("insert is finished successfully, posx : %d, posy : %d, inst : %c\n", data.pos_x, data.pos_y, data.instruction);
	}
	else {
		altinstructionset[alttail] = data;
		alttail++;
//		printf("insert is finished successfully, posx : %d, posy : %d, inst : %c\n", data.pos_x, data.pos_y, data.instruction);
	}
}

struct inst delQ(int altchk) {
	struct inst temp;
	if (altchk == 0) {
		if (head == tail) {
			temp.instruction = 'Z';
			temp.pos_x = '-1';
			temp.pos_y = '-1';
		}
		else {
			temp = instructionset[head];
			head++;
		}
	}
	else {
		if (althead == alttail) {
			temp.instruction = 'Z';
			temp.pos_x = '-1';
			temp.pos_y = '-1';
		}
		else {
			temp = altinstructionset[althead];
			althead++;
		}
	}
	return temp;
}


//Adding instructions and coordinations of nodes
void addInstructions(int path[], int altchk, int sendUturn) {
	struct inst tempinst;

	int prev[V][2] = { 0,0 }, curr[V][2] = { 0,0 }, next[V][2] = { 0,0 };
	initQ(V, altchk);

	for (int x = 0; x < V; x++) {
		//printf("\nfor starts");
		if (path[x] == 0) {
			if (sendUturn == 1 && altchk == 1) {
				tempinst.instruction = 'U';
				tempinst.pos_x = coord[path[x]][0];
				tempinst.pos_y = coord[path[x]][1];
			}
			else {
				tempinst.instruction = 'F';
				tempinst.pos_x = coord[path[x]][0];
				tempinst.pos_y = coord[path[x]][1];
			}
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
//			printf("coord has been inserted\n");


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
		insertQ(tempinst, altchk);
		if (path[x + 1] == -1)
			break;
	}
//	getchar();
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
//			printf("the initial car next order is %d\n", car_next);
			break;
		}
	}
	car_prev = prevnode[car_next]; // �翬�� carnext�� ���� ���

	inter_next1 = closenode(interruptloc[0], interruptloc[1]); // interrupt�� ����� ����� �ϳ�. closenode�� ���� ���
	if (coord[inter_next1][0] == interruptloc[0] && coord[inter_next1][1] == interruptloc[1]) {
		interruptatnode = 1;
		printf("the interruption is occured ON the node %d!\n", inter_next1);
	}
	else {
		for (inter_next2 = V - 1; inter_next2 >= 0; inter_next2--) { // interrupt ���� �� �ٸ� �ϳ��� ����. graph matrix�� ���� ������� �� 0�̾ƴ� ��忡 ���ؼ� �˻�.
			if (graph[inter_next1][inter_next2] != 0) {
				if (coord[inter_next2][1] == coord[inter_next1][1]) {//�� ��尡 y��ǥ�� ���� ���
					if ((coord[inter_next2][0] - interruptloc[0])*(coord[inter_next1][0] - interruptloc[0]) < 0) { //�� ��尡x�࿡�� interruptloc[]�� ���ΰ� �ִٸ� break;
						printf("we found an interruption! same y\n");
						printf("the interrupted node is %d and %d\n", inter_next1, inter_next2);
						break;
					}
				}
				else if (coord[inter_next2][0] == coord[inter_next1][0]) { // �� ��尡 x��ǥ�� ���� ���
					if ((coord[inter_next2][1] - interruptloc[1])*(coord[inter_next1][1] - interruptloc[1]) < 0) { // �� ��尡 y �࿡�� interruptloc�� ���ΰ� �ִٸ� break/
						printf("we found an interruption! same x\n");
						printf("the interrupted node is %d and %d\n", inter_next1, inter_next2);
						break;
					}
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
//	printf("the next node the car has to go is %d\n", path[car_nextorder]);

	if (interruptatnode != 0) {
		for (i = 0; i < length - 1; i++) {
			if (path[i] == inter_next1) {//the interruption is in our path
				printf("interruption is in our calculated path!\n");
				if (car_nextorder <= i) {// the car hasn't passed the interrupt node yet
					printf("the interruption ON the node! is in upcoming path\n");
					whatinterruptnode = inter_next1;
					return true;
				}
				else {// the car has passed the node
					printf("we have passed the interruption ON the node!\n");
					return false;
				}
			}
		}
		printf("the interruption ON the node is not in our path! \n");
		return false;
	}

	else {
		for (i = 0; i < length - 1; i++) {
//			printf("length is : %d", length);
			if ((path[i] == inter_next1 && path[i + 1] == inter_next2) || (path[i] == inter_next2 && path[i + 1] == inter_next1)) { // ��� ���� interrupt�� �ִ� ���. �� �� path[i]�� interrupt��������� ����, path[i+1]�� ��
				printf("interruption is in our calculated path!\n");
				if (car_nextorder <= i) { // car�� ���� �� interrupt�� �ִ� edge�� ������ ���� ���. return true.
					interruptedNode = path[i + 1];
					printf("interruption in upcoming path!\n interrupted node is : %d \n", i);
					return true;
				}
				else if (car_prevorder >= i + 1) { // car �̹� �� interrupt edge�� ���� ���. interrupt�� �־ �Ű�Ⱦ� return false.
					printf("interruption not in upcoming path!\n");
					return false;
				}
				else if (car_prevorder == path[i] && car_nextorder == path[i + 1]) { // car�� interrupt edge�� �ִ� ���. path[i+1]���� distance�� ����Ͽ� car�� �� �ָ��ִٸ� ���� interrupt�����������Ƿ� return true.
					if (distance(coord[path[i + 1]][0], coord[path[i + 1]][1], carloc[0], carloc[1]) > distance(coord[path[i + 1]][0], coord[path[i + 1]][1], interruptloc[0], interruptloc[1])) {
						printf("we found an interruption in current path!\n");
						return true;
					}

					else {
						printf("we found an interruption in current path but we passed it!\n");
						return false;
					}
				}
			}

		}
		printf("the interruption is not in our path!\n");
		return false;
	}
}


void makeNewGraph(int carloc[2], int graph[][V], int path[]) {
	for (u = 0; u < V; u++) {
		for (v = 0; v < V; v++) {
			altgraph[u][v] = graph[u][v];
		}
	}

	altgraph[recoveryweight1[0]][recoveryweight1[1]] = recoveryweight1[2];
	altgraph[recoveryweight1[1]][recoveryweight1[0]] = recoveryweight1[2];
	altgraph[0][recoveryweight1[0]] = 0;
	altgraph[0][recoveryweight1[1]] = 0;
	altgraph[recoveryweight1[0]][0] = 0;
	altgraph[recoveryweight1[1]][0] = 0;

	altgraph[recoveryweight2[0]][recoveryweight2[1]] = recoveryweight2[2];
	altgraph[recoveryweight2[1]][recoveryweight2[0]] = recoveryweight2[2];
	altgraph[12][recoveryweight2[0]] = 0;
	altgraph[12][recoveryweight2[1]] = 0;
	altgraph[recoveryweight2[0]][12] = 0;
	altgraph[recoveryweight2[1]][12] = 0;
	// dijkstra���� graph�� source / destination ���� ����(editweight) �������� weight ���� ����



	if (interruptatnode == 1) {      //when there is an interruption AT a node, not in bewteen nodes
		interruptrecovery[0][2] = 1;//interrupt recovery has been inserted
//		printf("interrupt recovery has been inserted ! : %d\n", interruptrecovery[0][2]);
		for (int j = 0; j < V; j++) {
			if (altgraph[j][whatinterruptnode] != 0) {
				interruptrecovery[j][0] = whatinterruptnode;
				interruptrecovery[j][1] = altgraph[whatinterruptnode][j];
//				printf("interrupt recovery saved : %d and %d had %d\n", interruptrecovery[j][0], j, interruptrecovery[j][1]);
			}
			altgraph[j][whatinterruptnode] = 0;// whatinterruptnode: the fully interrupted node
			altgraph[whatinterruptnode][j] = 0;
		}
		altgraph[recoveryweight1[0]][recoveryweight1[1]] = recoveryweight1[2];
		altgraph[recoveryweight1[1]][recoveryweight1[0]] = recoveryweight1[2];

		altgraph[recoveryweight2[0]][recoveryweight2[1]] = recoveryweight2[2];
		altgraph[recoveryweight2[1]][recoveryweight2[0]] = recoveryweight2[2];
		for (int j = 0; j < V; j++) {
//			printf("\n disconnected the connection with interrupted node: %d and %d has cost %d \n", whatinterruptnode, j, altgraph[whatinterruptnode][j]);
		}

	}
	else {
		altgraph[interruptedNode][prevnode[interruptedNode]] = 0; // interruptedNode : ������ �߻��� interruption fn���� �� ������
		altgraph[prevnode[interruptedNode]][interruptedNode] = 0;
		// interruption ���� �� ��尣 weight 0���� ����
//		printf("\n disconnected the interrupted nodes: %d and %d has cost %d \n", interruptedNode, prevnode[interruptedNode], altgraph[interruptedNode][prevnode[interruptedNode]]);

	}

	for (int i = 0; i < 2; i++) {
		coord[0][i] = carloc[i];
	} // ������ ���� ���� ��ġ�� �缳��


	dijkstra(altgraph, altpath, &altlength, 1); // graph�� ������ �κ� ���� �� interruption �κ� ����, �������� �缳���� �� dijkstra.
												//thorugh this dijkstra,
												//altgraph : source, destination ���� ��尣 weight ����
												//altpath : altgraph�� ���� set, altlength : altpath�� length
												//altinstructionset : Queue�� altpath�� ���� instruction �Է�
												//sour
	for (j = 0; j < sizeof(path) / sizeof(int); j++) {
		if (altpath[j] == path[j])
			maxnode = path[j]; // maxnode : ���� ������ ��ġ�� ���
		else
			break;
	}
//	printf("\n maxnode is %d ", maxnode);
	altassign = 0;
}


char Sendinstruction(int location[], int location2[], int path[]) { // location : car's location, location2 : interruption's location.
	char sendinstruction;
	struct inst temp;
	struct inst tempnext;



	if (interruption(location, location2) == true) {
		for (int i = 0; i < V; i++) {
			graph[0][i] = 0;
			graph[i][12] = 0;
		}
		if (interruptrecovery[0][2] == 1) {
			for (int j = 0; j < V; j++) {
				graph[interruptrecovery[j][0]][j] = interruptrecovery[j][1];
				graph[j][interruptrecovery[j][0]] = interruptrecovery[j][1];
//				printf("re-inserted the lost weights due to interruption ON node: %d and %d has %d now\n", interruptrecovery[j][0], j, interruptrecovery[j][1]);
			}
			interruptrecovery[0][2] = 0;
		}

		printf("there is an interruption! calculated the max common node of the alternative graph \n");
		makeNewGraph(location, graph, path);
		whatinterruptnode = 0;
	}


	if (start == 0) {
		temp = delQ(0);
		if (temp.instruction == 'U') {
			continueUturn = 1;
			start = 1;
			uturn_x = temp.pos_x;
			uturn_y = temp.pos_y;
			return temp.instruction;
		}
		else {
			start = 1;
		}
	}
//	printf("continueUturn : %d / start : %d \n", continueUturn, start);
//	printf("uturn_x: %d, uturn_y: %d \n", uturn_x, uturn_y);

	if (continueUturn == 1) {
		if ((location[0] != uturn_x) || (location[1] != uturn_y)) {
			printf("the car is not at its original position yet\n");
			sendinstruction = 'U';
			return sendinstruction;
		}
	}

	continueUturn = 0;

	temp = instructionset[head];
	tempnext = instructionset[head + 1];
	if (temp.instruction == 'Z') {
		return;
	}



	if (maxnode == 0 && altassign == 0) {

		altassign = 1;
		printf("\n constant interruption detected, changing path with new graph\n\n");
		for (u = 0; u < V; u++) {
			path[u] = altpath[u];
			for (v = 0; v < V; v++) {
				graph[u][v] = altgraph[u][v];
			}
		}
		initQ(V, 0);
		for (int a = althead; a < alttail; a++) {
			insertQ(delQ(1), 0); // -> �Ǵ� �� Ȯ�οϷ�
		}
		start = 0;
		recoveryweight1[0] = altrecoveryweight1[0];
		recoveryweight1[1] = altrecoveryweight1[1];
		recoveryweight1[2] = altrecoveryweight1[2];
	}

	//���� maxnode���� �� ������ interruption�� ������� �ʾ��� ��. altgraph, altpath, altinstruction, altrecoveryweight�� �����ϰ�, start = 0���� �缳��
	else if ((coord[maxnode][0] == temp.pos_x && coord[maxnode][1] == temp.pos_y) && (interruption(location, location2) == true) && altassign == 0) {
		altassign = 1;
		printf("\n constant interruption detected, changing path with new graph\n\n");
		for (u = 0; u < V; u++) {

			path[u] = altpath[u];
			for (v = 0; v < V; v++) {
				graph[u][v] = altgraph[u][v];
			}
		}
		initQ(V, 0);
		for (int a = althead; a < alttail; a++) {
			insertQ(delQ(1), 0); // -> �Ǵ� �� Ȯ�οϷ�
		}
		start = 0;
		recoveryweight1[0] = altrecoveryweight1[0];
		recoveryweight1[1] = altrecoveryweight1[1];
		recoveryweight1[2] = altrecoveryweight1[2];
		//instruction�� altinstructionset�� ����ִ� ���� �ʿ�    

	}





	if (distance(location[0], location[1], temp.pos_x, temp.pos_y) < ROTATIONDISTANCE) {
		if (tempnext.instruction == 'S' && distance(location[0], location[1], tempnext.pos_x, tempnext.pos_y) <= HALTDISTANCE) {
			sendinstruction = 'H'; //Crash Halt
		}
		else {
			sendinstruction = temp.instruction;
		}
	}
	else if (distance(location[0], location[1], tempnext.pos_x, tempnext.pos_y) <= ROTATIONDISTANCE) {
		sendinstruction = tempnext.instruction;
		temp = delQ(0);
//		printf("deleting queue is : %c (%d,%d)\n", temp.instruction, temp.pos_x, temp.pos_y);
	}
	else {
		printf("not in rotation distance\n");
		sendinstruction = 'F';
	}

	printf("%c at %d, %d \n", sendinstruction, temp.pos_x, temp.pos_y);
	return sendinstruction;

}


void editweight(int editgraph[][V], int nearestnode, int source_or_destination, int altchk) { // editgraph���� source or destination ���� �� ��尣 weight���� setting

//	printf("\n\n start edit weight \n\n");
	int inter_next1, inter_next2;
	double distanceratio;
	int editedweight, nextnearnode_edit = 0;

	inter_next1 = nearestnode; // startV or endV
	for (inter_next2 = V - 1; inter_next2 >= 0; inter_next2--) { //startV�Ǵ� endV�� ������带 ����. graph matrix�� ���� ������� �� 0�̾ƴ� ��忡 ���ؼ� �˻�.
		if (editgraph[inter_next1][inter_next2] != 0) {
//			printf("%d and %d has connected\n", inter_next1, inter_next2);
			if (coord[inter_next2][1] == coord[inter_next1][1]) {//�� ��尡 y��ǥ�� ���� ���
//				printf("%d and %d is same y\n", inter_next2, inter_next1);
				if ((coord[inter_next2][0] - coord[source_or_destination][0])*(coord[inter_next1][0] - coord[source_or_destination][0]) < 0) { //�� ��尡x�࿡�� source/destination node�� ���ΰ� �ִٸ� break;
//					printf("the two nodes adjacent to %d is %d and %d\n", source_or_destination, inter_next1, inter_next2);
					break;
				}
			}
			else if (coord[inter_next2][0] == coord[inter_next1][0]) { // �� ��尡 x��ǥ�� ���� ���
//				printf("%d and %d is same x\n", inter_next2, inter_next1);
				if ((coord[inter_next2][1] - coord[source_or_destination][1])*(coord[inter_next1][1] - coord[source_or_destination][1]) < 0) { // �� ��尡 y �࿡�� source/destination node�� ���ΰ� �ִٸ� break/
//					printf("the two nodes adjacent to %d is %d and %d\n", source_or_destination, inter_next1, inter_next2);
					break;
				}
			}
			else
				break;
		}
	} // inter_next1, 2�� �����.

	distanceratio = distance(coord[inter_next1][0], coord[inter_next1][1], coord[source_or_destination][0], coord[source_or_destination][1]) / distance(coord[inter_next1][0], coord[inter_next1][1], coord[inter_next2][0], coord[inter_next2][1]);
	// distance ratio : (internext1�� source/destination �Ÿ�) / (internext1, 2���� �Ÿ�)
//	printf("\n distance ratio: %f \n", distanceratio);
	editedweight = (int)(editgraph[inter_next1][inter_next2] * distanceratio);
	if (editedweight == 0) editedweight = 1;
//	printf("edited weight: %d \n", editedweight);
	nextnearnode_edit = editgraph[inter_next1][inter_next2] - editedweight;

	editgraph[source_or_destination][inter_next1] = editedweight;// source�� �� ������� �� weight
	editgraph[inter_next1][source_or_destination] = editedweight;

	editgraph[source_or_destination][inter_next2] = nextnearnode_edit;// source�� �� ���� ������� �� weight
	editgraph[inter_next2][source_or_destination] = nextnearnode_edit;
	// source�� ������尣 weight ���� �Ϸ�.

	if (altchk == 0 && source_or_destination == 0) { // Graph�� ���� editweight
		recoveryweight1[0] = inter_next1;
		recoveryweight1[1] = inter_next2;
		recoveryweight1[2] = editgraph[inter_next1][inter_next2];
//		printf("\n recovery weight 1 : %d %d  --> %d\n", recoveryweight1[0], recoveryweight1[1], recoveryweight1[2]);
	}
	else if (altchk == 1 && source_or_destination == 0) { // altGraph�� ���� editweight -> destination�� ���� ����, source�� ���� ���� ��ġ�� �����.
		altrecoveryweight1[0] = inter_next1;
		altrecoveryweight1[1] = inter_next2;
		altrecoveryweight1[2] = editgraph[inter_next1][inter_next2];
//		printf("\n altrecovery weight 1 : %d %d  --> %d\n", altrecoveryweight1[0], altrecoveryweight1[1], altrecoveryweight1[2]);
	}


	if (source_or_destination == 12) {
		recoveryweight2[0] = inter_next1;
		recoveryweight2[1] = inter_next2;
		recoveryweight2[2] = editgraph[inter_next1][inter_next2];
//		printf("\n recovery weight 2 : %d %d  --> %d\n", recoveryweight2[0], recoveryweight2[1], recoveryweight2[2]);
	}

	editgraph[inter_next1][inter_next2] = 0;
	editgraph[inter_next2][inter_next1] = 0;
	if (inter_next1 == whatinterruptnode) {
		editgraph[inter_next1][source_or_destination] = 0;
		editgraph[source_or_destination][inter_next1] = 0;
	}
	// ������尣 weight 0���� ���� �Ϸ�.
	return;
}

void dijkstra(int editgraph[][V], int* editpath, int* editlength, int altchk) { // chkalt : altgraph�� ���� dijkstra���� chk. chk ==1 �̸� altgraph
	int  i, sptSet[V], j = 0;
	int startV, endV; // source or destination ��ǥ�� ���� ������ vertex.
					  // sptSet[i] will true if vertex i is included in shortest
					  // path tree or shortest distance from src to i is finalized                    


	startV = closenode(coord[0][0], coord[0][1]);
	endV = closenode(coord[12][0], coord[12][1]);
	printf("\nStartV : %d // EndV : %d\n", startV, endV);

	editweight(editgraph, startV, 0, altchk); //editing the weight of adjacent nodes of the source node  [�� --5-- S -3- ��], weight between �� : 0.
	editweight(editgraph, endV, 12, altchk);  //editing the weight of adjacent nodes of the destination node   
											  /*	if (startV == whatinterruptnode) {
											  startV =
											  }*/
	for (i = 0; i < V; i++) // initial setting.
		best[i] = 99999, sptSet[i] = 0, inversepath[i] = -1, editpath[i] = -1;

	best[0] = 0; // distance from 0 to 0 : 0
	prevnode[0] = -1; // 0 is source

	for (i = 0; i < V; i++) {
		for (int j = 0; j < V; j++) {
			printf("%2d  ", editgraph[i][j]);  // printing editgraph
		}
		printf("\n");
	}

	for (int count = 0; count < V - 1; count++) // dijkstra calculating part
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
		printf("%d \t\t %d\n", i, best[i]);

	inversepath[j] = 12;

	while (1) {
		if (prevnode[inversepath[j]] == -1 && inversepath[j] != 12) {
			j++;
			break;
		}
		inversepath[j + 1] = prevnode[inversepath[j]];
		j++;
	} // j : length of path.

	*editlength = j;
	for (i = 0; i < *editlength; i++) {
		editpath[i] = inversepath[*editlength - i - 1];
	}// dijkstra�� ��� ������ editpath�� �Է� �Ϸ�. editpath�� length�� editlength�� ����. editpath[0] ~ editpath[length-1]

	 //source�� ������ ��ġ�� graph�� important node�� ���� ��� 0->2->3->12��� 0->3->12�� �ٲ���.
	if ((coord[editpath[0]][0] == coord[editpath[1]][0]) && (coord[editpath[0]][1] == coord[editpath[1]][1])) {
		for (i = 1; i < *editlength; i++) {
			editpath[i] = editpath[i + 1];
		}
		best[12]--;
		*editlength--;
	}
	//destination���� ������ ��ġ�� graph�� important node�� ���� ���. 0->2->3->4->12��� 0->2->3->12�� �ٲ���.(12�� 4�� ��ġ�϶�)
	if ((coord[12][0] == coord[prevnode[12]][0]) && (coord[12][1] == coord[prevnode[12]][1])) {
		editpath[*editlength - 2] = editpath[*editlength - 1];
		editpath[*editlength - 1] = -1;
		*editlength--;
		best[12]--;
	}

	for (i = 0; i < *editlength; i++) { // print out
		if (i == *editlength - 1)
			printf("%d\n", editpath[i]);
		else printf("%d->", editpath[i]);
	}
//	printf("dijkstra is fine\n");
	sendUturn = checkUturn(path, altpath);
	addInstructions(editpath, altchk, sendUturn); // altchk�� ���� instructionset or altinstructionset�� insertQ ����
	sendUturn = 0;
}

struct car_info FindNearestDijkstra(int carselected, int limit, char serial) {
	struct car_info data;
	while (carselected == 0) {
		if (limit * 10 > best[12]) {
			data.serial = serial;
			data.cost = best[12];
		}
		else
			data.serial = 'Z';
	}
	return data;
}



bool checkUturn(int path[], int altpath[]) {
	if (path[1] != altpath[1])
		return true;
	else
		return false;
}