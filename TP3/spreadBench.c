#include <sp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define GROUP_NAME "vsoa_tp3"
#define MAX_MESSAGE_LENGTH 10000

char SPREAD_NAME[80] = "4803@";

int main( int argc, char *argv[] )
{
	if(argc < 5 || argc > 6)
	{
		printf("Incorrect number of arguments.\nExpected execution: spreadBench <NODE ID> <MAX NODES> <MSG COUNT> <MSG LEN> [<DAEMON IP>]\n");
		return 0;
	}
	
	if(argc==5) strcat(SPREAD_NAME, "localhost");
	else strcat(SPREAD_NAME, argv[5]);
	
	clcok_t tiempo_inicio, tiempo_final;
	double segundos,bytesMen=0.0,throughput=0.0;
	int membersExpected = atoi(argv[2]);
	int membersConnected = 0;
	int numGroups;
	int mismatch;
	int messagesReceived;
	int nodeId = atoi(argv[1]);
	int cantMessages = atoi(argv[3]);
	int messageLength = atoi(argv[4]);
	int spResult;
	char privateGroupName[MAX_GROUP_NAME];
	char sender[MAX_GROUP_NAME];
	char groups[10][MAX_GROUP_NAME];
	char auxStr[2];
	char  message[MAX_MESSAGE_LENGTH];
	char receivedMessage[MAX_MESSAGE_LENGTH]; 
	mailbox mbox;
	service service_type;
	int16 mess_type;
	bool everyoneIn = 0;

	if(messageLength >= MAX_MESSAGE_LENGTH)
	{
		printf("ERROR! Message length must be less than %d\n", MAX_MESSAGE_LENGTH);
		return 0;
	}

	if (SP_connect(SPREAD_NAME, NULL, 0, 1, &mbox, privateGroupName) != ACCEPT_SESSION)
	{
		printf("ERROR! connection not accepted\n");
		return 0; 
	}

	if (SP_join (mbox,GROUP_NAME) != 0)
	{
		printf("ERROR while trying to join the group %s\n", GROUP_NAME);
		return 0; 
	}

	while(!everyoneIn)
	{
		printf("Waiting for message...\n");
		spResult = SP_receive(mbox, &service_type, sender, 10, &numGroups, groups, &mess_type, &mismatch, sizeof(receivedMessage), receivedMessage);
		if(spResult<0)
		{
	  		printf("ERROR while receiving a message: %d\n", spResult);
	  		SP_leave (mbox,GROUP_NAME);
			return 0;
		}
		printf("Message received: ");
		if(Is_caused_join_mess(service_type)!=0)
		{
			printf("join\n");
			membersConnected++;
			if(membersConnected == membersExpected && !everyoneIn)
			{
				strcpy(message, "EVERYONE_IN");
				printf("Sending %s multicast\n", message);
				if(SP_multicast(mbox, RELIABLE_MESS, GROUP_NAME, 0, strlen(message), message) < 0)
				{
					printf("ERROR while sending message: %s\n", message);
					SP_leave (mbox,GROUP_NAME);
					return 0;
				}
			}
		}
		else if(strcmp(receivedMessage, "EVERYONE_IN") == 0)
		{
			printf("%s\n", receivedMessage);
			everyoneIn = 1;
		}
		else if(Is_caused_leave_mess(service_type)!=0)
		{
			printf("ERROR. Leave message received, aborting...\n");
			SP_leave (mbox,GROUP_NAME);
			return 0;
		}
		else
		{
			printf("\nERROR. Unexpected message received: %s\n", receivedMessage);
			SP_leave (mbox,GROUP_NAME);
			return 0;
		}
	}
	
	tiempo_inicio = clock();	

	for (int i = 0; i <  cantMessages; ++i)
	{
		strcpy(message, "");
		auxStr[1] = '\0';
		message[messageLength] = '\0';
		receivedMessage[messageLength] = '\0';
		for (int j = 0; j < messageLength; ++j)
		{
			auxStr[0] = (char)(33+i%(126-33));
			strcat(message,auxStr);
		}


		printf("Sending message: %s\n", message);
		if (SP_multicast(mbox, RELIABLE_MESS, GROUP_NAME, 0, strlen(message), message) < 0)
		{
			printf("ERROR while sending message\n");
			SP_leave (mbox,GROUP_NAME);
			return 0; 
		}

		messagesReceived = 0;	

		while(messagesReceived < membersExpected)
		{
			printf("Waiting for message...\n");
			strcpy(receivedMessage, "");
			if (SP_receive(mbox, &service_type, sender, 10, &numGroups, groups, &mess_type, &mismatch, sizeof(receivedMessage), receivedMessage) < 0)
			{
			  	printf("ERROR while receiving message\n");
			  	SP_leave (mbox,GROUP_NAME);
				return 0;
			}
			if(Is_caused_leave_mess(service_type)!=0)
			{
				printf("ERROR. Leave message received, aborting...\n");
				SP_leave (mbox,GROUP_NAME);
				return 0;
			}
			if(strcmp(message, receivedMessage) != 0)
			{
				printf("ERROR. Expected %s and received %s, aborting...\n", message, receivedMessage);
				SP_leave(mbox,GROUP_NAME);
				return 0;
			}
			printf("Message received: %s (sender = %s)\n", receivedMessage, sender);
			messagesReceived++;
		}
	}
	tiempo_final = clock();
	bytesMen = cantMessages * messageLength * membersExpected;
	segundos = (double) (tiempo_inicio - tiempo_final) / CLOCKS_PER_SEC;
	throughput = (double) byteMen/segundos;

	printf("Time: %i\n", segundos);
	printf("Throughput: %d\n", throughput);
	
	SP_leave(mbox,GROUP_NAME);
	
	return 0;
}
