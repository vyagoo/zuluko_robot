#ifndef STANDBY_H
#define STANDBY_H
#ifdef __cplusplus
extern "C" {
#endif

int standby_init();// 0 sucess, or fail
int standby_enable(); //0 sucess, or fail
int standby_disable();//0 sucess,or fail
int standby_wait_wakeup();//0 sucess,or fail
int is_ecy_ok();// > 0 sucess, or fail

#ifdef __cplusplus
}
#endif
#endif