#ifndef COMMON_H_
#define COMMON_H_

#endif /*COMMON_H_*/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <native/task.h>
#include <native/mutex.h>
#include <native/timer.h>
#include <native/queue.h>
#include <native/types.h>
#include <time.h>
#include <fcntl.h>
#include <stdbool.h>
#include <math.h>
#include <rtdk.h>

#define PIXELADJUSTMENT 0

#define BUFFER 50

#define FILENAME "log.txt"


/* server on off variable 1 for on. */
int running = 1;
// int filelist[BUFFER];
// int filelisthead = 0;

int clientfd1[BUFFER]; 
int clientfd2[BUFFER];

/**
 * Mutex to control gamestart and number of player 
 * these are critical variables 
 */
RT_MUTEX playermtx[BUFFER];

/* number of player threads running */
//int playersconnected = 0;
/* game starts off set to 1 to start but only modify by calling methods. */
// int gamestart = 0;

void Die(char *mess) { perror(mess); exit(EXIT_FAILURE); }

// void timestamp(FILE *fp,char * msg)
// {
// 	char name[30];
// 	struct tm tim;
// 	time_t now;
// 	now = time(NULL);
// 	tim = *(localtime(&now));
// 	strftime(name,30, "%r",&tim);
// 	fprintf(fp,"%s %s",msg, name );
// }

// void onefile()
// {
// 	int i;
// 	char line[BUFFER];
// 	char *socknum = malloc(sizeof(char*));
// 	if (socknum == NULL) { Die("failed to malloc log filename"); }
// 
// 	FILE* fp = fopen("ENDLOG.txt", "w");
// 	if (fp == NULL) { Die("failed to create or open ENDLOG"); }
// 
// 	FILE *fr;
// 	rt_mutex_acquire(&playermtx,TM_INFINITE);
// 	while (playersconnected != 0)
// 	{
// 		rt_mutex_release(&playermtx);
// 		rt_task_wait_period(NULL);
// 		rt_mutex_acquire(&playermtx, TM_INFINITE);
// 	}
// 	rt_mutex_release(&playermtx);
// 
// 	for(i = 0; i< filelisthead; i++)
// 	{
// 		sprintf(socknum, "%d",filelist[i]);
// 		fr = fopen(strcat(socknum, FILENAME), "r");
// 		if (fp == NULL) { Die("failed to open an socket log file"); }
// 		while(fgets(line, BUFFER, fr) != NULL)
// 		{
// 			fprintf(fp,"%s", line);
// 		}
// 		fclose(fr);
// 	}
// 	fclose(fp);
// }


//GameState.c Code

#include "GameState1.h"

void catch_signal(int sig)
{
	//onefile();
	rt_mutex_delete(playermtx);
// 	goalscored = 1;
	running = 0;
	Die("Signal sent to close");
}

//GameState Methods-100
void GameState(int i)
{
	topScore[i] = 0;
	bottomScore[i] = 0;
	clientfd1[i] = -1;
	clientfd2[i] = -1;
}

//BallWidget methods

/*
* Gets the point of the ball with its spped for the current game.
*/
void BallWidget(int i)
{
	char msg[25];
	ball[i].center.x = HEIGHT/2;
	ball[i].center.y = WIDTH/2;
	ball[i].radius = PONG_DEFAULT_RADIUS;
	char name[4];
	sprintf(name,"B%d",i);
	sprintf(msg,"B %d %d\n", (int)ball[i].center.x, (int)ball[i].center.y);
	

	rt_mutex_acquire(&playermtx[i],TM_INFINITE);
	if (clientfd1 > 0) { send(clientfd1[i], msg, sizeof(msg),0); }
	rt_mutex_release(&playermtx[i]);

	rt_mutex_acquire(&playermtx[i],TM_INFINITE);
	if (clientfd2 > 0) { send(clientfd2[i], msg, sizeof(msg),0); }
	rt_mutex_release(&playermtx[i]);


 	vector_x[i] = DEFAULT_SPEED_X;
 	vector_y[i] = DEFAULT_SPEED_Y;

// 	goalscored = 0;

 	rt_task_create(&ballMover[i],name, 0, 99, 0);
	
 	rt_task_start(&ballMover[i], &move_ball,(void*) i);
}

