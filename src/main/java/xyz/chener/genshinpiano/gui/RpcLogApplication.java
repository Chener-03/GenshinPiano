package xyz.chener.genshinpiano.gui;

import javafx.application.Application;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.TextArea;
import javafx.scene.image.Image;
import javafx.stage.Stage;
import org.kordamp.bootstrapfx.BootstrapFX;

public class RpcLogApplication{

    Stage stage = null;

    public void start() throws Exception {
        stage = new Stage();
        FXMLLoader fxmlLoader = new FXMLLoader(MainApplication.class.getClassLoader().getResource("xyz/chener/genshinpiano/log.fxml"));
        Parent load = fxmlLoader.load();
        Scene scene = new Scene(load);
        stage.setTitle("");
        stage.setScene(scene);
        stage.show();
    }

    public static void main0() {
        RpcLogApplication app = new RpcLogApplication();
        try {
            app.start();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

}
