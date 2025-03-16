#ifndef __VERSION__H
#define __VERSION__H

#include <stdint.h>

struct Project_Version_Str
{
    uint16_t    major_version_number;	// ���汾
    uint16_t    minor_version_number;	// �ΰ汾
    uint16_t    revision_number;			// �޶��汾
    uint16_t    build_number;					// �����汾
};

struct Project_Date_Time_Str
{
    uint16_t hardware_year;     //���̱������
    uint16_t hardware_mon;      //���̱����·�
    uint16_t hardware_day;      //���̱����շ�
		uint16_t hardware_hour;			//���̱���Сʱ
		uint16_t hardware_minute;		//���̱������
		uint16_t hardware_second;		//���̱�������
};



void show_ctu_msg(void);


#endif

