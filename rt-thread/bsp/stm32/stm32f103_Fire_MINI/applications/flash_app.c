#include <flash_app.h>

#include <drv_spi.h>
#include <dfs_fs.h>
#include <dfs.h>
#include <spi_flash_sfud.h>

#define W25Q_SPI_DEVICE_NAME     "spi10"

rt_err_t rt_hw_spi_flash_init(void)
{	
    rt_err_t err_message = rt_hw_spi_device_attach("spi1", "spi10", GPIOA, GPIO_PIN_4);
	if(err_message != RT_EOK)
	{
		rt_kprintf("spi Init failed!");
		return RT_NULL;
	}
	
	struct rt_spi_configuration cfg;
    cfg.data_width = 8;
    cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
    cfg.max_hz = 20 * 1000 *1000;                           /* 20M */

    rt_spi_configure((struct rt_spi_device *)rt_device_find(W25Q_SPI_DEVICE_NAME), &cfg);
	
	rt_sfud_flash_probe("W25Q256", "spi10");	
	dfs_mkfs("elm", "W25Q256");
	dfs_mount("W25Q256","/","elm",0,0);
	
    return RT_EOK;
}


static void flash_app(int argc, char *argv[])
{
    struct rt_spi_device *spi_dev_w25q;
    rt_uint8_t w25x_read_id = 0x9F;
    rt_uint8_t id[3] = {0};

    /* 查找 spi 设备获取设备句柄 */
    spi_dev_w25q = (struct rt_spi_device *)rt_device_find(W25Q_SPI_DEVICE_NAME);
    if (!spi_dev_w25q)
    {
        rt_kprintf("spi sample run failed! can't find %s device!\n", W25Q_SPI_DEVICE_NAME);
    }
    else
    {
        /* 方式1：使用 rt_spi_send_then_recv()发送命令读取ID */
        rt_spi_send_then_recv(spi_dev_w25q, &w25x_read_id, 1, id, 5);
        rt_kprintf("use rt_spi_send_then_recv() read w25q ID is:\n%x%x%x\n",id[0],id[1],id[2]);
    }
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(flash_app, flash ID test);
