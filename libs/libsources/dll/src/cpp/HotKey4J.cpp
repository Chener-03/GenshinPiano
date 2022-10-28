//
// Created by chen on 2022/10/13.
//
#include "../include/xyz_chener_javafx_jni_HotKey4J.h"
#include "../include/MessageProcess.h"
#include <iostream>
#include <locale>
#include <codecvt>

char* _JString2CStr(JNIEnv* env, jstring jstr) {
    char * rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("UTF-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr,mid,strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte * ba = env->GetByteArrayElements(barr,JNI_FALSE);
    if(alen > 0)
    {
        rtn = (char*)malloc(alen+1); //new char[alen+1];
        memcpy(rtn,ba,alen);
        rtn[alen]=0;
    }
    env->ReleaseByteArrayElements(barr,ba,0);
    return rtn;
}

jstring str2Jstring(JNIEnv* env, const char* pat)
{
    jclass strClass = env->FindClass("Ljava/lang/String;");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
    jstring encoding = env->NewStringUTF("utf-8");
    return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}


JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_start
        (JNIEnv *, jclass)
{
    auto ins = MessageProcess::getInstance();
    ins->startHook();
}


JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_stop
        (JNIEnv *, jclass)
{
    auto ins = MessageProcess::getInstance();
    ins->stopHook();
}

JNIEXPORT jboolean JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_addEventListener
        (JNIEnv * env, jclass clazz, jint event, jint hwnd, jobject fun)
{
    auto ins = MessageProcess::getInstance();
    auto supplierClass = env->FindClass("java/util/function/Supplier");
    auto getMethodId = env->GetMethodID(supplierClass,"get","()Ljava/lang/Object;");
    auto ref = env->NewGlobalRef(fun);
    FUNCTION fu  = [](JNIEnv * e,jmethodID i,jobject obj){
        return (bool)e->CallBooleanMethod(obj,i);
    };
    return ins->addEvent(event,hwnd,fu,env,getMethodId,ref);
}

JNIEXPORT jboolean JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_deleteEventListener
        (JNIEnv *, jclass, jint event)
{
    auto ins = MessageProcess::getInstance();
    return ins->deleteEvent(event);
}

JNIEXPORT jint JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_addHotKey
        (JNIEnv * env, jclass, jint hwnd, jstring name, jint modifiers, jint key)
{
    auto ins = MessageProcess::getInstance();
    return ins->addHotKey(_JString2CStr(env,name),hwnd,modifiers,key);
}

JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_deleteHotKey
        (JNIEnv *, jclass, jint hwnd, jint key)
{
    auto ins = MessageProcess::getInstance();
    ins->deleteHotKey(hwnd,key);
}




JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_keyDown
        (JNIEnv *, jclass, jint k)
{
    KEYBOARD_INPUT_DATA  kid;
    DWORD dwOutput;
    memset(&kid, 0, sizeof(KEYBOARD_INPUT_DATA));
    kid.Flags = 0;
    kid.MakeCode = (USHORT)MapVirtualKey(k, 0);
    DeviceIoControl(DeviceHandle, CTLKEYBORD, &kid, sizeof(KEYBOARD_INPUT_DATA), NULL, 0, &dwOutput, NULL);
}

JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_keyUp
        (JNIEnv *, jclass, jint k)
{
    KEYBOARD_INPUT_DATA  kid;
    DWORD dwOutput;
    memset(&kid, 0, sizeof(KEYBOARD_INPUT_DATA));
    kid.Flags = 1;
    kid.MakeCode = (USHORT)MapVirtualKey(k, 0);
    DeviceIoControl(DeviceHandle, CTLKEYBORD, &kid, sizeof(KEYBOARD_INPUT_DATA), NULL, 0, &dwOutput, NULL);
}



JNIEXPORT jstring JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_GetForegroundWindowTitle
        (JNIEnv * env, jclass)
{
    auto hwnd  = GetForegroundWindow();
    WCHAR text[256] = {0};
    GetWindowTextW(hwnd,text,256);
    std::wstring_convert<std::codecvt_utf8<wchar_t >> convert;
    return str2Jstring(env,convert.to_bytes(text).c_str());
}


JNIEXPORT jboolean JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_getKeyIsDown
        (JNIEnv *, jclass, jint key)
{
    return ( GetKeyState(key) >> 2) != 0;
}

JNIEXPORT jlong JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_FindWindowW
        (JNIEnv * e, jclass,  jstring c, jstring w)
{
    LPCWSTR c1=0,c2=0;
    if (c != nullptr)
    {
        std::string st1 = _JString2CStr(e,c);
        std::wstring st2(st1.begin(),st1.end());
        c1 = st2.c_str();
    }
    if (w != nullptr)
    {
        std::string st1 = _JString2CStr(e,w);
        std::wstring st2(st1.begin(),st1.end());
        c2 = st2.c_str();
    }
    if  (c == nullptr && w == nullptr) return NULL;
    return (UINT64)FindWindowW(c1,c2);
}

JNIEXPORT jlong JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_GetActiveWindow
        (JNIEnv *, jclass )
{
    return (UINT64)GetActiveWindow();
}

JNIEXPORT jstring JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_GetWindowTextW
        (JNIEnv * e, jclass ,jlong h)
{
    WCHAR name[256] = {0};
    GetWindowTextW((HWND)h,name,256);
    std::wstring wstr = name;
    std::string cstr(wstr.begin(),wstr.end());
    return str2Jstring(e,cstr.c_str());
}


void setUrlStart(wchar_t * exePath)
{
    HKEY hKEY;
    LPCTSTR data_Set=L"genshinpiano";
    wchar_t * w1 = (wchar_t *) L"genshinpiano Protocol";
    RegCreateKeyW(HKEY_CLASSES_ROOT,data_Set,&hKEY);
    RegSetKeyValueW(HKEY_CLASSES_ROOT, data_Set,L"URL Protocol",REG_SZ,0,0);
    RegSetKeyValueW(HKEY_CLASSES_ROOT, data_Set,L"",REG_SZ,w1,lstrlenW(w1)* sizeof(WCHAR)+1);

    data_Set=L"genshinpiano\\DefaultIcon";
    RegCreateKeyW(HKEY_CLASSES_ROOT,data_Set,&hKEY);
    RegSetKeyValueW(HKEY_CLASSES_ROOT, data_Set,L"",REG_SZ,exePath,lstrlenW(exePath)* sizeof(WCHAR)+1);

    data_Set=L"genshinpiano\\shell\\open\\command";
    RegCreateKeyW(HKEY_CLASSES_ROOT,data_Set,&hKEY);
    std::wstring wstr = exePath;
    wstr = L"\""+wstr + L"\" \"%1\"";
    auto o = wstr.c_str();
    RegSetKeyValueW(HKEY_CLASSES_ROOT, data_Set,L"",REG_SZ,o,lstrlenW(o)* sizeof(WCHAR)+1);
}

void setAdminStart(wchar_t * exePath)
{
    LPCTSTR data_Set=L"Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers";
    wchar_t * w1 = (wchar_t *) L"RUNASADMIN";
    RegSetKeyValueW(HKEY_CURRENT_USER, data_Set,exePath,REG_SZ,w1,lstrlenW(w1)* sizeof(WCHAR)+1);
}

JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_SetUrlStart
        (JNIEnv * e, jclass)
{
/*    if (str == nullptr)
        return;
    std::string astr = _JString2CStr(e,str);
    std::wstring wstr(astr.begin(),astr.end());
    wchar_t *ar =(wchar_t*) wstr.c_str();*/

    WCHAR exeFullPath[MAX_PATH];
    GetModuleFileName(NULL,exeFullPath,MAX_PATH);
    setUrlStart(exeFullPath);
    setAdminStart(exeFullPath);
}