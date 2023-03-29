#ifndef _MOTION_CONTROL_H_
#define _MOTION_CONTROL_H_

void motion_control_start( void );
void motion_control_suspend( void );
void motion_control_resume( void );
float motion_get_yaw( void );
void motion_reset_yaw( void );

#endif  //_MOTION_CONTROL_H_

