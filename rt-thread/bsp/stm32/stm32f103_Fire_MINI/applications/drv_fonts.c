#include "drv_fonts.h"
#include "stm32f1xx_hal.h"

/* 液晶屏字模相关（仅支持英文）*/ 
//ROM存储字模方式下需要填充下列数组
//Flash存储字模方式数组为空即可
/*
 * 常用ASCII表，偏移量32，大小:16（高度）* 8 （宽度）
 */
const uint8_t ASCII8x16_Table [ ] = {0       //@conslons字体，阴码点阵格式，逐行顺向取摸
};  

/*
 * 常用ASCII表，偏移量32，大小:24（高度）* 16 （宽度）
 */
const uint8_t ASCII16x24_Table [ ] = {0       //@conslons字体，阴码点阵格式，逐行顺向取摸
};

/*
 * 常用ASCII表，偏移量32，大小:32（高度）* 24 （宽度）
 */
const uint8_t ASCII24x32_Table [ ] = {0       //@conslons字体，阴码点阵格式，逐行顺向取摸
};

/*
 * 字模表，仅供向Flash中下载使用
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


//手动下载字符指令
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
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(Download, download fonts to flash);


/*----------------------------end of file--------------------------*/







