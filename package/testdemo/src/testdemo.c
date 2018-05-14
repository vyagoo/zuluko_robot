

#include <utils/Timers.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <sys/dir.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include "iio_utils.h"
#include "CameraHardware.h"
#include "water_mark.h"

#include "wifiswitch.h"
#include <rtsp_server_live.h>
//#include <standby.h>
#include "debug.h"


static int Vclose = 0;
static int Isopen = 0;
void stream_close(int sig)
{
    signal(sig, SIG_IGN);
    Vclose = 1;
}
//PD20, LED3, 116, nor，蓝
//PD21, LED4, 117, imu，绿
//PD22, LED1, 118, cam，红
//PD23, LED2, 119, ency，黄
//PD24, LED5, 120, wifi，单独红
//PH07, UART3, 231, rx ,wakeup cpu
void uart3_low(){
	system("echo 231 > /sys/class/gpio/export");
	system("echo out > /sys/devices/platform/sunxi-pinctrl/gpio/gpio231/direction");
	system("echo 0 > /sys/devices/platform/sunxi-pinctrl/gpio/gpio231/value");
}
void uart3_high(){
	system("echo 231 > /sys/class/gpio/export");
	system("echo out > /sys/devices/platform/sunxi-pinctrl/gpio/gpio231/direction");
	system("echo 1 > /sys/devices/platform/sunxi-pinctrl/gpio/gpio231/value");
}

void cam_ok(){
	system("echo 118 > /sys/class/gpio/export");
	system("echo out > /sys/devices/platform/sunxi-pinctrl/gpio/gpio118/direction");
	system("echo 1 > /sys/devices/platform/sunxi-pinctrl/gpio/gpio118/value");
}
void cam_fail() {
	system("echo 118 > /sys/class/gpio/export");
	system("echo out > /sys/devices/platform/sunxi-pinctrl/gpio/gpio118/direction");
	system("echo 0 > /sys/devices/platform/sunxi-pinctrl/gpio/gpio118/value");
}

void ency_ok(){
	system("echo 119 > /sys/class/gpio/export");
	system("echo out > /sys/devices/platform/sunxi-pinctrl/gpio/gpio119/direction");
	system("echo 1 > /sys/devices/platform/sunxi-pinctrl/gpio/gpio119/value");
}

void ency_fail() {
	system("echo 119 > /sys/class/gpio/export");
	system("echo out > /sys/devices/platform/sunxi-pinctrl/gpio/gpio119/direction");
	system("echo 0 > /sys/devices/platform/sunxi-pinctrl/gpio/gpio119/value");
}

void wifi_ok(){
	system("echo 120 > /sys/class/gpio/export");
	system("echo out > /sys/devices/platform/sunxi-pinctrl/gpio/gpio120/direction");
	system("echo 1 > /sys/devices/platform/sunxi-pinctrl/gpio/gpio120/value");
}

void wifi_fail() {
	system("echo 120 > /sys/class/gpio/export");
	system("echo out > /sys/devices/platform/sunxi-pinctrl/gpio/gpio120/direction");
	system("echo 0 > /sys/devices/platform/sunxi-pinctrl/gpio/gpio120/value");
}

void nor_ok(){
	system("echo 116 > /sys/class/gpio/export");
	system("echo out > /sys/devices/platform/sunxi-pinctrl/gpio/gpio116/direction");
	system("echo 1 > /sys/devices/platform/sunxi-pinctrl/gpio/gpio116/value");
}

void nor_fail() {
	system("echo 116 > /sys/class/gpio/export");
	system("echo out > /sys/devices/platform/sunxi-pinctrl/gpio/gpio116/direction");
	system("echo 0 > /sys/devices/platform/sunxi-pinctrl/gpio/gpio116/value");
}

void imu_ok(){
	system("echo 117 > /sys/class/gpio/export");
	system("echo out > /sys/devices/platform/sunxi-pinctrl/gpio/gpio117/direction");
	system("echo 1 > /sys/devices/platform/sunxi-pinctrl/gpio/gpio117/value");
}

