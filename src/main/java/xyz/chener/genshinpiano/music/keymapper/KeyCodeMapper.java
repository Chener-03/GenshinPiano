package xyz.chener.genshinpiano.music.keymapper;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;


import java.io.InputStream;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public class KeyCodeMapper {
    private static final Map<String,Integer> map;
    static {
        // TODO: 2022/10/14 load resource
        try(InputStream is = KeyCodeMapper.class.getClassLoader().getResourceAsStream("keymap.json")) {
            ObjectMapper mapper = new ObjectMapper();
            Map<String, String> m = mapper.readValue(is, new TypeReference<Map<String, String >>() {});
            HashMap<String, Integer> m1 = new HashMap<>();
            m.forEach((k,v)->{
                m1.put(k,Integer.parseInt(v.substring(2),16));
            });
            map = Collections.unmodifiableMap(m1);
        }catch (Exception e)
        {
            throw new RuntimeException(e);
        }
    }

    public static Map<String,Integer> getMap()
    {
        return map;
    }
}
