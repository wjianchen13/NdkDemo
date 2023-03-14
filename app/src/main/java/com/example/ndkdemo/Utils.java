package com.example.ndkdemo;

import android.content.Context;
import android.widget.Toast;

public class Utils {
    
    public static void log(String str) {
        System.out.println("======================> " + str);
    }
    
    public static void toast(Context context, String str) {
        Toast.makeText(context, str, Toast.LENGTH_SHORT).show();
    }
    
}
