#ifndef _rtsp_server_live_h_
#define _rtsp_server_live_h_

#if defined(__cplusplus)
extern "C" {
#endif

//参数，监听的IP，为NULL则默认当前IP。port监听端口，为0则默认8554
//返回值0成功，-1编码器初始化失败，-2网络初始化失败
int init_mediaserver(char *ip, int port);

//参数，data指向数据buffer， size指定数据大小, type指定数据格式，当前默认为0代表（YUV420sp），仅支持YUV420sp
//返回值0成功，-1编码失败，-2网络发送失败
int sendYUV(char *data, int size, int type);

//返回值0成功，-1参数非法（比如没有调用init_mediaserver）
int destory_mediaserver();

#if defined(__cplusplus)
}
#endif
#endif /* !_rtsp_server_live_h_ */
