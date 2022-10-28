package xyz.chener.genshinpiano.music.helper;

import xyz.chener.genshinpiano.music.jni.DrawCall;
import xyz.chener.genshinpiano.music.jni.HotKey4J;

import java.awt.*;

/**
 * @Author: chenzp
 * @Date: 2022/10/21/10:44
 * @Email: chen@chener.xyz
 */
public class WindHelper {

    private static WindHelper windHelper = null;

    public static WindHelper getInstance()
    {
        if (windHelper == null)
        {
            synchronized (WindHelper.class)
            {
                if (windHelper == null)
                {
                    windHelper = new WindHelper();
                }
            }
        }
        return windHelper;
    }

    public static enum NoteState{
        CONCURRENT(0),NEXT(1),KEYMAP(3);
        private final int flag;
        NoteState(int i){
            this.flag = i;
        }
        @Override
        public String toString() {
            return String.valueOf(flag);
        }
        public int get()
        {
            return this.flag;
        }
    }

    private WindHelper()
    {
        DrawCall.addDrawCallback(drawCallBack);
    }

    private Runnable drawCallBack = new Runnable(){
        @Override
        public void run() {
            Config.KeyPointConfig pts = Config.KeyPointConfig.newInstance(DrawCall.getConcurrentResolvingPower());
            GmHelper.concurrentShowKeys.forEach(e->{
                if (e.contains("h"))
                {
                    doDraw(e.replace("h", ""),pts.getHp(),NoteState.CONCURRENT);
                }else if (e.contains("l"))
                {
                    doDraw(e.replace("l", ""),pts.getLp(),NoteState.CONCURRENT);
                }else
                {
                    doDraw(e,pts.getP(),NoteState.CONCURRENT);
                }
            });


            if (DrawCall.getShowKey())
            {
                for (int i = 0; i < pts.getHp().length; i++) {
                    doDrawKey(pts.getHp(), i);
                }
                for (int i = 0; i < pts.getP().length; i++) {
                    doDrawKey(pts.getP(), i);
                }
                for (int i = 0; i < pts.getLp().length; i++) {
                    doDrawKey(pts.getLp(), i);
                }
            }

        }

        private void doDrawKey(Config.KeyPointConfig.WinPoint[] pts, int i) {

            char key = (char) pts[i].key;
            var str = key + " " + (i +1);
            Point p = pts[i].point;
            float[] color = DrawCall.getColor(NoteState.KEYMAP.get());
            DrawCall.addText(p.x-15,p.y-12,p.x+30,p.y+20, str ,color[0],color[1],color[2]);
        }

        private void doDraw(String e, Config.KeyPointConfig.WinPoint[] pts,NoteState state) {
            Integer si = getInt(e);
            if (si != null && si>=1 && si <=7)
            {
                si-=1;
                boolean keyIsDown = HotKey4J.getKeyIsDown(pts[si].key);
                DrawCall.addEllipse((float) (pts[si].point.getX() ),(float) pts[si].point.getY()
                ,DrawCall.getButtonSize()+ (keyIsDown?5.0f:0.0f),DrawCall.getButtonSize()+ (keyIsDown?5.0f:0.0f)
                        ,3.5f,state.get());
            }
        }
    };


    private Integer getInt(String str)
    {
        try {
            return Integer.parseInt(str);
        }catch (Exception e){
            return null;
        }
    }

}
