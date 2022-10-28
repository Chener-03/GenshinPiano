package xyz.chener.genshinpiano.music.utils;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

public class TheadPool {

    private static final ExecutorService pool = Executors.newCachedThreadPool();

    public static void exec(Runnable runnable)
    {
        try {
            pool.execute(runnable);
        }catch (Throwable throwable)
        {
            System.out.println("线程启动异常");
        }
    }

}
