package xyz.chener.genshinpiano.music.prase;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import xyz.chener.genshinpiano.music.prase.impl.PraseBG;
import xyz.chener.genshinpiano.music.prase.impl.PraseDefault;
import xyz.chener.genshinpiano.music.prase.impl.PraseDisomo;

public interface Prase <T> {

    T prase(String txt);

    default String toJson(String txt)
    {
        try {
            return new ObjectMapper().writerWithDefaultPrettyPrinter().writeValueAsString(prase(txt));
        } catch (JsonProcessingException e) {
            System.err.println(e.getMessage());
            return null;
        }
    }


    public static class PraseFactory {

        public static Prase get(SourceType type)
        {
            switch (type)
            {
                case BG -> {
                    return new PraseBG();
                }
                case DEFAULT -> {
                    return new PraseDefault();
                }
                case DOSIMO -> {
                    return new PraseDisomo();
                }
                default -> {
                    System.err.println("Unknown SourceType");
                    return null;
                }
            }
        }
    }

}
