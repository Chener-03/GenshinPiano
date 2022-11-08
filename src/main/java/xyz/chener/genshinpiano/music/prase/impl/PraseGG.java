package xyz.chener.genshinpiano.music.prase.impl;

import xyz.chener.genshinpiano.music.entity.defaults.MusicData;
import xyz.chener.genshinpiano.music.entity.defaults.MusicFrame;
import xyz.chener.genshinpiano.music.keymapper.KeyMapper;
import xyz.chener.genshinpiano.music.prase.Prase;
import xyz.chener.genshinpiano.music.prase.SourceType;

import java.util.ArrayList;

public class PraseGG implements Prase<MusicData> {
    @Override
    public MusicData prase(String txt) {
        int speed = findSpeed(txt);
        txt = txt.substring(0,txt.indexOf("+++++++++++++++")).replace("\r\n","");
        MusicData md = new MusicData();
        ArrayList<String> buffer = new ArrayList<>();
        boolean isEnd = false;
        int yc = 0;
        for (int i = 0; i < txt.length(); i++) {
            char c = txt.charAt(i);
            if (c >='a' && c<='z' || c>='A' && c <='Z')
            {
                if (isEnd)
                {
                    MusicFrame mf = new MusicFrame();
                    mf.setNextDelay(yc);
                    buffer.stream().forEach(ec->{
                        mf.getKeys().add(KeyMapper.getMap().get(ec.toLowerCase()));
                    });
                    buffer.clear();
                    yc = 0;
                    md.getList().add(mf);
                    isEnd = false;
                }
                buffer.add(String.valueOf(c));
            }
            if (c == '=' || c == '-' || c=='+')
            {
                isEnd = true;
                yc += toMs(speed,String.valueOf(c));
            }
        }
        md.setAddr("Q群:644482967");
        md.setAuthor("bilibili:呱呱能有什么坏心思");
        md.setSupportAuto(true);
        return md;
    }

    private int toMs(int speed,String ch)
    {
        switch (ch)
        {
            case "="->{
                return speed;
            }
            case "-"->{return speed*2;}
            case "+"->{return speed*4;}
            default -> {return 0;}
        }
    }


    private int findSpeed(String txt)
    {
        txt = txt.substring(txt.indexOf("按键精灵速度："));
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < txt.length(); i++) {
            char c = txt.charAt(i);
            if (c >= '0' && c <= '9')
            {
                sb.append(c);
            }
        }
        return Integer.parseInt(sb.toString());
    }
}
