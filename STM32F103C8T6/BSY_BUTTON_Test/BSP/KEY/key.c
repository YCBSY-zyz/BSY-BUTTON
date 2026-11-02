#include  "key.h"
#include  "bsy_os.h"
#include  "bsy_button.h"


//设置该任务优先级为4
#define    THREAD4                  4 

//定义读取端口电平有效值， 0 低电平有效， 1  高电平有效
#define    KEY_LEVEL                0

//是否打印输出事件，0 不打印，  1  打印  
#define   PRINT_ENABLE    1

//设置按键通道数
#define    KEY_MAX   4


//宏定义  EUNM 转 字符串
#define ENUM_TO_STR(e) (#e)


//定义按键数组
static bsy_button_t  btn_t[KEY_MAX];




//ENUM 事件转字符串
static char *enum_event_string[] = {
    ENUM_TO_STR(BSY_BTN_PRESS_NONE),
    ENUM_TO_STR(BSY_BTN_PRESS_DOWN),
    ENUM_TO_STR(BSY_BTN_PRESS_CLICK),
    ENUM_TO_STR(BSY_BTN_PRESS_DOUBLE_CLICK),
    ENUM_TO_STR(BSY_BTN_PRESS_SHORT_CLICK),
    ENUM_TO_STR(BSY_BTN_PRESS_LONG_CLICK),
    ENUM_TO_STR(BSY_BTN_PRESS_LONG_HOLD),
    ENUM_TO_STR(BSY_BTN_PRESS_LONG_HOLD_UP),
};


/*******************************************************************************
* 函数名  : Key_GPIO_Config
* 描述    : KEY IO配置
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : KEY设置的引脚为:PB12 ---  PB15
*******************************************************************************/
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;				//定义一个GPIO_InitTypeDef类型的GPIO初始化结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			 //使能GPIOB的外设时钟	
	
	GPIO_InitStructure.GPIO_Pin = KEY0_PIN | KEY1_PIN | KEY2_PIN | KEY3_PIN;			//选择要初始化的GPIOB引脚(PB12,PB13,PB14,PB15)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//设置引脚工作模式为上拉输入 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			//调用库函数中的GPIO初始化函数，初始化GPIOB中的PB12,PB13,PB14,PB15引脚
    
}


//按键初始化
static void key_init(void)
{
    uint8_t i = 0;
    Key_GPIO_Config();                      //按键硬件初始化
    for(i = 0; i < KEY_MAX; i++)            //按键数组初始化
    {
        btn_t[i].event = BTN_PRESS_NONE;
        btn_t[i].up_cnt = 0;
        btn_t[i].scan_cnt = 0;
        btn_t[i].flag = 0;
    }
}


//按键读取数据，有按键返回1，没有返回0
static uint8_t key_read(uint8_t k)
{
    uint8_t  val = 2;
    if( k == 0){
        val = KEY0;     //读取KEY0按键引脚电平
    }else if (k == 1){
        val = KEY1;     //读取KEY1按键引脚电平
    }else if (k == 2){
        val = KEY2;     //读取KEY2按键引脚电平
    }else if (k == 3){
        val = KEY3;     //读取KEY3按键引脚电平
    }       //如有更多按键，再次添加
    if(val == KEY_LEVEL) return 1;      //判断输出电平是否是有效电平，有按键按下返回1
    return 0;                           //没有按键按下，返回0
}



//按键扫描任务
static void  Key_Scan(void * pmt)
{
    bsy_button_t  * btn = (bsy_button_t *) pmt;
    
    int i = 0;
    uint8_t  key_val = 0;
    for(i = 0; i < KEY_MAX; i++){           //轮询每个按键通道
        key_val = key_read(i);              //读取按键值
        key_process(btn, i, key_val);            //按键处理
    }

#if  PRINT_ENABLE 
    
    if(btn[0].event != BTN_PRESS_NONE){                             //按键有事件产生
        printf("Key0: %s!\r\n",enum_event_string[btn[0].event]);    //打印按键事件
        btn[0].event = BTN_PRESS_NONE;                              //按键事件使用后，清除事件
    }  
    if(btn[1].event != BTN_PRESS_NONE){
        printf("Key1: %s!\r\n",enum_event_string[btn[1].event]);
        btn[1].event = BTN_PRESS_NONE;
    }
    if(btn[2].event != BTN_PRESS_NONE){
        printf("Key2: %s!\r\n",enum_event_string[btn[2].event]);
        btn[2].event = BTN_PRESS_NONE;
    }
    if(btn[3].event != BTN_PRESS_NONE){
        printf("Key3: %s!\r\n",enum_event_string[btn[3].event]);
        btn[3].event = BTN_PRESS_NONE;
    }
#endif
    
}


//创建任务结构体函数
RUN_Handle_Type Key_Task_Handle;


//创建UART运行任务
void Task_Key(void)
{
    key_init();                             //按键初始化
    
    Key_Task_Handle.R_CYCLE = SCAN_NMS;     //20ms读一次数据
    Key_Task_Handle.NMS = SCAN_NMS;         //装载延时计数值
    Key_Task_Handle.R_STATUS = STOP;        //设置状态为停止态
    Key_Task_Handle.LEVEL = THREAD4;        //设置该任务优先级为4
    Key_Task_Handle.LOCK = false;           //0任务锁打开，可以执行该任务
    Key_Task_Handle.pmt = &btn_t;           //回调函数参数，这里只是测试   
    Key_Task_Handle.pf = Key_Scan;          //任务回调函数
    Task_Add(&Key_Task_Handle);             //添加任务到任务列表
}













