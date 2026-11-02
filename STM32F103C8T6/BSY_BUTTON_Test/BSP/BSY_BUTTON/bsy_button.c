#include  "bsy_button.h"



uint8_t key_flag = 0;       //按键状态标识



/*******************************************************************************
* 函数名  : key_process
* 描述    : 按键处理程序
* 输入    : ch  当前处理按键通道    val  当前通道按键有效值
* 返回    : 无 
* 说明    : 调用扫描周期SCAN_NMS   20ms扫描一次
*******************************************************************************/
void key_process(bsy_button_t *btn, uint8_t ch, uint8_t val)
{
    if(btn[ch].flag == 0){          //按键未按下
        if(val != 0){
            btn[ch].scan_cnt++;
            if(btn[ch].scan_cnt > 1){                 //连续2次扫描到按键
                btn[ch].event = BTN_PRESS_DOWN;       //按键事件已按下
                btn[ch].flag = 1;
            }
        }else{
            btn[ch].scan_cnt = 0;         //没有按键时计数清零
            btn[ch].up_cnt = 0;           //按键抬起计数清零
            btn[ch].flag = 0;
        }  
    }else if(btn[ch].flag == 1){      //按键已第一次按下
        btn[ch].scan_cnt++;
        if(val != 0){                               //按键一直触发
            if(btn[ch].scan_cnt > (SHORT_PRESS_CNT/SCAN_NMS)){        //如果按下超过短按时间
                btn[ch].flag = 5;                 //进入短按处理
                btn[ch].up_cnt = 0;           //按键抬起计数清零
            }
        }else{          //按键释放
            btn[ch].up_cnt++;   
            if(btn[ch].up_cnt > 1)    {       //按键检测到释放2个周期，消抖处理
                btn[ch].flag = 2;             //进入单击检测
                btn[ch].up_cnt = 0;           //按键抬起计数清零
            }
        }
    }else if(btn[ch].flag == 2){      //按键已第一次抬起
        if(val != 0) { 
            if(btn[ch].up_cnt > 4){
                btn[ch].flag = 3;     //确认是按键释放重新按下
                btn[ch].up_cnt = 0;
            }       
        }else{          //按键释放
            btn[ch].up_cnt++;
            if(btn[ch].up_cnt > SCAN_END)    {                //按键检测到释放大于10个周期
                btn[ch].event = BTN_PRESS_CLICK;              //单击事件产生
                btn[ch].flag = 0;                             //恢复原始状态
            }
        }
    }else if(btn[ch].flag == 3){      //按键已第二次按下
        btn[ch].scan_cnt++;
        if(val == 0) { 
            btn[ch].up_cnt++;
            if(btn[ch].up_cnt > 1)    {                       //按键检测到释放大于10个周期
                btn[ch].flag = 4;                             //恢复原始状态
                btn[ch].up_cnt = 0;
            }
        }
    }else if(btn[ch].flag == 4){      //按键已第二次抬起
        if(val == 0) { 
            btn[ch].up_cnt++;
            if(btn[ch].up_cnt > SCAN_END){                    //按键检测到释放大于10个周期
                btn[ch].flag = 0;                             //恢复原始状态
                btn[ch].event = BTN_PRESS_DOUBLE_CLICK;       //双击事件产生
            }
        }
    }else if(btn[ch].flag == 5){      //BTN_PRESS_SHORT_CLICK
        btn[ch].scan_cnt++;
        if(val != 0){
            if(btn[ch].scan_cnt > (LONG_PRESS_CNT/SCAN_NMS)){
                btn[ch].flag = 6;     //进入长按状态
                btn[ch].up_cnt = 0;
            }
        }else{
            btn[ch].up_cnt++;
            if(btn[ch].up_cnt > SCAN_END)    {                       //按键检测到释放大于10个周期
                btn[ch].flag = 0;                             //恢复原始状态
                btn[ch].event = BTN_PRESS_SHORT_CLICK;        //双击事件产生
            }
        }
    }else if(btn[ch].flag == 6){      //BTN_PRESS_LONG_CLICK
        btn[ch].scan_cnt++;
        if(val != 0){
            if(btn[ch].scan_cnt > (LONG_PRESS_HOLD_CNT/SCAN_NMS)){
                btn[ch].flag = 7;     //进入长按状态
                btn[ch].up_cnt = 0;
            }
        }else{
            btn[ch].up_cnt++;
            if(btn[ch].up_cnt > SCAN_END) {                       //按键检测到释放大于10个周期
                btn[ch].flag = 0;                             //恢复原始状态
                btn[ch].event = BTN_PRESS_LONG_CLICK;        //双击事件产生
            }
        }
    }else if(btn[ch].flag == 7){      //BTN_PRESS_LONG_HOLD
        btn[ch].scan_cnt++;
        if((btn[ch].scan_cnt) % 5 == 0 )  btn[ch].event = BTN_PRESS_LONG_HOLD;        //长按保持事件产生,100ms触发一次
        
        if(val == 0){
            btn[ch].up_cnt++;
            if(btn[ch].up_cnt > 1) {
                btn[ch].up_cnt = 0;
                btn[ch].flag = 8;     //长按释放
            }
        }
    }else if(btn[ch].flag == 8){      //BTN_PRESS_LONG_HOLD_UP
        if(val == 0){
            btn[ch].up_cnt++;
            if(btn[ch].up_cnt > SCAN_END) {
                btn[ch].flag = 0;     //长按释放
                btn[ch].event = BTN_PRESS_LONG_HOLD_UP;        //长按保持事件产生
            }
        }
    }
}






























