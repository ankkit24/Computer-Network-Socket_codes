import java.io.*; 
import java.net.*; 
  
class myUDPClient { 
    public static void main(String args[]) throws Exception 
    { 
  
      BufferedReader inFromUser = 
        new BufferedReader(new InputStreamReader(System.in)); 
  
      DatagramSocket clientSocket = new DatagramSocket(); 
   
      System.out.println("Client Started, Enter string to be sent: "); 
      byte[] sendData = new byte[1024]; 
      byte[] receiveData = new byte[1024]; 
  
      String sentence = inFromUser.readLine(); 
      sendData = sentence.getBytes();         
	
      System.out.println("Enter the Client address to be used:");
      String ip = inFromUser.readLine();
      InetAddress ip_addr = InetAddress.getByName(ip);
 
      System.out.println("Enter the server port to be used:");
      int new_port = Integer.parseInt(inFromUser.readLine());

      DatagramPacket sendPacket = 
         new DatagramPacket(sendData, sendData.length, ip_addr, new_port); 
  
      clientSocket.send(sendPacket); 
  
      DatagramPacket receivePacket = 
         new DatagramPacket(receiveData, receiveData.length); 
  
      clientSocket.receive(receivePacket); 
  
      String modifiedSentence = 
          new String(receivePacket.getData()); 
  
      System.out.println("FROM SERVER:" + modifiedSentence); 
      clientSocket.close(); 
      } 
} 

