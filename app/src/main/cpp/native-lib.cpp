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

extern "C"
JNIEXPORT void JNICALL
Java_com_example_ndkdemo_BaseActivity_testArrayAction(JNIEnv *env, jobject thiz, jint count,
                                                      jstring text_info, jintArray ints,
                                                      jobjectArray strs) {
    int _count = count;
    LOGD("C++ _count:%d\n", count)

    // 非0就是true， 0就是false
    const char *_text_info = env->GetStringUTFChars(text_info, NULL); // 0false 在本地内部完成翻译转换，不需要开辟Copy机制转换
    LOGD("C++ _text_info:%s\n", _text_info)

    // 作为C工程师，要记住一句话（时时刻刻想着回收你的代码）
    env->ReleaseStringUTFChars(text_info, _text_info); // 已经释放了

    // TODO =============================  下面是 int[] 基本类型的数组

    jint *_ints = env->GetIntArrayElements(ints, NULL);

    // sizeof(_ints) / sizeof(int) 你还停留在c工程师的思想

    // jarray父类   jintArray子类   jobjectArray子类     jxxxArray子类
    int intsLen = env->GetArrayLength(ints);
    for (int i = 0; i < intsLen; ++i) {
        // 挪动指针 变量指针 就是 遍历数组
        LOGI("修改前 C++ ints item:%d\n", *(_ints+i))

        *(_ints+i) = (i + 100001);

        LOGI("修改后 C++ ints item:%d\n", *(_ints+i))
    }

    // JNI_OK     本次C++的修改的数组， 刷新给JVM Java层， 并且释放C++数组
    // JNI_COMMIT 本次C++的修改的数组， 刷新给JVM Java层
    // JNI_ABORT  只释放C++数组
    env->ReleaseIntArrayElements(ints, _ints, JNI_OK); // 已经释放了

    // TODO =============================  下面是 引用类型的 数组  Student[]  Person[]  Test[]  String[]
    // env->GetIntArrayElements(ints, NULL);

    // jobject GetObjectArrayElement(jobjectArray array, jsize index)
    // jsize index == 灵感 index 这个是不是 引用类型数组的 下标，  这句话就应该在 for里面 env->GetObjectArrayElement()
    int strsLen  = env->GetArrayLength(strs);
    for (int i = 0; i < strsLen; ++i) {
        jstring strItemS = (jstring) env->GetObjectArrayElement(strs, i);
        const char * strItemC = env->GetStringUTFChars(strItemS, NULL);
        LOGI("修改前 C++ strItemC:%s\n", strItemC)

        // 我不用了，就要释放，这个是规范 （养成好习惯）
        env->ReleaseStringUTFChars(strItemS,strItemC); // 已经释放了

        // env->ReleaseIntArrayElements(ints, _ints, JNI_OK);
        // env.ReleaseObj

        // 寻找灵感
        jstring updateValue = env->NewStringUTF("Beyond"); // 给上层用
        env->SetObjectArrayElement(strs, i, updateValue); // 内部会操纵杆刷新

        jstring strItemS2 = (jstring) env->GetObjectArrayElement(strs, i);
        const char * strItemC2 = env->GetStringUTFChars(strItemS2, NULL);
        LOGI("修改后 C++ strItemC2:%s\n", strItemC2)

        // 我不用了，就要释放，这个是规范
        env->ReleaseStringUTFChars(strItemS2,strItemC2); // 已经释放了

        // 20000行代码
        // ...
    } // JNI函数结束，会自动释放，所有的局部成员
    
    
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_ndkdemo_BaseActivity_putObject(JNIEnv *env, jobject thiz, jobject student,
                                                jstring str) {
    // 最简单的搞定
    const char * _str = env->GetStringUTFChars(str, NULL);
    LOGD("_str:%s\n", _str)
    // 作为C工程师，要记住一句话（时时刻刻想着回收你的代码）
    env->ReleaseStringUTFChars(str, _str); // 已经释放了

    // 只玩Student对象里面的成员

    jclass studentClass = env->FindClass("com/example/ndkdemo/jni_base/Student");
    jmethodID toStringMethod = env->GetMethodID(studentClass, "toString", "()Ljava/lang/String;");
    jstring toStringValueS = (jstring) env->CallObjectMethod(student, toStringMethod);

    const char * toStringValueC = env->GetStringUTFChars(toStringValueS, NULL);
    LOGD("toStringValueC:%s\n", toStringValueC);
    env->ReleaseStringUTFChars(toStringValueS, toStringValueC); // 已经释放了

    // setName
    jmethodID setNameMethod = env->GetMethodID(studentClass, "setName", "(Ljava/lang/String;)V");
    jstring value1 = env->NewStringUTF("李元霸");
    env->CallVoidMethod(student, setNameMethod, value1);

    // getName
    jmethodID getNameMethod = env->GetMethodID(studentClass, "getName", "()Ljava/lang/String;");
    jstring nameS = (jstring) env->CallObjectMethod(student, getNameMethod);
    const char * nameC = env->GetStringUTFChars(nameS, NULL);
    LOGD("nameC:%s\n", nameC)

    // setAge
    jmethodID setAgeMethod = env->GetMethodID(studentClass, "setAge", "(I)V");
    env->CallVoidMethod(student, setAgeMethod, 99);

    // getAge
    jmethodID getAgeMethod = env->GetMethodID(studentClass, "getAge", "()I");
    int age1 = env->CallIntMethod(student, getAgeMethod);
    LOGD("age1:%d\n", age1)

    // void showInfo(String)
    jmethodID showInfo = env->GetStaticMethodID(studentClass, "showInfo", "(Ljava/lang/String;)V");
    jstring value2 = env->NewStringUTF("静态的函数 李元霸");
    env->CallStaticVoidMethod(studentClass, showInfo, value2);

    env->DeleteLocalRef(studentClass); // 已经释放了

    // 基操勿6
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_ndkdemo_BaseActivity_insertObject(JNIEnv *env, jobject thiz) {
    // @Student对象
    jclass studentClass = env->FindClass("com/example/ndkdemo/jni_base/Student");

    jobject student = env->AllocObject(studentClass);

    // setName
    jmethodID setNameMethod = env->GetMethodID(studentClass, "setName", "(Ljava/lang/String;)V");
    jstring value1 = env->NewStringUTF("刘奋");
    env->CallVoidMethod(student, setNameMethod, value1);
    // setAge
    jmethodID setAgeMethod = env->GetMethodID(studentClass, "setAge", "(I)V");
    env->CallVoidMethod(student, setAgeMethod, 99);

    jclass personClass = env->FindClass("com/example/ndkdemo/jni_base/Person");
    jobject person = env->AllocObject(personClass); // C++ 分配一个对象出来，不会调用此对象的构造函数
    // env->NewObject();  // C++ 实例化一个对象出来，会调用此对象的构造函数，相当于： new XXX();

    // void setStudent(Student student)

    // void CallVoidMethod(person obj, jmethodID methodID, ...)
    jmethodID setStudent = env->GetMethodID(personClass, "setStudent", "(Lcom/example/ndkdemo/jni_base/Student;)V");
    env->CallVoidMethod(person, setStudent, student);

    // static void putStudent
    jmethodID putStudent = env->GetStaticMethodID(personClass, "putStudent", "(Lcom/example/ndkdemo/jni_base/Student;)V");
    env->CallStaticVoidMethod(personClass, putStudent, student);

    // TODO 释放工作
    // 释放方式一
    env->DeleteLocalRef(personClass);
    env->DeleteLocalRef(student);
    env->DeleteLocalRef(value1);
    env->DeleteLocalRef(person);
    // 释放方式二
    /*env->GetStringUTFChars()
    env->ReleaseStringUTFChars()*/

    // 这个就是你自己负责的
    /*new StudentCPP对象
    delete StudentCPP对象*/
}

jclass dogClass = nullptr; // 在Java的思想中，这个就是全局变量 全局成员，注意了：现在是JNI，这句话 依然是 局部成员

extern "C"
JNIEXPORT void JNICALL
Java_com_example_ndkdemo_BaseActivity_testQuote(JNIEnv *env, jobject thiz) {
    if(!dogClass) { // dogClass == nullptr
        // dogClass = env->FindClass("com/derry/as_jni_2/Dog");

        // 提升全局成员
        jclass temp = env->FindClass("com/example/ndkdemo/jni_base/Dog");
        dogClass = (jclass) env->NewGlobalRef((jobject)temp); // 把 dogClass 提升全局成员
        env->DeleteLocalRef(temp);
    }

    // env->NewObject();  // C++ 实例化一个对象出来，会调用此对象的构造函数，相当于： new XXX();
    jmethodID dogInit = env->GetMethodID(dogClass, "<init>", "()V");
    jobject dog = env->NewObject(dogClass, dogInit); // Dog dog = new Dog();

    jmethodID dogInit1 = env->GetMethodID(dogClass, "<init>", "(I)V");
    jobject dog2 = env->NewObject(dogClass, dogInit1, 1); // Dog dog = new Dog(1);

    jmethodID dogInit3 = env->GetMethodID(dogClass, "<init>", "(II)V");
    jobject dog3 = env->NewObject(dogClass, dogInit3, 1, 2); // Dog dog = new Dog(1, 2);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_ndkdemo_BaseActivity_delQuote(JNIEnv *env, jobject thiz) {
    if (dogClass) { // dogClass != null
        env->DeleteGlobalRef(dogClass); // JNI函数弹栈，会自动释放所有的“局部成员”，释放后，不会置为NULL
        dogClass = nullptr;
        LOGI("手动释放全局引用成功success...")
    }
}