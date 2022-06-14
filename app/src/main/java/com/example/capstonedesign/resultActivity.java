package com.example.capstonedesign;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.firestore.FirebaseFirestore;
import com.google.firebase.firestore.QueryDocumentSnapshot;
import com.google.firebase.firestore.QuerySnapshot;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;

public class resultActivity extends AppCompatActivity {
    private static final String TAG = "resultActivity";
    FirebaseFirestore db;
    TextView tv_result, tv_C, tv_M, tv_Y, tv_H, tv_S, tv_V, tv_color;

    String sC, sM, sY, sH, sS, sV;
    String personalColor;

    private RecyclerView recyclerView;
    private RecyclerView.Adapter mAdapter;
    private RecyclerView.LayoutManager layoutManager;
    List<String> list;
    String[] colorData;


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_result);
        Intent intent = getIntent();
        String data = intent.getStringExtra("rgb");
        Log.i(TAG, "data ->  " + data);
        processingData(data);
        resultPersonalColor();
        tv_result = (TextView) findViewById(R.id.tv_result);
        tv_result.setText(personalColor);
        tv_C = (TextView) findViewById(R.id.tv_C);
        tv_M = (TextView) findViewById(R.id.tv_M);
        tv_Y = (TextView) findViewById(R.id.tv_Y);
        tv_H = (TextView) findViewById(R.id.tv_H);
        tv_S = (TextView) findViewById(R.id.tv_S);
        tv_V = (TextView) findViewById(R.id.tv_V);
        tv_C.setText(" C: " + sC);
        tv_M.setText(" M: " + sM);
        tv_Y.setText(" Y: " + sY);
        tv_H.setText(" H: " + sH);
        tv_S.setText(" S: " + sS);
        tv_V.setText(" V: " + sV);
        tv_color = (TextView) findViewById(R.id.tv_color);
        recyclerView = (RecyclerView) findViewById(R.id.recyclerView);
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

        sC = String.valueOf((int) C * 100);
        sM = String.valueOf((int) (M * 100));
        sY = String.valueOf((int) (Y * 100));
        sH = String.valueOf((int) h);
        sS = String.valueOf((int) Math.floor(s * 100));
        sV = String.valueOf((int) Math.floor(v * 100));

        Log.i(TAG, "processingData: R': " + rgb[0] + " G': " + rgb[1] + " B': " + rgb[2] + " max: " + max);
        Log.i(TAG, "processingData: C: " + C + " M: " + M + " Y: " + Y + " K: " + K);
        Log.i(TAG, "processingData: H: " + h + " S: " + s + " V: " + v);
    }

    private void resultPersonalColor() {
        int checkM, checkY, checkS, checkV;
        checkM = Integer.parseInt(sM);
        checkY = Integer.parseInt(sY);
        checkS = Integer.parseInt(sS);
        checkV = Integer.parseInt(sV);

        if (checkM > checkY) {
            if (checkS < 50) {
                personalColor = "여름쿨톤";
                readData("summerCool");
            } else {
                personalColor = "겨울쿨톤";
                readData("winterCool");
            }

        } else {
            if (checkS > 50) {
                personalColor = "봄웜톤";
                readData("springWarm");
            } else {
                personalColor = "가을웜톤";
                readData("fallWarm");
            }
        }
    }

    private void readData(String pc) {
        list = new ArrayList<>();
        db = FirebaseFirestore.getInstance();
        db.collection(pc)
                .get()
                .addOnCompleteListener(new OnCompleteListener<QuerySnapshot>() {
                    @Override
                    public void onComplete(@NonNull Task<QuerySnapshot> task) {
                        if (task.isSuccessful()) {
                            for (QueryDocumentSnapshot document : task.getResult()) {
                                String hex = document.getString("HEX");
                                list.add(hex);
                                Log.i(TAG, "HEX: " + hex);
                            }
                        } else {
                            Log.w(TAG, "Error getting documents.", task.getException());
                        }
                       colorData = list.toArray(new String[list.size()]);

                        // 리사이클러뷰 사이즈 고정
                        recyclerView.setHasFixedSize(true);

                        // LinearLayoutManager로 리사이클러뷰의 세팅을 변경
                        layoutManager = new LinearLayoutManager(resultActivity.this);
                        recyclerView.setLayoutManager(layoutManager);

                        // 리사이클러뷰 어댑터
                        mAdapter = new recyclerViewAdapter(colorData);
                        recyclerView.setAdapter(mAdapter);
                    }
                });
    }
}
