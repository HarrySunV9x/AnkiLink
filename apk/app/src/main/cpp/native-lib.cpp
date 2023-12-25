#include <jni.h>
#include <string>
#include <thread>
#include <android/log.h>

#include "AnkiSocket.h"

// 全局变量，用于存储 Java 虚拟机 (JVM) 的指针。
JavaVM* gJvm = nullptr;

/**
 * 在 JNI 库被加载时调用。
 * 用于存储 JVM 的指针，以便在其他地方使用。
 *
 * @param vm Java 虚拟机指针。
 * @param reserved 保留参数。
 * @return 返回 JNI 版本。
 */
extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    gJvm = vm;
    return JNI_VERSION_1_6; // 指定 JNI 版本
}

/**
 * 在独立线程中执行 socket 操作的函数。
 * 此函数将在完成操作后调用 Java 层的回调。
 *
 * @param globalThiz 全局引用，指向 Java 层的对象。
 */
void startSocketOperation(jobject globalThiz) {
    JNIEnv *env;
    // 附加当前线程到 JVM。
    if (gJvm->AttachCurrentThread(&env, NULL) == JNI_OK) {
        AnkiSocket receiveFileSocket;
        receiveFileSocket.ReceiveSocket();

        // 获取 Java 层对象的类并找到回调方法的 ID。
        jclass clazz = env->GetObjectClass(globalThiz);
        jmethodID methodId = env->GetMethodID(clazz, "onSocketOperationComplete", "()V");

        // 调用 Java 层的回调方法。
        env->CallVoidMethod(globalThiz, methodId);

        // 清理全局引用并从 JVM 分离当前线程。
        env->DeleteGlobalRef(globalThiz);
        gJvm->DetachCurrentThread();
    }
}

/**
 * 由 Java 层调用的 native 方法。
 * 创建一个新线程执行 socket 操作，并在操作完成后通知 Java 层。
 *
 * @param env JNI 接口指针。
 * @param thiz 指向 Java 对象的引用。
 */
extern "C" JNIEXPORT void JNICALL
Java_com_harry_apk_MainActivity_socketButtonFromJNI(
        JNIEnv* env,
        jobject thiz /* this */) {
    __android_log_print(ANDROID_LOG_INFO, "AnkiLink", "AnkiSocket init...");

    // 创建指向 Java 对象的全局引用。
    jobject globalThiz = env->NewGlobalRef(thiz);

    // 创建并启动一个新线程执行 socket 操作。
    std::thread socketThread(startSocketOperation, globalThiz);
    socketThread.detach(); // 线程脱离，独立运行。
}
