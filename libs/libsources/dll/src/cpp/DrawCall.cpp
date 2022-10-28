//
// Created by chen on 2022/10/22.
//
#include "../include/xyz_chener_javafx_jni_DrawCall.h"
#include "../include/tWind.h"
#include <iostream>
#include <locale>
#include <codecvt>


char* JString2CStr(JNIEnv* env, jstring jstr) {
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("UTF-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray)env->CallObjectMethod( jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements( barr, JNI_FALSE);
    if(alen > 0) {
        rtn = (char*)malloc(alen+1);
        memcpy(rtn, ba, alen);
        rtn[alen]=0;
    }
    env->ReleaseByteArrayElements( barr, ba,0);
    return rtn;
}

JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_DrawCall_addLine
        (JNIEnv *, jclass, jfloat a, jfloat b, jfloat c, jfloat d, jfloat e , jfloat f, jfloat g, jfloat h)
{

    ID2D1SolidColorBrush* brush;
    tWind::getInstance()->pd2dRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(f,g,h,1.f ), &brush);
    tWind::getInstance()->pd2dRenderTarget->DrawLine(
            D2D1::Point2F( a, b ),
            D2D1::Point2F( c, d ),
            brush, e );
    brush->Release();
}


JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_DrawCall_addRectangle
        (JNIEnv *, jclass, jfloat a, jfloat b, jfloat c, jfloat d, jfloat e , jfloat f , jfloat g , jfloat h)
{
    ID2D1SolidColorBrush* brush;
    tWind::getInstance()->pd2dRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(f,g,h,1.f ), &brush);
    tWind::getInstance()->pd2dRenderTarget->DrawRectangle(
            D2D1_RECT_F{ a, b, c, d},
            brush, e );
    brush->Release();
}


JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_DrawCall_addEllipse
        (JNIEnv *, jclass, jfloat a, jfloat b, jfloat c, jfloat d, jfloat e, jfloat f, jfloat g, jfloat h)
{
    ID2D1SolidColorBrush* brush;
    tWind::getInstance()->pd2dRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(f,g,h,1.f ), &brush);
    tWind::getInstance()->pd2dRenderTarget->DrawEllipse(D2D1_ELLIPSE{D2D1_POINT_2F{a,b},c,d},brush,e);
    brush->Release();

}



JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_DrawCall_addText
        (JNIEnv * env, jclass, jfloat a, jfloat b, jfloat c , jfloat d, jstring str , jfloat  f, jfloat g, jfloat h)
{
    ID2D1SolidColorBrush* brush;
    tWind::getInstance()->pd2dRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(f,g,h,1.f ), &brush);
    std::string astr = JString2CStr(env,str);
    std::wstring wstr;
    wstr.assign(astr.begin(),astr.end());
    tWind::getInstance()->pd2dRenderTarget->DrawTextW(
            wstr.c_str(), (UINT32)wstr.size(),tWind::getInstance()->pd2dTextFormat ,
            D2D1_RECT_F{ a, b, c, d }, brush);
    brush->Release();
}



JNIEXPORT jfloatArray JNICALL Java_xyz_chener_genshinpiano_music_jni_DrawCall_getColor
        (JNIEnv * env, jclass, jint i)
{
    float color[3] = {0};
    switch (i)
    {
        case 0:{
            color[0] = GVer::concurrentButtonClolr.x;
            color[1] = GVer::concurrentButtonClolr.y;
            color[2] = GVer::concurrentButtonClolr.z;
            break;
        }
        case 1:{
            color[0] = GVer::nextButtonClolr.x;
            color[1] = GVer::nextButtonClolr.y;
            color[2] = GVer::nextButtonClolr.z;
            break;
        }
        case 3:{
            color[0] = GVer::showKeyMapClolr.x;
            color[1] = GVer::showKeyMapClolr.y;
            color[2] = GVer::showKeyMapClolr.z;
            break;
        }
        default:{
            break;
        }
    }
    auto array = env->NewFloatArray(3);
    env->SetFloatArrayRegion(array,0,3,color);
    return array;
}



JNIEXPORT jint JNICALL Java_xyz_chener_genshinpiano_music_jni_DrawCall_getConcurrentResolvingPower
        (JNIEnv *, jclass)
{
    return GVer::fbl;
}



JNIEXPORT jfloat JNICALL Java_xyz_chener_genshinpiano_music_jni_DrawCall_getButtonSize
        (JNIEnv *, jclass)
{
    return GVer::buttonSize;
}

JNIEXPORT jboolean JNICALL Java_xyz_chener_genshinpiano_music_jni_DrawCall_getShowKey
        (JNIEnv *, jclass)
{
    return GVer::showKeyMap;
}


JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_DrawCall_addDrawCallback
        (JNIEnv * env, jclass, jobject obj)
{
    auto ref = env->NewGlobalRef(obj);
    auto supplierClass = env->FindClass("java/lang/Runnable");
    auto getMethodId = env->GetMethodID(supplierClass,"run","()V");
    JavaFunctionPointer jfp;
    JavaVM* jvm;
    env->GetJavaVM(&jvm);
    jfp.jvm = jvm;
    jfp.obj = ref;
    jfp.method = getMethodId;
    std::lock_guard<std::mutex> guard(tWind::getInstance()->drawFunctionsMutex);
    tWind::getInstance()->addDrawCallbackFunction(jfp);
}


