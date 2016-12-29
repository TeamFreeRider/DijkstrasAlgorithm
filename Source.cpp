#include <stdio.h>
#include <string.h>

#define MAX_VERTICES 17
#define Inf 10000 //임의로 큰 값을 정함
#define Zero 0
#define TRUE 1
#define FALSE 0 

int cost[MAX_VERTICES][MAX_VERTICES] = {	
	{0,13,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf},//0
	{13,0,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,7,Inf,Inf,Inf,Inf},//1
	{Inf,Inf,0,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,10,Inf,Inf,Inf,12},//2
	{Inf,Inf,Inf,0,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,10},//3
	{Inf,Inf,Inf,Inf,0,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,12,Inf,Inf},//4
	{Inf,Inf,Inf,Inf,Inf,0,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,4,Inf,Inf},//5
	{Inf,Inf,Inf,Inf,Inf,Inf,0,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,4,Inf},//6
	{Inf,Inf,Inf,Inf,Inf,Inf,Inf,0,2,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf},//7
	{Inf,Inf,Inf,Inf,Inf,Inf,Inf,2,0,8,Inf,Inf,Inf,Inf,Inf,6,Inf},//8
	{Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,8,0,5,Inf,Inf,Inf,Inf,Inf,Inf},//9
	{Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,5,0,6,Inf,Inf,Inf,Inf,Inf},//10
	{Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,6,0,4,Inf,Inf,Inf,Inf},//11
	{Inf,7,10,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,4,0,5,Inf,Inf,Inf},//12
	{Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,5,0,4,Inf,2},//13
	{Inf,Inf,Inf,Inf,12,4,Inf,Inf,Inf,Inf,Inf,Inf,Inf,4,0,7,Inf},//14
	{Inf,Inf,Inf,Inf,Inf,Inf,4, Inf,6,Inf,Inf,Inf,Inf,Inf,7,0,Inf},//15
	{Inf,Inf,12,10,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,Inf,2,Inf,Inf,0},//16
};

int distance[MAX_VERTICES];
short int found[MAX_VERTICES];
char trace[MAX_VERTICES][1];

int choose(int distance[], int n, short int found[]){
	int i, min, minpos; 
	min = Inf; // INF로 초기화
	minpos = -1; // 우리가 사용하지 않는 노드와 겹치지 않기위해 임의로 -1로 초기화
	for(i=0 ; i<n ; i++){
		if(distance[i] < min && !found[i]){ // 발견되지 않은 최소 경로 중에서 최소 거리 값의 인덱스
			min = distance[i];
			minpos = i;
		}
	}
	return minpos;
}
void shortestpath(int v, int cost[][MAX_VERTICES], int distance[], int n, short int found[]){
	// v는 출발 노드, cost[][]는 비용 배열, distance[]는 출발노드 v로 부터의 각 노드의 거리 배열,
	// n은 노드의 개수, found[]는 최단 경로 유무 배열 
	int i, u, w;
	for(i=0 ; i<n ; i++){ // 초기화 시켜준다. 만약 출발노드와 연결되지 않으면 Infinity
		found[i] = FALSE;
		distance[i] = cost[v][i];
	}
	found[v] = TRUE; // 출발 노드와의 거리는 당연히 0이기 때문에 True
	distance[v] = 0; // 당연히 0~
	trace[v][0] = -1; 

	for(i=0 ; i<n-2 ; i++){ // 일단 출발노드는 빼고 노드가 하나 남았을 때는
								// 갱신시켜줄 값이 없기 때문에 n-2까지만..
		u = choose(distance, n, found); // found 되지 않은 노드중에서 거리가 가장 작은
												 // 노드의 인덱스를 가져온다.
		found[u] = TRUE;
	
		if( trace[u][0] == 0){ // 출발지에서 바로 온다고 파악한다면?
			trace[u][0] = v;
		}

		for(w=0 ; w<n ; w++){
			if(!found[w]){ // 찾아지지 않은 것 중에서...
				if(distance[u] + cost[u][w] < distance[w]){ // 만약 기존의 거리 값과 비교해서 
																	  // 작은 값이 나오면 최소 거리 값 갱신
					distance[w] = distance[u] + cost[u][w];
					trace[w][0] = u;
				}
			}
		}
	}
}
void show_route(int v){ 
	int temp;
	printf("%d 로부터 최단경로 찾기\n", v);
	for(int i=0 ; i<MAX_VERTICES ; i++){
		printf("\n%d from to %d = %4d\n", v, i, distance[i]);
		temp = i;
		int reverse[MAX_VERTICES] = {0};
		int cnt = MAX_VERTICES;
		while(temp != -1){
			temp = trace[temp][0];
			if( temp != -1)
				reverse[--cnt]= temp;
		}
		for(int j=0 ; j<MAX_VERTICES ; j++){
			if(reverse[j] != 0)
				printf("%d -> " , reverse[j]);
		}
		printf("%d" , i);
		printf("\n");
	}
}

int main(){
	int node_num = 0;
	printf("Input starting point : ");
	scanf_s("%d", &node_num);

	/*	printf("Input ending point : ");
	scanf_s("%d", &node_num);*/
	
	shortestpath(node_num, cost, distance, MAX_VERTICES, found);
	show_route(node_num);
	return 0;
}