
import javax.swing.JLabel;
import javax.swing.JPanel;

/**
 * This tracks the score of the game for each user, depending upon who has scored a goal.
 * 
 * @author Jef Statham modified by Luis Lopez
 * @date 08/02/10
 * @version 2.0
 *
 */
@SuppressWarnings("serial")
public class ScorePanel extends JPanel
{

	private JLabel top, bottom;
	
	/**
	 * This displays the score on the Pong GUI.
	 */
	public ScorePanel()
	{
		add( new  JLabel("Top: ") );
		top = new JLabel("    0000");
		add( top );
		add(  new JLabel( "Bottom: " ) );
		bottom = new JLabel("    0000");
		add( bottom );
				
	}
 	
	/**
	 * Sets the score for the top and the bottom of the score panel.
	 * @param x a String for the top pongWidget score
	 * @param y a String for the bottom pongWidget score
	 */
	public void setScore(String x, String y){	
		top.setText(x);
		bottom.setText(y);
	}
}

 





