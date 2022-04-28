package com.example.capstonedesign;

import android.os.Bundle;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {
    private TextView text;

    static {
        System.loadLibrary("helloNdk");
    }
    public native String print_ndk(String text);

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        text = (TextView) findViewById(R.id.text);

        String print = print_ndk("hello_ndk");

        text.setText(print);
    }
}