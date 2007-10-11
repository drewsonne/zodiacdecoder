
import java.io.*; 
import java.util.*; 
import java.net.*; 
 
//sample code for server. 
//very bad code, but it looks like it works. I don't have the java sdk however to test that. Feel free to implement in whatever as long as it uses a socket 
//which should be specified on the standards page at some point. This will put all of the keys sent into a text file. 
 
public class jServer implements Runnable { 
     LinkedList queue,q2,q3; 
     Thread th; 
     public static void main(String[] args) throws IOException { 
          jServer zs = new jServer(); 
          Thread t = new Thread(zs); 
          zs.th = t; 
          t.start(); 
          ServerSocket ss = new ServerSocket(10000); 
          while(true) { 
               zs.q2.add(ss.accept()); 
               zs.q3.add(new BufferedReader(new InputStreamReader(((Socket)zs.q2.getLast()).getInputStream()))); 
          } 
     } 
     public jServer() { 
          queue = new LinkedList(); 
          q2 = new LinkedList(); 
          q3 = new LinkedList(); 
     } 
     public void run() { 
          while(true) { 
               try { 
                    th.sleep(1000); 
                    ListIterator i = q2.listIterator(); 
                    ListIterator i2 = q3.listIterator(); 
                    while(i.hasNext()&&i2.hasNext()) { 
                         BufferedReader r = (BufferedReader)i2.next(); 
                         Object o = r.ready()?queue.add(r):"LOL!"; 
                    } 
                    while(queue.size()>0) { 
                         Object o = queue.remove(); 
                         BufferedReader s = (BufferedReader)o; 
                         System.out.println(s.readLine()); 
 
          /*                    FileWriter f = new FileWriter("textfile.txt",true); 
                         f.write(new BufferedReader(s.getOutputStream()).read()); 
                         f.flush(); 
                         f.close();*/ 
                    } 
               } catch (Exception e) {e.printStackTrace();} 
          } 
     } 
}