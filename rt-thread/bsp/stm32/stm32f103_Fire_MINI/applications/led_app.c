#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

//LED�����̵߳Ĳ�������
#define THREAD_PRIORITY         25	//�߳����ȼ���rtconfig.h �е� RT_THREAD_PRIORITY_MAX �궨�����ȼ���Χ��
#define THREAD_STACK_SIZE       512	//�߳�ջ��С����λ���ֽ�
#define THREAD_TIMESLICE        5	//ͬ���ȼ��߳�ʱ��Ƭ��С

#define GPIONum_LED GET_PIN(C, 2)	//��ȡC2���ź�

/* �߳� 1 ����ں��� */
static void thread_App_LED_entry(void *parameter)
{
	rt_kprintf("LED is On\n");						//���������д�ӡ
	rt_pin_write( GPIONum_LED, PIN_LOW );			//�������ŵ�ƽ

}

/*���������Ӻ������ڷ����߳���תʱ������*/
static void hook_of_scheduler(struct rt_thread* from, struct rt_thread* to)
{
	//��ӡ�̴߳���������������
    rt_kprintf("\n from: %s -->  to: %s \n", from->name , to->name);
}

static rt_thread_t tid1 = RT_NULL;//���������߳̿��ƿ�
int thread_LED_On(void)
{
	//���õ���������
    rt_scheduler_sethook(hook_of_scheduler);
	//����GPIO����ģʽ
	rt_pin_mode( GPIONum_LED, PIN_MODE_OUTPUT );
	//�����߳�
    tid1 = rt_thread_create("thr_LED",
                            thread_App_LED_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* �������߳̿��ƿ飬��������߳� */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    return 0;
}

/* ������ msh �����б��� */
MSH_CMD_EXPORT(thread_LED_On, thread LED On);


