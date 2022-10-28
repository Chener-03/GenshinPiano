package xyz.chener.genshinpiano.music.autoplay;


import xyz.chener.genshinpiano.MainApplication;
import xyz.chener.genshinpiano.music.entity.defaults.MusicData;
import xyz.chener.genshinpiano.music.entity.defaults.MusicFrame;
import xyz.chener.genshinpiano.music.entity.defaults.PlayData;
import xyz.chener.genshinpiano.music.jni.HotKey4J;
import xyz.chener.genshinpiano.music.keymapper.KeyCodeMapper;


import java.awt.*;
import java.awt.event.KeyEvent;
import java.util.Objects;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class DefaultStepPlay {

    private static DefaultStepPlay instance = null;
    private static final Lock stepLock = new ReentrantLock();

    private int hotKeyId = 0;

    private DefaultStepPlay(){

        hotKeyId = HotKey4J.addHotKey((int) MainApplication.HWND,"hotkey01",HotKey4J.Modifiers.MOD_ALT, 0x28);
    }

    public static DefaultStepPlay getInstance()
    {
        if (instance == null)
        {
            synchronized (DefaultStepPlay.class)
            {
                if (instance == null)
                    instance = new DefaultStepPlay();
            }
        }
        return instance;
    }

    public void play(MusicData musicData)
    {
         try {
             if (!stepLock.tryLock()) {
                 System.out.println("stepPlayer is occupy");
                 return;
             }

            PlayData playData = new PlayData(musicData);
            Robot robot = new Robot();
            if (hotKeyId == 0)
            {
                System.out.println("注册热键失败");
                return;
            }
            HotKey4J.addEventListener(hotKeyId,(int) MainApplication.HWND,()->{
                if (!playData.hasNext())
                {
                    System.out.println("已结束");
                    return true;
                }
                MusicFrame next = playData.getNext();
                robot.keyRelease(KeyEvent.VK_ALT);

                next.getKeys().forEach(ke->{
                    Integer integer = KeyCodeMapper.getMap().get(ke);
                    if (integer!=null)
                    {
                        HotKey4J.keyDown(integer);
                        HotKey4J.keyUp(integer);
                    }
                });
                robot.keyPress(KeyEvent.VK_ALT);
                if (!playData.hasNext())
                {
                    System.out.println("结束");
                    return true;
                }
                return true;
            });
            HotKey4J.start();
            System.out.println("已开始单步播放,游戏内按下 ALT+↓键 播放一帧");
        }catch (Exception exception){
             System.out.println(exception.getMessage());
         }
    }


    public void stop()
    {
        System.out.println("End...");
        HotKey4J.stop();
        HotKey4J.deleteEventListener(hotKeyId);
        stepLock.unlock();
    }



}
