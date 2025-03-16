#include "version.h"
#include "my_printf.h"

#include <stdio.h>
#include <string.h>

#define LOG_D					MyPrintf
#define rt_sprintf 		sprintf

// Retrieve year info
#define OS_YEAR     ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 \
                                    + (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))

// Retrieve month info
#define OS_MONTH    (__DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 1 : 6) \
                                : __DATE__ [2] == 'b' ? 2 \
                                : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
                                : __DATE__ [2] == 'y' ? 5 \
                                : __DATE__ [2] == 'l' ? 7 \
                                : __DATE__ [2] == 'g' ? 8 \
                                : __DATE__ [2] == 'p' ? 9 \
                                : __DATE__ [2] == 't' ? 10 \
                                : __DATE__ [2] == 'v' ? 11 : 12)

// Retrieve day info
#define OS_DAY      ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 \
                                + (__DATE__ [5] - '0'))

// Retrieve hour info
#define OS_HOUR     ((__TIME__ [0] - '0') * 10 + (__TIME__ [1] - '0'))

// Retrieve minute info
#define OS_MINUTE   ((__TIME__ [3] - '0') * 10 + (__TIME__ [4] - '0'))

// Retrieve second info
#define OS_SECOND   ((__TIME__ [6] - '0') * 10 + (__TIME__ [7] - '0'))



struct Project_Version_Str Boot_Project_Version = {2, 0, 0, 1};		//����汾--�з��ڲ��汾
struct Project_Version_Str g_show_app_ver    = {2, 0, 0, 1};     //����汾--���Ҷ���汾
char app_firmware_ver_ascll[32]     = {0};              //�̼��汾���Լ���������--ASCII�ַ���
char app_show_ver_ascll[32]         = {0};              //����汾���Ѿ���������--ASCLL�ַ���
struct Project_Date_Time_Str Project_Date_Time = {OS_YEAR, OS_MONTH, OS_DAY, OS_HOUR, OS_SECOND};

const char project_name[] = "GSE8625-DG-N Boot";
void printf_project_name(void)
{
    LOG_D("DEVICE NAME:   %s\r\n", project_name);
}







void check_compile_time(void)
{
		LOG_D("COMPILED DATE: %s at %s\r\n", __DATE__, __TIME__);
}

void show_ctu_msg(void)
{
     /* ��ӡװ������ */
    printf_project_name();

    /* ��ӡ����ʱ�� */
    check_compile_time();

    /* ��ӡ�̼��汾(��ʵ�汾) */
    rt_sprintf(app_firmware_ver_ascll, "%d.%d.%d.%d",
                Boot_Project_Version.major_version_number,
                Boot_Project_Version.minor_version_number,
                Boot_Project_Version.revision_number,
                Boot_Project_Version.build_number);
    LOG_D("FIRMWARE VER:  %s\r\n", app_firmware_ver_ascll);
    rt_sprintf(app_firmware_ver_ascll, "%s %02d%02d%02d",
                app_firmware_ver_ascll,
                Project_Date_Time.hardware_year-2000,
                Project_Date_Time.hardware_mon,
                Project_Date_Time.hardware_day);

    /* ��ӡ����汾(���⹫���汾) */
    rt_sprintf((char *)app_show_ver_ascll, "%d.%d.%d.%d",
            g_show_app_ver.major_version_number,
            g_show_app_ver.minor_version_number,
            g_show_app_ver.revision_number,
            g_show_app_ver.build_number);
    LOG_D("SOFT VER:      %s\r\n", app_show_ver_ascll);
    rt_sprintf((char *)app_show_ver_ascll, "%s %02d%02d%02d", app_show_ver_ascll, 22, 10, 1);
}
