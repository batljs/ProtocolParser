#include<stdlib.h>
#include<string.h>

//#pragma warning(disable:4996)
#define BUFFER_SIZE 128//假定接收数据的最大长度为128
class ProtocolParser
{
public:
  ProtocolParser(char header);
  ~ProtocolParser();
  void Append(char *data);
  void AppendChar(char ch);//将从串口接收到的字符存到buffer

  size_t GetDeviceType();//获取设备类型
  size_t GetDeviceNumber();//获取设备号
  size_t GetPort();//获取设备所连接的端口号

private:
  bool m_bInCmd;//标志位，标识一条控制指令是否接收完毕
  char m_cHeader;//协议首部
  size_t m_nCmdLength;//指令长度
  size_t m_nRecvCheckSum;//实际接收到的校验和
  char buffer[BUFFER_SIZE];//接收的数据暂时存在buffer中
  size_t m_nRecvDataIndex;//记录接收数据的索引

  char GetHeader();//获取协议首部指定索引的字符
  size_t GetCmdLength();//获取控制指令长度
  size_t GetRecvDataLength();//获取接收到的一条控制指令的长度
  size_t GetCheckSum();//获取实际接收到的校验和
};

//构造函数初始化，header为协议首部
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
  return (buffer[1]-'0') & 0xff;
}
//从buffer中解析出设备号
size_t ProtocolParser::GetDeviceNumber()
{
  //int len = strlen(buffer);
  //return (buffer[4] - '0') * 16 + (buffer[5] - '0') * 1;
  return (buffer[2]-'0') & 0xff;
}
//从buffer中解析出端口号
size_t ProtocolParser::GetPort()
{
  //int len = strlen(buffer);
  //return (buffer[6] - '0') * 16 + (buffer[7] - '0') * 1;
  return (buffer[3]-'0') & 0xff;
}

//将从串口接收的字符串存入buffer中
void ProtocolParser::AppendChar(char ch)
{
  //Serial.println("OK!");
  size_t bufferLength = strlen(buffer);
  //Serial.println(bufferLength);
  if (m_nRecvDataIndex==0&&ch==GetHeader())//接收到的数据的索引值为1表示是首部
  {
    m_bInCmd = true;
    buffer[0] = 0;
    m_nRecvDataIndex++;
    return;
  }
  //Serial.println("OK!");
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
    Serial.println(chksum);
    if (chksum == GetCheckSum())//判断实际接收到的校验和跟计算出来的校验和是否相等
    {
      Serial.println("DeviceType:");
      Serial.println(GetDeviceType(),DEC);
      Serial.println("DeviceNumber:");
      Serial.println(GetDeviceNumber(),DEC);
      Serial.println("Port:");
      Serial.println(GetPort(),DEC);
      //解析buffer的各个字段的含义
      //cout << "DeviceType:" << GetDeviceType() << endl;
      //cout << "DeviceNumber:" << GetDeviceNumber() << endl;
      //cout << "Port:" << GetPort() << endl;
    }
    else//不相等说明出错了
    {
      //cout << "Error" << endl;
      Serial.println("Error");
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

char value;
ProtocolParser protocolParser('A');
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial.println("Parser:");
  //pinMode(led,OUTPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()>0){
    value=Serial.read();
    //Serial.print(value);
    protocolParser.AppendChar(value);
    //Serial.println(protocolParser.getPort());
  }
}
