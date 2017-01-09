#include "Freerider.h"


void main() {
	//int nextnode[1][2], location[1][2];
	int carloc[2] = { 0,0 }, interruptloc[2] = { 0,0 }, userloc[2], dest[2];
	int d, carselected = 0;
	double sendcost;
	char receivedinstruction, myserial, selectedserial, sendserial;

	bool onboard = false;
	bool available = true;


	myserial = 'R';

	printf("insert user location:\n");
	scanf_s("%d %d", &userloc[0], &userloc[1]);
	getchar();
	printf("insert user destination:\n");
	scanf_s("%d %d", &dest[0], &dest[1]);
	getchar();
	printf("insert car location:\n");
	scanf_s("%d %d", &carloc[0], &carloc[1]);
	getchar();
	printf("insert interruption location:\n");
	scanf_s("%d %d", &interruptloc[0], &interruptloc[1]);
	getchar();

	for (int i = 0; i < 2; i++) {
		coord[0][i] = carloc[i];
		coord[12][i] = userloc[i];
	}
	dijkstra(graph, path, &length, 0);

	if (interruption(carloc, interruptloc) == true) {
		makeNewGraph(carloc, graph, path); // graph는 동일. path만 바뀌면됨.
		for (int j = 0; j < V; j++) {
			path[j] = altpath[j];
		}
		initQ(V, 0);
		for (int a = althead; a < alttail; a++) {
			insertQ(delQ(1), 0); // -> 작동 확인완료
		}
		recoveryweight1[0] = altrecoveryweight1[0];
		recoveryweight1[1] = altrecoveryweight1[1];
		recoveryweight1[2] = altrecoveryweight1[2];
		length = altlength;
		altassign = 1;
	}
	// Queue 초기화 후, althead~alttail까지 
	// delQ(1) : altQueue에서 꺼내 insertQ(,0) : Queue에 insert

	while ((carselected == 0) && (available == true)) {// and if the car is available..
		printf("get server cost limit\n");
		scanf_s("%d", &carselected);
		getchar();
		scanf_s("%d", &d);
		getchar();
		if (best[12] < d * 10) {
			sendserial = myserial;
			sendcost = best[12];
		}
		else {
			sendserial = 'Z';
			sendcost = 9999;
		}
	}


	printf("what is the selected car?");
	scanf_s("%c", &selectedserial, sizeof(selectedserial));
	getchar();
	// First : source = carloc,  destination : userloc
	//Second : source = userloc, destination : dest

	// assume carloc & interruptloc is updated continuously
	if (selectedserial == myserial) { // if the car is selected;
		available = false;
		printf(" my serial %c is selected!\n", myserial);
		while (!(carloc[0] == coord[12][0] && carloc[1] == coord[12][1])) {
			printf("insert car location:\n");
			scanf_s("%d %d", &carloc[0], &carloc[1]);
			getchar();
			printf("insert interruption location:\n");
			scanf_s("%d %d", &interruptloc[0], &interruptloc[1]);
			getchar();
			receivedinstruction = Sendinstruction(carloc, interruptloc, path);
			printf("\n received instruction is : %c\n\n", receivedinstruction);
		}

		///////////////////////////////////user에게 도착↑ /// destination으로 이동↓
		printf("Is user onboard?(0 if no, 1 if yes)\n");
		scanf_s("%d", &onboard);
		getchar();
		start = 0;
		if (onboard == true) {
			printf("user is onboard\n");
			//initial setting
			for (int i = 0; i < 2; i++) {
				coord[0][i] = userloc[i];
				coord[12][i] = dest[i];
			}
			for (int i = 0; i < V; i++) {
				graph[0][i] = 0;
				graph[i][0] = 0;
				graph[i][12] = 0;
				graph[12][i] = 0;
			}
			graph[recoveryweight1[0]][recoveryweight1[1]] = recoveryweight1[2];
			graph[recoveryweight1[1]][recoveryweight1[0]] = recoveryweight1[2];
			graph[recoveryweight2[0]][recoveryweight2[1]] = recoveryweight2[2];
			graph[recoveryweight2[1]][recoveryweight2[0]] = recoveryweight2[2];
			//graph 복구 완료
			dijkstra(graph, path, &length, 0);
			while (!(carloc[0] == coord[12][0] && carloc[1] == coord[12][1])) {
				printf("insert car location:\n");
				scanf_s("%d %d", &carloc[0], &carloc[1]);
				getchar();
				printf("insert interruption location:\n");
				scanf_s("%d %d", &interruptloc[0], &interruptloc[1]);
				getchar();
				receivedinstruction = Sendinstruction(carloc, interruptloc, path);
				printf("received instruction is : %c\n\n", receivedinstruction);
			}
			printf("arrived at destination\n");
			available = true;
		}
		else { //case of onboard == false;
			available = true;
			printf("client has cancelled it's reservation\n");
		}
	}
	getchar();
	getchar();
}