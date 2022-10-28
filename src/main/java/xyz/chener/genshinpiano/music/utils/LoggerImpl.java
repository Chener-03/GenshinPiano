package xyz.chener.genshinpiano.music.utils;

import javafx.application.Platform;
import xyz.chener.genshinpiano.MainController;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;

public class LoggerImpl {

    private static ByteArrayOutputStream bos = new ByteArrayOutputStream();

    static {
        var oldOut = System.out;
        System.setOut(new PrintStream(bos));
        System.setErr(new PrintStream(bos));
        var t = new Thread(()->{
            while (true)
            {
                String s = bos.toString();
                if (s!=null && s.trim().length()>0)
                {
                    oldOut.println(s.replace("\r\n",""));
                    if (MainController.g_log!=null)
                    {
                        Platform.runLater(()->{
                            MainController.g_log.appendText(s);
                        });
                    }
                }
                bos.reset();
                try {
                    Thread.sleep(200);
                } catch (InterruptedException e) { }
            }
        });
        t.setDaemon(true);
        t.start();
    }
}
