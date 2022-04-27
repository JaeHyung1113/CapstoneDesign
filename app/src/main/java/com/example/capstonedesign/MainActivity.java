package com.example.capstonedesign;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;
import android.util.Log;

import android.os.Bundle;

public class MainActivity extends AppCompatActivity {
    private TextView text;

    static {
        System.loadLibrary("helloNdk");
    }
    public native String print_ndk(String text);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        text = (TextView) findViewById(R.id.text);
        String print = print_ndk("hello_ndk");
        Log.d("test",print);
        text.setText(print);
    }
}