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

    // Used to load the 'ndkdemo' library on application startup.
    static {
        System.loadLibrary("ndkdemo");
    }

    private ActivityBaseBinding binding;

    /**
     * 从C++中获取字符串
     */
    public native String stringFromJNI();
    
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityBaseBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
    }
    
    public void onTest1(View v) {
        String str = stringFromJNI();
        Utils.log(str);
    }

    public void onTest2(View v) {

    }

    public void onTest3(View v) {

    }
    

    
}