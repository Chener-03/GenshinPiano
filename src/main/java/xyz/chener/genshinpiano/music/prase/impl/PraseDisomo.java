package xyz.chener.genshinpiano.music.prase.impl;

import xyz.chener.genshinpiano.music.entity.defaults.MusicData;
import xyz.chener.genshinpiano.music.entity.defaults.MusicFrame;
import xyz.chener.genshinpiano.music.keymapper.KeyMapper;
import xyz.chener.genshinpiano.music.prase.Prase;

import java.util.ArrayList;
import java.util.List;

public class PraseDisomo implements Prase<MusicData> {
    @Override
    public MusicData prase(String txt) {

        String[] spl = txt.split("\n");

        MusicData md = new MusicData();
        int lastT = 0;
        List<String> keys = new ArrayList<>();
        for (int i = 0; i < spl.length; i++)
        {
            String[] s = spl[i].split("[ ]");
            s[0] = KeyMapper.getMap().get(s[0].toLowerCase());
            if (i+1< spl.length)
            {
                String[] sn = spl[i+1].split("[ ]");
                lastT = Integer.parseInt(sn[1]);
            }else lastT=0;

            if (lastT == 0)
            {
                keys.add(s[0]);
            }else
            {
                keys.add(s[0]);
                MusicFrame mf = new MusicFrame();
                mf.getKeys().addAll(keys);
                mf.setNextDelay(lastT);
                md.getList().add(mf);
                keys.clear();
            }

            if (i == spl.length-1)
            {
                keys.add(s[0]);
                MusicFrame mf = new MusicFrame();
                mf.getKeys().addAll(keys);
                mf.setNextDelay(lastT);
                md.getList().add(mf);
                keys.clear();
            }

        }

        md.getList().add(new MusicFrame());
        md.setSupportAuto(true);
        md.setAuthor("此乐谱来自: DoMiSo社区下作者 (遵循CC BY-SA 4.0 License)");
        md.setAddr("https://github.com/Nigh/DoMiSo-genshin");
        return md;
    }
}
