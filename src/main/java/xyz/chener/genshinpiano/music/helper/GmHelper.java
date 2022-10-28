package xyz.chener.genshinpiano.music.helper;

import xyz.chener.genshinpiano.music.entity.defaults.MusicData;
import xyz.chener.genshinpiano.music.entity.defaults.MusicFrame;
import xyz.chener.genshinpiano.music.entity.defaults.PlayData;
import xyz.chener.genshinpiano.music.keymapper.KeyCodeMapper;
import xyz.chener.genshinpiano.music.utils.MapBuilder;

import javax.print.attribute.standard.JobKOctets;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.function.Function;
import java.util.function.ToIntFunction;


public class GmHelper {

    private static GmHelper ins = null;

    public static GmHelper getInstance()
    {
        if (ins == null)
        {
            synchronized (GmHelper.class)
            {
                if (ins == null)
                    ins = new GmHelper();
            }
        }
        return ins;
    }


    private GmHelper(){}

    private static final Lock gameHelperLock = new ReentrantLock();

    public static final List<String> concurrentShowKeys = new CopyOnWriteArrayList<>();
    public static final List<String> nextShowKeys = new CopyOnWriteArrayList<>();

    private volatile Thread concurrentThread = null;

    private String concurrentUid = "";

    public void stop()
    {
        concurrentThread.interrupt();
        try {
            concurrentThread.join();
        } catch (InterruptedException ignored) { }
        concurrentThread = null;
    }

    public boolean start(MusicData musicData)
    {
        if (concurrentThread != null && concurrentThread.isAlive())
        {
            System.err.println("concurrent is start");
            return false;
        }
        concurrentThread = new Thread(()->{
            Function<String,Boolean> callback = null;
            try {
                if (!gameHelperLock.tryLock())
                {
                    System.err.println("other thread is start");
                    return;
                }
                PlayData playData = new PlayData(musicData);
                final ReentrantLock lo1 = new ReentrantLock();
                final Condition condition = lo1.newCondition();

                callback = s -> {
                    if (s.equals(concurrentUid))
                    {
                        lo1.lock();
                        condition.signalAll();
                        lo1.unlock();
                        return Boolean.TRUE;
                    }
                    return Boolean.FALSE;
                };
                KeyStateNotify.getInstance().addListener(callback);
                while (playData.hasNext() && !concurrentThread.isInterrupted())
                {
                    try {
                        lo1.lock();
                        MusicFrame next = playData.getNext();
                        List<Integer> keys = next.getKeys().stream()
                                .map(s -> KeyCodeMapper.getMap().get(s)).toList();
                        // todo add show lists
                        concurrentShowKeys.clear();
                        //nextShowKeys.clear();
                        concurrentShowKeys.addAll(next.getKeys());
                        /*if (playData.getConcurrentFrame()+1 < playData.getFrame()) {
                            MusicFrame nextnext = playData.getMusicData().getList().get(playData.getConcurrentFrame() + 1);
                            nextShowKeys.addAll(nextnext.getKeys());
                        }*/
                        // todo prepare wait keys down
                        concurrentUid = UUID.randomUUID().toString();
                        Map<String, Object> m = MapBuilder.<String, Object>getIns()
                                .put("k", concurrentUid)
                                .put("v", keys).build();
                        KeyStateNotify.getInstance().addQueue(m);
                        condition.await();
                    }catch (Exception ignored){
                        throw new RuntimeException(ignored);
                    }
                    finally {
                        lo1.unlock();
                    }
                }


            }catch (Exception ignored){ }
            finally {
                gameHelperLock.unlock();
                if (callback!=null)
                    KeyStateNotify.getInstance().removeListener(callback);
                concurrentShowKeys.clear();
                nextShowKeys.clear();
            }
        });
        concurrentThread.start();
        return true;
    }

}
