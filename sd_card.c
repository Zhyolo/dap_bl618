#include "board.h"
#include "fatfs_diskio_register.h"
#include "ff.h"
#include <string.h>

#define DBG_TAG "SD"
#include "log.h"

static FATFS fs;

int filesystem_init(void)
{
    FRESULT ret;

    board_sdh_gpio_init();

    fatfs_sdh_driver_register();

    ret = f_mount(&fs, "/sd", 1);

    if (ret == FR_NO_FILESYSTEM) {
        LOG_W("No filesystem yet, try to be formatted...\r\n");

        __attribute((aligned(64))) static uint32_t workbuf[4096];
        MKFS_PARM fs_para = {
            .fmt = FM_ANY,       /* Format option (FM_FAT, FM_FAT32, FM_EXFAT and FM_SFD) */
            .n_fat = 1,          /* Number of FATs */
            .align = 0,          /* Data area alignment (sector) */
            .n_root = 0,         /* Number of root directory entries */
            .au_size = 512 * 32, /* Cluster size (byte) */
        };

        ret = f_mkfs("/sd", &fs_para, workbuf, sizeof(workbuf));

        if (ret == FR_OK) {
            LOG_I("done with formatting.\r\n");
            LOG_I("first start to unmount.\r\n");
            ret = f_mount(NULL, "/sd", 1);
            LOG_I("then start to remount.\r\n");
        } else {
            LOG_F("fail to make filesystem %d\r\n", ret);
        }
    }

    if (ret != FR_OK) {
        LOG_F("fail to mount filesystem,error= %d\r\n", ret);
        LOG_F("SD card might fail to initialise.\r\n");
        return -1;
    } else {
        LOG_D("Succeed to mount filesystem\r\n");
        LOG_I("FileSystem cluster size:%d-sectors (%d-Byte)\r\n", fs.csize, fs.csize * 512);
    }

    return 0;
}
