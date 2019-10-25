#include <sp.h>

const char SPREAD_NAME[4] = "4803";


int main( int argc, char *argv[] )
{
	if(argc < 5)
	{
		printf("Incorrect number of arguments.\n
				Expected execution: spreadBench <NODE ID>
				<MAX NODES> <MSG COUNT> <MSG LEN>\n");
	}
	int membersExpected = atoi(argv[2]), membersConnected = 0, *numGroups, *mismatch, messagesReceived;
	int nodeId = atoi(argv[1]), cantMessages = atoi(argv[3]), messageLength = atoi(argv[4]);
	char *GROUP_NAME, *message, *receivedMessage, sender[128], groups[1][128];
	mailbox *mbox;
	service *service_type;
	int16 *mess_type;

	if(messageLength < strlen("EVERYONE_IN")) messageLength = strlen("EVERYONE_IN");

	if (SP_connect(SPREAD_NAME, NULL, 1, 1, mbox, GROUP_NAME) != ACCEPT_SESSION)
	{
		printf("ERROR! connection not accepted\n");
		return 0; 
	}

	if (SP_join (mbox,GROUP_NAME) != 0)
	{
		printf("ERROR while trying to join the group %s\n", GROUP_NAME);
		return 0; 
	}

	while(members < membersExpected)
	{
		printf("Waiting for message...\n");
		if(SP_receive(mbox, service_type, sender, 1, numGroups, groups, mess_type, mismatch, messageLength,receivedMessage)<0)
		{
	  		printf("ERROR while receiving a message\n");
	  		SP_leave (mbox,GROUP_NAME);
			return 0;
		}
		printf("Message received: ");
		if(Is_caused_join_mess(service_type))
		{
			printf("join\n");
			members++;
			if(members == membersExpected)
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
		else if(Is_regular_mess(service_type) && strcmp(receivedMessage, "EVERYONE_IN") == 0)
		{
			printf("%s\n", receivedMessage);
			members = membersExpected;
		}
		else if(Is_caused_leave_mess(service_type))
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

	for (int i = 0; i <  cantMessages; ++i)
	{
		strcpy(message, "");	
		for (int j = 0; j < messageLength; ++j)
		{
			strcat(message,(char)((33+i)%(126-33)));
		}

		printf("Sending message: %s\n", message);
		if (SP_multicast(mbox, RELIABLE_MESS, GROUP_NAME, 0, strlen(message), message) < 0)
		{
			printf("ERROR while sending message\n");
			SP_leave (mbox,GROUP_NAME);
			return 0; 
		}

		messagesReceived = 0;	

		while(cont < argv[2])
		{
			printf("Waiting for message...\n");
			if (SP_receive(mbox, service_type, sender, 1, numGroups, groups, mess_type, mismatch, messageLength, receivedMessage) < 0)
			{
			  	printf("ERROR while receiving message\n");
			  	SP_leave (mbox,GROUP_NAME);
				return 0;
			}
			if(Is_caused_leave_mess(service_type))
			{
				printf("ERROR. Leave message received, aborting...\n");
				SP_leave (mbox,GROUP_NAME);
				return 0;
			}
			printf("Message received: %s\n", receivedMessage);
			messagesReceived++;
		}
	}
	
	return 0;
}
