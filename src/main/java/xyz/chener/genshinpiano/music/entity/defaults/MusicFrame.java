package xyz.chener.genshinpiano.music.entity.defaults;

import java.util.ArrayList;
import java.util.List;

public class MusicFrame {

    private final List<String> keys = new ArrayList<>();
    @Deprecated
    private int delay = 0;
    private int nextDelay = 0;

    public List<String> getKeys() {
        return keys;
    }

    public int getDelay() {
        return delay;
    }

    public void setDelay(int delay) {
        this.delay = delay;
    }

    public int getNextDelay() {
        return nextDelay;
    }

    public void setNextDelay(int nextDelay) {
        this.nextDelay = nextDelay;
    }
}
