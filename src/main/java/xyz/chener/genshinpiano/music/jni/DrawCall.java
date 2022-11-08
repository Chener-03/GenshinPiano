package xyz.chener.genshinpiano.music.jni;

import xyz.chener.genshinpiano.music.helper.Config;
import xyz.chener.genshinpiano.music.helper.WindHelper;
import xyz.chener.genshinpiano.music.utils.Asserts;

public class DrawCall {

    public static native void addLine(float x1,float y1,float x2,float y2,float width ,float r,float g,float b);
    public static native void addRectangle(float x,float y,float w,float h,float width ,float r,float g,float b);
    public static native void addEllipse(float x,float y,float xrideo,float yrideo,float width ,float r,float g,float b);
    public static native void addText(float x1,float y1,float x2,float y2,String text,float r,float g,float b );


    /**
     * 获取按钮颜色
     * @param noteState 按钮状态 {@link WindHelper.NoteState}
     * @return
     */
    public static native float[] getColor(int noteState);

    /**
     * 获取当前分辨率
     * @return {@link Config.KeyPointConfig.resolvingPower}
     */
    public static native int getConcurrentResolvingPower();
    public static native float getButtonSize();

    public static native boolean getShowKey();

    public static native void addDrawCallback(Runnable runnable);


    public static native void shutDownDraw();

    public static void addEllipse(float x,float y,float xrideo,float yrideo,float width,int noteState)
    {
        float[] color = getColor(noteState);
        Asserts.assertBool(color.length==3,"r g b array is null");
        addEllipse(x, y, xrideo, yrideo, width,color[0],color[1],color[2]);
    }

}
