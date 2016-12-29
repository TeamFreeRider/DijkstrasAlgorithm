#include "Freerider.h"




char main()
{

	int nextnode[1][2];
	int carloc[2] = { 0,0 }, interruptloc[2] = { 0,0 };
	int location[1][2];
	char receivedinstruction;


	while (!(carloc[0] = coord[12][0] && carloc[1] == coord[12][1])) {
		scanf_s("%d %d", &coord[0][0], &coord[0][1]); // start
		scanf_s("%d %d", &coord[12][0], &coord[12][1]);   //end 
		dijkstra();
		receivedinstruction = Sendinstruction(carloc, interruptloc, path, instructionset);
	}

	printf("send Instruction finished\n");
	getchar();
	getchar();
}
