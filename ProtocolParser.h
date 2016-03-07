#ifndef PROTOCOLPARSER_H
#define PROTOCOLPARSER_H
/*
�Զ���Ŀ⺯��

Э������� V1.0
���������ݸ�ʽ��
Э���ײ�-ָ���-����ָ��-У���
"����ָ��"��ʽ��
�豸����-�豸��-�˿ں�
ע�⣺����ÿ���ֶζ�ֻռ��һ���ֽ�

*/
#include<stdlib.h>
#include<string.h>

#pragma warning(disable:4996)
#define BUFFER_SIZE 128//�ٶ��������ݵ���󳤶�Ϊ128
class ProtocolParser
{
public:
	ProtocolParser(char header);
	~ProtocolParser();
	void Append(char *data);
	void AppendChar(char ch);//���Ӵ��ڽ��յ����ַ��浽buffer

	size_t GetDeviceType();//��ȡ�豸����
	size_t GetDeviceNumber();//��ȡ�豸��
	size_t GetPort();//��ȡ�豸�����ӵĶ˿ں�

private:
	bool m_bInCmd;//��־λ����ʶһ������ָ���Ƿ�������
	char m_cHeader;//Э���ײ�
	size_t m_nCmdLength;//ָ���
	size_t m_nRecvCheckSum;//ʵ�ʽ��յ���У���
	char buffer[BUFFER_SIZE];//���յ�������ʱ����buffer��
	size_t m_nRecvDataIndex;//��¼�������ݵ�����

	char GetHeader();//��ȡЭ���ײ�ָ���������ַ�
	size_t GetCmdLength();//��ȡ����ָ���
	size_t GetRecvDataLength();//��ȡ���յ���һ������ָ��ĳ���
	size_t GetCheckSum();//��ȡʵ�ʽ��յ���У���
};

//���캯����ʼ����headerΪЭ���ײ�
ProtocolParser::ProtocolParser(char header)
{
	m_bInCmd = false;

	m_cHeader = header;
	m_nCmdLength = 0;
	m_nRecvCheckSum = 0;
	m_nRecvDataIndex = 0;

	buffer[0] = '\0';
}

ProtocolParser::~ProtocolParser()
{
}
#endif