package com.example.capstonedesign;

import static android.Manifest.permission.CAMERA;
import static android.Manifest.permission.WRITE_EXTERNAL_STORAGE;
import static android.Manifest.permission.READ_EXTERNAL_STORAGE;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.util.ArrayList;
import java.util.List;

public class MultiplePermission {

    private static final int PERMISSION_REQUEST_CODE = 803;

    private Context context;
    private Activity activity;

    private String[] permissions = {
            CAMERA,
            WRITE_EXTERNAL_STORAGE,
            READ_EXTERNAL_STORAGE
    };

    private List permissionList;

    public MultiplePermission(Activity _activity, Context _context) {
        this.activity = _activity;
        this.context = _context;
    }

    public boolean checkPermission() {
        int result;
        permissionList = new ArrayList<>();

        for (String pm : permissions) {
            result = ContextCompat.checkSelfPermission(context, pm);
            if (result != PackageManager.PERMISSION_GRANTED) {
                permissionList.add(pm);
            }
            if (!permissionList.isEmpty()) {
                return false;
            }
        }
        return true;
    }

    public void requestPermission() {
        ActivityCompat.requestPermissions(activity, (String[]) permissionList.toArray(new String[permissionList.size()]), PERMISSION_REQUEST_CODE);
    }

    public boolean permissionResultRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode == PERMISSION_REQUEST_CODE && grantResults.length > 0) {
            // grantResult = -1 거부, 0 허용
            for (int i = 0; i < grantResults.length; i++) {
                if (grantResults[i] == -1) {
                    return false;
                }
            }
        }
        return true;
    }
}
