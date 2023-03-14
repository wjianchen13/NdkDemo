package com.example.ndkdemo;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.example.ndkdemo.databinding.ActivityBaseBinding;
import com.example.ndkdemo.databinding.ActivityMainBinding;

/**
 * JNI基础
 */
public class BaseActivity extends AppCompatActivity {

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


}