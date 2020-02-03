package com.example.cpptask;

import androidx.appcompat.app.AppCompatActivity;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.graphics.Point;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

import android.opengl.GLSurfaceView.Renderer;


import static android.opengl.GLES10.GL_COLOR_BUFFER_BIT;
import static android.opengl.GLES10.glClear;
import static android.opengl.GLES10.glClearColor;


public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    Button btn ;
    private GLSurfaceView glSurfaceView;
    private boolean rendererSet;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btn = (Button)findViewById(R.id.btnDoSomeThing);

        ActivityManager activityManager
                = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();

        final boolean supportsEs2 =
                configurationInfo.reqGlEsVersion >= 0x20000 || isProbablyEmulator();



        if (supportsEs2) {
            glSurfaceView = new GLSurfaceView(this);

            glSurfaceView.setEGLContextClientVersion(2);
            glSurfaceView.setRenderer(new RendererWrapper());
            rendererSet = true;
            setContentView(glSurfaceView);
        } else {
            Toast.makeText(this, "This device does not support OpenGL ES 2.0.",
                    Toast.LENGTH_LONG).show();
            return;
        }

    }


    private boolean isProbablyEmulator() {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1
                && (Build.FINGERPRINT.startsWith("generic")
                || Build.FINGERPRINT.startsWith("unknown")
                || Build.MODEL.contains("google_sdk")
                || Build.MODEL.contains("Emulator")
                || Build.MODEL.contains("Android SDK built for x86"));
    }


    @Override
    protected void onPause() {
        super.onPause();

        if (rendererSet) {
            glSurfaceView.onPause();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (rendererSet) {
            glSurfaceView.onResume();
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

    public static native void onSurfaceCreatedC( int width, int height );

    public static native void onSurfaceChangedC( int width, int height );

    public static native void onDrawFrameC();

    public static native String onCreateModel( float[] fArray );


    public static float[] f = new float[20];


    public class RendererWrapper implements Renderer {
        @Override
        public void onSurfaceCreated( GL10 gl, EGLConfig config ) {

            f[0] = 0.155f;
            f[1] = 0.476f;

            f[2] = -0.077f;
            f[3] = 0.238f;

            f[4] = -0.405f;
            f[5] = 0.294f;

            f[6] = -0.25f;
            f[7] = 0.0f;

            f[8] = -0.405f;
            f[9] = -0.294f;

            f[10] = -0.077f;
            f[11] = -0.238f;

            f[12] = 0.155f;
            f[13] = -0.476f;

            f[14] = 0.202f;
            f[15] = -0.147f;

            f[16] = 0.5f;
            f[17] = 0.0f;

            f[18] = 0.202f;
            f[19] = 0.157f;


            // glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
            Display display = getWindowManager().getDefaultDisplay();
            Point size = new Point();
            display.getSize(size);
            int width = size.x;
            int height = size.y;

            onSurfaceCreatedC(width,height-200);


            String modelID = onCreateModel(f);


            System.out.println(modelID);

        }


        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            // No-op
        }


        @Override
        public void onDrawFrame(GL10 gl) {
            onDrawFrameC();
        }


    }



}

