package xyz.chener.genshinpiano.music.entity.defaults;

public class PlayData {
    private final MusicData musicData;

    private final int frame;

    private volatile int concurrentFrame = 0;

    public PlayData(MusicData musicData) {
        this.musicData = musicData;
        this.frame = musicData.getList().size();
    }

    public void concurrentFrameAdd()
    {
        this.concurrentFrameAdd(1);
    }

    public void concurrentFrameAdd(int i)
    {
        if (concurrentFrame+i >= frame)
            return;
        concurrentFrame+=i;
    }

    public MusicFrame getNext()
    {
        if (concurrentFrame >= this.musicData.getList().size())
            throw new RuntimeException("concurrentFrame too large");
        MusicFrame musicFrame = this.musicData.getList().get(concurrentFrame);
        this.concurrentFrameAdd();
        return musicFrame;
    }

    public boolean hasNext()
    {
        return concurrentFrame < frame-1;
    }

    public MusicData getMusicData() {
        return musicData;
    }

    public int getFrame() {
        return frame;
    }

    public int getConcurrentFrame() {
        return concurrentFrame;
    }

    public boolean setConcurrentFrame(int concurrentFrame) {
        if (concurrentFrame>=0 && concurrentFrame < frame)
        {
            this.concurrentFrame = concurrentFrame;
            return true;
        }
        return false;
    }

    public void reset()
    {
        setConcurrentFrame(0);
    }
}
