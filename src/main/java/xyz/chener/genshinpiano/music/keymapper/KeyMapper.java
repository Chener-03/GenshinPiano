package xyz.chener.genshinpiano.music.keymapper;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.InputStream;
import java.util.Collections;
import java.util.Map;

public class KeyMapper {
    private static final Map<String,String > map;
    static {
        try(InputStream is = KeyCodeMapper.class.getClassLoader().getResourceAsStream("keymap2.json")) {
            ObjectMapper mapper = new ObjectMapper();
            Map<String, String> m = mapper.readValue(is, new TypeReference<Map<String, String >>() {});
            map = Collections.unmodifiableMap(m);
        }catch (Exception e)
        {
            throw new RuntimeException(e);
        }
    }

    public static Map<String,String> getMap()
    {
        return map;
    }
}
