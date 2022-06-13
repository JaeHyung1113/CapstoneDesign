package com.example.capstonedesign;

import static android.Manifest.permission.CAMERA;
import static android.Manifest.permission.READ_EXTERNAL_STORAGE;
import static android.Manifest.permission.WRITE_EXTERNAL_STORAGE;

import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.media.ExifInterface;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.ParcelFileDescriptor;
import android.provider.MediaStore;
import android.provider.Settings;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.FileProvider;

import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.imgcodecs.Imgcodecs;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

public class MainActivity extends AppCompatActivity {

    static final int REQUEST_IMAGE_CAPTURE = 333;
    private final static int REQUEST_IMAGE_OPEN = 777;
    private MultiplePermission permission;

    static File currentPhotoFile;
    static Uri currentPhotoUri;
    static String currentPhotoPath;
    static String currentPhotoFileName;

    private Uri getImageUri;
    private Uri newImageFileUri;
    private String imagePath;

    Button check;
    Button capture;
    Button get;
    Button test;

    ImageView imageView;

    private Mat matInput;
    private Mat matResult;

    static {
        System.loadLibrary("detect_skin");
    }
    public native long loadCascade(String cascadeFileName);
    public native void loadImage(String imageFileName, long img);
    public native void detectSkin(long cascadeClassifier_face, long addrInputImage, long addrResultImage);
    public long cascadeClassifier_face = 0;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        permissionCheck();

        imageView = findViewById(R.id.imageView);
        imageView.setVisibility(View.INVISIBLE);

        test = (Button) findViewById(R.id.btn_test);
        test.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), CameraActivity.class);
                startActivity(intent);
            }
        });
        capture = (Button) findViewById(R.id.btn_capture);
        capture.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    capturePhoto();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        check = (Button) findViewById(R.id.btn_check);
        check.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                read_cascade_file();
                matInput = new Mat();
                //loadImage(getPath(getImageUri), matInput.getNativeObjAddr());
                matInput = Imgcodecs.imread(imagePath, Imgcodecs.IMREAD_COLOR);
                Log.i("main", "이미지 경로: " + imagePath);
                // imageView의 resource를 bitmap으로 가져옴
//                BitmapDrawable drawable = (BitmapDrawable) imageView.getDrawable();
//                Bitmap bitmap = drawable.getBitmap();
//
//                int w = bitmap.getWidth();
//                int h = bitmap.getHeight();
//
//                Bitmap resize = bitmap.createScaledBitmap(bitmap, w / 4, h / 4, true);
//
//
//                Bitmap bmp32 = resize.copy(Bitmap.Config.ARGB_8888, true);
//                Utils.bitmapToMat(bmp32, matInput);

                if (matResult == null)
                    matResult = new Mat(matInput.rows(), matInput.cols(), matInput.type());

                detectSkin(cascadeClassifier_face, matInput.getNativeObjAddr(), matResult.getNativeObjAddr());
                Bitmap bitmapOutput = Bitmap.createBitmap(matResult.cols(), matResult.rows(), Bitmap.Config.ARGB_8888);
                Utils.matToBitmap(matResult, bitmapOutput);
                imageView.setImageBitmap(bitmapOutput);