/*
* moves the ball to the appropriate place on the board
*/
void move_ball(void* cookie){
	int i= (int) cookie;
	rt_task_set_periodic(NULL, TM_NOW, 10000000);	
	char msg[25];
	while(1){


		ball[i].center.x += vector_x[i];
		ball[i].center.y += vector_y[i];

		hit_vertical_wall(i);
		

		if(IntersectsCircle(pong[i][0],ball[i])){
			bounce_vertical(i);
		}
		else if(IntersectsCircle(pong[i][1],ball[i])){
			bounce_vertical(i);
		}	

		score_goal(i);
		
		sprintf(msg,"B %d %d\n", (int)ball[i].center.x, (int)ball[i].center.y - PIXELADJUSTMENT);
		//rt_printf("%s %d", msg,i);
		rt_mutex_acquire(&playermtx[i],TM_INFINITE);
		if (clientfd1[i] > 0) { send(clientfd1[i], msg, strlen(msg),0); }
		rt_mutex_release(&playermtx[i]);
		
		rt_mutex_acquire(&playermtx[i],TM_INFINITE);
		if (clientfd2[i] > 0) { send(clientfd2[i], msg, strlen(msg),0); }
		rt_mutex_release(&playermtx[i]);

		rt_task_wait_period(NULL);
	}
	
	rt_task_delete(NULL);
}

/*
* Resets the ball to the original position on the board.
*/
void reset_ball(int i){

 	ball[i].center.x = HEIGHT/2;
 	ball[i].center.y = WIDTH/2;

 	
 	vector_x[i] = DEFAULT_SPEED_X;
 	vector_y[i] = DEFAULT_SPEED_Y;
}

/*
* changes the balls direction for the y coordinates
*/
void bounce_vertical(int i){
 	vector_y[i] = -(vector_y[i]);
}

/*(((ball.center.x - PONG_DEFAULT_RADIUS) <= 0) || ((ball.center.x + PONG_DEFAULT_RADIUS) >= WIDTH)
* changes the ball direction for the x coordinates
*/
void bounce_horizontal(int i){
 	vector_x[i] = -(vector_x[i]);
}

/*
* checks if the ball has hit a wall
*/
bool hit_vertical_wall(int i)
{

	segment wallleft;
	wallleft.one.x = 0;
	wallleft.one.y = 0;
	wallleft.two.x = 0;
	wallleft.two.y = HEIGHT;

	segment wallright;

	wallright.one.x = WIDTH;
	wallright.one.y = 0;
	wallright.two.x = WIDTH;
	wallright.two.y = HEIGHT;

 	if ((IntersectsCircle(wallleft,ball[i])) || (IntersectsCircle(wallright,ball[i]))){

 		bounce_horizontal(i);
 		return true;
 	}
 	return false;


}
/*
* checks if a goal has been scored.
*/
void score_goal(int i){
	char msg[25];

	segment goaltop;
	goaltop.one.x = 0;
	goaltop.one.y = 0;
	goaltop.two.x = WIDTH;
	goaltop.two.y = 0;

	segment goalbottom;

	goalbottom.one.x = 0;
	goalbottom.one.y = HEIGHT;
	goalbottom.two.x = WIDTH;
	goalbottom.two.y = HEIGHT;

	if (IntersectsCircle(goaltop,ball[i])){

		bottomScore[i] += 1;
		reset_ball(i);
		resetPongs(i);
		
		sprintf(msg,"S %d %d\n", topScore[i], bottomScore[i]);
		rt_mutex_acquire(&playermtx[i],TM_INFINITE);
		if (clientfd1 > 0) { send(clientfd1[i], msg, sizeof(msg),0); }
		rt_mutex_release(&playermtx[i]);
		
		rt_mutex_acquire(&playermtx[i],TM_INFINITE);
		if (clientfd2 > 0) { send(clientfd2[i], msg, sizeof(msg),0); }
		rt_mutex_release(&playermtx[i]);

	}
	else if (IntersectsCircle(goalbottom,ball[i])){
		
		topScore[i] += 1;
		reset_ball(i);
		resetPongs(i);

		sprintf(msg,"S %d %d\n", topScore[i], bottomScore[i]);
		rt_mutex_acquire(&playermtx[i],TM_INFINITE);
		if (clientfd1[i] > 0) { send(clientfd1[i], msg, sizeof(msg),0); }
		rt_mutex_release(&playermtx[i]);
		
		rt_mutex_acquire(&playermtx[i],TM_INFINITE);
		if (clientfd2[i] > 0) { send(clientfd2[i], msg, sizeof(msg),0); }
		rt_mutex_release(&playermtx[i]);
 	}
}

