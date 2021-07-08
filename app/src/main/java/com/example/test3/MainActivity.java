package com.example.test3;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);

        //connect as a root
        try {
            Process p = Runtime.getRuntime().exec("su 0 chmod 777 /dev/ttyS1");
        } catch (IOException e) {
            e.printStackTrace();
        }

        tv.setText(stringFromJNI());

        int fd = serialOpen();

        //while (fd != -1) {
            send_system_data(fd);
        //}

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native void sleep_ms(int milliseconds);
    public native void send_system_data(int fd);
    public native int serialOpen();

}