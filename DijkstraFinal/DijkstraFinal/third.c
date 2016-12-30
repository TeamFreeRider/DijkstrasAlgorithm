#include "Freerider.h"





void main() {
	int nextnode[1][2];
	int carloc[2] = { 0,0 }, interruptloc[2] = { 0,0 };
	int userloc[2], dest[2], start[2];
	int location[1][2];
	char receivedinstruction;
	char myserial;
	char selectedserial = '';
	bool available = false;

	myserial = 'R';

	scanf_s("%c", &selectedserial);
	if (selectedserial == myserial) {
		available = true;
	}
	scanf_s("%d %d", &userloc[0], &userloc[1]);
	scanf_s("%d %d", &dest[0], &dest[1]);
	scanf_s("%d %d", &carloc[0], &carloc[1]);
	scanf_s("%d %d", &interruptloc[0], &interruptloc[1]);

	// 처음. 출발지 : 자신의 위치, 도착지 : user's 위치
	// 다음. 출발지 : user's위치, 도착지 : user가 보낸 도착지.

	// 자신의 위치와 interrupt위치는 지속적으로 업뎃한다고 가정하고 코딩

	for (int i = 0; i < 2; i++) {
		coord[0][i] = carloc[i];
		coord[12][i] = userloc[i];
	}
	dijkstra(graph, path);
	if (available == true) {
		while (!(carloc[0] = coord[12][0] && carloc[1] == coord[12][1])) {
			receivedinstruction = Sendinstruction(carloc, interruptloc, path, instructionset);
		}
		for (int i = 0; i < 2; i++) {
			coord[0][i] = userloc[i];
			coord[12][i] = destloc[i];
		}
		dijkstra(graph, path);
		while (!(carloc[0] = coord[12][0] && carloc[1] == coord[12][1])) {
			receivedinstruction = Sendinstruction(carloc, interruptloc, path, instructionset);
		}
	}



	printf("send Instruction finished\n");
	getchar();
	getchar();
}

