
import javax.swing.*;

import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.util.Observable;
import java.util.StringTokenizer;
import java.util.Observer;
import java.awt.Robot;

/**
 * This is the frame for the entire pong game view. It initialises all the set up for the gui.
 *
 * @author Jef Statham modified by Luis Lopez
 * @date 09/02/10
 * @version 2.0
 *
 */
@SuppressWarnings("serial")
public class PongGUI extends JFrame implements Observer, WindowListener, MouseMotionListener
{
	private PongPanel pPanel;
	private ScorePanel sPanel;
	private ViewServer server;
	
	private long current;
	
	private GameState gameState = new GameState();
	
	Robot ro;
	
	private int mouseX;

	/**
	 *  This draws the gui for the Pong Game and includes initialising many important aspects that will be required later.
	 */
	public PongGUI(ViewServer server) {
		super("SYSC3303 PongGame");
		
		this.server = server;

		mouseX = 300;
		
		try {
			ro = new Robot();
		} catch (AWTException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		current = System.currentTimeMillis();
		
		ro.mouseMove(this.getX()+300, this.getY()+100);
		
		setSize( GameState.X,  GameState.Y);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		sPanel = new ScorePanel();
		pPanel = new PongPanel( gameState );

		Container cp = getContentPane();
		cp.add( pPanel, BorderLayout.CENTER );
		cp.add( sPanel, BorderLayout.NORTH );
		this.setResizable(false);
		
		this.addWindowListener(this);
		this.addMouseMotionListener(this);
		
	}

	/**
	 * when receives an update message, it parses the message so that other classes can update the screen
	 */
	@Override
	public void update(Observable o, Object arg) {

		String msg = (String) arg;
		StringTokenizer st;
		
		PongWidget pong;
		
		int x,y;
		int index;

		if (msg.indexOf('P') != -1)
		{
			index = msg.indexOf('P');
		}
		else if (msg.indexOf('R') != -1)
		{
			index = msg.indexOf('R');
		}
		else if (msg.indexOf('S') != -1)
		{
			index = msg.indexOf('S');
		}
		else if (msg.indexOf('B') != -1)
		{
			index = msg.indexOf('B');
		} 
		else 
		{
			index = 0;
		}
	

		switch (msg.charAt(index)) {
		case 'P':  //player 1
			st= new StringTokenizer(msg);
			st.nextToken();
			try{
				x = Integer.parseInt( st.nextToken());
				y = Integer.parseInt( st.nextToken());
				pong = new PongWidget(new Point(x,y));
				gameState.setPongs(pong,0);
				pPanel.player1 = true;
				pPanel.repaint();
			}catch (Exception e)
			{
				e.printStackTrace();
			}
			break;

		case 'R': //player 2
			try{
				st = new StringTokenizer(msg);
				st.nextToken();
				x = Integer.parseInt( st.nextToken());
				y = Integer.parseInt( st.nextToken());
				pong = new PongWidget(new Point(x,y));
				gameState.setPongs(pong,1);
				pPanel.player2 = true;
				pPanel.repaint();
			}catch (Exception e)
			{
				e.printStackTrace();
			}
			break;

		case 'B':   //pass values for the Ball
			try{
				st = new StringTokenizer(msg);
				st.nextToken();
				x = Integer.parseInt( st.nextToken());
				y = Integer.parseInt( st.nextToken());
				Ball B = new Ball(new Point(x,y));
				gameState.setBall(B);
				pPanel.repaint();
			}catch (Exception e)
			{
				e.printStackTrace();
			}
			break;

		case 'S': //pass values for Score
			try{
				st = new StringTokenizer(msg);
				st.nextToken();
				sPanel.setScore(st.nextToken(),st.nextToken()); //player 1
				ro.mouseMove(this.getX()+300, this.getY()+100);
			}catch (Exception e)
			{
				e.printStackTrace();
			}
			break;

		}//end switch
	}//end update

	@Override
	public void windowActivated(WindowEvent e) {
		
	}

	@Override
	public void windowClosed(WindowEvent e) {
		server.shutdown();
		
	}

	/**
	 * calls the shutdown method if the window is closing to ensure a proper ending to the game.
	 */
	@Override
	public void windowClosing(WindowEvent e) {
		server.shutdown();
		System.exit(0);
		
	}

	@Override
	public void windowDeactivated(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void windowDeiconified(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void windowIconified(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void windowOpened(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseDragged(MouseEvent e) 
	{
	}

	/**
	 * If the mouse moved, a new AWT command is created, that calculates the movement to be sent to the game server.
	 * Limited to a specified number of miliseconds
	 */
	@Override
	public void mouseMoved(MouseEvent e) 
	{
		//if ((System.currentTimeMillis()- current) > 20){
			//current = System.currentTimeMillis();
			server.sender(Integer.toString(mouseX-e.getX()));
			
			mouseX = e.getX();
		//}
		
	}
}//end class

