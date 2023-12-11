#include <jni.h>
#include <string>
#include <thread>

#include "AnkiSocket.h"

// 假设您已经定义了 AnkiSocket 类
void startSocketOperation() {
    AnkiSocket receiveFileSocket;
    receiveFileSocket.ReceiveSocket();
}

extern "C" JNIEXPORT void JNICALL
Java_com_harry_apk_MainActivity_socketButtonFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "AnkiSocket init...");
    std::thread socketThread(startSocketOperation);
    socketThread.detach(); // 使线程独立执行
}