//PongWidget methods


void sendPongData(int i, int number)
{
	char msg[25];
	if (number  == 0)
	{
		sprintf(msg,"P %d %d\n", (int)pong[i][number].one.x, (int)pong[i][number].one.y-PONG_DEFAULT_HEIGHT - PIXELADJUSTMENT);
		rt_mutex_acquire(&playermtx[i],TM_INFINITE);
		if (clientfd1 > 0) { send(clientfd1[i], msg, sizeof(msg),0); }
		rt_mutex_release(&playermtx[i]);
		
		rt_mutex_acquire(&playermtx[i],TM_INFINITE);
		if (clientfd2 > 0) { send(clientfd2[i], msg, sizeof(msg),0); }
		rt_mutex_release(&playermtx[i]);
	}
	else if (number == 1)
	{
		sprintf(msg,"R %d %d\n", (int)pong[i][number].one.x, (int)pong[i][number].one.y - PIXELADJUSTMENT);
		rt_mutex_acquire(&playermtx[i],TM_INFINITE);
		if (clientfd1 > 0) { send(clientfd1[i], msg, sizeof(msg),0); }
		rt_mutex_release(&playermtx[i]);
		
		rt_mutex_acquire(&playermtx[i],TM_INFINITE);
		if (clientfd2 > 0) { send(clientfd2[i], msg, sizeof(msg),0); }
		rt_mutex_release(&playermtx[i]);
	}
}

void PongWidget(int number, double y, int i)
{
	pong[i][number].one.x = WIDTH / 2 - PONG_DEFAULT_LENGTH / 2;
	pong[i][number].two.x = WIDTH / 2 + PONG_DEFAULT_LENGTH / 2;
	pong[i][number].one.y = y;
	pong[i][number].two.y = y;
	sendPongData(i, number);
}

void movePong(int number, int delta, int i)
{
	pong[i][number].one.x = pong[i][number].one.x + delta;
	pong[i][number].two.x = pong[i][number].two.x + delta;
	if (pong[i][number].one.x < 0) 
	{
		pong[i][number].one.x = 0;
		pong[i][number].two.x = PONG_DEFAULT_LENGTH;
	}
	if (pong[i][number].two.x > WIDTH)
	{
		pong[i][number].two.x = WIDTH;
		pong[i][number].one.x = WIDTH - PONG_DEFAULT_LENGTH;
	}
	
	sendPongData(i, number);
}

void resetPongs(int i)
{
	rt_mutex_acquire(&playermtx[i],TM_INFINITE);
	if (clientfd1[i] > 0 && clientfd2[i] > 0 )
	{
		rt_mutex_release(&playermtx[i]);
		PongWidget(0,50,i);
		PongWidget(1,500,i);

	} else if (clientfd1[i] > 0 || clientfd2[i] > 0)
	{
		rt_mutex_release(&playermtx[i]);
		PongWidget(0,50,i);
	} else
	{
		rt_mutex_release(&playermtx[i]);
	}
}

double distance(point p, segment s) 
{
// s.one must be less than s.two for correct function
//

    // if it's a vertical line return the horizontal distance
	if ( s.one.x == s.two.x)
	return fabs(s.one.x - p.x);

	double b = 0;
	double a = 0;
	//case 1 p.x >= p.two.x find the distance on the x-axis
	if (p.x >= s.two.x) { a = p.x  - s.two.x; }
	//case 2 p.x <= p.one.x find the distance on the x-axis
	if (p.x <= s.one.x) { a = s.one.x - p.x; }
	//case 3 the point is within the segment region one.x <= p.x <= two.x
	//return the y distance
	if (p.x <= s.two.x &&  p.x >= s.one.x)  { return fabs(s.one.y - p.y); }
	
	//for this version s is assumed to be vertical find the y difference
	if ( s.one.y == s.two.y ) { b = fabs(s.one.y - p.y); }
	
	return sqrt(b*b+a*a);
}


bool IntersectsCircle(segment s, circle c) 
{ 
    return distance(c.center, s) <= c.radius;
}

/* sets game start to 1 playermtx must be created first */
void startgame(int i)
{
// 	rt_mutex_acquire(&playermtx,TM_INFINITE);
// 	if (gamestart != 1)
// 	{
// 		gamestart = 1;
		
		BallWidget(i);
// 	}
// 	rt_mutex_release(&playermtx);

}
