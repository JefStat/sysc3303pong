#include "Common.h"


/* Thread defines */
#define TASKSTKSIZE 0 /* 0 for default */
#define TASKPRIO 99   /* 99 highest priority */
#define TASKMODE T_JOINABLE

#define POOLSIZE 5 /* 6 bytes, max msgqueue poolsize*/
#define QLIMIT 2 /* 2 messages, num of messages allows in queue. */

RT_TASK player1[BUFFER], player2[BUFFER];
RT_QUEUE queue[BUFFER];

void playerTask(void * cookie)
{
	int clientfd;
	int connected = 0;
	int movement;
	int player;
	int err;
	int i= (int) cookie;

	char msg[10];
	/* Block until a client is sending data */
	while (running && (rt_queue_read(&queue[i], &clientfd, sizeof(clientfd),TM_INFINITE)))
	{
		
		rt_printf("Q%d: %d\n",i,clientfd);

		/* Set what player paddle to move */
		rt_mutex_acquire(&playermtx[i],TM_INFINITE);
		if (clientfd1[i] == -1)
		{
			clientfd1[i] = clientfd;
			player = 0;
		}
		else if (clientfd2[i] == -1)
		{
			clientfd2[i] = clientfd;
			player = 1;
		}
		rt_mutex_release(&playermtx[i]);
		resetPongs(i);

		/* Receive data until client disconnects */
		connected = 1;
		while (connected)
		{
			err = recv(clientfd, msg,10,0);
			movement = atoi(msg);
			if (err < 0) 
			{
				connected = 0;
				rt_mutex_acquire(&playermtx[i],TM_INFINITE);
				if (player == 0)
				{
					clientfd1[i] = -1;
					player = -1;
				}
				else if (player == 1)
				{
					clientfd2[i] = -1;
					player = -1;
				}
				rt_mutex_release(&playermtx[i]);
			}
			
			//err = write(clientfd, "", sizeof(""));
			if (err < 0) 
			{
				connected = 0;
				rt_mutex_acquire(&playermtx[i],TM_INFINITE);
				if (player == 0)
				{
					clientfd1[i] = -1;
					player = -1;
				}
				else if (player == 1)
				{
					clientfd2[i] = -1;
					player = -1;
				}
				rt_mutex_release(&playermtx[i]);
			}
			
			movePong(player,-movement, i);
			
			//rt_printf("M %d ",movement,msg);
		}
	}

	rt_task_delete(NULL);
}

void  gameFun(void * cookie)
{

	struct sockaddr_in client_addr, server_addr;
	
	int err;
	int gameSocket;
	int port = (int) cookie;
	int i= port - 5001;
	char name[10];
	sprintf(name, "q%d",port);
	
	/* Create the new game socket */
	if ((gameSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) { Die("Failed to create socket"); }
	
	/* Construct the games sockaddr_in structure */
	memset(&server_addr, 0, sizeof(server_addr));       /* Clear struct */
	server_addr.sin_family = AF_INET;                  /* Internet/IP */
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
	server_addr.sin_port = htons(port);       /* server port */

	if (bind(gameSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) { Die("Failed to bind the server socket"); }
	if (listen(gameSocket, 2) < 0) { Die("Failed to listen on server socket"); }

	if ((err = rt_queue_create(&queue[i], name,POOLSIZE,QLIMIT,Q_FIFO|Q_SHARED)) != 0) { Die("Failed to create queue"); }

	sprintf(name, "p1%d",port);
	if ((err = rt_task_create(&player1[i],name,TASKSTKSIZE,TASKPRIO,TASKMODE)) != 0){ Die("Creating Player 1"); }
	sprintf(name, "p2%d",port);
	if ((err = rt_task_create(&player2[i],name,TASKSTKSIZE,TASKPRIO,TASKMODE)) != 0){ Die("Creating Player 2"); }
	if ((err = rt_task_start (&player1[i], &playerTask, (void *)i)) != 0) { Die("Starting player 1"); }
	if ((err = rt_task_start (&player2[i], &playerTask, (void *)i)) != 0) { Die("Starting player 2"); }

	/* game mutex creation */
	sprintf(name, "m%d",port);
	err = rt_mutex_create (&playermtx[i], name);
	if (err != 0 ) { Die("Failed to create player mutex"); }


	unsigned int len = sizeof(client_addr);
	int clientfd;
	GameState(i);
	int flag;
	/* game runs forever*/
	while (running && (0 < (clientfd = accept(gameSocket, (struct sockaddr *) &client_addr, &len))))
	{
		flag = 1;
		setsockopt(clientfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
		rt_printf("GC: %s\n", inet_ntoa(client_addr.sin_addr));

		if ((err = rt_queue_write(&queue[i], &clientfd, sizeof(clientfd1),Q_NORMAL)) < 0) { Die("rt_queue_write"); }
		startgame(i);

	}
	rt_task_delete(NULL);
}

RT_TASK gameThread[BUFFER];

/* May need a cleanup delete ect on the threads */
int main(int argc, char* argv[])
{
	signal(SIGTERM, catch_signal);
	signal(SIGINT, catch_signal);
	/* Avoids memory swapping for this program */
	mlockall(MCL_CURRENT|MCL_FUTURE);

	rt_print_auto_init(1);

	struct sockaddr_in client_addr, server_addr;
	
	int err;
	int serverSocket;
	int i = 1;
	char name[10];

	/* Create the GameServer socket */
	if ((serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) { Die("Failed to create socket"); }

	/* Construct the server sockaddr_in structure */
	memset(&server_addr, 0, sizeof(server_addr));       /* Clear struct */
	server_addr.sin_family = AF_INET;                  /* Internet/IP */
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
	server_addr.sin_port = htons(5000);       /* server port */

	if (bind(serverSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) { Die("Failed to bind the server socket"); }
	if (listen(serverSocket, 10) < 0) { Die("Failed to listen on server socket"); }

	unsigned int len = sizeof(client_addr);
	int clientfd;
	int numAccepted = 0;
	int data;

	/* server runs forever*/
	while (running && (0 < (clientfd = accept(serverSocket, (struct sockaddr *) &client_addr, &len))))
	{
		rt_printf("SC: %s\n", inet_ntoa(client_addr.sin_addr));
		sprintf(name, "NG%d%d", i,numAccepted);

		numAccepted++;
		data = 5000 + i;
		
		if (numAccepted % 2 == 0 )
		{
 			sprintf(name, "%d", data);
			/* Send game client 2 his new port to connect to */
			send(clientfd, name, strlen(name),0);
			numAccepted = 0;
			i++;
			close(clientfd);
		}
		else
		{
			/* spawn game Thread */
			err = rt_task_create(&gameThread[i-1], name, TASKSTKSIZE , TASKPRIO , TASKMODE );
			if (0 != err) { Die(name); }
		
			rt_task_start (&gameThread[i-1], &gameFun, (void *)data);
			if (0 != err) { Die(name); }
			/* Send game client 1 his new port to connect to */
 			sprintf(name, "%d", data);
			send(clientfd, name, strlen(name),0);
			close(clientfd);
		}

	}
	exit(EXIT_SUCCESS);
}

