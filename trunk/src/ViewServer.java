
import java.io.*;
import java.net.*;
import java.util.Observable;

/**
 * The ViewServer communicates with the Linux modelC server.
 * 
 * @author Jef Statham
 * @date 08/02/10
 * @version 2.0
 *
 */

public class ViewServer extends Observable {
	SocketAddress endpoint;
	Socket clientSocket;
	PrintWriter out;
	BufferedReader in;

	/**
	 * The constructor for the ViewServer creates a new server from the appropriate port and ip of the Linux server computer.
	 * 
	 * @param port the port that the linux server is operating on.
	 * @param ip the ip of the linux server computer.
	 */
	public ViewServer(String port, String ip)
	{
		try {
			clientSocket = new Socket(ip, new Integer(port));
			in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
			//out = new PrintWriter(clientSocket.getOutputStream(), true);
			recieveAndReconnect(ip);
			
		} catch (IOException e) {
			e.printStackTrace();
			System.exit(0);
		}
	}
	
	private void recieveAndReconnect(String ip)
	{
		try
		{
			String port = in.readLine();
			in.close();
			clientSocket.close();
			clientSocket = new Socket(ip, new Integer(port));
			in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
			out = new PrintWriter(clientSocket.getOutputStream(), true);
			
		} catch (IOException e) { e.printStackTrace(); System.exit(0);}
	}

	/**
	 * Receives data from the linux server and notifies all the observers.
	 */
	public void receive()
	{
		String msg;
		try {
			while ( (msg = in.readLine()) != null) 
			{

				this.setChanged();
				this.notifyObservers(msg);
			}
		} catch (SocketException e2) { System.out.println("Done"); System.exit(0); }
		catch (IOException e) { e.printStackTrace(System.err); System.exit(0);  }
	}

	
	/**
	 * sends the string to the Linux server
	 * @param s the location of the pongWidget
	 */
	public void sender(String s)
	{
		if(out!= null){
			out.println(s);
		}
		
	}

	/**
	 * sends a message to the linux server that the program has been closed and appropriate closes the game.
	 */
	public void shutdown()
	{
		try { 
			if( clientSocket != null) clientSocket.close();
			if (in != null) in.close();
			if (out != null) out.close();
		} catch (IOException e) { e.printStackTrace(System.err); System.exit(1); }
	}
	
	/**
	 * The main program runs the pong GUI in order to create a visible image for the user.
	 * @param args
	 */
	public static void main( String args[] )
	{
		ViewServer c = new ViewServer("5000","134.117.59.186");

		PongGUI pong = new PongGUI(c);
		pong.setVisible(true);

		c.addObserver(pong);
		c.receive();


	}	
	
}






