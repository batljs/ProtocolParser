/*
ProtocolParser类的各个函数的实现
*/
#include "ProtocolParser.h"
#include <iostream>

using namespace std;

//获取协议首部指定索引的字符,这里默认首部为一个字节，比如说为0xAA
char ProtocolParser::GetHeader()
{
	//int headerLength = strlen(m_cHeader);
	return m_cHeader;
}
//获取“控制指令”字段的长度，相当于从一个字节转换成int
size_t ProtocolParser::GetCmdLength()
{
	int len = strlen(buffer);
	if (len)
	{
		m_nCmdLength = (buffer[0] - '0') & 0xff;
		//return m_nCmdLength;
	}
	return m_nCmdLength;
}
//获取实际接收到的校验和
size_t ProtocolParser::GetCheckSum()
{
	//int len = strlen(buffer);
	m_nRecvCheckSum = buffer[GetCmdLength() + 1] & 0xff;
	return m_nRecvCheckSum;
}
//从buffer中解析出设备类型
size_t ProtocolParser::GetDeviceType()
{
	//int len = strlen(buffer);
	//return (buffer[2] - '0') * 16 + (buffer[3] - '0') * 1;
	return buffer[1] & 0xff;
}
//从buffer中解析出设备号
size_t ProtocolParser::GetDeviceNumber()
{
	//int len = strlen(buffer);
	//return (buffer[4] - '0') * 16 + (buffer[5] - '0') * 1;
	return buffer[2] & 0xff;
}
//从buffer中解析出端口号
size_t ProtocolParser::GetPort()
{
	//int len = strlen(buffer);
	//return (buffer[6] - '0') * 16 + (buffer[7] - '0') * 1;
	return buffer[3] & 0xff;
}

//将从串口接收的字符串存入buffer中
void ProtocolParser::AppendChar(char ch)
{
	size_t bufferLength = strlen(buffer);
	if (m_nRecvDataIndex==0&&ch==GetHeader())//接收到的数据的索引值为1表示是首部
	{
		m_bInCmd = true;
		buffer[0] = 0;
		m_nRecvDataIndex++;
		return;
	}
	if ((m_nRecvDataIndex == (GetCmdLength()+2))&&m_nCmdLength)//达到了索引值
	{
		buffer[bufferLength] = ch;
		buffer[bufferLength + 1] = '\0';
		size_t chksum = 0;
		//根据接收的数据计算得出校验和
		for (size_t i = 0; i < GetCmdLength(); ++i)
		{
			chksum ^= buffer[i + 1];
		}
		if (chksum == GetCheckSum())//判断实际接收到的校验和跟计算出来的校验和是否相等
		{
			//解析buffer的各个字段的含义
			cout << "DeviceType:" << GetDeviceType() << endl;
			cout << "DeviceNumber:" << GetDeviceNumber() << endl;
			cout << "Port:" << GetPort() << endl;
		}
		else//不相等说明出错了
		{
			cout << "Error" << endl;
		}
		buffer[0] = '\0';
		m_bInCmd = false;
		m_nRecvDataIndex = 0;
		m_nRecvCheckSum = 0;
	}
	else if (m_bInCmd)//指令未接收完毕
	{
		buffer[bufferLength] = ch;
		buffer[bufferLength + 1] = '\0';
		m_nRecvDataIndex++;
	}
}
void ProtocolParser::Append(char *data)
{
	for (size_t i = 0; i < strlen(data); ++i)
	{
		AppendChar(data[i]);
	}
}

int main()
{

	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	ProtocolParser protocolParser(65);
	char ch;
	while (cin >> ch)
	{
		protocolParser.AppendChar(ch);
	}
	return 0;
}

