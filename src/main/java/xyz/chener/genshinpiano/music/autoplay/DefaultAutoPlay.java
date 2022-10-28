package xyz.chener.genshinpiano.music.autoplay;

import xyz.chener.genshinpiano.music.jni.HotKey4J;
import xyz.chener.genshinpiano.music.entity.defaults.MusicData;
import xyz.chener.genshinpiano.music.entity.defaults.MusicFrame;
import xyz.chener.genshinpiano.music.entity.defaults.PlayData;
import xyz.chener.genshinpiano.music.keymapper.KeyCodeMapper;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class DefaultAutoPlay {

    private static final Lock playLock = new ReentrantLock();

    private volatile static DefaultAutoPlay instance = null;

    private boolean mustGenshinWindowTop = true;

    private Thread concurrentThread = null;

    private DefaultAutoPlay(){}

    public static DefaultAutoPlay getInstance()
    {
        if (instance == null)
        {
            synchronized (DefaultAutoPlay.class)
            {
                if (instance == null)
                    instance = new DefaultAutoPlay();
            }
        }
        return instance;
    }

    public void play(MusicData musicData)
    {
        if (concurrentThread!=null && concurrentThread.isAlive())
        {
            System.err.println("concurrent is play music");
            return;
        }
        if (!musicData.isSupportAuto())
        {
            System.err.println("this music does not support auto play");
            return;
        }


        concurrentThread = new Thread(()->{
            try {
                if (!playLock.tryLock()) {
                    System.err.println("Play object is occupied by other threads");
                    return;
                }
                PlayData playData = new PlayData(musicData);
                while (!Thread.interrupted() && playData.hasNext())
                {
                    if (mustGenshinWindowTop && !HotKey4J.GetForegroundWindowTitle().equals("原神"))
                    {
                        Thread.sleep(2000);
                        System.err.println("Wait for genshin window to top");
                        continue;
                    }
                    MusicFrame nextFrame = playData.getNext();
                    nextFrame.getKeys().forEach(key->{
                        Integer ink = KeyCodeMapper.getMap().get(key);
                        if (ink!=null)
                        {
                            HotKey4J.keyDown(ink);
                            HotKey4J.keyUp(ink);
                        }
                    });
                    Thread.sleep(nextFrame.getNextDelay());
                }

            }catch (Exception ignored)
            {

            }finally {
                playLock.unlock();
                System.out.println("播放结束");
            }
        });

        concurrentThread.start();
    }

    public void stop()
    {
        if (concurrentThread != null)
        {
            concurrentThread.interrupt();
            concurrentThread = null;
        }
    }

    public boolean isMustGenshinWindowTop() {
        return mustGenshinWindowTop;
    }

    public void setMustGenshinWindowTop(boolean mustGenshinWindowTop) {
        this.mustGenshinWindowTop = mustGenshinWindowTop;
    }
}
