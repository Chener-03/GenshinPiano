package xyz.chener.genshinpiano.music.jni;

import java.util.function.Supplier;

public class HotKey4J {

    public static native void start();
    public static native void stop();
    public static native boolean addEventListener(int event, int hwnd, Supplier<Boolean> fun);
    public static native boolean deleteEventListener(int event);
    public static native int addHotKey(int hwnd,String name,int modifiers,int key);
    public static native void deleteHotKey(int hwnd,int key);

    public static native void keyDown(int key);

    public static native void keyUp(int key);

    public static native String GetForegroundWindowTitle();

    public static native boolean getKeyIsDown(int keyCode);

    public static native long FindWindowW(String windowClass,String windowName);

    public static native long GetActiveWindow();

    public static native String GetWindowTextW(long hwnd);

    public static native void SetUrlStart();

    public static class Modifiers {
        public static final int MOD_ALT = 1;
        public static final int MOD_CONTROL = 2;
        public static final int MOD_NOREPEAT = 16384;
        public static final int MOD_SHIFT = 4;
        public static final int MOD_WIN = 8;
    }

}
