//
// Created by chen on 2022/10/20.
//

#ifndef GLOBLEHOTKEY4J_GAMEVER_H
#define GLOBLEHOTKEY4J_GAMEVER_H

#include <windows.h>
#include "imgui.h"

namespace GVer{
    struct ImVec3Volatile{
        volatile float                                                     x, y, z;
        constexpr ImVec3Volatile()                                        : x(0.0f), y(0.0f), z(0.0f) { }
        constexpr ImVec3Volatile(float _x, float _y, float _z)  : x(_x), y(_y), z(_z) { }
    };

    extern float buttonSize  ;
    extern ImVec3Volatile concurrentButtonClolr ;
    extern ImVec3Volatile nextButtonClolr  ;
    extern int fbl  ;
    extern bool screenshotHidden ;
    extern bool showKeyMap;
    extern ImVec3Volatile showKeyMapClolr ;
}



#endif //GLOBLEHOTKEY4J_GAMEVER_H
