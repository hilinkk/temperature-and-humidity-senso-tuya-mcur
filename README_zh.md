# temperature-and-humidity-senso-tuya-mcur
This project is developed using Tuya SDK, which enables you to quickly develop brandedapps connecting and controlling smart scenarios of many devices.
For more information, please check Tuya Developer Website.



演示视频：



# 使用说明

- 长按key0进行开关机
- 开机后长按key1进入或退出设置界面
- 短按key0和key1则进行页面切换
- 在设置界面中，短按key1进行功能选择，短按key0确定执行对应的功能



## 硬件设计

先上图：


![61c946bee2bd0fbfcc09786e8bf4a04](https://user-images.githubusercontent.com/62596403/109374783-d079e400-78f2-11eb-8d18-fda1e505a30f.jpg)
![65ea7a58b4f5b6314ac65d68243af2b](https://user-images.githubusercontent.com/62596403/109374789-d4a60180-78f2-11eb-9f46-a1ffeabdea10.jpg)


注意到上面的时间了吗，这篇文章是我熬夜赶出来的，看来临时抱佛脚的习惯得改改了，hhh。

具体电路请参考项目原理图

- 控制芯片采用的是STM32F103C8T6，原因是我对这块芯片最为熟悉。
- 采用锂电池进行供电，并可通过typec接口进行充电。
- 板载了CH340N芯片，可与电脑进行串口通信，方便调试。
- 采用两片ME6212C33芯片进行稳压，其中一块单独给MCU供电，另一块给其他外设供电。这样有利于电路稳定，也可通过MCU控制断开其他外设的电源，起到节电的目的。

- 联网模组采用的是这次活动官方推荐的“WB3S-IPEX WiFi&BLE 双协议模组”，具有wifi和蓝牙功能。为避免模组的发热影响温度的测量，同时也为了避免模组的信号受到干扰，我将模组放到了另一块板子上，两个板子通过排针排母进行连接。涂鸦扩展板开源链接：[https://lceda.cn/linkk/zhi-neng-wen-shi-du-chuan-gan-qi_-tu-ya-kuo-zhan-ban](https://lceda.cn/linkk/zhi-neng-wen-shi-du-chuan-gan-qi_-tu-ya-kuo-zhan-ban)
- 温湿度传感器采用的是常用的SHT30，同时在它周围进行了开槽处理，尽量降低板子温度对SHT30的干扰。不过实际使用中测量到的温度还是会比真实温度高一点。
- 通过OLED屏进行显示。
- 板载两个按键和三个LED。
- 虽然搭载了这么多硬件，但板子的大小只有40mm X 40mm，非常小巧。



## 涂鸦 MCU SDK 的移植

> 1、首先在涂鸦工作台中下载所需要的 MCU SDK，如下图所示：
>
> ![image-20210226170231931](https://user-images.githubusercontent.com/62596403/109374797-e687a480-78f2-11eb-8697-afd0dd56c666.png)


> 2、然后将里面所有的 .c 文件添加进过程，并添加头文件路径。


> 3、进入wifi.h，添加 #include "stm32f10x.h" （根据自己的芯片进行修改，我选用的芯片是stm32f103c8t6）
>
> ![image-20210226170814389](https://user-images.githubusercontent.com/62596403/109374802-f99a7480-78f2-11eb-885c-6404bca4400c.png)


> 4、最后进入protocol.h中，根据自己的需要进行配置，再根据编译错误提示进行修改即可。
>
> 要修改的地方有如下几个：
>
> 4.1、在uart_transmit_output函数中调用串口单字节发送函数，注意是单字节发送！！！
>
> ![image-20210226171831080](https://user-images.githubusercontent.com/62596403/109374806-00c18280-78f3-11eb-94db-707ecebd07a6.png)
>
> 4.2、在串口接收中断中调用uart_receive_input(value)，参数value是中断接收到的数据。记得先打开串口中断，程序运行过程中也尽量不要关闭所用到的串口中断。
>
> ![image-20210226172113816](https://user-images.githubusercontent.com/62596403/109374815-0a4aea80-78f3-11eb-8522-9ce7936d1e17.png)
>
> 4.3、进入函数my_strcpy(char *dest, const char *src) ，将里面指针p的定义放到函数最前面。
>
> ![image-20210226173120708](https://user-images.githubusercontent.com/62596403/109374820-15057f80-78f3-11eb-8927-7fa4aa753efe.png)
>
> 4.4、在MCU主函数while循环中调用函数：wifi_uart_service()
>
> 4.5、在main函数中添加wifi_protocol_init()完成wifi协议初始化
>
> 4.6、根据自己的需求处理自动化生成数据上报函数all_data_update()
>
> 4.7、其他报错直接注释掉引起报错的那行即可
>
> 最后修改结果如下，0 Error, 0 Warning，哈哈哈
>
> ![image-20210226173327109](https://user-images.githubusercontent.com/62596403/109374825-1df65100-78f3-11eb-8d8e-8e71c12a4998.png)



## 踩坑记录

> 1、如果需要用到获取天气的功能，一定要吧串口数据处理缓存区大小改大点，不然接收不到处理后的数据。
>
> ![image-20210226201316139](https://user-images.githubusercontent.com/62596403/109374826-251d5f00-78f3-11eb-8d4c-78257cae6533.png)
>
> 2、上面移植过程中已经提到过的，在uart_transmit_output函数中调用的串口发送函数，发送的数据一定要是单字节的。
>
> 3、获取天气数据时一定要看官方参考文档，确认数据的类型，不要只凭自己的感觉。比如风速是字符串类型的，我就在这卡了大半天，服了自己了。
>
> ![image-20210226234043536](https://user-images.githubusercontent.com/62596403/109374837-323a4e00-78f3-11eb-9d4d-46e70c3bc4ce.png)