void imu_fail() {
	system("echo 117 > /sys/class/gpio/export");
	system("echo out > /sys/devices/platform/sunxi-pinctrl/gpio/gpio117/direction");
	system("echo 0 > /sys/devices/platform/sunxi-pinctrl/gpio/gpio117/value");
}

void mtd_erase() {
	system("mtd erase /dev/mtd0");
}
int iio_fp = -1;
char *data = NULL;

int start_iio()
{
	unsigned long buf_len = 128;

	int ret, c, i, j, toread;

	int num_channels = 0;
	char *trigger_name = NULL, *device_name = NULL;

	ssize_t read_size;
	int scan_size;

	/* Setup ring buffer parameters */
	ret = write_sysfs_int("length", "/sys/devices/platform/twi.0/i2c-0/0-0068/iio:device0/buffer", buf_len);
	if (ret < 0)
		goto error;

	/* Enable the buffer */
	ret = write_sysfs_int("enable", "/sys/devices/platform/twi.0/i2c-0/0-0068/iio:device0/buffer", 1);
	if (ret < 0) {
		db_error("Failed to enable buffer: %s\n", strerror(-ret));
		goto error;
	}

	/* Enable the accel */
	ret = write_sysfs_int("in_accel_enable", "/sys/devices/platform/twi.0/i2c-0/0-0068/iio:device0/", 1);
	if (ret < 0) {
		db_error("Failed to enable in_accel_enable: %s\n", strerror(-ret));
		goto error;
	}
	db_error("only accel---------------------\n");
	/* Enable the anglvel */
	
	ret = write_sysfs_int("in_anglvel_enable", "/sys/devices/platform/twi.0/i2c-0/0-0068/iio:device0/", 1);
	if (ret < 0) {
		db_error("Failed to enable in_accel_enable: %s\n", strerror(-ret));
		goto error;
	}
	

	//scan_size = size_from_channelarray(channels, num_channels);
	data = malloc(4096 * buf_len);
	if (!data) {
		ret = -ENOMEM;
		goto error;
	}


	/* Attempt to open non blocking the access dev */
	iio_fp = open("/dev/iio:device0", O_RDONLY | O_NONBLOCK);
	if (iio_fp == -1) { /* TODO: If it isn't there make the node */
		ret = -errno;
		db_error("Failed to open /dev/iio:device0\n");
		goto error;
	}
	return 0;

error:

	if (iio_fp >= 0 && close(iio_fp) == -1)
		db_error("Failed to close buffer");
	if (data)
		free(data);

	return ret;
}

int read_iio(int fp){
	int read_size = 0;
	read_size = read(fp, data, 128 * 2);
	return read_size;
}

int stop_iio(int fp, char *fdata) {
	if (fp > 0)
		close(fp);
	if (data) 
		free(fdata);
	fp = -1;
	fdata =NULL;
	return 0;
}

