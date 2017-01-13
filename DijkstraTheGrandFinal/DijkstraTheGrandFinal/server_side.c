#pragma once
#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<stdlib.h>
#include"Freerider.h"
#define NUMOFCARS 3


struct car_info carlist[50];
bool carselected;
char mincar = 'N'; // no car
int mincarcost = 9999;

void main() {
	int q;
	int d = 1;
	int r;
	carlist[0].serial = 'R';
	carlist[1].serial = 'B';

	int user[2] = { 28,13 };
	//server should send user location and 'd'
	//server should store info that cars sent in 'carlist' array
	while (1) {// loop while we find a updated mincarcost
		for (r = 0; r< NUMOFCARS; r++) {
			scanf_s("%c", &carlist[r].serial, sizeof(carlist[r].serial));
//			getchar();
			scanf_s("%d", &carlist[r].cost);
//			getchar();
		}
		for (q = 0; q< NUMOFCARS; q++) { // int+char = 5
			if (carlist[q].serial != 'Z') {
				if (carlist[q].cost == min(carlist[q].cost, mincarcost)) {
					mincar = carlist[q].serial;
					mincarcost = carlist[q].cost;
				}
			}
		}
		if (mincarcost != 9999) {
			break;
		}
		printf("no cars within %d cost....\n looking for cars within %d cost \n", d, d + 1);
		d++; // if no car within d*10 cost, increase 'd' by 1

			 //there should be a time interval where we wait for inputs from cars;
		if (d > 10) { // if there is no car within 10*10 cost, there is no available car for the user
			printf("no available cars found within user area\n");
			break;
		}

	}

	if (mincarcost != 9999) {
		printf("avaiable car is %c \n The car will arrive in %d", mincar, mincarcost);
	}
//	getchar();



}


//struct car_info FindNearestDijkstra(int carselected, int limit, char serial) 