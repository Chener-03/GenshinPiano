package xyz.chener.genshinpiano.gui;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.TextArea;
import xyz.chener.genshinpiano.tcpctl.MsInfo;
import xyz.chener.genshinpiano.tcpctl.RpcCtl;

import java.net.URL;
import java.util.List;
import java.util.ResourceBundle;

public class RpcLogController implements Initializable {
    public static RpcLogController rpcLogController = null;
    @FXML
    private TextArea text;
    @Override
    public void initialize(URL location, ResourceBundle resources) {
        rpcLogController = this;
    }

    public void set(List<MsInfo> list)
    {
        StringBuilder sb = new StringBuilder();
        sb.append("点歌请发送点歌+歌名 eg:点歌富士山下\n\n");
        sb.append("当前播放:").append(RpcCtl.concurrentMusicName).append("\n\n");
        sb.append("当前播放顺序:\n");
        list.forEach(e->{
            sb.append(e.getMusicName()).append("\n");
        });
        Platform.runLater(()->{
            text.clear();
            text.setText(sb.toString());
        });
    }
}
