/*
 * @Author: CodingBugStd 3096470072@qq.com
 * @Date: 2022-11-19 20:52:56
 * @LastEditors: CodingBugStd 3096470072@qq.com
 * @LastEditTime: 2022-11-20 02:05:31
 * @FilePath: \MDK-ARMe:\UnderwayProject\iot_macenum_car\code\iot_mecanum_car_controller\USER\service\motion_service.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _MOTION_CONTROL_H_
#define _MOTION_CONTROL_H_

typedef float motion_speed_t[3];     //[0]x speed [1]y speed [2]yaw speed
typedef float motion_postion_t[3];   //[0]x坐标 [1]y坐标 [2]航向角

typedef enum{
    MOTION_MODE_SPEED = 0,      //控制速度
    MOTION_MODE_POSITION = 1,   //控制位置
    MOTION_MODE_STANBY = 2      //待命
}motion_control_mode_t;

void motion_service_start(void);
void motion_service_stop(void);

void motion_service_set_mode(motion_control_mode_t mode);
motion_control_mode_t motion_service_get_mode(void);

void motion_service_set_target_position(motion_postion_t* target);
void motion_service_set_target_speed(motion_speed_t* target);

void motion_control_serivce_get_position(motion_postion_t* position);
void motion_service_get_speed(motion_speed_t* speed);

#endif  //_MOTION_CONTROL_H_

