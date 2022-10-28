package xyz.chener.genshinpiano;

import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.stage.Stage;
import javafx.stage.Window;
import javafx.stage.WindowEvent;
import okhttp3.OkHttpClient;
import org.kordamp.bootstrapfx.BootstrapFX;
import xyz.chener.genshinpiano.music.autoplay.DefaultStepPlay;
import xyz.chener.genshinpiano.music.helper.WindHelper;
import xyz.chener.genshinpiano.music.jni.HotKey4J;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Method;
import java.nio.file.Files;
import java.nio.file.Path;


public class MainApplication extends Application {
    public static  final String title = "GenshinPiano";

    public static long HWND = 0L;

    @Override
    public void start(Stage stage) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(MainApplication.class.getClassLoader().getResource("xyz/chener/genshinpiano/hello-view.fxml"));
        Parent load = fxmlLoader.load();
        Scene scene = new Scene(load, 638, 389);
        stage.setTitle(title);
        stage.setScene(scene);
        stage.setResizable(false);
        stage.setOnCloseRequest(event -> System.exit(0));
        stage.getScene().getStylesheets().add(BootstrapFX.bootstrapFXStylesheet());
        stage.getIcons().add(new Image("icon.png"));
        stage.show();
        WindHelper.getInstance();
        HWND = HotKey4J.GetActiveWindow();
    }


    public static void main0(String[] args) {
        launch();
    }


}