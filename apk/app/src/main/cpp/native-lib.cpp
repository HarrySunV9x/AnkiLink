#include <jni.h>
#include <string>

#include "AnkiSocket.h"

void startSocket(){
    AnkiSocket ankiSocket;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_harry_apk_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "AnkiSocket init...");
    startSocket();
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

