# temperature-and-humidity-senso-tuya-mcur
[中文页](https://github.com/lin-kangkang/temperature-and-humidity-senso-tuya-mcur/blob/main/README_zh.md)

This project is developed using Tuya SDK, which enables you to quickly develop brandedapps connecting and controlling smart scenarios of many devices.
For more information, please check Tuya Developer Website.


Video presentation:

## Directions for use

- Long press KEY0 to switch the machine.
- Long press KEY1 to enter or exit the setting interface after boot.
- Press key0 and key1 to switch pages.
- In the setting interface, press key1 for function selection, and press key0 to execute the corresponding function.



## The hardware design

![61c946bee2bd0fbfcc09786e8bf4a04](https://user-images.githubusercontent.com/62596403/109381559-16946f00-7916-11eb-8e1d-841dfe77d1e0.jpg)

![65ea7a58b4f5b6314ac65d68243af2b](https://user-images.githubusercontent.com/62596403/109384244-77c43e80-7926-11eb-844c-c91bcdf67741.jpg)


Please refer to the schematic diagram for the specific circuit

- The control chip is STM32F103C8T6, because I am most familiar with this chip.
- Lithium-ion battery is used for power supply and can be charged through TypeC interface.
- CH340N chip on board, can carry on serial communication with computer, convenient debugging.
- The networking module is the official recommendation of this event "WB3S-IPEX WIFI&BLE dual-protocol module", with WiFi and Bluetooth functions.https://lceda.cn/linkk/zhi-neng-wen-shi-du-chuan-gan-qi_-tu-ya-kuo-zhan-ban](https://lceda.cn/linkk/zhi-neng-wen-shi-du-chuan-gan-qi_-tu-ya-kuo-zhan-ban)
- Temperature and humidity sensor is commonly used SHT30, at the same time around it for grooving treatment, as far as possible to reduce the interference of the board temperature on SHT30. However, in actual use, the measured temperature is still slightly higher than the real temperature.
- Display via an OLED screen.
- The board has two buttons and three LEDs.
- Even with all that hardware, the board is only 40mm X 40mm, which is pretty compact.

