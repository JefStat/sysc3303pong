

/**
 * This is the Pong GameState, it updates the game based on the movement of the ball and paddles. 
 * At any given time it contains the current state of the Pong Game
 * 
 * @author Nsicole Waldrum
 * @date 16/01/10
 * @version GameState 1.0
 *
 */
public class GameState  {
	//changed x and y to 590 from 600

	public static final int X = 600;
	public static final int Y = 600;


	private PongWidget[] pongs = new PongWidget[2];
	private Ball ball;
	int npongs;

	/**
	 * This is the GameState constructor, it continually updates as the Pong Game as it is played.
	 */
	public GameState(){
		npongs = 0; 

	}

	/**
	 * sets the ball current ball to the game ball
	 * @param ball the ball that is created outside of gamestate
	 */
	public void setBall(Ball ball) {
		this.ball = ball;
	}
	
	/**
	 * returns the number of maximum pongs allowed per game.
	 * @return pongs
	 */
	public PongWidget[] getPongs() {
		return pongs;
	}

	/**
	 * Sets the maximum number of pongs that are allowed per game.
	 * @param pongs, maximum number of pongs
	 */
	public void setPongs(PongWidget[] pongs) {

		this.pongs = pongs;
	}

	/**
	 * sets the pongWidget to an identifiable int
	 * 
	 * @param pongs the pongWidget
	 * @param x the integer the pongWidget will be set to
	 */
	public void setPongs(PongWidget pongs, int x) {

		this.pongs[x] = pongs;
	}

	/**
	 * Checks to ensure that the ball is existing
	 * @return ball the location of the ball
	 */
	public Ball getBall() {
		return ball;
	}


}


