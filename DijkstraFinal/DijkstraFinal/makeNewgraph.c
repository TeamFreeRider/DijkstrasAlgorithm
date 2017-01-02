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

				for (u = 0; u < V; u++) {
					for (v = 0; v < V; v++) {
						altgraph[u][v] = graph[u][v];
					}
				}
				for(u=0;u<V;u++)
				altgraph[interruptedNode][V] = 0;
				return true;
			}
			else if (car_prevorder >= path[i + 1]) { // car �̹� �� interrupt edge�� ���� ���. interrupt�� �־ �Ű�Ⱦ� return false.
				return false;
			}
			else if (car_prevorder == path[i] && car_nextorder == path[i + 1]) { // car�� interrupt edge�� �ִ� ���. path[i+1]���� distance�� ����Ͽ� car�� �� �ָ��ִٸ� ���� interrupt�����������Ƿ� return true.
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
	scanf_s("%d %d", &coord[0][0], &coord[0][1]); // start ������� ������
	scanf_s("%d %d", &coord[12][0], &coord[12][1]);   //end 




	while (!(carloc[0] = coord[12][0] && carloc[1] == coord[12][1])) {

		dijkstra();
		receivedinstruction = Sendinstruction(carloc, interruptloc, path, instructionset);
	}

	printf("send Instruction finished\n");
	getchar();
	getchar();
}