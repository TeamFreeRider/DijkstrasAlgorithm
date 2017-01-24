#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <math.h>
#include "FreeRider.h"

#define  BUFF_SIZE   1024

double distance(int, int, int, int);

int CharToInt(char chunk[], int index, int length)
{
	int result = 0; int i;
	char arr[2];
	for (i = 0; i < length; i++)
	{
		arr[i] = chunk[i + index];
	}
	result = atoi(arr);
	return result;//ascii ��ȯ �ڵ�� �ݿ�
}

int main(int argc, char **argv)
{
	int userloc[2], dest[2], weight, carloc[2], interruptloc[2], onboard;//���ͽ�Ʈ�� �ʿ��� ���� �������� �����Ѵ�.
	int calculated_best = -1;//�⺻���� -1�̰� �� ������ ���̽�Ʈ�󿡼� �Ÿ������ ������� �����ϴ� �����̴�.

	char orderstatus = 'x';//Ŭ���̾�Ʈ �ڽ��� ���� ���¸� ��Ÿ���ִ� ������. �ڵ� �� �ؿ� ���º��� ������ �����Ǿ� �ִ�.
						   /* orderstatus�� ���� ����
						   x: �ڵ����� �������϶�
						   y: cost ��� �� �����͸� �ǳ��ް� select�� ����ϴ� ����
						   z: ���� ���� ����ڷκ��� ���õǾ� �ְų� ����ǰ� �ִ� ���¿��� ��� ���°� �ƴ� ����s
						   */
	const char serial = 'B';//�ڵ����� �ø��� �ѹ�
	char selectedserial = 'x';//�����κ��� ���õ� �ڵ����� �ø��� �ѹ��� �����ϴ� ����. ���� x �̸� ���� �ƹ��� ���� ���޹��� ���� ���̴�.
	char Activate_arduino = 'F';//�Ƶ��̳� ���� ���θ� Ȯ�����ִ� boolean ����. ���̸� T�̴�
	char bool_UserReceived = 'F';//�����κ��� ���������� ������ ������ �޾Ҵ°� ���θ� Ȯ�����ִ� ����, ������ F, ���� T�̴�.

								 //char Activate_wait = 'F';//
	char Data_Received[100];//�����κ��� ���۹��� �����͸� �����ϴ� ����  
	char receivedinstruction;

	while (1) {

		int client_socket;

		struct sockaddr_in   server_addr;
		char buff[BUFF_SIZE + 5];
		char Data_send[100];//�������� ���� ������� �����ϴ� ����
							//Data_send[0] = �ڵ��� ������ �ø��� �ѹ�, Data_send[1] = �ڵ����� ���� ����(x,y,z), Data_send[2] = �Ÿ��� ��ȯ�� ���� �ڸ�, Data_send[3] = �Ÿ��� ��ȯ�� ���� �ڸ�

		client_socket = socket(PF_INET, SOCK_STREAM, 0);

		if (-1 == client_socket)
		{
			printf("socket ���� ����\n");
			exit(1);
		}
		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(4000);
		server_addr.sin_addr.s_addr = inet_addr("192.168.0.16");//write the server ip address
		if (-1 == connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
		{
			printf("���� ����\n");
			exit(1);
		}

		char arr_t[20]; int k;
		if (calculated_best == -1)
		{
			//         printf("���ͽ�Ʈ�� �˰����� ������ ���� �ȵ� \n"); //�̰� �Ž����� �ϴ� �ּ�ó��
		}
		else
		{
			sprintf(arr_t, "%d", calculated_best);
			for (k = 0; k < strlen(arr_t); k++)
			{
				Data_send[k + 2] = arr_t[k];
			}
		}
		Data_send[0] = orderstatus;
		Data_send[1] = serial;
		//write( client_socket, argv[1], strlen( argv[1])+1);      // +1: NULL���� �����ؼ� ����
		//write( client_socket, "R", strlen("R") + 1);//sending its car serial number car1 : 1, car2 : 2
		write(client_socket, Data_send, strlen(Data_send) + 1);
		read(client_socket, buff, BUFF_SIZE);//���������� buff���� �����κ��� �����͸� ������... �̻ڰ� �����ϱ� ���ؼ�
											 //Data_received�������ٰ� �����Ѵ�.

		int i; int b = 0;
		for (i = 0; i < strlen(buff); i++)//�̰Ŵ� �������� Ŭ���̾�Ʈ�� �ö� ���ʿ��� �����Ⱚ���� �پ ���µ� ���ʿ��� �����͸� �����ϱ� ���� �ڵ��̴�.
		{
			if (buff[i] == 'E' || b == 1)//�迭���� �����µ� E�� ������ �����Ͱ� �����ٴ� ���� �ǹ��Ѵ�.
			{
				Data_Received[i] = '\0';
				b = 1;
			}
			else
			{
				Data_Received[i] = buff[i];
			}
		}

		switch (Data_Received[0])
		{
		case 'P'://�迭���� �����µ� P�� ������ x state // weight(d)�� & user's location ���� --> ó���� user�� ��û���� �� �� ������ �Ѹ��� ���°���
			if (orderstatus == 'x') {   //x�����϶��� �����ϵ���
				orderstatus = 'y';
				bool_UserReceived = 'T';
				weight = CharToInt(Data_Received, 1, 2);
				userloc[0] = CharToInt(Data_Received, 3, 2);
				userloc[1] = CharToInt(Data_Received, 5, 2);
				printf("orderstatus : %c\n", orderstatus);	
			}
			if (orderstatus == 'y' && bool_UserReceived == 'T')
			{
				//���̽�Ʈ�� �����µ� �Ÿ��� ��ȯ�ϴ� ���ͽ�Ʈ�� �ڵ带 ����. (+)
				/*!!carloc �� userloc�� �����ֳ�?*/
				for (int i = 0; i < 2; i++) {
					coord[0][i] = carloc[i];
					coord[12][i] = userloc[i];
				}

				dijkstra(graph, path, &length, 0);
				/*!!����Ǵ� ���̸��� calculated_best���� ��*/
				if (interruption(carloc, interruptloc) == true) {
					makeNewGraph(carloc, graph, path); // graph�� ����. path�� �ٲ���.
					for (int j = 0; j < V; j++) {
						path[j] = altpath[j];
					}
					initQ(V, 0);
					for (int a = althead; a < alttail; a++) {
						insertQ(delQ(1), 0); // -> �۵� Ȯ�οϷ�
					}
					recoveryweight1[0] = altrecoveryweight1[0];
					recoveryweight1[1] = altrecoveryweight1[1];
					recoveryweight1[2] = altrecoveryweight1[2];
					length = altlength;
					altassign = 1;
				}

				calculated_best = best[12];

				//calculated_best = �����
				if (calculated_best == -1) // �� �ڵ� �߰� calculated_best == -1 || ���ͽ�Ʈ���ȯ�� == z �� ���
				{// ���� calculated_best�� -1�� �ִ��� ���� �ʿ�. - �̰� �⺻���̾� �� ���� ���̽�Ʈ�� �������� �ƿ� ���� �ȵ� ��쿡 -1s
					Data_send[2] = 'Z';//�̰� ���º��� �ƴϴϱ� ���ǹٶ�
				}
				else if (calculated_best != -1) //�̰� ��/���� �ڸ� ������ ���� �� ĭ�� �����Կ� by���� 
				{
					if (calculated_best <= weight * 10)
					{ // ���� cost�� ������ range(weight�� ���� ����) �̳��� ���� ��쿡�� Data_send ����
						Data_send[2] = calculated_best + '0';
					}//�Ÿ����� 10�̻��� �������ʴ´ٰ� �����ϰ� § �ڵ��̴ϱ� ���ǹٶ�
					 // range �ۿ� �ִٸ� �ƹ��� �۾������ʰ�, do not send any Data send to server.
				}
			}
			break;
		case 'S':// car's location & interrupt location ���� --> �̵��� ���� ����
			carloc[0] = CharToInt(Data_Received, 1, 2);
			carloc[1] = CharToInt(Data_Received, 3, 2);
			interruptloc[0] = CharToInt(Data_Received, 5, 2);
			interruptloc[1] = CharToInt(Data_Received, 7, 2);
			break;
		case 'D'://�迭���� �����µ� D�� ������ onboard, destination location ����. --> user�� ž���� ��쿡 �ִ°Ͱ���
				 //�����Ҷ��� D�� ù ĭ�� �´�. onboard���� 1�̸� �����ѰŰ� 0�̸� �����Ѱ��� �����̰� ������
			printf("received %c, Data_Received[1] : %c\n", Data_Received[0], Data_Received[1]);
			if (orderstatus == 'z' && Data_Received[1] == '1') {
				onboard = 1;
				dest[0] = CharToInt(Data_Received, 2, 2);
				dest[1] = CharToInt(Data_Received, 4, 2);
				Activate_arduino = 'T';
				printf("%s : user checked in the car\n", Data_Received);
			}
			else if (orderstatus == 'z' && Data_Received[1] == '0') {
				onboard = 0;
				printf("%s : user checked out the car\n", Data_Received);
				Activate_arduino = 'F';
				orderstatus = 'x';
			}
			break;
		case 'C'://C �迭���� broadcasting�Ǵ� selected car�� �ڱ��ڽ��̸� z�� �ٲ��. �ƴϸ� �ٽ� x��
			selectedserial = Data_Received[1];
			//orderstatus = 'z';
			break;
		}


		if (orderstatus == 'y' && selectedserial == serial) //z�����϶� selectedserial�� �޾Ƶ� ��������
		{
			orderstatus = 'z';
			if (Activate_arduino == 'T' && onboard == 0)
			{
				//�Ƶ��̳� ������ �ڵ�
				//ȣ������ ������ġ �������� ���� ���ͽ�Ʈ�� ������. (+)
				/*is carloc and interruptloc inserted constantly?*/
				printf(" my serial %c is selected!\n", serial);
				if (!(carloc[0] == coord[12][0] && carloc[1] == coord[12][1])) {
					receivedinstruction = Sendinstruction(carloc, interruptloc, path);
					printf("\n#received instruction is : %c\n\n", receivedinstruction);
				}
				else if (Activate_arduino == 'T' && onboard == 1)
				{
					//�Ƶ��̳� ������ �ڵ�
					//ȣ������ ������ġ���� ���������� ���� ���̽�Ʈ�� �ڵ� ���� (+)
					start = 0;
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
					//graph ���� �Ϸ�
					dijkstra(graph, path, &length, 0);
					if (!(carloc[0] == coord[12][0] && carloc[1] == coord[12][1])) {
						receivedinstruction = Sendinstruction(carloc, interruptloc, path);
						printf("received instruction is : %c\n\n", receivedinstruction);
					}
					else
						printf("arrived at destination\n");
				}
			}
		}

		else if (orderstatus == 'y' && selectedserial != serial)
		{
			orderstatus = 'x';
			selectedserial = 'x';
			bool_UserReceived = 'F';
			//         selectedserial = Data_Received[1];
			break;
		}

		// �� Dijkstra�� ������ ��ȯ�ϱ� ���� Data_send ���� ��, Data_send�� server�� �۽�
		// �� server�� Data_send�� ���� ��, Serial�� ��� ���鿡�� �Ѱ���. - �̰� ���� ���� �ּ��̾�?? (�̵���) 
		//     printf("%s\n", buff);
		printf("Data_Received : %s, orderstatus : %c\n", Data_Received, orderstatus);
		//     printf("%d \n", userloc[0]); printf("%d \n", userloc[1]); printf("%d \n", dest[0]); printf("%d \n", dest[1]); printf("%d \n", weight);
		//     printf("%d \n", carloc[0]); printf("%d \n", carloc[1]); printf("%d \n", interruptloc[0]); printf("%d \n", interruptloc[1]);
		close(client_socket);

		//�Ʒ��� if������...�� �ִµ� �������� �ڵ����� ������ ���� ��� �������� ��ġ�� ���� �ڵ����� ���õǸ�
		//������ �ǵ��� �ϰų� �ƴϸ� ���ù����� ���ϸ� �ٽ� ��� ���·� ���� �ϴ� ������ �ڵ�� ��Ÿ�� ���̴�.
		//�����ε� �˰��򿡼� ������ ���� ���� ������ ���ǡ� ������

		sleep(1);//1 second    
	}
	return 0;
}