package xyz.chener.genshinpiano.music.utils;

import java.util.LinkedHashMap;
import java.util.Map;


public class MapBuilder<K,V> {

    private MapBuilder(){};

    private Map<K,V> map;

    public static <K,V> MapBuilder <K,V>getIns()
    {
        MapBuilder<K,V> mb = new MapBuilder<>();
        mb.map = new LinkedHashMap<>();
        return mb;
    }

    public MapBuilder <K,V>put(K key,V value)
    {
        map.put(key, value);
        return this;
    }


    public Map<K ,V> build()
    {
        return map;
    }

}
