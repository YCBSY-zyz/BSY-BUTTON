#ifndef   __BSY_BUTTON_
#define   __BSY_BUTTON_

#include   <stm32f10x.h>
#include   <stdio.h>




//短按起始时间MS
#define    SHORT_PRESS_CNT          500

//长按起始时间MS
#define    LONG_PRESS_CNT           1500

//长按保持起始时间MS
#define    LONG_PRESS_HOLD_CNT      2000

//按键间隔时间MS
//#define    CLICK_INTERVAL           50

//扫描时间周期MS
#define    SCAN_NMS                 20

//按键退出扫描次数
#define    SCAN_END                 15




//按键事件类型
typedef enum
{
    BTN_PRESS_NONE = 0,         //初始状态
    BTN_PRESS_DOWN,             //按键按下
    BTN_PRESS_CLICK,            //单击
    BTN_PRESS_DOUBLE_CLICK,     //双击
    BTN_PRESS_SHORT_CLICK,      //短按
    BTN_PRESS_LONG_CLICK,       //长按
    BTN_PRESS_LONG_HOLD,        //长按保持
    BTN_PRESS_LONG_HOLD_UP,     //长按释放
} bsy_button_event_t;


//按键结构体
typedef struct bsy_button
{
    uint16_t scan_cnt;      //扫描次数计数
    uint16_t up_cnt;        //按键抬起计数
    uint8_t event;          //按键发生的事件
    uint8_t flag;           //按键状态标识
} bsy_button_t;





void key_process(bsy_button_t *btn, uint8_t ch, uint8_t val);




#endif



