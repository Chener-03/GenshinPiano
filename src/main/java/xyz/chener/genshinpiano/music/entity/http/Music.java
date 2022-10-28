package xyz.chener.genshinpiano.music.entity.http;



@SuppressWarnings("serial")
public class Music {


    private String id;
    
    private String musicName;
    
    private String data;


    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getMusicName() {
        return musicName;
    }

    public void setMusicName(String musicName) {
        this.musicName = musicName;
    }

    public String getData() {
        return data;
    }

    public void setData(String data) {
        this.data = data;
    }


}

