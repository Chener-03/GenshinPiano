package xyz.chener.genshinpiano.music.entity.defaults;

import java.util.ArrayList;
import java.util.List;


public class MusicData {

    private final List<MusicFrame> list = new ArrayList<>();

    private boolean supportAuto = false;

    private String author = null;

    private String addr = null;

    public String getAddr() {
        return addr;
    }

    public void setAddr(String addr) {
        this.addr = addr;
    }

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public boolean isSupportAuto() {
        return supportAuto;
    }

    public void setSupportAuto(boolean supportAuto) {
        this.supportAuto = supportAuto;
    }

    public List<MusicFrame> getList() {
        return list;
    }
}
