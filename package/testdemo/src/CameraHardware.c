
#include "CameraHardware.h"
#include "water_mark.h"

//#define SAVE_YUV_DATA_PREV
#ifdef SAVE_YUV_DATA_PREV
char *pFile_PREV = NULL;
char prev_path[128] = { 0 };
char prev_buff[1088*1080*3/2] = "\0";
#endif

WaterMark* mWaterMark = NULL;

void __ReleaseOneFrame(VStream *stream, int index) {

	int ret = -1;
	struct v4l2_buffer buf;

	memset(&buf, 0, sizeof(struct v4l2_buffer));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = index;
	ret = ioctl(stream->handle, VIDIOC_QBUF, &buf);
	if (ret != 0) {
		printf("VIDIOC_QBUF failed");
	}
}
int __setExposure(VStream *stream, int exp) {
	int ret = -1;
	struct v4l2_control ctrl;
	int mV4l2Handle = stream->handle;
    memset(&ctrl, 0, sizeof(struct v4l2_control));
	ctrl.id = V4L2_CID_AUTO_EXPOSURE_BIAS;
	ctrl.value = exp;
	ret = ioctl(mV4l2Handle, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		printf("setExposure failed!\n");
	else
		printf("setExposure ok\n");
	return ret;
}

int __setLED(VStream *stream, int onoff) {
	int ret = -1;
	struct v4l2_control ctrl;
	int mV4l2Handle = stream->handle;
    memset(&ctrl, 0, sizeof(struct v4l2_control));
	ctrl.id = V4L2_CID_FLASH_LED_MODE;
	ctrl.value = onoff;
	ret = ioctl(mV4l2Handle, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		printf("__setLED failed:%d!\n", ret);
	else
		printf("__setLED ok\n");
	return ret;
}

static int v4l2Init(VStream *stream) {

    int index = 0;
	int ret = -1;
	struct v4l2_capability cap;
	struct v4l2_streamparm params;
	struct v4l2_format format;
	struct v4l2_requestbuffers rb;
	struct v4l2_buffer buf;
	int mVideoFormat = stream->format;
	int mVideoWidth = stream->width;
	int mVideoHeight = stream->height;
	int mV4l2Handle = stream->handle;
	unsigned int mBufferCnt = stream->bufferCnt;
	
	// 查询兼容的模式
	printf("handle, %d\n", mV4l2Handle);
	ret = ioctl (mV4l2Handle, VIDIOC_QUERYCAP, &cap);
	if (ret < 0) {
		printf("Error opening device: unable to query device.\n");
		return -1;
	}

	if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) {
		printf("Error opening device: video capture not supported.\n");
		return -1;
	}

	if ((cap.capabilities & V4L2_CAP_STREAMING) == 0) {
		printf("Capture device does not support streaming i/o\n");
		return -1;
	}

    if (-1 == ioctl(mV4l2Handle, VIDIOC_S_INPUT, &index)) {
        printf("VIDIOC_S_INPUT error errno: %d!\n", errno);
        return -1;
    }

	// 设置流模式
	params.parm.capture.timeperframe.numerator = 1;
	params.parm.capture.timeperframe.denominator = stream->fps;
    params.parm.capture.capability = V4L2_CAP_TIMEPERFRAME;
	params.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	params.parm.capture.capturemode = V4L2_MODE_VIDEO;

    // set v4l2 device CaptureParams
	//CHECK_ERROR(v4l2setCaptureParams(&params));

	ret = ioctl(mV4l2Handle, VIDIOC_S_PARM, &params);
	if (ret < 0) {
		printf("v4l2setCaptureParams failed\n");
		return -1;
	} else {
		printf("v4l2setCaptureParams ok\n");
	}
	ret = ioctl(mV4l2Handle, VIDIOC_G_PARM, &params);

	printf("=====joey===== VIDIOC_G_PARM fps is %d\n",params.parm.capture.timeperframe.denominator);
	/*
	// set v4l2 device parameters
	//CHECK_ERROR(v4l2SetVideoParams());
	//获取支持的格式
	//ret = tryFmtSize(&mVideoWidth, &mVideoHeight);

	memset(&fmt, 0, sizeof(fmt));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = mVideoWidth;
	fmt.fmt.pix.height = mVideoHeight;
	fmt.fmt.pix.pixelformat = stream->format;//V4L2_PIX_FMT_NV12;
	fmt.fmt.pix.field = V4L2_FIELD_NONE;
	ret = ioctl(mV4l2Handle, VIDIOC_TRY_FMT, &fmt);
	if (ret < 0) {
		printf("VIDIOC_TRY_FMT Failed: %s", strerror(errno));
		return ret;
	}

	// driver surpport this size
	//mVideoWidth = fmt.fmt.pix.width;
	//mVideoHeight = fmt.fmt.pix.height;
	
	*/
	memset(&format, 0, sizeof(format));
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.width = mVideoWidth;
	format.fmt.pix.height = mVideoHeight;
	format.fmt.pix.pixelformat = mVideoFormat;
	format.fmt.pix.field = V4L2_FIELD_NONE;

	ret = ioctl(mV4l2Handle, VIDIOC_S_FMT, &format);
	if (ret < 0) {
		//LOGE("VIDIOC_S_FMT Failed: %s", strerror(errno));
		return ret;
	}
	
	//__setExposure(stream, 3);
	printf("VIDIOC_S_FMT:  width = %d,  height = %d \n", format.fmt.pix.width, format.fmt.pix.height);
	// v4l2 request buffers
	//CHECK_ERROR(v4l2ReqBufs());

	//申请buffer
	//LOGD("Device[%d] TO VIDIOC_REQBUFS count: %d", mCameraId, mBufferCnt);
	memset(&rb, 0, sizeof(rb));
	rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	rb.memory = V4L2_MEMORY_MMAP;
	rb.count = mBufferCnt;

	ret = ioctl(mV4l2Handle, VIDIOC_REQBUFS, &rb);
	if (ret < 0) {
		printf("Init: VIDIOC_REQBUFS failed: %s\n", strerror(errno));
		return ret;
	}

	if (mBufferCnt != rb.count) {
		mBufferCnt = rb.count;
		printf("VIDIOC_REQBUFS count: %d\n", mBufferCnt);
	}

	// v4l2 query buffers
	//CHECK_ERROR(v4l2QueryBuf());
	//对申请到的buffer 做一次释放动作
	for (unsigned int i = 0; i < mBufferCnt; i++) {
		memset(&buf, 0, sizeof(struct v4l2_buffer));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		ret = ioctl (mV4l2Handle, VIDIOC_QUERYBUF, &buf);
		if (ret < 0) {
			printf("Unable to query buffer (%s)\n", strerror(errno));
			return ret;
		}
		mMapMem.mem[i] = mmap(0, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, mV4l2Handle, buf.m.offset);
		mMapMem.length = buf.length;
		//LOGI("mV4l2Handle :%d ,index: %d, mem: %x, len: %x, offset: %x", mV4l2Handle, i, (int )mMapMem.mem[i], buf.length, buf.m.offset);
		if (mMapMem.mem[i] == MAP_FAILED) {
			printf("Unable to map buffer (%s)\n", strerror(errno));
			return -1;
		}
		// start with all buffers in queue
		ret = ioctl(mV4l2Handle, VIDIOC_QBUF, &buf);
		if (ret < 0) {
			printf("VIDIOC_QBUF Failed\n");
			return ret;
		}
	}

#ifdef EN_WATERMARK
	if (mV4l2Handle > 0) {
		mWaterMark = (WaterMark *) malloc(sizeof(WaterMark));
		memset(mWaterMark, 0x0, sizeof(WaterMark));
		mWaterMark->bgInfo.width = mVideoWidth;
		mWaterMark->bgInfo.height = mVideoHeight;
        if (mVideoWidth == 1280) {
            mWaterMark->srcPathPrefix = "/tmp/watermark/water_mark_720_";
        } else if (mVideoWidth == 1920){
            mWaterMark->srcPathPrefix = "/tmp/watermark/water_mark_1080_";
        } else {
            mWaterMark->srcPathPrefix = "/tmp/watermark/water_mark_480_";
        }
		mWaterMark->srcNum = 13;
		waterMarkInit(mWaterMark);
	} else {
		mWaterMark = NULL;
	}
#endif
	printf("start camera ok\n");
	return OK;

}

int v4l2DeInit(VStream *stream) {
	int ret = 0;
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	ret = ioctl(stream->handle, VIDIOC_STREAMOFF, &type);
	if (ret < 0) {
		printf("StopStreaming: Unable to stop capture: mV4l2Handle:%d  %s", stream->handle, strerror(errno));
		return ret;
	}

    for (int i = 0; i < stream->bufferCnt; i++) {
		ret = munmap(mMapMem.mem[i], mMapMem.length);
		if (ret < 0) {
			printf("v4l2CloseBuf Unmap failed ,mem[%d] ,mem:0x%p,length:%d", i, mMapMem.mem[i], mMapMem.length);

		}
        mMapMem.mem[i] = NULL;
	}

	
#ifdef	EN_WATERMARK
	if (mWaterMark != NULL) {
		waterMarkExit(mWaterMark);
		free(mWaterMark);
		mWaterMark = NULL;
	}
#endif
	return ret;
}


V4L2BUF_t __GetOneFrame(VStream *stream) {
	int ret = 0;
	//Mutex::Autolock autoLock(mLockGetFrame);
	void *addrVirY = NULL;
    struct timespec t;
	struct v4l2_buffer buf;
	fd_set fds;
	struct timeval tv;
	int r;
	int64_t mCurFrameTimestamp = 0;
	int mV4l2Handle = stream->handle;
	int mVideoWidth =stream->width;
	int mVideoHeight = stream->height;
	V4L2BUF_t v4l2_buf = {0};
	FD_ZERO(&fds);
	FD_SET(mV4l2Handle, &fds);

#ifdef SAVE_YUV_DATA_PREV
	static int size_prev_y = 0;
	static int size_prev_c = 0;
	static unsigned char* addrPrevVirY = NULL;
	static unsigned char* addrPrevVirC = NULL;
#endif

	/* Timeout */
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	r = select(mV4l2Handle + 1, &fds, NULL, NULL, &tv);
	if (r == -1) {
		printf("select err, %s", strerror(errno));
		ret =  -1;
	} else if (r == 0) {
		printf("v412 waitcamerReady select timeout\n");
		ret =  -1;
	}

	if (ret != 0) {
		printf("#########mCameraId:%s wait time out############\n", stream->path);
        //resetCamera();
		//GlobalEventCallback(RECORDER_TRACK_EVENT_INFO, MSG_TYPE_CAMERA_WAIT_TIME_OUT, mCameraId, NULL);
		//exit(1);
		return v4l2_buf;
	}
/*
	if(!mIsUsbCamera && uvcPowerSupply == false){
		//LOGE("V4L2Camera::closeUvcPower & openUvcPower");
		closeUvcPower();
		usleep(10*1000);			
		openUvcPower();
		uvcPowerSupply = true;
	}
*/
	memset(&buf, 0, sizeof(struct v4l2_buffer));
	//ret = getPreviewFrame(&buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	ret = ioctl(mV4l2Handle, VIDIOC_DQBUF, &buf);
	if (ret < 0) {
		/* notify Previewthread release PreviewFrame */
		//GlobalEventCallback(RECORDER_TRACK_EVENT_INFO, MSG_TYPE_CAMERA_QBUF_FAIL, mCameraId, NULL);
		//mListener->postDataTimestamp(mCameraId, 0, NULL, NULL, false);
        //wait app to reset
		printf("VIDIOC_DQBUF failed-------------------\n");
		usleep(3*1000);
		return v4l2_buf;
	}

    clock_gettime(CLOCK_MONOTONIC, &t);
    mCurFrameTimestamp = (int64_t)((int64_t) t.tv_nsec/1000 + (((int64_t) t.tv_sec) * 1000000));//微秒


	v4l2_buf.width = mVideoWidth;
	v4l2_buf.height = mVideoHeight;
	v4l2_buf.index = buf.index;
	v4l2_buf.timeStamp = mCurFrameTimestamp;
	v4l2_buf.crop_rect.left = 0;
	v4l2_buf.crop_rect.top = 0;
	v4l2_buf.crop_rect.width = mVideoWidth;
	v4l2_buf.crop_rect.height = mVideoHeight;
	v4l2_buf.overlay_info = NULL;

    if (stream->format == V4L2_PIX_FMT_NV12 || stream->format == V4L2_PIX_FMT_NV21) {//for csi camera
 		addrVirY = mMapMem.mem[buf.index];

 		v4l2_buf.addrPhyY = buf.m.offset;
    	v4l2_buf.addrVirY = (unsigned int) addrVirY;
    	v4l2_buf.addrPhyC = buf.m.offset + mVideoWidth * mVideoHeight;
    	v4l2_buf.addrVirC = (unsigned int) addrVirY + mVideoWidth * mVideoHeight;
	} 
	
#ifdef SAVE_YUV_DATA_PREV
	memcpy(prev_buff, addrVirY, 1088*1080*3/2);
#endif

#ifdef	EN_WATERMARK
	//if (NULL != addrVirY && true == mBackCarStatus && mCameraId == 1) {
	//	CameraHal_LRFlipFrame(V4L2_PIX_FMT_NV12, mVideoWidth, mVideoHeight, (unsigned char *)v4l2_buf.addrVirY, (unsigned //char*)v4l2_buf.addrVirC);
	//}

    if (addrVirY != NULL && mWaterMark != NULL) {
        mWaterMark->bgInfo.y = (unsigned char*)v4l2_buf.addrVirY;
        mWaterMark->bgInfo.c = (unsigned char*)v4l2_buf.addrVirC;

        if (mVideoWidth == 1920) {
            waterMarkShowTime(mWaterMark, 48, 84, (int)mCurFrameTimestamp);
        } else {
            waterMarkShowTime(mWaterMark, 16, 30, (int)mCurFrameTimestamp);
        }
    }
#endif
   // MemAdapterFlushCache((void *)v4l2_buf.addrVirY, mVideoWidth*mVideoHeight);
   // MemAdapterFlushCache((void *)v4l2_buf.addrVirC, mVideoWidth*mVideoHeight/2);

	if (v4l2_buf.addrVirY != 0 && v4l2_buf.addrVirC != 0) {
		//printf("__GetOneFrame success\n");
		v4l2_buf.isInvalide = 1;

#ifdef SAVE_YUV_DATA_PREV
	size_prev_y = 1088*1080;
	size_prev_c = 1088*1080/4;
	addrPrevVirY = (unsigned char*)prev_buff;
	addrPrevVirC = (unsigned char*)prev_buff + size_prev_y;
	if (pFile_PREV){
		fwrite(addrPrevVirY, size_prev_y, 1, (FILE*)pFile_PREV);
		fwrite(addrPrevVirC, size_prev_c * 2, 1, (FILE*)pFile_PREV);
	}
	//gettimeofday( &tv_end, NULL );
	printf("SAVE_YUV_DATA_PREV OK \n");
#endif
		
	}

	return v4l2_buf;
}


int __StartCam(VStream *stream){

	int ret = 0;
	int handle = 0;

	if (stream->handle) {
		printf("stream alreay open, please check\n");
		return OK;
	}
	// snprintf(dev, sizeof(dev),"%s%d", "/dev/video", id);

    if (access(stream->path,F_OK) == 0) {
        handle = open(stream->path, O_RDWR | O_NONBLOCK, 0);
        if (handle <= 0) {
            printf("stream open failed: %d, please check\n", handle);
            return handle;
        }
		printf("v4l2Init 33333 access\n");
		stream->handle = handle;
    }

	ret = v4l2Init(stream);
	if (ret < 0) {
		printf("v4l2Init failed: %d, please check\n", ret);
		close(handle);
		return -1;
	}

#ifdef SAVE_YUV_DATA_PREV
	snprintf(prev_path, sizeof(prev_buff),"/mnt/SDCARD/prev.yuv");

	if(pFile_PREV == NULL){
		pFile_PREV  = (char *)fopen(prev_path, "wb");
	}else{
		printf("File exists !!!\n");
	}

	if(pFile_PREV == NULL){
		printf("fopen %s fail !", prev_path);
	}
#endif

	return OK;
}


int __StartStream(VStream *stream) {
	int mV4l2Handle = stream->handle;
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int ret = 0;
	// stream on the v4l2 device
	//CHECK_ERROR(v4l2StartStreaming());
	// 开始获取流

	ret = ioctl (mV4l2Handle, VIDIOC_STREAMON, &type);
	if (ret < 0) {
		printf("StartStreaming: Unable to start capture: %s", strerror(errno));
		return ret;
	}
	printf("StartStreaming ok-----------------\n");

	return ret;
}

int __StopCam(VStream *stream) {
	if (!stream) {
		printf("stream is null----------\n");
		return OK;
	}
	if (!stream->handle) {
		printf("CameraHardware::stopTVDecoder: state already stopped\n");
		return OK;
	}
	
	v4l2DeInit(stream);

	close(stream->handle);
	stream->handle = 0;
	printf("now close came sueccess----------------\n");
	return OK;
}

