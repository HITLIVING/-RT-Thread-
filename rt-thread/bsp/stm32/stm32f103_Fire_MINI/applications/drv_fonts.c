#include "drv_fonts.h"
#include "stm32f1xx_hal.h"

/* Һ������ģ��أ���֧��Ӣ�ģ�*/ 
//ROM�洢��ģ��ʽ����Ҫ�����������
//Flash�洢��ģ��ʽ����Ϊ�ռ���
/*
 * ����ASCII��ƫ����32����С:16���߶ȣ�* 8 ����ȣ�
 */
const uint8_t ASCII8x16_Table [ ] = {0       //@conslons���壬��������ʽ������˳��ȡ��
};  

/*
 * ����ASCII��ƫ����32����С:24���߶ȣ�* 16 ����ȣ�
 */
const uint8_t ASCII16x24_Table [ ] = {0       //@conslons���壬��������ʽ������˳��ȡ��
};

/*
 * ����ASCII��ƫ����32����С:32���߶ȣ�* 24 ����ȣ�
 */
const uint8_t ASCII24x32_Table [ ] = {0       //@conslons���壬��������ʽ������˳��ȡ��
};

/*
 * ��ģ��������Flash������ʹ��
 */
const uint8_t Fonts_DownloadTable [ ] = {0
};


sFONT Font8x16 = {
  ASCII8x16_Table, 
  8, /* Width */
  16, /* Height */
};

sFONT Font16x24 = {
  ASCII16x24_Table, 
  16, /* Width */
  24, /* Height */
};

sFONT Font24x32 = {
  ASCII24x32_Table, 
  24, /* Width */
  32, /* Height */
};


//�ֶ������ַ�ָ��
#include <dfs_posix.h>
static int Download(int argc, char *argv[])
{
	int fd;
	char Filename[] = "/ASCII24x32_Table.txt";
	
	fd = open(Filename, O_WRONLY | O_CREAT);
    if (fd>= 0)
    {
        write(fd, Fonts_DownloadTable, sizeof(Fonts_DownloadTable));
        close(fd);
        rt_kprintf("Write done.\n");
    }
	
	return 0;
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(Download, download fonts to flash);


/*----------------------------end of file--------------------------*/







