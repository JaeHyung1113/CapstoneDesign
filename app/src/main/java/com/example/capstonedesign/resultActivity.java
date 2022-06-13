package com.example.capstonedesign;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import java.util.StringTokenizer;

public class resultActivity extends AppCompatActivity {
    private static final String TAG = "resultActivity";

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_result);

        Intent intent = getIntent();
        String data = intent.getStringExtra("rgb");
        Log.i(TAG, "data ->  " + data);
        processingData(data);
    }

    private void processingData(String data) {
        StringTokenizer stk = new StringTokenizer(data, "//");
        float[] rgb = new float[3];
        int i = 0;
        float C = 0, Y = 0, M = 0, K = 0;
        float max, min, v, s, h;
        while (stk.hasMoreTokens()) {
            rgb[i] = Float.parseFloat(stk.nextToken());
            i++;
        }
        max = Math.max(rgb[0], rgb[1]);
        max = Math.max(max, rgb[2]);

        min = Math.min(rgb[0], rgb[1]);
        min = Math.min(min, rgb[2]);

        v = max;
        s = (max != 0.0) ? (max - min) / max : (float) 0.0;
        h = (float) 0.0;


        float delta = max - min;
        if (rgb[0] == max)
            h = (rgb[1] - rgb[2]) / delta;         // 색상이 Yello와 Magenta사이
        else if (rgb[1] == max)
            h = (float) (2.0 + (rgb[2] - rgb[0]) / delta);     // 색상이 Cyan와 Yello사이
        else if (rgb[2] == max)
            h = (float) (4.0 + (rgb[0] - rgb[1]) / delta);    // 색상이 Magenta와 Cyan사이
        h *= 60.0;
        if (h < 0.0)                            // 색상값을 각도로 바꿈
            h += 360.0;


        K = 1 - max;
        C = (1 - rgb[0] - K) / (1 - K);
        M = (1 - rgb[1] - K) / (1 - K);
        Y = (1 - rgb[2] - K) / (1 - K);

        Log.i(TAG, "processingData: R': " + rgb[0] + " G': " + rgb[1] + " B': " + rgb[2] + " max: " + max);
        Log.i(TAG, "processingData: C: " + C + " M: " + M + " Y: " + Y + " K: " + K);
        Log.i(TAG, "processingData: H: " + h + " S: " + s + " V: " + v);

    }
}
