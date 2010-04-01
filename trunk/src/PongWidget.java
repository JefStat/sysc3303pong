
import java.awt.Dimension;
import java.awt.Point;
import java.awt.Rectangle;

/**
 * Model class for a pong paddle aka pongwidget. Current issue paddle can travel further then the pong panel frame.
 * 
 * 
 * @author Jef Statham
 * @date 15/01/10
 * @version 1.0
 *
 */
@SuppressWarnings("serial")
public class PongWidget extends Rectangle
{

	public final static int DEFAULT_HEIGHT = 20;
	public final static int LENGTH_DELTA = 80;

	
	/**
	 * Creates a new pongwidget with the left and rights keys of the players preference
	 * @param centre, the middle of the side the paddle is created on
	 * @param left, the left key
	 * @param right, the right key
	 */
	public PongWidget(Point centre)
	{
		super(centre, new Dimension(LENGTH_DELTA,DEFAULT_HEIGHT));
	
	}
	
	/**
	 * Moves the paddle to the left according to the players pressed keys 
	 */
	public void moveLeft()
	{
		if (x <= 0){
		} else {
			translate(-2, 0);
		}
	}
	/**
	 * Moves the paddle to the right according to the players pressed keys
	 */
	public void moveRight()
	{
		if (x + LENGTH_DELTA >= GameState.X){
		} else { 
			translate(2, 0); 
		}
	}
	
}
