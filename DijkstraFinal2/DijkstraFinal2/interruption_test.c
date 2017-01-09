#include "Freerider.h""

int main() {
	int carloc[2];
	int interruptloc[2];
	int userloc[2];
	int dest[2];

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
	dijkstra(graph, path);
	printf("wtf?\n");
	printf("insert car location:\n");
	scanf_s("%d %d", &carloc[0], &carloc[1]);
	getchar();
	printf("insert interruption location:\n");
	scanf_s("%d %d", &interruptloc[0], &interruptloc[1]);
	getchar();
	printf("is there an interruption? %d \n", interruption(carloc, interruptloc));
	getchar();
}