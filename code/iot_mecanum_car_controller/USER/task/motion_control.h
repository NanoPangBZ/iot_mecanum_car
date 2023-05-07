#ifndef _MOTION_CONTROL_H_
#define _MOTION_CONTROL_H_

//参考系
typedef enum{
    SOFT_REF = 0,   //模块start时自动定义的坐标系
    CAR_REF = 1,    //以小车为原点的坐标系
    TARGET_REF = 2  //以目标位置为原点的坐标系
}position_reference_t;

typedef enum{
    YAW_LOCK_ENABLE = (1<<0),
    TARGET_POSITION_ENABLE = (1<<1),
    POSITION_PID_ENABLE = (1<<2),
    ENABLE_ALL = 0xff
}motion_control_function_t;

//运动控制模块开关
void motion_control_start( void );
void motion_control_suspend( void );
void motion_control_resume( void );

//运动控制模块信息输入
void motion_control_function_enable( motion_control_function_t function );
void motion_control_function_disable( motion_control_function_t function );
motion_control_function_t motion_get_control_function( void );
void motion_reset_yaw( void );
void motion_reset_position( void );
void motion_set_target_position( float x , float y , position_reference_t ref );
void motion_set_target_speed( float x , float y , float yaw_speed , position_reference_t ref );
void motion_set_target_yaw( float yaw );

//运动控制模块信息输出
float motion_get_yaw( void );
void motion_get_position( float* x , float* y , position_reference_t ref);
void motion_get_target_position( float* x , float*y , position_reference_t ref );

#endif  //_MOTION_CONTROL_H_

