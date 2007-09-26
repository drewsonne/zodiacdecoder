import java.io.*;
import java.util.LinkedList;
import java.net.*;

//sample code for server.
//very bad code, but it looks like it works. I don't have the java sdk however to test that. Feel free to implement in whatever as long as it uses a socket
//which should be specified on the standards page at some point. This will put all of the keys sent into a text file.

public class ZodiacServer implements Runnable {
	LinkedList queue;
	Thread th;
	public static void main(String[] args) {
		ZodiacServer zs = new ZodiacServer();
		Thread t = new Thread(zs);
		zs.th = t;
		t.start();
		SocketServer ss = new SocketServer(10000);
		while(true) {
			zs.queue.append(ss.accept());
		}
	}
	public ZodiacServer() {
		queue = new LinkedList();
	}
	public void run() {
		while(true) {
			th.sleep(1000);
			if(queue.size()>0) {
				Object o = queue.remove();
				Socket s = (Socket)o;
				System.out.println(new BufferedReader(s.getOutputStream()).read());
/*				FileWriter f = new FileWriter("textfile.txt",true);
				f.write(new BufferedReader(s.getOutputStream()).read());
				f.flush();
				f.close();*/
			}
		}
	}
}