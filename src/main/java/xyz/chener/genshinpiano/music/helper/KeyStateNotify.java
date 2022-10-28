package xyz.chener.genshinpiano.music.helper;

import xyz.chener.genshinpiano.music.utils.Asserts;
import xyz.chener.genshinpiano.music.jni.HotKey4J;

import java.util.List;
import java.util.Map;
import java.util.concurrent.BlockingDeque;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.function.Function;


public class KeyStateNotify {

    private static KeyStateNotify instance = null;


    // k -> name   v->List<Integer>
    private final BlockingDeque<Map<String, Object>> queue = new LinkedBlockingDeque<>();

    private Thread thread;

    private final List<Function<String,Boolean>> consumerList = new CopyOnWriteArrayList<>();

    private KeyStateNotify()
    {
        Asserts.assertBool(init(),"KeyStateNotify Init Error");
    }

    public static KeyStateNotify getInstance()
    {
        if (instance == null)
        {
            synchronized (KeyStateNotify.class)
            {
                if (instance == null)
                    instance = new KeyStateNotify();
            }
        }
        return instance;
    }

    private boolean init()
    {
        thread = new Thread(()->{
            while (!thread.isInterrupted())
            {
                try {
                    Map<String, Object> map = queue.takeFirst();
                    String k = map.get("k").toString();
                    List<Integer> v = (List<Integer>)map.get("v");
                    // todo spin wait for keys up
                    while (true)
                    {
                        boolean allUp = true;
                        for (Integer key : v) {
                            if (key!=null && HotKey4J.getKeyIsDown(key)) {
                                allUp = false;
                                Thread.sleep(1);
                            }
                        }
                        if (allUp) break;
                    }

                    // todo spin wait for keys down

                    while (true)
                    {
                        boolean allDown = true;
                        for (Integer key : v) {
                            if (key!=null && !HotKey4J.getKeyIsDown(key)) {
                                allDown = false;
                                Thread.sleep(1);
                            }
                        }
                        if (allDown) break;
                    }
                    // todo notify callback
                    for (Function<String, Boolean> fun : consumerList) {
                        if (fun.apply(k)) {
                            break;
                        }
                    }


                }catch (Exception exception)
                {
                    System.out.println();
                }
            }
        });
        thread.start();
        Runtime.getRuntime().addShutdownHook(new Thread(()->{
            if (thread!=null) thread.interrupt();
        }));
        return true;
    }


    public void addListener(Function<String,Boolean> function)
    {
        consumerList.add(function);
    }


    public void removeListener(Function<String,Boolean> function)
    {
        consumerList.remove(function);
    }

    public void addQueue(Map<String, Object> map)
    {
        try {
            queue.putLast(map);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

}
