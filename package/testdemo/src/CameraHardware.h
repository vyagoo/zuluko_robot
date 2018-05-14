#ifndef CAMERA_I_H
#define CAMERA_I_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <utils/Log.h>
#include "videodev.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

//#define EN_WATERMARK 0
#define NB_BUFFER 8
#define ALIGN_16B(x) (((x) + (15)) & ~(15))
#define ALIGN_4K(x) (((x) + (4095)) & ~(4095))
#define OK 0

typedef struct v4l2_mem_map_t {
	void *mem[NB_BUFFER];
	void *bufVirAddr[NB_BUFFER]; // for usb camera
	void *thumbAddrPhyY[NB_BUFFER]; //for double stream
	void *thumbAddrVirY[NB_BUFFER]; //for double stream
	int  thumbIsAvailable[NB_BUFFER];//for double stream
	int thumbWidth;//for double stream
	int thumbHeight;//for double stream
	int length;
} v4l2_mem_map_t;
v4l2_mem_map_t mMapMem;

typedef struct PREVIEWINFO_t
{
	int left;
	int top;
	int width;			// preview width
	int height;			// preview height
}PREVIEWINFO_t, RECT_t;

typedef struct V4L2BUF_t
{
	unsigned int	addrPhyY;		// physical Y address of this frame
	unsigned int	addrPhyC;		// physical C address of this frame
	
	unsigned int	addrVirY;		// virtual Y address of this frame
	unsigned int	addrVirC;		// virtual C address of this frame

	unsigned int	width;
	unsigned int	height;
	int 			index;			// DQUE id number
	long long		timeStamp;		// time stamp of this frame
	RECT_t			crop_rect;
	int				format;
	void*           overlay_info;

	// thumb
	unsigned char	isThumbAvailable;
	unsigned char	thumbUsedForPreview;
	unsigned char	thumbUsedForPhoto;
	unsigned char	thumbUsedForVideo;
	unsigned int	thumbAddrPhyY;		// physical Y address of thumb buffer
	unsigned int	thumbAddrVirY;		// virtual Y address of thumb buffer
	unsigned int	thumbWidth;
	unsigned int	thumbHeight;
	RECT_t			thumb_crop_rect;
	int 			thumbFormat;

	int 			refCnt; 		// used for releasing this frame
	void *          privateData;
	int isInvalide;
}V4L2BUF_t;

typedef struct vstream {
	int handle;
	int format;
	int fps; 
	int bufferCnt;
	int width;
	int height;
	char path[256];
}VStream;

int __StartCam(VStream *stream);
int __StopCam(VStream *stream);
int __StartStream(VStream *stream);
void __ReleaseOneFrame(VStream *stream, int index);
V4L2BUF_t __GetOneFrame(VStream *stream);
int __setLED(VStream *stream, int value);

#endif // CAMERA_I_H