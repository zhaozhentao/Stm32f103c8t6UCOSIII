#include <stdio.h>
#include "oled.h"

void showDisplayMessage(int p) {
    u8 buf[32] = {0};

    switch (p) {
        case 1:
            sprintf(buf, "正在检测Wifi模块");
            break;
        case 2:
            sprintf(buf, "Wifi模块初始化..");
            break;
        case 3:
            sprintf(buf, "正在连接热点... ");
            break;
        case 4:
            sprintf(buf, "Connecting NTP");
            break;
        case 5:
            sprintf(buf, "Synchronizing ");
            break;
        case 6:
            sprintf(buf, "NTP同步完成... ");
            break;
        case 7:
            sprintf(buf, "%-16s", "");
            break;
        case 8:
            sprintf(buf, "查询天气成功");
            break;
        case 9:
            sprintf(buf, "查询天气超时");
            break;
    }

    OLED_Display_GB2312_string(0, 2, buf);
}
