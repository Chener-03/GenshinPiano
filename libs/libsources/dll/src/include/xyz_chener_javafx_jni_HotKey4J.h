/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class xyz_chener_javafx_jni_HotKey4J */

#ifndef _Included_xyz_chener_javafx_jni_HotKey4J
#define _Included_xyz_chener_javafx_jni_HotKey4J
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_start
  (JNIEnv *, jclass);


JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_stop
  (JNIEnv *, jclass);


JNIEXPORT jboolean JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_addEventListener
  (JNIEnv *, jclass, jint, jint, jobject);


JNIEXPORT jboolean JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_deleteEventListener
  (JNIEnv *, jclass, jint);


JNIEXPORT jint JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_addHotKey
  (JNIEnv *, jclass, jint, jstring, jint, jint);


JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_deleteHotKey
  (JNIEnv *, jclass, jint, jint);



JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_keyDown
        (JNIEnv *, jclass, jint);

JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_keyUp
        (JNIEnv *, jclass, jint);


JNIEXPORT jstring JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_GetForegroundWindowTitle
        (JNIEnv *, jclass);

JNIEXPORT jboolean JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_getKeyIsDown
        (JNIEnv *, jclass, jint);


JNIEXPORT jlong JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_FindWindowW
        (JNIEnv *, jclass,  jstring, jstring);

JNIEXPORT jlong JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_GetActiveWindow
        (JNIEnv *, jclass );

JNIEXPORT jstring JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_GetWindowTextW
        (JNIEnv *, jclass ,jlong h);

JNIEXPORT void JNICALL Java_xyz_chener_genshinpiano_music_jni_HotKey4J_SetUrlStart
        (JNIEnv *, jclass);


#ifdef __cplusplus
}
#endif
#endif
