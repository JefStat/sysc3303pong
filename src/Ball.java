
import java.awt.Point;

/**
 * This class is the Pong ball with a radius at point "center". This class calculates the ball movement, if a goal occurs or if the ball hits a wall.
 * 
 * @author Jef Statham
 * @date 24/01/10
 * @version 2.0
 */
public class Ball 
{
	public final static int DEFAULT_RADIUS = 15;
	public Point p; 
	
	/**
	 * gets the point of the ball with its speed for the current game
	 * @param centre, the centre of the ball
	 * @param gameState, the current game of pong
	 */
	public Ball(Point centre)
	{
		p = centre;
	}
	
}

