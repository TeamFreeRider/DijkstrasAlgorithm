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

	// ó��. ����� : �ڽ��� ��ġ, ������ : user's ��ġ
	// ����. ����� : user's��ġ, ������ : user�� ���� ������.

	// �ڽ��� ��ġ�� interrupt��ġ�� ���������� �����Ѵٰ� �����ϰ� �ڵ�

	for (int i = 0; i < 2; i++) {
		coord[0][i] = carloc[i];
		coord[12][i] = userloc[i];
	}
	dijkstra(graph, path);
	if (available == true) {
		while (!(carloc[0] = coord[12][0] && carloc[1] == coord[12][1])) {
			if (interruption(carloc, interruptloc) == true) { // interruption�� �߻��ϸ� �ϴ� altgraph, altpath�� ����.
				makeNewGraph(true, graph, path);
				dijkstra(altgraph, altpath); // ���� dijkstra���� addinstruction�� �����ϴ� �κ��� �־��µ� �̸� ����� ���� else������ addinstruction�ϵ��� ����.
			}
			temp = instructionset[head]; // ó���� DeQueue���� �ʰ� just ���� �������ִٰ�, ���� ��忡�� �ʿ��� �� delQ�ϵ��� ����.
			//temp = delQ();
			receivedinstruction = Sendinstruction(carloc, interruptloc, path, instructionset);

		}

		///////////////////////////////////user���� ������ /// destination���� �̵���
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

