#include <stdio.h>
#include "oled.h"

void showDisplayMessage(int p) {
    u8 buf[32] = {0};

    switch (p) {
        case 1:
            sprintf(buf, "正在检测Wifi模块");
            break;
        case 2:
            sprintf(buf, "%-16s", "查询IP...");
            break;
        case 3:
            sprintf(buf, "%-16s", "设置AP模式...");
            break;
        case 4:
            sprintf(buf, "%-16s", "正在连接WIFI...");
            break;
        case 5:
            sprintf(buf, "%-16s", "建立HTTP请求...");
            break;
        case 6:
            sprintf(buf, "%-16s", "设置请求长度...");
            break;
        case 7:
            sprintf(buf, "%-16s", "查询天气成功   ");
            break;
        case 8:
            sprintf(buf, "%-16s", "查询天气超时   ");
            break;
        case 9:
            sprintf(buf, "%-16s", "");
            break;
        case 10:
            sprintf(buf, "%-16s", "建立NTP连接...");
            break;
        case 11:
            sprintf(buf, "%-16s", "NTP同步成功...");
            break;
        case 12:
            sprintf(buf, "%-16s", "断开TCP连接...");
            break;
    }

    OLED_Display_GB2312_string(0, 2, buf);
}
