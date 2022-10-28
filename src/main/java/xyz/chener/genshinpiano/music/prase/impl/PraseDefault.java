package xyz.chener.genshinpiano.music.prase.impl;

import xyz.chener.genshinpiano.music.entity.defaults.MusicData;
import xyz.chener.genshinpiano.music.entity.defaults.MusicFrame;
import xyz.chener.genshinpiano.music.prase.Prase;

import java.util.Arrays;
import java.util.List;

public class PraseDefault implements Prase<MusicData> {

    @Override
    public MusicData prase(String txt) {
        String[] spl = txt.split("\r\n");
        List<String> strings = Arrays.asList(spl);
        MusicData md = new MusicData();
        strings.forEach(e->{
            String s = e.trim();
            String[] split = s.split("[ ]");
            MusicFrame mf = new MusicFrame();
            Arrays.stream(split).forEach(m->{
                if (!m.equals("") && !m.equals(" "))
                {
                    mf.getKeys().add(m);
                }
            });
            md.getList().add(mf);
        });
        md.getList().add(new MusicFrame());
        md.setSupportAuto(false);
        return md;
    }
}
