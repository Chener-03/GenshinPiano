package xyz.chener.genshinpiano.music.utils;


public class Asserts {

    public static void assertBool(boolean expression,String message)
    {
        if (!expression) throw new RuntimeException(message);
    }

}
