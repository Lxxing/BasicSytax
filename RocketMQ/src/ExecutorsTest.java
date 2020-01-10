
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;


public class ExecutorsTest {
    public static void main(String[] args) throws InterruptedException  {
    	
    	System.out.println("ExecutorsTest.");
    	System.out.println(System.getProperty("java.class.path"));
    	System.out.println(System.getProperty("user.dir"));


        final ScheduledExecutorService scheduledExecutorService = 
        		Executors.newSingleThreadScheduledExecutor(
        				new MyTreadFactory("NSScheduledThread")
        				);
        final Runnable testrun = new Runnable() {
        	@Override
            public void run() {
        		TestNum.printNum();
        	}
        };
            
        scheduledExecutorService.scheduleAtFixedRate(testrun, 2, 1, TimeUnit.SECONDS);
        
        while(true)
        {
        	Thread.sleep(1000);
        }

    }

}
