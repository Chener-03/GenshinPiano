open module genshinpiano.main {

    requires javafx.controls;
    requires javafx.fxml;

    requires org.controlsfx.controls;
    requires com.dlsc.formsfx;
    requires org.kordamp.ikonli.javafx;
    requires org.kordamp.bootstrapfx.core;
    requires java.desktop;
    requires java.management;
    requires com.fasterxml.jackson.databind;
    requires okhttp3;
    requires okio;
    requires kotlin.stdlib;
    requires javafx.base;
    requires javafx.graphics;
    requires io.netty.transport;
    requires io.netty.all;
    requires io.netty.codec;


    exports xyz.chener.genshinpiano;
}