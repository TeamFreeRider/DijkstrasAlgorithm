#pragma once
#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<conio.h>
#include<stdlib.h>


typedef enum { false, true } bool;
#define INT_MAX 2147483647
#define V 13 // number of vertex
#define ROTATIONDISTANCE 5


extern int path[V], inversepath[V], prevnode[V];
extern double best[V];
extern int length;
struct car_info {
	int serial;
	int pos_x;
	int pos_y;
};
extern struct car_info cararray[50];

extern int coord[V][2];
// node number 설정 total 11개
// 7    8    9    10
// 4    5    6   
// 0    1    2    3

extern int graph[V][V];

int closenode(int x, int y);
double distance(int x1, int y1, int x2, int y2);
int mindistance(double best[], int sptSet[]);

struct inst {
	char instruction;
	int pos_x;
	int pos_y;
};

extern int size, head, tail;
extern struct inst* instructionset;

void initQ(int _size);
void insertQ(struct inst data);

struct inst delQ();


void addInstructions(int path[]);

bool interruption(int carloc[], int interruptloc[]);

char Sendinstruction(int location[], int location2[], int path[], struct inst* instruction);

void dijkstra();

extern struct car_info cararray[50];
extern int numofcar;// 임의지정

char FindNearestDijkstra(struct car_info cararray[], int user[]);
