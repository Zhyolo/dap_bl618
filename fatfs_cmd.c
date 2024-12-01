#include "ff.h"
#include "shell.h"
#include <stdio.h>
#include <string.h>

static FRESULT ls_path(char *path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    char *fn;
#if _USE_LFN
    char lfn[_MAX_LFN * 2 + 1] = {0};
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    res = f_opendir(&dir, path);
    if (res == FR_OK)
    {
        printf("Directory %s:\n", path);
        for (;;)
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0)
            {
                break;
            }
            if (fno.fname[0] == '.')
            {
                continue;
            }
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            printf("%-20s", fn);
            if ((fno.fattrib & AM_DIR) && (fno.fname[0] != '.') && (fno.fname[0] != '.'))
            {
                printf("%-25s\n", "<DIR>");
            }
            else
            {
                printf("%-25lu\n", fno.fsize);
            }
        }
        res = f_closedir(&dir);
    }

    return res;
}

static int ls(uint8_t argc, char **argv)
{
    if(argc > 2)
    {
        printf("ls <path>\r\n");
        return 0;
    }

    return ls_path(argc == 1 ? "/" : argv[1]);
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), ls, ls, list directory contents);

static int mv(uint8_t argc, char **argv)
{
    if(argc != 3)
    {
        printf("mv <old_path> <new_path>\r\n");
        return 0;
    }

    if(FR_OK != f_rename(argv[1], argv[2]))
    {
        printf("move %s to %s failed\r\n", argv[1], argv[2]);
        return -1;
    }
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), mv, mv, rename or moves a file or directory);

static int rm(uint8_t argc, char **argv)
{
    if(argc != 2)
    {
        printf("rm <path>\r\n");
        return 0;
    }

    if(FR_OK != f_unlink(argv[1]))
    {
        printf("rm %s error\r\n", argv[1]);
        return -1;
    }
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), rm, rm, remove a file or directory);
