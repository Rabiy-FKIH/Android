#include <jni.h>
#include <android/log.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>



#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h> // for usleep
#endif

#include "greio.h"
#include "dashboard0_events.h"


#define MAX_SPEED 200
#define MIN_SPEED 0


/*static const char *TAG="serial_port";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)*/

extern "C" JNIEXPORT jstring JNICALL Java_com_example_test3_MainActivity_stringFromJNI( JNIEnv* env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_test3_MainActivity_sleep_1ms(JNIEnv *env, jobject thiz, jint milliseconds) {
#ifdef WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}


void send_system_codes_initialize(gre_io_t *send_handle) {
    gre_io_serialized_data_t    *nbuffer = NULL;
    update_data_event_t event_data;
    int ret;

    event_data.distance_traveled = 0;
    event_data.rpm = 0;
    event_data.fuel = 70;
    event_data.speed = 0;

    // Serialize the data to a buffer
    nbuffer = gre_io_serialize(nbuffer, NULL, UPDATE_DATA_EVENT, UPDATE_DATA_FMT, &event_data, sizeof(event_data));
    if (!nbuffer) {
        fprintf(stderr, "Can't serialized data to buffer, exiting\n");
        //LOGE("Can't serialized data to buffer, exiting\n");
        return;
    }

    // Send the serialized event buffer
    ret = gre_io_send(send_handle, nbuffer);
    if (ret < 0) {
        fprintf(stderr, "Send failed, exiting\n");
		//LOG_ERROR("Send failed, exiting\n");
    }
    //Release the buffer memory
    gre_io_free_buffer(nbuffer);
}

void send_system_codes_startup(gre_io_t *send_handle) {

	gre_io_serialized_data_t	*nbuffer = NULL;
	update_data_event_t			event_data;
	int							ret;

	event_data.distance_traveled = 1000;
	event_data.rpm = 0;
	event_data.fuel = 70;
	event_data.speed = 0;

	// Serialize the data to a buffer
	nbuffer = gre_io_serialize(nbuffer, NULL, UPDATE_DATA_EVENT, UPDATE_DATA_FMT, &event_data, sizeof(event_data));
	if (!nbuffer) {
		fprintf(stderr, "Can't serialized data to buffer, exiting\n");
		//LOG_ERROR("Can't serialized data to buffer, exiting\n");
		return;
	}

	// Send the serialized event buffer
	ret = gre_io_send(send_handle, nbuffer);
	if (ret < 0) {
		fprintf(stderr, "Send failed, exiting\n");
		//LOG_ERROR("Send failed, exiting\n");
	}
	//Release the buffer memory
	gre_io_free_buffer(nbuffer);
}

void sleep_ms(int milliseconds) {
#ifdef WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_test3_MainActivity_send_1system_1data(JNIEnv *env, jobject thiz) {
	gre_io_t					*send_handle;
	gre_io_serialized_data_t	*nbuffer = NULL;
	update_data_event_t			event_data;
	int 						ret;

	// Connect to a channel to send messages (write)
	send_handle = gre_io_open("tcp://dashboard0", GRE_IO_TYPE_WRONLY);
	if (send_handle == nullptr) {
	    //FILE* file =fopen("error.txt","a");
	    //fputs("test test",file);

		fprintf(stderr, "Can't open send channel\n");
        //LOGE("Can't open send channel\n");
		return ;
	}

	memset(&event_data, 0, sizeof(event_data));
	send_system_codes_initialize(send_handle);
	sleep_ms(3000);
	send_system_codes_startup(send_handle);

    //Release the buffer memory
    gre_io_free_buffer(nbuffer);
    //Close the send handle
    gre_io_close(send_handle);

}