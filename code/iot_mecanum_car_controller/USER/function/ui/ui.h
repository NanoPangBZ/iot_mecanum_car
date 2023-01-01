/*
 * @Date: 2022-11-24 02:47:06
 * @LastEditors: 没人要的野指针 https://github.com/CodingBugStd
 * @LastEditTime: 2022-11-24 02:47:27
 * @Description: 
 */
#ifndef _UI_H_
#define _UI_H_

typedef enum
{
	DEBUG_PAGE = -1,
	MAIN_PAGE = 0,
	SETTING_PAGE = 1,
	WIFI_CFG_PAGE = 2
}ui_page_t;

typedef enum
{
	ERROR_EVENT = -1
}ui_event_t;

typedef enum
{
	UP = 0,
	DOWN = 1,
	CONFIRM = 2,
	CANCEL = 3
}ui_input_t;

typedef void(*ui_cb_t)(ui_event_t event,void *data);

void ui_page_set(ui_page_t page);
void ui_input(ui_input_t input);
void ui_callback_register(ui_cb_t *cb);

#endif  //_UI_H_

