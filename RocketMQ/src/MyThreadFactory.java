
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.atomic.AtomicLong;


class MyTreadFactory implements ThreadFactory {
    private final String threadNamePrefix;
    private final AtomicLong threadIndex = new AtomicLong(0);
	   public MyTreadFactory(String name) {
		// TODO Auto-generated constructor stub
		   threadNamePrefix = name;
	}

	public Thread newThread(Runnable r) {
		String tn = threadNamePrefix + this.threadIndex.incrementAndGet();
		System.out.println(tn);
		Thread th = new Thread(r,tn); 
		th.setDaemon(false);
	     return th;
	   }
}
