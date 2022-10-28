package xyz.chener.genshinpiano.music.prase.impl;

import xyz.chener.genshinpiano.music.entity.defaults.MusicData;
import xyz.chener.genshinpiano.music.entity.defaults.MusicFrame;
import xyz.chener.genshinpiano.music.keymapper.KeyMapper;
import xyz.chener.genshinpiano.music.prase.Prase;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;

public class PraseBG implements Prase<MusicData> {
    @Override
    public MusicData prase(String txt) {
        String[] split = txt.split("[;]");
        List<String> list = Arrays.asList(split);

        Map<String, String> keym2 = KeyMapper.getMap();
        MusicData md = new MusicData();
        ArrayList<String> cache = new ArrayList<>();
        list.forEach(e->{
            if (e==null || e.length()==0 || e.trim().length()==0 || e.contains("\t\n"))
                return;
            String substring = e.substring(0, 1);
            String time = e.substring(e.indexOf("(")+1, e.indexOf(")"));
            if (time == null || time.length()==0)
            {
                if (!substring.equals("t"))
                    cache.add(substring);
                return;
            }
            int i = Integer.parseInt(time);
            MusicFrame mf = new MusicFrame();
            if (!substring.equals("t"))
                cache.add(substring);
            List<String > strings1 = cache.stream().map(s2 -> keym2.get(s2.toLowerCase())).toList();
            mf.getKeys().addAll(strings1);
            mf.setNextDelay(i);
            md.getList().add(mf);
            cache.clear();
        });
        md.getList().add(new MusicFrame());
        md.setSupportAuto(true);
        md.setAuthor("此乐谱来自: 抖音-原神弹琴的波某人");
        md.setAddr("波某人q群: 524846830");
        return md;
    }
}
