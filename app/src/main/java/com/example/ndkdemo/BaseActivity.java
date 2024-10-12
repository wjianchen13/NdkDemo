package com.example.ndkdemo;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.example.ndkdemo.databinding.ActivityBaseBinding;
import com.example.ndkdemo.databinding.ActivityMainBinding;
import com.example.ndkdemo.jni_base.Student;

/**
 * JNI基础
 */
public class BaseActivity extends AppCompatActivity {

    private static final String TAG = BaseActivity.class.getSimpleName();
    private ActivityBaseBinding binding;

    private String name = "Derry"; // 等下 用 C++代码，修改为Beyond
    public static int age = 28; // 等下 用native代码 修改为：29
    private double number = 100000;
    private final double number2 = 900000; // 我想用C++暴力修改  get是没有问题， set在之前的版本有问题， 也没有问题了

    /**
     * 从C++中获取字符串
     */
    public native String stringFromJNI();
    public native void changeName(); // 改变我们的属性name
    public static native void changeAge(); // 改变我们属性age
    private native void changeNumber();
    public native void callAddMathod(); // c 调用java 方法---> public int add(int number1,int number2){}
    
    public native void testArrayAction(int count, String textInfo, int[] ints, String[] strs); // String引用类型，玩数组

    /**
     * 只操作Student对象里面的成员
     */
    public native void putObject(Student student, String str); // 传递引用类型，传递对象

    /**
     * 凭空创建Java对象
     */
    public native void insertObject();

    /**
     * 测试引用
     */
    public native void testQuote(); 

    /**
     * 释放全局引用
     */
    public native void delQuote(); 
    
    // "(II)I"
    // 被C调用的方法
    public int add(int number1,int number2){
        System.out.println("C居然调用了我");
        return number1+number2 + 90;
    }

    // (I)V
    public int add(int number1){
        return number1;
    }

    // 被C调用的方法 (Ljava/lang/String;I)Ljava/lang/String;
    public String showString(String str,int value) {
        System.out.println("C居然调用了我 showString str:" + str + " value:" + value);
        return "【" + str + "】";
    }
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityBaseBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
    }

    /**
     * 从C++中获取字符串
     * @param v
     */
    public void onTest1(View v) {
        String str = stringFromJNI();
        Utils.log(str);
        Utils.toast(this, str);
    }

    /**
     * 改变属性
     * @param v
     */
    public void onTest2(View v) {
        System.out.println("name修改前是：" + name);
        changeName();
        System.out.println("name修改后是：" + name);
        Utils.toast(this, name);
    }

    /**
     * 改变静态属性
     * @param v
     */
    public void onTest3(View v) {
        System.out.println("age修改前的值是："+ age);
        changeAge();
        System.out.println("age修改后的值是："+ age);
        Utils.toast(this, age + "");
    }

    /**
     * 修改私有变量
     * @param v
     */
    public void onTest4(View v) {
        System.out.println("number修改前的值是："+ number);
        changeNumber();
        System.out.println("number修改后的值是："+ number);
    }

    /**
     * 调用java方法
     * @param v
     */
    public void onTest5(View v) {
        callAddMathod();
    }

    /**
     * JNI数组操作
     * @param v
     */
    public void onTest6(View v) {
        int[] ints = new int[]{1, 2, 3, 4, 5, 6};
        String[] strs = new String[]{"李小龙", "李连杰", "李元霸"};
        testArrayAction(99, "你好", ints, strs);

        for (int anInt : ints) { // Java 输出 int 数组
            Log.d(TAG, "Java test01: java ints:" + anInt);
        }

        for (String str : strs) { // 输出 String 数组
            Log.e(TAG, "Java test01: java strs:" + str);
        }
    }

    /**
     * JNI对象操作
     * @param v
     */
    public void onTest7(View v) {
        Student student = new Student();
        student.name = "史泰龙";
        student.age = 88;
        putObject(student, "九阳神功");

        System.out.println("studnet:" + student.toString());
    }

    /**
     * JNI创建对象
     * @param v
     */
    public void onTest8(View v) {
        insertObject(); // Java层 不准你传递对象给C++,C++凭空创建对象
    }

    /**
     * 测试引用
     * @param v
     */
    public void onTest9(View v) {
        testQuote();
    }

    /**
     * 释放全局引用
     * @param v
     */
    public void onTest10(View v) {
        delQuote();
    }
}

// JNI_OK     本次C++的修改的数组， 刷新给JVM Java层， 并且释放C++数组
// JNI_COMMIT 本次C++的修改的数组， 刷新给JVM Java层
// JNI_ABORT  只释放C++数组

