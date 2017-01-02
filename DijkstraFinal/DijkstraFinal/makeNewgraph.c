#include "Freerider.h"

int altpath[V];
int altgraph[V][V];
int interruptedNode;
int u, v, j;
int graph[V][V] =
{ //  s  0  1  2  3  4  5  6  7  8  9 10, e
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0 }, //s
	{ 0, 0, 5, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 }, // 1
	{ 0, 5, 0, 4, 0, 0, 5, 0, 0, 0, 0, 0, 0 }, // 2
	{ 0, 0, 4, 0, 7, 0, 0,12, 0, 0, 0, 0, 0 }, // 3
	{ 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,10, 0 }, // 4
	{ 0, 2, 0, 0, 0, 0,10, 0, 7, 0, 0, 0, 0 }, // 5
	{ 0, 0, 5, 0, 0,10, 0, 5, 0,10, 0, 0, 0 }, //6
	{ 0, 0, 0,12, 0, 0, 5, 0, 0, 0, 3, 0, 0 }, //7
	{ 0, 0, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0 }, // 8
	{ 0, 0, 0, 0, 0, 0,10, 0, 7, 0, 9, 0, 0 }, // 9
	{ 0, 0, 0, 0, 0, 0, 0, 3, 0, 9, 0, 4, 0 }, // 10
	{ 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 4, 0, 0 }, // 11
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // e 
};


bool interruption(int carloc[], int interruptloc[]) {
	struct inst temp;

	int car_next; // 차가 다음에 지나야 할 노드
	int car_prev; // 차가 직전에 지난 노드
	int car_nextorder, car_prevorder; // next or prev 노드의 path array에서의 순서

	int inter_next1, inter_next2; // interruption 인접 노드 2개. 순서상관 x
	int i = 0;
	temp = instructionset[head]; // 다음에 지나야 할 노드의 정보를 담고있는 instructionset
	for (car_next = 0; car_next < V; car_next++) {
		if ((temp.pos_x == coord[car_next][0]) && temp.pos_y == coord[car_next][1]) { // car_next를 정하는 for 문
			break;
		}
	}
	car_prev = prevnode[car_next]; // 당연히 carnext의 이전 노드

	inter_next1 = closenode(interruptloc[0], interruptloc[1]); // interrupt와 가까운 노드중 하나. closenode를 통해 계산
	for (inter_next2 = 0; inter_next2 < V; inter_next2++) { // interrupt 인접 노 다른 하나를 구함. graph matrix를 통해 인접노드 중 0이아닌 노드에 대해서 검사.
		if (!graph[inter_next1][inter_next2]) {
			if (coord[inter_next2][1] == coord[inter_next1][1]) {//두 노드가 y좌표가 같은 경우
				if ((coord[inter_next2][0] - interruptloc[0])*(coord[inter_next1][0] - interruptloc[0]) < 0) { //두 노드가x축에서 interruptloc[]을 감싸고 있다면 break;
					break;
				}
			}
			else if (coord[inter_next2][0] == coord[inter_next1][0]) { // 두 노드가 x좌표가 같은 경우
				if ((coord[inter_next2][1] - interruptloc[1])*(coord[inter_next1][1] - interruptloc[1]) < 0) { // 두 노드가 y 축에서 interruptloc을 감싸고 있다면 break/
					break;
				}
			}
		}
	}
	for (i = 0; i < length; i++) {// car nextorder 계산.
		if (path[i] == car_next) {
			break;
		}
	}
	car_nextorder = i;
	car_prevorder = car_nextorder - 1; // 당연

	for (i = 0; i < length - 1; i++) {
		if ((path[i] == inter_next1 && path[i + 1] == inter_next2) || (path[i] == inter_next2 && path[i + 1] == inter_next1)) { // 경로 내에 interrupt가 있는 경우. 이 때 path[i]는 interrupt인접노드중 앞쪽, path[i+1]은 뒷
			if (car_nextorder <= path[i]) { // car이 아직 그 interrupt가 있는 edge를 지나지 않은 경우. return true.
				interruptedNode = i;

				for (u = 0; u < V; u++) {
					for (v = 0; v < V; v++) {
						altgraph[u][v] = graph[u][v];
					}
				}
				for(u=0;u<V;u++)
				altgraph[interruptedNode][V] = 0;
				return true;
			}
			else if (car_prevorder >= path[i + 1]) { // car 이미 그 interrupt edge를 지난 경우. interrupt가 있어도 신경안씀 return false.
				return false;
			}
			else if (car_prevorder == path[i] && car_nextorder == path[i + 1]) { // car이 interrupt edge에 있는 경우. path[i+1]과의 distance를 계산하여 car이 더 멀리있다면 아직 interrupt를안지낫으므로 return true.
				if (distance(coord[path[i + 1]][0], coord[path[i + 1]][1], carloc[0], carloc[1]) > distance(coord[path[i + 1]][0], coord[path[i + 1]][1], interruptloc[0], interruptloc[1])) {
					return true;
				}
				else return false;
			}
		}
		else return false;
	}

}


void main() {
	int nextnode[1][2];
	int carloc[2] = { 0,0 }, interruptloc[2] = { 0,0 };
	int location[1][2];
	char receivedinstruction;
	char myserial;
	char selectedserial = '';
	bool available = false;

	myseiral = 'R';

	scanf_s("%c", &selectedserial);
	if (selectedserial == myserial) {

	}
	scanf_s("%d %d", &coord[0][0], &coord[0][1]); // start 출발지와 도착지
	scanf_s("%d %d", &coord[12][0], &coord[12][1]);   //end 




	while (!(carloc[0] = coord[12][0] && carloc[1] == coord[12][1])) {

		dijkstra();
		receivedinstruction = Sendinstruction(carloc, interruptloc, path, instructionset);
	}

	printf("send Instruction finished\n");
	getchar();
	getchar();
}