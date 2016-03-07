/*
ProtocolParser��ĸ���������ʵ��
*/
#include "ProtocolParser.h"
#include <iostream>

using namespace std;

//��ȡЭ���ײ�ָ���������ַ�,����Ĭ���ײ�Ϊһ���ֽڣ�����˵Ϊ0xAA
char ProtocolParser::GetHeader()
{
	//int headerLength = strlen(m_cHeader);
	return m_cHeader;
}
//��ȡ������ָ��ֶεĳ��ȣ��൱�ڴ�һ���ֽ�ת����int
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
//��ȡʵ�ʽ��յ���У���
size_t ProtocolParser::GetCheckSum()
{
	//int len = strlen(buffer);
	m_nRecvCheckSum = buffer[GetCmdLength() + 1] & 0xff;
	return m_nRecvCheckSum;
}
//��buffer�н������豸����
size_t ProtocolParser::GetDeviceType()
{
	//int len = strlen(buffer);
	//return (buffer[2] - '0') * 16 + (buffer[3] - '0') * 1;
	return buffer[1] & 0xff;
}
//��buffer�н������豸��
size_t ProtocolParser::GetDeviceNumber()
{
	//int len = strlen(buffer);
	//return (buffer[4] - '0') * 16 + (buffer[5] - '0') * 1;
	return buffer[2] & 0xff;
}
//��buffer�н������˿ں�
size_t ProtocolParser::GetPort()
{
	//int len = strlen(buffer);
	//return (buffer[6] - '0') * 16 + (buffer[7] - '0') * 1;
	return buffer[3] & 0xff;
}

//���Ӵ��ڽ��յ��ַ�������buffer��
void ProtocolParser::AppendChar(char ch)
{
	size_t bufferLength = strlen(buffer);
	if (m_nRecvDataIndex==0&&ch==GetHeader())//���յ������ݵ�����ֵΪ1��ʾ���ײ�
	{
		m_bInCmd = true;
		buffer[0] = 0;
		m_nRecvDataIndex++;
		return;
	}
	if ((m_nRecvDataIndex == (GetCmdLength()+2))&&m_nCmdLength)//�ﵽ������ֵ
	{
		buffer[bufferLength] = ch;
		buffer[bufferLength + 1] = '\0';
		size_t chksum = 0;
		//���ݽ��յ����ݼ���ó�У���
		for (size_t i = 0; i < GetCmdLength(); ++i)
		{
			chksum ^= buffer[i + 1];
		}
		if (chksum == GetCheckSum())//�ж�ʵ�ʽ��յ���У��͸����������У����Ƿ����
		{
			//����buffer�ĸ����ֶεĺ���
			cout << "DeviceType:" << GetDeviceType() << endl;
			cout << "DeviceNumber:" << GetDeviceNumber() << endl;
			cout << "Port:" << GetPort() << endl;
		}
		else//�����˵��������
		{
			cout << "Error" << endl;
		}
		buffer[0] = '\0';
		m_bInCmd = false;
		m_nRecvDataIndex = 0;
		m_nRecvCheckSum = 0;
	}
	else if (m_bInCmd)//ָ��δ�������
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

