#include "protocol.h"
#include "crc32.h"
#include "crc8.h"
#include <stdint.h>
#include "my_printf.h"
#include <string.h>
#include "uart_mgmt.h"


#pragma pack(1) // 1字节对齐

struct IAP_Hand_Pack_Str
{
    uint8_t  Head;          // 帧头
    uint8_t  Pack_Type;     // 帧类型，01握手帧，02升级帧，03校验帧

    uint16_t Upgrade_Type;  // 升级类型，1:Boot，2:App
    uint16_t File_Divide_Size;  // 分片大小
    uint16_t File_All_Divids;   // 总分片大小
    uint32_t File_Size;     // 程序总大小
    uint32_t File_CRC;      // 程序CRC32
    
    uint8_t  Head_CRC;      // 帧CRC
};

#pragma pack()

struct IAP_Hand_Pack_Str IAP_Hand_Pack;

void Hand_Pack_Write(uint8_t *pbuf, uint8_t len)
{
    memcpy(&IAP_Hand_Pack, pbuf, sizeof(IAP_Hand_Pack));
}

void Hand_Pack_Deal(void)
{
    uint8_t crc8_cal = 0;

    

    show_arr("IAP_Hand_Pack Get", &IAP_Hand_Pack, sizeof(IAP_Hand_Pack));

    crc8_cal = cal_crc8(&IAP_Hand_Pack, sizeof(IAP_Hand_Pack)-1);
    if(crc8_cal == IAP_Hand_Pack.Head_CRC)
    {
        MyPrintf("Hand_Pack Check Success!!!\r\n");
        IAP_Hand_Pack.Head = 0xA5;
        IAP_Hand_Pack.Pack_Type = IAP_Hand_Pack.Pack_Type | 0x80;
        IAP_Hand_Pack.Head_CRC = cal_crc8(&IAP_Hand_Pack, sizeof(IAP_Hand_Pack)-1);
        Uart_Send(&IAP_Hand_Pack, sizeof(IAP_Hand_Pack));	// 发送数据
        show_arr("IAP_Hand_Pack Send", &IAP_Hand_Pack, sizeof(IAP_Hand_Pack));
    }
}

struct IAP_Master_Updata_Pack_Str
{
    uint8_t  Head;          // 帧头
    uint8_t  Pack_Type;     // 帧类型，01握手帧，02升级帧，03校验帧

    uint16_t File_Divide_idx;       // 分包序号,1~n
    uint16_t File_Divide_Len;       // 分包长度512
    uint8_t  File_Divide_Data[512]; // 升级包

    uint8_t  Head_CRC;      // 帧CRC
};
struct IAP_Master_Updata_Pack_Str IAP_Master_Updata_Pack;

void Updata_Pack_Write(uint8_t *pbuf, uint8_t len)
{
    memcpy(&IAP_Master_Updata_Pack, pbuf, sizeof(IAP_Master_Updata_Pack));
}

void Updata_Pack_Deal(void)
{
    uint8_t crc8_cal = 0;

    show_arr("IAP_Master_Updata_Pack Get", &IAP_Master_Updata_Pack, sizeof(IAP_Master_Updata_Pack));

    crc8_cal = cal_crc8(&IAP_Master_Updata_Pack, sizeof(IAP_Master_Updata_Pack)-1);
    if(crc8_cal == IAP_Master_Updata_Pack.Head_CRC)
    {
        MyPrintf("IAP_Master_Updata_Pack Check Success!!!\r\n");
    }
}



