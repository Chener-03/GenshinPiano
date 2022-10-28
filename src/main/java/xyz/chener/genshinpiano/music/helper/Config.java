package xyz.chener.genshinpiano.music.helper;

import xyz.chener.genshinpiano.music.keymapper.KeyCodeMapper;

import java.awt.*;


public class Config {

    public static class KeyPointConfig{
        public static enum resolvingPower
        {
            W_1440X900(0);
            private final int flag;
            resolvingPower(int i){
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

        public static class WinPoint{
            public Point point;
            public int key;
        }

        private final WinPoint[] hp = new WinPoint[7];
        private final WinPoint[] p = new WinPoint[7];
        private final WinPoint[] lp = new WinPoint[7];

        private KeyPointConfig(){}

        public static KeyPointConfig newInstance(int resolvingPower)
        {
            switch (resolvingPower)
            {
                case 0 ->{
                    return new KeyPointConfig().build1440x900();
                }
                default -> throw new RuntimeException("no such resolving power");
            }
        }

        private KeyPointConfig build1440x900()
        {
            int x1=344,x2=470,x3=597,x4=725,x5=849,x6=973,x7=1101;
            int y1 = 595,y2=699,y3=800;
            {

                WinPoint wp0 = new WinPoint();
                wp0.point = new Point(x1,y1);
                wp0.key = KeyCodeMapper.getMap().get("1h");
                hp[0] = wp0;

                WinPoint wp1 = new WinPoint();
                wp1.point = new Point(x2,y1);
                wp1.key = KeyCodeMapper.getMap().get("2h");
                hp[1] = wp1;

                WinPoint wp2 = new WinPoint();
                wp2.point = new Point(x3,y1);
                wp2.key = KeyCodeMapper.getMap().get("3h");
                hp[2] = wp2;

                WinPoint wp3 = new WinPoint();
                wp3.point = new Point(x4,y1);
                wp3.key = KeyCodeMapper.getMap().get("4h");
                hp[3] = wp3;

                WinPoint wp4 = new WinPoint();
                wp4.point = new Point(x5,y1);
                wp4.key = KeyCodeMapper.getMap().get("5h");
                hp[4] = wp4;

                WinPoint wp5 = new WinPoint();
                wp5.point = new Point(x6,y1);
                wp5.key = KeyCodeMapper.getMap().get("6h");
                hp[5] = wp5;

                WinPoint wp6 = new WinPoint();
                wp6.point = new Point(x7,y1);
                wp6.key = KeyCodeMapper.getMap().get("7h");
                hp[6] = wp6;
            }

            {
                WinPoint wp0 = new WinPoint();
                wp0.point = new Point(x1,y2);
                wp0.key = KeyCodeMapper.getMap().get("1");
                p[0] = wp0;

                WinPoint wp1 = new WinPoint();
                wp1.point = new Point(x2,y2);
                wp1.key = KeyCodeMapper.getMap().get("2");
                p[1] = wp1;

                WinPoint wp2 = new WinPoint();
                wp2.point = new Point(x3,y2);
                wp2.key = KeyCodeMapper.getMap().get("3");
                p[2] = wp2;

                WinPoint wp3 = new WinPoint();
                wp3.point = new Point(x4,y2);
                wp3.key = KeyCodeMapper.getMap().get("4");
                p[3] = wp3;

                WinPoint wp4 = new WinPoint();
                wp4.point = new Point(x5,y2);
                wp4.key = KeyCodeMapper.getMap().get("5");
                p[4] = wp4;

                WinPoint wp5 = new WinPoint();
                wp5.point = new Point(x6,y2);
                wp5.key = KeyCodeMapper.getMap().get("6");
                p[5] = wp5;

                WinPoint wp6 = new WinPoint();
                wp6.point = new Point(x7,y2);
                wp6.key = KeyCodeMapper.getMap().get("7");
                p[6] = wp6;
            }

            {
                WinPoint wp0 = new WinPoint();
                wp0.point = new Point(x1,y3);
                wp0.key = KeyCodeMapper.getMap().get("1l");
                lp[0] = wp0;

                WinPoint wp1 = new WinPoint();
                wp1.point = new Point(x2,y3);
                wp1.key = KeyCodeMapper.getMap().get("2l");
                lp[1] = wp1;

                WinPoint wp2 = new WinPoint();
                wp2.point = new Point(x3,y3);
                wp2.key = KeyCodeMapper.getMap().get("3l");
                lp[2] = wp2;

                WinPoint wp3 = new WinPoint();
                wp3.point = new Point(x4,y3);
                wp3.key = KeyCodeMapper.getMap().get("4l");
                lp[3] = wp3;

                WinPoint wp4 = new WinPoint();
                wp4.point = new Point(x5,y3);
                wp4.key = KeyCodeMapper.getMap().get("5l");
                lp[4] = wp4;

                WinPoint wp5 = new WinPoint();
                wp5.point = new Point(x6,y3);
                wp5.key = KeyCodeMapper.getMap().get("6l");
                lp[5] = wp5;

                WinPoint wp6 = new WinPoint();
                wp6.point = new Point(x7,y3);
                wp6.key = KeyCodeMapper.getMap().get("7l");
                lp[6] = wp6;
            }

            return this;
        }

        public WinPoint[] getHp() {
            return hp;
        }

        public WinPoint[] getP() {
            return p;
        }

        public WinPoint[] getLp() {
            return lp;
        }
    }



}
