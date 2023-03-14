#include <jni.h>
#include <string>

// 日志输出
#include <android/log.h>

#define TAG "Derry"

// __VA_ARGS__ 代表 ...的可变参数
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG,  __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG,  __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,  __VA_ARGS__);

extern "C"
JNIEXPORT 
jstring JNICALL
Java_com_example_ndkdemo_BaseActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

/**
 * 改变我们的属性name
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_example_ndkdemo_BaseActivity_changeName(JNIEnv *env, jobject thiz) {
    // String  Student Person Object 引用类型 ==  JNI全部命名为 Object

    jclass mainActivityCls = env->FindClass("com/example/ndkdemo/BaseActivity"); // 方式一

    // jfieldID GetFieldID(jclass clazz, const char* name, const char* sig)
    jfieldID nameFid = env->GetFieldID(mainActivityCls, "name", "Ljava/lang/String;");

    // void SetObjectField(jobject obj, jfieldID fieldID, jobject value)
    jstring value = env->NewStringUTF("修改为Beyond");
    env->SetObjectField(thiz, nameFid, value);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_ndkdemo_BaseActivity_changeAge(JNIEnv *env, jclass mainActivityCls) {
    //  jfieldID GetStaticFieldID(jclass clazz, const char* name, const char* sig)
    jfieldID ageFid = env->GetStaticFieldID(mainActivityCls, "age", "I");

    // jint 背后就是int，所以可以直接用，但是String 必须用jstring
    int age = env->GetStaticIntField(mainActivityCls, ageFid); // 获取之前的age

    //  void SetStaticIntField(jclass clazz, jfieldID fieldID, jint value)
    env->SetStaticIntField(mainActivityCls, ageFid, age + 10);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_ndkdemo_BaseActivity_changeNumber(JNIEnv *env, jobject mainActivitThis) {
    jclass mainActivityCls = env->GetObjectClass(mainActivitThis);  // 方式二

    // jfieldID GetFieldID(jclass clazz, const char* name, const char* sig)
    jfieldID numberFid  = env->GetFieldID(mainActivityCls, "number", "D");

    // void SetDoubleField(jobject obj, jfieldID fieldID, jdouble value)
    env->SetDoubleField(mainActivitThis, numberFid, 885.55);


    // 修改 final number2 ========================

    jfieldID numberFid2  = env->GetFieldID(mainActivityCls, "number2", "D");

    env->SetDoubleField(mainActivitThis, numberFid2, 7777.99);

    double result = env->GetDoubleField(mainActivitThis, numberFid2);

    // __android_log_print(ANDROID_LOG_DEBUG, TAG,  __VA_ARGS__);
    LOGE("final result:%lf\n", result);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_ndkdemo_BaseActivity_callAddMathod(JNIEnv *env, jobject mainActivitThis) {
    jclass mainActivitCls = env->GetObjectClass(mainActivitThis);

    // jmethodID   (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID addMid = env->GetMethodID(mainActivitCls, "add", "(II)I");

    // jint CallIntMethod(jobject obj, jmethodID methodID, ...)
    int result = env->CallIntMethod(mainActivitThis, addMid, 1, 1);
    LOGD("result:%d\n", result)

    // ++++++++++++++++++++++ C调用 public String showString(String str,int value) 函数
    jmethodID showStringMid = env->GetMethodID(mainActivitCls, "showString", "(Ljava/lang/String;I)Ljava/lang/String;");
    // jobject     (*CallObjectMethod)(jobject, jmethodID, ...);
    jstring value = env->NewStringUTF("李元霸");
    jstring resultStr = (jstring) env->CallObjectMethod(mainActivitThis, showStringMid, value, 9527);
    const char * resultCstr = env->GetStringUTFChars(resultStr, NULL);
    LOGD("r==:%s\n", resultCstr)
}