int testdemo(char *ip, int port)
{
	int ret = 0;
    VStream *Recorder = NULL;//calloc(1, sizeof(VStream));
	int i = 5;
	int j = 0;
	char prev_path[128] = { 0 };
	FILE *pFile_PREV = NULL;
	struct timespec t;
	int64_t  mCurFrameTimestamp, now, diff;
	int badframe = 0;
	int camera_ok = -1;
	int iio_ok = -1;
	WaterMark *mWaterMark = NULL;
	
	while (i) {
		if(access("/mnt/SDCARD/zuluko_testmode.txt", R_OK) != 0) {
			//exit(0);
			i--;
			if (i == 0)
				exit(0);
			usleep(1000*1000*1);
		} else {
			break;
		}
		
	}
	
	

	usleep(100);
	//system("wifi_connect_ap_test ssid password");
	system("softap_up zulukowifi open broadcast");
	usleep(1000*1000*3);
	//system("echo mem > /sys/power/state");
	//exit();
	standby_init();
	//sys_log_init();
	
	
	if (access("/dev/iio:device0", R_OK)==0){
		printf("imu OK\n");
		imu_ok();
	} else {
		imu_fail();
	}
	
	if (access("/dev/sunxi-lcd", R_OK)==0){
		printf("ency OK\n");
		ency_ok();
	} else {
		ency_fail();
	}
	
	if (access("/dev/mtd0", R_OK)==0){
		printf("nor OK\n");
		nor_ok();
	} else {
		nor_fail();
	}
	
	if (access("/sys/class/net/wlan0", R_OK)==0){

		printf("wlan0 OK\n");
		wifi_ok();
	} else {
		wifi_fail();
	}

	//imu start init
	/*
	wifiSwitch(1, NULL , "wldd", NULL);
	printf("ap \n\n\n\n");
		
	usleep(10000*1000);
	wifiSwitch(0, NULL , "wldd", NULL);
	printf("sta \n\n\n\n");
		
	usleep(10000*1000);
	connectRouter("ssid", "password");
	printf("router connect\n\n\n\n");
	*/
	start_iio();
	while(!Vclose){

		if (Recorder == NULL) {
			//standby_disable();
			printf("11111111111111111\n");
			Recorder = calloc(1, sizeof(VStream));
			Recorder->handle = 0;
			Recorder->format = V4L2_PIX_FMT_NV21;
			Recorder->fps = 30;
			Recorder->bufferCnt = 8;
			Recorder->width = 640;
			Recorder->height = 480;
/*
			mWaterMark = (WaterMark *) malloc(sizeof(WaterMark));
			memset(mWaterMark, 0x0, sizeof(WaterMark));
			mWaterMark->bgInfo.width = Recorder->width;
			mWaterMark->bgInfo.height = Recorder->height;
			if (mWaterMark->bgInfo.width == 1280) {
				mWaterMark->srcPathPrefix = "/tmp/watermark/water_mark_720_";
			} else if (mWaterMark->bgInfo.width == 1920){
				mWaterMark->srcPathPrefix = "/tmp/watermark/water_mark_1080_";
			} else if (mWaterMark->bgInfo.width == 1440){
				mWaterMark->srcPathPrefix = "/tmp/watermark/water_mark_720_";
			} else {
				mWaterMark->srcPathPrefix = "/tmp/watermark/water_mark_480_";
			}
			mWaterMark->srcNum = 13;
			waterMarkInit(mWaterMark);
*/		
			
			memcpy(Recorder->path, "/dev/video0", strlen("/dev/video0"));
reopen:
			ret = __StartCam(Recorder);
			if(0 != ret){
				db_error("_StartCam failed, May be camera is not ready!!!\n");
				cam_fail();
				//break;
			}
	
			ret = __StartStream(Recorder);
			if(0 != ret){
				db_error("__StartStream failed!!!\n");
				cam_fail();
				//break;
			} else {
				printf("cam ok\n");
				cam_ok();
			}
			/*
			memcpy(prev_path, "/mnt/SDCARD/prev.yuv", strlen("/mnt/SDCARD/prev.yuv"));
			 if(pFile_PREV == NULL){
				 pFile_PREV  = fopen(prev_path, "wb");
			 }else{
				 db_error("File exists !!!\n");
			 }
			 */
			// //now open led
			// __setLED(Recorder, 1);

			//init_mediaserver(ip, port);
			init_mediaserver("192.168.5.1", 8554);
			printf("cam init ok: %s, %d\n", ip, port);
			
		}

		clock_gettime(CLOCK_MONOTONIC, &t);
		mCurFrameTimestamp = (int64_t)((int64_t) t.tv_nsec/1000 + (((int64_t) t.tv_sec) * 1000000));

		V4L2BUF_t buf = __GetOneFrame(Recorder);
		
		if (buf.isInvalide == 1){
			
			if (camera_ok != 1) {
				cam_ok();
				camera_ok = 1;
			} 
			//else {
			//	cam_fail();
			//	ok = 0;
			//}
			//printf("get frame\n");
			clock_gettime(CLOCK_MONOTONIC, &t);
			now = (int64_t)((int64_t) t.tv_nsec/1000 + (((int64_t) t.tv_sec) * 1000000));
			diff = (now - mCurFrameTimestamp)/1000;
			//db_error("1111one frame take: %dms\n", (int)diff);
			/*
			mWaterMark->bgInfo.y = (unsigned char*)buf.addrVirY;
			mWaterMark->bgInfo.c = (unsigned char*)buf.addrVirC;
			
			printf("waterMarkShowTime\n");
			waterMarkShowTime(mWaterMark, 16, 30, (int)(mCurFrameTimestamp));
			*/
			//printf("send yuv, %d, %d\n", buf.width, buf.height);
			sendYUV((unsigned char*)buf.addrVirY, buf.width * buf.height*3/2, 0);
			//printf("send yuv end\n");
			__ReleaseOneFrame(Recorder, buf.index);
			//__StopCam(Recorder);
			//free(Recorder);
			//break;
			//printf("get one frame-----------:%ld\n", (int)(now));
			//do something here
			//.......
			
			//i++;
			//__setLED(Recorder, 0);
			// if ((i % 31) == 0) {
				// //close led
				// //i = 0;
				// __setLED(Recorder, 0);
				// printf("close--fsfdsd----\n");
			// } else if((i % 61) == 0){
				// i = 0;
				// __setLED(Recorder, 1);
				// printf("open----fsfdsdf-----\n");
			// }
			
			
			 if (pFile_PREV){
				// ret = fwrite((char *)buf.addrVirY, buf.width * buf.height, 1, (FILE*)pFile_PREV); // y
				// //if (ret < 1)
				// //	goto err;
				 //fwrite(buf.addrVirC, buf.width * buf.height/2, 1, (FILE*)pFile_PREV);// uv
			 }
			
			
			// __setLED(Recorder, 1);
			
		} else {
			__ReleaseOneFrame(Recorder, buf.index);
			__StopCam(Recorder);
			free(Recorder);
			printf("cam failed\n");
			if (camera_ok != 0)
				cam_fail();
			camera_ok = 0;
			//break;
			
		}
		
		
		ret = read_iio(iio_fp);
		if (ret > 0){
			if (iio_ok != 1)
				imu_ok();
			iio_ok = 1;
		} else {
			printf("read_iio failed:%d", ret);
			if (iio_ok != 0)
				imu_fail();
			iio_ok = 0;
		}
		
		
		/*
		//i++;
		if (i > 500) {
			i = 0;
			
			db_error("standby __StopCam-------");
			__StopCam(Recorder);
			free(Recorder);
			Recorder = NULL;
			cam_fail();
			
			db_error("standby stop_iio-------");
			stop_iio(iio_fp, data);
			standby_enable();
			db_error("standby standby_enable-------");
			usleep(2000*1000);
			standby_disable();
			db_error("standby standby_disable-------");
			standby_wait_wakeup();
			db_error("standby standby_wait_wakeup-------");
			usleep(3000*1000);
			imu_ok();
			start_iio();

		} 
		*/
	}

	//if (pFile_PREV)
	//	fclose(pFile_PREV);
    db_error("\n");
    db_error("******************************************************************************************\n");
    db_error("* Quit the program, goodbye!\n");
    db_error("******************************************************************************************\n");
    db_error("\n");

	return 0;
}

int main(int argc, char** argv)
{	
	//if (argc == 3) {
		printf("0000000000000000\n");
		testdemo(NULL, 8554);
		printf("111111111111111111\n");
	//} else {
	//	printf("usage: testdemo ip port\n");
	//}
	printf("3333333333333333333\n");
	//standby_disable();
	destory_mediaserver();
	printf("destory_mediaserver\n\n");
    exit(0);
}

//}


