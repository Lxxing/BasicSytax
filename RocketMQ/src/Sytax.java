import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Iterator;
import java.util.Properties;
import java.util.concurrent.LinkedBlockingQueue;



public class Sytax {
    public static void main(String[] args) throws InterruptedException  {
    	
    	System.out.println("sytax test.");
    	System.out.println(System.getProperty("java.class.path"));
    	System.out.println(System.getProperty("user.dir"));

    	///properties
    	Properties property = new Properties();
    	
    	try {
    		FileInputStream filep = new FileInputStream("test.properties");
			property.load(filep);
			Iterator<String> it=property.stringPropertyNames().iterator();
			while(it.hasNext()){
				String key=it.next();
				System.out.println(key+":"+property.getProperty(key));
			}
	    	filep.close();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}

    	
    	/////test LinkedBlockingQueue
    	LinkedBlockingQueue<String> TestQueue =
    			new LinkedBlockingQueue<String>(5);
    	
    	for (int i = 0; i < 5; i++) {
			try {
				TestQueue.put("Q"+i);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
    	
    	
    	
    	for (int i = 0; i < 5; i++) {
			try {
				System.out.println(TestQueue.take());
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
    	//////////test ByteBuffer
    	ByteBuffer buffer = ByteBuffer.allocate(102400);  
        System.out.println("buffer = " + buffer); 
        
        System.out.println("----------Test wrap--------");  
        byte[] bytes = new byte[32];  
        buffer = ByteBuffer.wrap(bytes);  
        System.out.println(buffer);  
          
        buffer = ByteBuffer.wrap(bytes, 10, 10);  
        System.out.println(buffer); 
        System.out.println("--------Test reset----------");  
        buffer.clear();  
        buffer.position(5);  
        buffer.mark();  
        buffer.position(10);  
        System.out.println("before reset:" + buffer);  
        buffer.reset();  
        System.out.println("after reset:" + buffer);  
      
        System.out.println("--------Test rewind--------");  
        buffer.clear();  
        buffer.position(10);  
        buffer.limit(15);  
        System.out.println("before rewind:" + buffer);  
        buffer.rewind();  
        System.out.println("before rewind:" + buffer);  
      
        System.out.println("--------Test compact--------");  
        buffer.clear();  
        buffer.put("abcd".getBytes());  
        System.out.println("before compact:" + buffer);  
        System.out.println(new String(buffer.array()));  
        buffer.flip();  
        System.out.println("after flip:" + buffer);  
        System.out.println((char) buffer.get());  
        System.out.println((char) buffer.get());  
        System.out.println((char) buffer.get());  
        System.out.println("after three gets:" + buffer);  
        System.out.println("\t" + new String(buffer.array()));  
        buffer.compact();  
        System.out.println("after compact:" + buffer);  
        System.out.println("\t" + new String(buffer.array()));  
      
        System.out.println("------Test get-------------");  
        buffer = ByteBuffer.allocate(32);  
        buffer.put((byte) 'a').put((byte) 'b').put((byte) 'c').put((byte) 'd')  
                .put((byte) 'e').put((byte) 'f');  
        System.out.println("before flip()" + buffer);  
        buffer.flip();  
        System.out.println("before get():" + buffer);  
        System.out.println((char) buffer.get());  
        System.out.println("after get():" + buffer);  
        // get(index)不影响position的值  
        System.out.println((char) buffer.get(2));  
        System.out.println("after get(index):" + buffer);  
        byte[] dst = new byte[10];  
        buffer.get(dst, 0, 2);  
        System.out.println("after get(dst, 0, 2):" + buffer);  
        System.out.println("\t dst:" + new String(dst));  
        System.out.println("buffer now is:" + buffer);  
        System.out.println("\t" + new String(buffer.array()));  
      
        System.out.println("--------Test put-------");  
        ByteBuffer bb = ByteBuffer.allocate(32);  
        System.out.println("before put(byte):" + bb);  
        System.out.println("after put(byte):" + bb.put((byte) 'z'));  
        System.out.println("\t" + bb.put(2, (byte) 'c'));  
        System.out.println("after put(2,(byte) 'c'):" + bb);  
        System.out.println("\t" + new String(bb.array()));  
        bb.put(buffer);  
        System.out.println("after put(buffer):" + bb);  
        System.out.println("\t" + new String(bb.array())); 

    }

}
