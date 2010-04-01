
//GameState constants
#define PONG_DEFAULT_HEIGHT 20
#define PONG_DEFAULT_LENGTH 80
#define PONG_DEFAULT_RADIUS 15

//Ball constants
#define DEFAULT_SPEED_X 3
#define DEFAULT_SPEED_Y 1
#define MIN_SPEED 20


//GameState constants
#define WIDTH 600
#define HEIGHT 600


//GameState struct point
typedef struct {
    double x;
    double y;
} point;

//GameState struct segment
typedef struct {
    point one;
    point two;
} segment;

//GameState struct circle
typedef struct { 
    point center;
    double radius;
} circle;

RT_TASK ballMover[BUFFER];
// int goalscored = 0;

circle ball[BUFFER];

int vector_x[BUFFER];
int vector_y[BUFFER];

//GameState variables
segment pong[BUFFER][2]; /* The 2 Pong paddles */
int topScore[BUFFER];
int bottomScore[BUFFER];
int nPongs[BUFFER]; /* number of Pong paddles represented in game */


//GameState methods
void GameState(int i);

//Ball methods
/*
* moves the ball around the screen
*/
void move_ball(void* cookie);

/*
*resets the ball to the centre of the screen
*/
void reset_ball(int i);

/*
* constructor for the ball
*/
void BallWidget(int i);

/*
* signals everything
*/
void catch_signal(int sig);

/*
* bounces the ball in the y direction
*/
void bounce_vertical(int i);

/*
* bounces the ball in the x direction
*/
void bounce_horizontal(int i);

/*
* checks if the ball hits a wall
*/
bool hit_vertical_wall(int i);

/*
* checks if a goal was scored
*/
void score_goal(int i);

//PongWidget methods

/*
 * Intializes a pong in the array and increment nPongs by 1 
 */
void PongWidget(int number, double y,int i);

/*
 * Resets Pong 0 and 1 to the center of WIDTH
 */
void resetPongs(int i);

/*
 * Calculates the distance of a point to a line on the cartesian plane
 */
double distance(point p, segment s);

/*
 * Returns the boolean value of the distance s <= to the circle c radius
 */
bool IntersectsCircle(segment s, circle c);
