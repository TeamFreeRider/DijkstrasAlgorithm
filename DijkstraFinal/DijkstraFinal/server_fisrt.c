#pragma once
#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<conio.h>
#include<stdlib.h>
#include"Freerider.h"

#define V 13 // number of vertex


/*struct car_info {
char serial;
int pos_x;
int pos_y;
};*/

numofcar = 2;

void main() {
	cararray[0].serial = 'R';
	cararray[0].pos_x = 14;
	cararray[0].pos_y = 2;

	cararray[1].serial = 'B';
	cararray[1].pos_x = 42;
	cararray[1].pos_y = 24;

	int user[2] = { 28,13 };

	char minserial = FindNearestDijkstra(cararray, user);
	printf("the nearest car is : %c ", minserial);
	getchar();




}