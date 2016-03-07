# ProtocolParser
A self-defined protocal parser based on arduino

Arduino通信协议设计-V1.0

分析
先从最简单的模块开始，控制电机的旋转：反转和正转。
Arduino开发板包含四个端口，每个端口都有标号，1-4。可以将电机连接在这四个端口的其中一个端口。上位机通过蓝牙模块向底层的硬件设备发送控制命令。比如说：某个端口上的某个电机正转或者反转。

协议
上位机实际发送的数据格式如下，主要包含4个字段：
协议首部	指令长度	控制指令	校验和
其中，“协议首部”就是数据包的开始部分，比如说统一指定为0xAA；“指令长度”表示“控制指令”字段的长度；“控制指令”就是上位机实际发送的对底层硬件设备的控制命令；“校验和”是“控制指令”各个数据位进行异或计算的结果。比如说：一个合法的数据包可以是这样的”AA 03 D1 D2 D3 D4”，其中，D4是校验和，D4=D1^D2^D3。
对于“控制指令”需要进一步设计。对于电机来说，控制指令可以设计成如下格式：
设备类型	设备号	端口号
其中，“设备类型”就是指定是哪一种设备，比如说，电机，LED，蜂鸣器等；“设备号”就是设备的标号，可以为1,2,3，等；“端口号”就是指该设备连接在哪个端口上面。
上述每个字段都只占用一个字节，取值范围都是从0~255.

编程思路
Arduino是串口发送的，也就是说数据包是一位一位地发送的，因此需要先将接收的数据保存起来，然后按位进行解析。解析出各个字段的含义以后就可以按照Arduino硬件平台的类C语言进行编程控制硬件设备了。

