package xyz.chener.genshinpiano;


import xyz.chener.genshinpiano.gui.MainApplication;
import xyz.chener.genshinpiano.tcpctl.RpcCtl;

import javax.swing.*;

import static javax.swing.JOptionPane.YES_NO_OPTION;

public class Main {

    public static final String version = "0.0.1 beta";

    public static String[] args0 = null;

    public static void main(String[] args) throws Exception {
        args0 = args;
        showStatement();
        Class.forName("xyz.chener.genshinpiano.music.utils.LoggerImpl");
        System.loadLibrary("libs/globleHotKey4J");
        RpcCtl.checkParam(args);
        //HotKey4J.SetUrlStart();
        MainApplication.main0(args);
    }

    private static void showStatement()
    {
        String statement = """
                乐谱均来自全网收集,并标注出处
                本软件将永久免费开源,并持续维护
                如果本软件有任何侵权行为,请尽快联系作者删除
                诚邀各路乐谱大神的加入
                
                是否了解以上协议?
                """;
        int i = JOptionPane.showConfirmDialog(null, statement,"声明",YES_NO_OPTION);
        if (i!=0)
            System.exit(0);
    }

}
