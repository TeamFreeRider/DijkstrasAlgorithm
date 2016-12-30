#include "Freerider.h"





void main() {
	int nextnode[1][2];
	int carloc[2] = { 0,0 }, interruptloc[2] = { 0,0 };
	int userloc[2], dest[2], start[2];
	int location[1][2];
	char receivedinstruction;
	char myserial;
	char selectedserial;
	bool available = false;
	struct inst temp;
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
			if (interruption(carloc, interruptloc) == true) { // interruption이 발생하면 일단 altgraph, altpath는 형성.
				makeNewGraph(true, graph, path);
				dijkstra(altgraph, altpath); // 기존 dijkstra에서 addinstruction을 시행하는 부분이 있었는데 이를 지우고 밑의 else문에서 addinstruction하도록 변경.
			}
			temp = instructionset[head]; // 처음엔 DeQueue하지 않고 just 값만 가지고있다가, 다음 노드에서 필요할 때 delQ하도록 변경.
			//temp = delQ();
			receivedinstruction = Sendinstruction(carloc, interruptloc, path, instructionset);

		}

		///////////////////////////////////user에게 도착↑ /// destination으로 이동↓
		for (int i = 0; i < 2; i++) {
			coord[0][i] = userloc[i];
			coord[12][i] = dest[i];
		}
		dijkstra(graph, path);
		while (!(carloc[0] = coord[12][0] && carloc[1] == coord[12][1])) {
			receivedinstruction = Sendinstruction(carloc, interruptloc, path, instructionset);
		}
	}
	getchar();
	getchar();
}

