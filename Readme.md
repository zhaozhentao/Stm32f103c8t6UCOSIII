### 烧录

不论是串口烧录还是 st-link 烧录，都要设置 boot0 引脚 1 (需要复位)，进入烧录模式。

1. 串口烧录

```sh
# /dev/cu.usbserial-0001 视具体的串口设备
$ stm32flash -w led.bin -v -g 0x0 /dev/cu.usbserial-0001
```

2. st-link 烧录
```sh
$ st-flash write led.bin 0x8000000
```
