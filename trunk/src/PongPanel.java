
import javax.swing.*;

import java.awt.*;
/**
 * This PongPanel displays and animates the gui on the screen.
 * @author Jef Statham
 * @date 20/01/10
 * @version 1.5
 *
 */
@SuppressWarnings("serial")
public class PongPanel extends JPanel
{ 

    private GameState gameState;
	public boolean player1;
	public boolean player2;
  
	/**
     * This displays the animation of the game at a certain frame rate per second.
     * @param gameState the current state of the game that the user sees.
     */
    public PongPanel(GameState gameState)
	{
    	this.gameState = gameState; 
    	setBackground(Color.black);
    	
  		//Allows the player keylistener to work
  		this.setFocusable(true);
  		player1 = false;
  		player2 = false;
  	  
 	}
  
   
  	/**
  	 * This paints the screen with the current location of the ball and widgets.
  	 */
  	public void paintComponent(Graphics g)
  	{
  		super.paintComponent(g);
  		g.setColor( Color.white);
  		
  		Ball ball = gameState.getBall();
  	
    	PongWidget pongs[] = gameState.getPongs();
    	
  		if (ball != null) 
  			g.fillOval( ball.p.x -Ball.DEFAULT_RADIUS, ball.p.y-Ball.DEFAULT_RADIUS, Ball.DEFAULT_RADIUS*2, Ball.DEFAULT_RADIUS*2) ;
		if (player1)
  		{
			PongWidget p = pongs[0];
  			g.fillRect( p.x, p.y, PongWidget.LENGTH_DELTA, PongWidget.DEFAULT_HEIGHT );
  		}
		if (player2)
  		{
			PongWidget p = pongs[1];
  			g.fillRect( p.x, p.y, PongWidget.LENGTH_DELTA, PongWidget.DEFAULT_HEIGHT );
  		}
  	}
}


