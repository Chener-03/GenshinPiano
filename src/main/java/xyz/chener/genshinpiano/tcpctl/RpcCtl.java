package xyz.chener.genshinpiano.tcpctl;

import javafx.application.Platform;
import xyz.chener.genshinpiano.gui.MainController;
import xyz.chener.genshinpiano.gui.RpcLogApplication;
import xyz.chener.genshinpiano.gui.RpcLogController;
import xyz.chener.genshinpiano.music.jni.DrawCall;

import java.util.ArrayList;
import java.util.Collections;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class RpcCtl {

    private static volatile RpcCtl rpcCtl = null;

    public static String concurrentMusicName = "";

    private final LinkedBlockingDeque<MsInfo> msInfos = new LinkedBlockingDeque<>(30);

    private final ReentrantLock lock = new ReentrantLock();
    private final Condition condition = lock.newCondition();

    private RpcCtl(){}

    public static RpcCtl getInstance()
    {

        if (rpcCtl == null)
        {
            synchronized (RpcCtl.class)
            {
                if (rpcCtl == null)
                    rpcCtl = new RpcCtl();
            }
        }
        return rpcCtl;
    }

    public void start()
    {
        new Thread(()->{
            System.out.println("start rpc call");
            while (!Thread.interrupted())
            {
                try {
                    lock.lock();
                    MsInfo msInfo = msInfos.takeFirst();
                    int musicId = msInfo.getMusicId();
                    concurrentMusicName = msInfo.getMusicName();
                    MainController.mainController.RpcCallAuto(musicId);
                    flushUiList();
                    condition.await();
                }catch (Exception exception)
                {
                    System.err.println(exception.getMessage());
                }finally {
                    lock.unlock();
                }
            }
        }).start();
        new Thread(()->{
            new RpcServer().run();
        }).start();
    }

    public static void postEnd()
    {
        if (rpcCtl == null)
            return;

        try {
            rpcCtl.lock.lock();
            concurrentMusicName = "";
            MainController.mainController.RpcCallAuto(-1);
            RpcCtl.getInstance().flushUiList();
            rpcCtl.condition.signalAll();
        }catch (Exception ignored){}
        finally {
        rpcCtl.lock.unlock();
        }
    }

    public void postMusic(MsInfo ms)
    {
        synchronized (RpcCtl.class){
            if (ms.isAdmin())
            {
                if (!msInfos.offerFirst(ms)) {
                    msInfos.removeLast();
                    msInfos.offerFirst(ms);
                }
            }
            msInfos.offerLast(ms);
            flushUiList();
        }
    }

    public void flushUiList()
    {
        if (RpcLogController.rpcLogController!=null) {
            ArrayList<MsInfo> l1 = new ArrayList<>(msInfos);
            RpcLogController.rpcLogController.set(Collections.unmodifiableList(l1));
        }
    }

    public static void checkParam(String[] args)
    {
        if (args.length > 0 && args[0].equals("-rpc"))
        {
            DrawCall.shutDownDraw();
            RpcCtl.getInstance().start();
            new Thread(()->{
                try {
                    Thread.sleep(3000);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
                Platform.runLater(RpcLogApplication::main0);
            }).start();
        }
    }

}