//                Log.i("Image_Info: ", "width: " + resize.getWidth() + " height: " + resize.getHeight());
            }
        });
        check.setVisibility(View.INVISIBLE);

        get = (Button) findViewById(R.id.btn_get);
        get.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                openImage(view);
            }
        });
    }

    private void permissionCheck() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            permission = new MultiplePermission(this, this);
        }
        if (!permission.checkPermission()) {
            permission.requestPermission();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (!permission.permissionResultRequestPermissionsResult(requestCode, permissions, grantResults)) {
            permission.requestPermission();
        }
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    private void capturePhoto() throws IOException {
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        if (takePictureIntent.resolveActivity(getPackageManager()) != null) {
            File imageFile = createImageFile();
            if (imageFile != null) {
                takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, currentPhotoUri);
                startActivityForResult(takePictureIntent, REQUEST_IMAGE_CAPTURE);
            }
        }
    }

    private File createImageFile() throws IOException {
        // 이미지 파일 이름
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        String imageFileName = "JPEG_" + timeStamp + "_";

        // 외부저장소
        File storageDir = getExternalFilesDir("CapstoneDesign/");

        // 파일 생성
        File newFile = File.createTempFile(imageFileName, ".jpg", storageDir);

        currentPhotoFile = newFile;
        currentPhotoFileName = newFile.getName();
        currentPhotoPath = newFile.getAbsolutePath(); // 파일의 절대경로

        try {
            currentPhotoUri = FileProvider.getUriForFile(this,
                    getApplicationContext().getPackageName() + ".fileprovider",
                    newFile);
        } catch (Exception ex) {
            Log.d("FileProvider", ex.getMessage());
            ex.printStackTrace();
            throw ex;
        }
        return newFile;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);


        if (requestCode == REQUEST_IMAGE_CAPTURE && resultCode == RESULT_OK) {

            if (currentPhotoFile != null) {

                // 갤러리에 이미지 파일 생성
                galleryAddPic(currentPhotoUri, currentPhotoFileName);
                imageView.setImageURI(newImageFileUri);
                imageView.setVisibility(View.VISIBLE);
                check.setVisibility(View.VISIBLE);
                imagePath = getPath(newImageFileUri);
            }
        }
        if (requestCode == REQUEST_IMAGE_OPEN && resultCode == RESULT_OK) {
            getImageUri = data.getData();
            //Bitmap getImageBitmap = createSampledBitmap(getImageUri, imageView.getWidth(), imageView.getHeight());

            imageView.setImageURI(getImageUri);
            imageView.setVisibility(View.VISIBLE);
            check.setVisibility(View.VISIBLE);
            imagePath = getPath(getImageUri);
        }
    }

    private Uri galleryAddPic(Uri srcImageFileUri, String srcImageFileName) {
        ContentValues contentValues = new ContentValues(); //  ContentResolver가 처리할 수 있는 값 집합을 저장하는 데 사용
        ContentResolver contentResolver = getApplicationContext().getContentResolver();
        contentValues.put(MediaStore.Images.Media.DISPLAY_NAME, srcImageFileName);
        contentValues.put(MediaStore.Images.Media.MIME_TYPE, "image/jpeg");
        contentValues.put(MediaStore.Images.Media.RELATIVE_PATH, "Pictures/CapstoneDesign");
        contentValues.put(MediaStore.Images.Media.IS_PENDING, 1);
        newImageFileUri = contentResolver.insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, contentValues);

        try {
            AssetFileDescriptor afdInput = contentResolver.openAssetFileDescriptor(srcImageFileUri, "r");
            AssetFileDescriptor afdOutput = contentResolver.openAssetFileDescriptor(newImageFileUri, "w");
            FileInputStream fis = afdInput.createInputStream();
            FileOutputStream fos = afdOutput.createOutputStream();

            byte[] readByteBuf = new byte[1024];
            while (true) {
                int readLen = fis.read(readByteBuf);
                if (readLen <= 0) {
                    break;
                }
                fos.write(readByteBuf, 0, readLen);
            }

            fos.flush();
            fos.close();
            afdOutput.close();

            fis.close();
            afdInput.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        contentValues.clear();
        contentValues.put(MediaStore.Images.Media.IS_PENDING, 0); //다른앱이 파일에 접근할수 있도록 함
        contentResolver.update(newImageFileUri, contentValues, null, null);
        return newImageFileUri;
    }

    public void openImage(View view) {
        Intent intent = new Intent(Intent.ACTION_PICK);
        intent.setDataAndType(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, "image/*");
        startActivityForResult(intent, REQUEST_IMAGE_OPEN);
    }

    public String getPath(Uri uri) {
        String[] projection = {MediaStore.Images.Media.DATA};
        Cursor cursor = managedQuery(uri, projection, null,null,null);
        startManagingCursor(cursor);
        int columnIndex = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
        cursor.moveToFirst();
        return cursor.getString(columnIndex);
    }

    private Bitmap createSampledBitmap(Uri srcImageFileUri, int dstWidth, int dstHeight) {

        ContentResolver contentResolver = getApplicationContext().getContentResolver();
        ParcelFileDescriptor pfdExif = null;
        int orientation = 0;
        try {
            pfdExif = contentResolver.openFileDescriptor(srcImageFileUri, "r");
            FileDescriptor fdExif = pfdExif.getFileDescriptor();
            ExifInterface exifInterface = null;
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                exifInterface = new ExifInterface(fdExif); //Android 7.0(API level 24) 이상에서 사용 가능
            }
            orientation = exifInterface.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_UNDEFINED);
            Log.d("orientation", String.valueOf(orientation));
            pfdExif.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        // 축소된 이미지의 비트맵을 생성
        Bitmap bitmap = createSampledBitmap(srcImageFileUri, dstWidth, dstHeight);

        Matrix matrix = new Matrix();

        switch (orientation) {
            case ExifInterface.ORIENTATION_NORMAL:
                return bitmap;
            case ExifInterface.ORIENTATION_FLIP_HORIZONTAL:
                matrix.setScale(-1, 1); // 좌우 반전
                break;
            case ExifInterface.ORIENTATION_ROTATE_180:
                matrix.setRotate(180);
                break;
            case ExifInterface.ORIENTATION_FLIP_VERTICAL:
//              matrix.setRotate(180);
//              matrix.postScale(-1, 1); // 좌우 반전
                matrix.setScale(1, -1); // 상하 반전
                break;
            case ExifInterface.ORIENTATION_TRANSPOSE:
                matrix.setRotate(90);
                matrix.postScale(-1, 1); // 좌우 반전
                break;
            case ExifInterface.ORIENTATION_ROTATE_90:
                matrix.setRotate(90);
                break;
            case ExifInterface.ORIENTATION_TRANSVERSE:
                matrix.setRotate(-90);
                matrix.postScale(-1, 1); // 좌우 반전
                break;
            case ExifInterface.ORIENTATION_ROTATE_270:
                matrix.setRotate(-90);
                break;
            default:
                return bitmap;
        }
        try {
            Bitmap antiRotationBitmap = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), matrix, true);
            bitmap.recycle(); // bitmap은 더이상 필요 없음으로 메모리에서 free시킴
            return antiRotationBitmap;
        } catch (OutOfMemoryError e) {
            e.printStackTrace();
            return null;
        }
    }

    private void copyFile(String filename) {

        AssetManager assetManager = this.getAssets();
        File outputFile = new File(getFilesDir() + "/" + filename);

        InputStream inputStream = null;
        OutputStream outputStream = null;

        try {
            Log.d("Main", "copyFile :: 다음 경로로 파일 복사 " + outputFile.toString());
            inputStream = assetManager.open(filename);
            outputStream = new FileOutputStream(outputFile);

            byte[] buffer = new byte[1024];
            int read;
            while ((read = inputStream.read(buffer)) != -1) {
                outputStream.write(buffer, 0, read);
            }
            inputStream.close();
            inputStream = null;
            outputStream.flush();
            outputStream.close();
            outputStream = null;
        } catch (Exception e) {
            Log.d("Main", "copyFile :: 파일 복사 중 예외 발생 " + e.toString());
        }

    }

    private void read_cascade_file() {
        copyFile("haarcascade_frontalface_alt2.xml");
        Log.d("Main", "read_cascade_file");
        cascadeClassifier_face = loadCascade( getFilesDir().getAbsolutePath() + "/haarcascade_frontalface_alt2.xml");
    }
}