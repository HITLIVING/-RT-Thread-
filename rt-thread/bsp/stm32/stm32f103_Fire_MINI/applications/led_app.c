#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

//LED点亮线程的参数定义
#define THREAD_PRIORITY         25	//线程优先级（rtconfig.h 中的 RT_THREAD_PRIORITY_MAX 宏定义优先级范围）
#define THREAD_STACK_SIZE       512	//线程栈大小，单位是字节
#define THREAD_TIMESLICE        5	//同优先级线程时间片大小

#define GPIONum_LED GET_PIN(C, 2)	//获取C2引脚号

/* 线程 1 的入口函数 */
static void thread_App_LED_entry(void *parameter)
{
	rt_kprintf("LED is On\n");						//串口命令行打印
	rt_pin_write( GPIONum_LED, PIN_LOW );			//设置引脚电平

}

/*调度器钩子函数，在发生线程跳转时被调用*/
static void hook_of_scheduler(struct rt_thread* from, struct rt_thread* to)
{
	//打印线程从哪里跳到了哪里
    rt_kprintf("\n from: %s -->  to: %s \n", from->name , to->name);
}

static rt_thread_t tid1 = RT_NULL;//用来接受线程控制块
int thread_LED_On(void)
{
	//设置调度器钩子
    rt_scheduler_sethook(hook_of_scheduler);
	//设置GPIO引脚模式
	rt_pin_mode( GPIONum_LED, PIN_MODE_OUTPUT );
	//创建线程
    tid1 = rt_thread_create("thr_LED",
                            thread_App_LED_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_LED_On, thread LED On);


