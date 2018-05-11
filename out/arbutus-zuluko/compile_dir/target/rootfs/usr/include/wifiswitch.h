#ifndef WIFISWITCH_H
#define WIFISWITCH_H

#ifdef __cplusplus
extern "C" {
#endif

//参数，type为0/1,0表示正常的station模式，1代表AP模式. IP当前没有使用，传NULL。SSID要设定的SSID, password表示AP时的密码
//返回值，0表示成功， -1 IP非法，-2 SSID非法，-3 密码非法，-4 其他错误
int wifiSwitch(int type, char *ip, char *ssid, char *password);

//参数，ssid表示要连接的路由器ssid，password表示路由器密码
//返回值，0表示成功，-1表示ssid无法连接，-2密码错误，-3其他错误
int connectRouter(char* ssid, char *password);

#ifdef __cplusplus
}
#endif
#endif