package xyz.chener.genshinpiano.gui;

import com.fasterxml.jackson.databind.ObjectMapper;
import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import xyz.chener.genshinpiano.Main;
import xyz.chener.genshinpiano.music.autoplay.DefaultAutoPlay;
import xyz.chener.genshinpiano.music.autoplay.DefaultStepPlay;
import xyz.chener.genshinpiano.music.entity.defaults.MusicData;
import xyz.chener.genshinpiano.music.entity.http.Rt;
import xyz.chener.genshinpiano.music.helper.GmHelper;
import xyz.chener.genshinpiano.music.utils.TheadPool;

import java.io.IOException;
import java.net.URL;
import java.net.URLDecoder;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.ResourceBundle;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class MainController implements Initializable {

    public static MainController mainController;

    @FXML
    private TextField searchText ;

    @FXML
    public TextArea logger;

    @FXML
    private ComboBox<String> combo;


    @FXML
    private Button playButton;

    @FXML
    private Button lxButton;

    @FXML
    private Button sdButton;

    public static TextArea g_log = null;

    private OkHttpClient client = new OkHttpClient();

    private List<Integer> concurrentComboxList = new ArrayList<>();

    @FXML
    protected void onPlayButtonClick() {
        if (playButton.getText().contains("停止"))
        {
            lxButton.setDisable(false);
            sdButton.setDisable(false);
            DefaultAutoPlay.getInstance().stop();
            playButton.setText("自动播放");
            return;
        }

        try {
            MusicData musicData = new MusicData();

            if (!getMusic(musicData)) {
                return;
            }
            DefaultAutoPlay instance = DefaultAutoPlay.getInstance();
            instance.setMustGenshinWindowTop(true);
            instance.play(musicData);

            if (playButton.getText().contains("自动播放"))
            {
                playButton.setText("停止");
                lxButton.setDisable(true);
                sdButton.setDisable(true);
            }
        }catch (Exception e){
            System.out.println("数据异常");
            System.out.println(e.getMessage());
        }
    }

    @FXML
    protected void onSearchButtonClick() {
        String text = searchText.getText();
        if (text == null || text.trim().length()==0)
        {
            System.out.println("搜索不能为空");
            return;
        }
        System.out.println("开始搜索:"+text);
         TheadPool.exec(()->{
            try {
                Request req = new Request.Builder().url("http://chener.xyz:8965/search?name=" + text)
                        .get().build();
                Response execute = client.newCall(req).execute();
                String res = execute.body().string();
                execute.close();
                ObjectMapper json = new ObjectMapper();
                Rt rt = json.readValue(res, Rt.class);
                if (rt.getCode()==406)
                {
                    System.out.println("服务器QPS过高,请稍后再试");
                    return;
                }
                if (rt.getCode()!=0)
                {
                    System.out.println("错误:"+rt.getCode());
                    return;
                }
                List<Map<String,String>> musics = (List<Map<String, String>>) rt.getObject();
                concurrentComboxList.clear();
                if (musics.size()==0)
                {
                    Platform.runLater(()->{
                        combo.getItems().clear();
                        combo.setValue("");
                        System.out.println("未找到");
                    });
                }
                else
                {
                    Platform.runLater(()->{
                        combo.getItems().clear();
                        musics.forEach(e->{
                            concurrentComboxList.add(Integer.parseInt(e.get("id")));
                            combo.getItems().add(e.get("musicName"));
                        });
                        combo.setValue(musics.get(0).get("musicName"));
                        System.out.println("搜索结束 找到"+musics.size()+"首");
                    });
                }

            }catch (Exception ex)
            {
                System.out.println("网络异常");
            }
        });

    }

    @FXML
    protected void onpracticeButtonClick() {
        try {
            if (lxButton.getText().equals("停止"))
            {
                GmHelper.getInstance().stop();
                System.out.println("End...");
                lxButton.setText("练习");
                playButton.setDisable(false);
                sdButton.setDisable(false);
                return;
            }
            MusicData musicData = new MusicData();
            if (getMusic(musicData)) {
                if (GmHelper.getInstance().start(musicData)) {
                    System.out.println("已开始按键提示,请在悬浮窗口上进行设置");
                    lxButton.setText("停止");
                    playButton.setDisable(true);
                    sdButton.setDisable(true);
                }
            }

        }catch (Exception e){
            System.out.println(e.getMessage());
        }
    }


    @FXML
    protected void onsdButtonClick() {
        try {
            if (sdButton.getText().equals("停止"))
            {
                DefaultStepPlay.getInstance().stop();
                playButton.setDisable(false);
                lxButton.setDisable(false);
                sdButton.setText("单步");
                return;
            }
            MusicData musicData = new MusicData();
            if (!getMusic(musicData)) {
                return;
            }
            DefaultStepPlay.getInstance().play(musicData);
            playButton.setDisable(true);
            lxButton.setDisable(true);
            sdButton.setText("停止");

        }catch (Exception e){}
    }


    @FXML
    public void jmpGit()
    {
        try {
            Runtime.getRuntime().exec("rundll32 url.dll,FileProtocolHandler " + "https://github.com/Chener-03");
        } catch (IOException e) {
            System.out.println("打开浏览器失败");
        }
    }

    @FXML
    public void onHelpClick()
    {
        final String help = """
                搜索后,下拉框选中,点击播放切如游戏即可自动播放
                更多请访问 http://chener.xyz:8966 
                """;
        System.out.println(help);
    }

    private boolean getMusic(MusicData _out_MusicData) throws IOException {
        if (concurrentComboxList.size()==0)
        {
            System.out.println("选择为空");
            return false;
        }
        g_log.appendText("正在获取数据...\n");
        int selectedIndex = combo.getSelectionModel().getSelectedIndex();
        long l = System.currentTimeMillis();
        Integer id = concurrentComboxList.get(selectedIndex);
        Request req = new Request.Builder().url("http://chener.xyz:8965/getData?id=" + id)
                .get().build();
        Response execute = client.newCall(req).execute();
        String str = execute.body().string();
        execute.close();
        ObjectMapper json = new ObjectMapper();
        Rt rt = json.readValue(str, Rt.class);

        if (rt.getCode()==406)
        {
            System.out.println("服务器QPS过高,请稍后再试");
            return false;
        }
        if (rt.getCode()!=0)
            throw new RuntimeException("错误");

        Map<String,String> object = (Map<String, String>) rt.getObject();
        String data = object.get("data");
        MusicData musicData = json.readValue(data, MusicData.class);
        _out_MusicData.setAddr(musicData.getAddr());
        _out_MusicData.setAuthor(musicData.getAuthor());
        _out_MusicData.setSupportAuto(musicData.isSupportAuto());
        _out_MusicData.getList().addAll(musicData.getList());
        System.out.println("歌曲 "+combo.getValue()+" 获取成功 耗时:"+(System.currentTimeMillis()-l)+"ms");
        System.out.println("作者:"+musicData.getAuthor());
        System.out.println("链接:"+musicData.getAddr());
        return true;
    }


    @Override
    public void initialize(URL location, ResourceBundle resources) {
        g_log = logger;
        mainController = this;
        System.out.println("--------日志---------");
        System.out.println("当前软件版本:"+ Main.version);
        TheadPool.exec(()->{
            try {
                Request req = new Request.Builder().url("http://chener.xyz:8965/countMusic")
                        .get().build();
                Response execute = client.newCall(req).execute();
                String res = execute.body().string();
                execute.close();
                Rt rt = new ObjectMapper().readValue(res, Rt.class);
                if (rt.getCode() == 0)
                {
                    System.out.println("曲库数量:"+rt.getObject());
                }
                req = new Request.Builder().url("http://chener.xyz:8965/getVersion")
                        .get().build();
                execute = client.newCall(req).execute();
                res = execute.body().string();
                execute.close();
                rt = new ObjectMapper().readValue(res, Rt.class);
                if (rt.getCode()==0 && !rt.getObject().equals(Main.version))
                {
                    System.out.println("有新版本咯,请去GitHub下载Release包");
                }
            }catch (Exception exception)
            {
                System.out.println("链接服务器失败");
            }
        });
        processArgs();
    }

    private void processArgs()
    {
        try {
            if (Main.args0.length == 0 || !Main.args0[0].contains("genshinpiano:"))
                return;
            String s = Main.args0[0];
            String name = s.substring(s.indexOf("://")+3).replace("/","");
            name = URLDecoder.decode(name, StandardCharsets.UTF_8);
            searchText.setText(name);
            onSearchButtonClick();
        }catch (Exception ignored){}
    }


    public void RpcCallAuto(int id)
    {
        if (id == -1)
        {
            concurrentComboxList.clear();
            ReentrantLock lsl = new ReentrantLock();
            Condition lsco = lsl.newCondition();
            try {
                lsl.lock();
                Platform.runLater(()->{
                    combo.getItems().clear();
                    onPlayButtonClick();
                    lsl.lock();
                    lsco.signalAll();
                    lsl.unlock();
                });
                lsco.await();
            }catch (Exception exception) {
                System.err.println(exception.getMessage());
            }
            finally {
                lsl.unlock();
            }
            return;
        }
        concurrentComboxList.clear();
        Platform.runLater(()->{
            concurrentComboxList.add(id);
            combo.getItems().clear();
            combo.getItems().add("rpc["+id+"]");
            combo.setValue("rpc["+id+"]");
            onPlayButtonClick();
        });
    }

}