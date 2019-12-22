
#define INTERVAL_SENSOR 1000 //定义传感器采样时间间隔 597000
#define INTERVAL_NET 1000 //定义发送时间
//传感器部分================================
const int TrigPin = 9; //发送超声波
const int EchoPin = 10; //接收超声波
float cm;
#include <SoftwareSerial.h>
#include <Wire.h> //调用库
 #include "ESP8266WiFi.h"//调用库
#include "I2Cdev.h" //调用库

#include <SHT2x.h>


#define sensorPin_1 A0//定义串口
#define SSID "z" // cannot be longer than 32 characters!//Wi-Fi名称
#define PASSWORD "*********"//wifi密码

#define IDLE_TIMEOUT_MS 3000 
//WEBSITE
#define HOST_NAME "api.heclouds.com"
#define DEVICEID "************"
#define PROJECTID "*******"//填写自己设备的apikey，api地址 ，设备id
#define HOST_PORT (80)
String apiKey="**********";
char buf[10];

//3,传感器值的设置
float sensor_tem, sensor_hum, sensor_lux; //传感器
char sensor_tem_c[7], sensor_hum_c[7], sensor_lux_c[7] ; //换成char数组传输
#include <SoftwareSerial.h>//调用类库模拟成的串口，称为软件模拟串口
SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 *///定义软串口2，3引脚分别为rx，tx
ESP8266 wifi(mySerial);
//ESP8266 wifi(Serial1); //定义一个ESP8266（wifi模块）的对象
unsigned long net_time1 = millis(); //数据上传服务器时间
unsigned long sensor_time = millis(); //传感器采样时间计时器

// include the library code:
#include <LiquidCrystal.h>//调用库

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
void setup(void) //初始化函数
{
//初始化串口波特率
Wire.begin();
Serial.begin(115200);
while(!Serial);
pinMode(sensorPin_1, INPUT);

//ESP8266初始化
Serial.print("setup begin\r\n");

Serial.print("FW Version:");
Serial.println(wifi.getVersion().c_str());

if (wifi.setOprToStationSoftAP()) {
Serial.print("to station + softap ok\r\n");
} else {
Serial.print("to station + softap err\r\n");
}//判断是否成功连接

if (wifi.joinAP(SSID, PASSWORD)) { //加入无线网
Serial.print("Join AP success\r\n");
Serial.print("IP: ");
Serial.println(wifi.getLocalIP().c_str());
} else {
Serial.print("Join AP failure\r\n");
}//判断是否成功加入AP并在串口中显示

if (wifi.disableMUX()) {
Serial.print("single ok\r\n");
} else {
Serial.print("single err\r\n");
}

Serial.print("setup end\r\n");
pinMode(TrigPin, OUTPUT);//设置超声波反馈端为输出
pinMode(EchoPin, INPUT);//设置超声波触发端为输入

}
void loop(void) //循环函数
{
// 产生一个10us的高电平脉冲去触发TrigPin
digitalWrite(TrigPin, LOW);
delayMicroseconds(2);
digitalWrite(TrigPin, HIGH);
delayMicroseconds(10);
digitalWrite(TrigPin, LOW);

// 检测脉冲宽度，并计算出距离
cm = pulseIn(EchoPin, HIGH) / 58.0
//通过串口监视器输出
Serial.print("垃圾深度");
Serial.print(170.00-cm);//计算垃圾的深度
Serial.println("cm");

if (sensor_time > millis()) sensor_time = millis();

if(millis() - sensor_time > INTERVAL_SENSOR) //传感器采样时间间隔
{
getSensorData(); //读串口中的传感器数据
sensor_time = millis();
}
if (net_time1 > millis()) net_time1 = millis();

if (millis() - net_time1 > INTERVAL_NET) //发送数据时间间隔
{
updateSensorData(); //将数据上传到服务器onenet的函数
net_time1 = millis();
}

}

void getSensorData(){
sensor_tem = cm ;
sensor_hum = SHT2x.readRH();//wifi模块获取超声波模块测得数据
//获取depth
sensor_lux = analogRead(A0);//用串口读取模拟口数据
delay(1000);//每次获取的时间间隔
dtostrf(sensor_tem, 2, 1, sensor_tem_c);
dtostrf(sensor_hum, 2, 1, sensor_hum_c);
dtostrf(sensor_lux, 3, 1, sensor_lux_c);
}
void updateSensorData() {
if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
Serial.print("create tcp ok\r\n");

jsonToSend="{\"Temperature\":";
dtostrf(sensor_tem,1,2,buf);
jsonToSend+="\""+String(buf)+"\"";
jsonToSend+=",\"Humidity\":";
dtostrf(sensor_hum,1,2,buf);
jsonToSend+="\""+String(buf)+"\"";
jsonToSend+=",\"Light\":";
dtostrf(sensor_lux,1,2,buf);
jsonToSend+="\""+String(buf)+"\"";
jsonToSend+="}";////把要发送的数据转成字符串

postString="POST /devices/";
postString+=DEVICEID;
postString+="/datapoints?type=3 HTTP/1.1";
postString+="\r\n";
postString+="api-key:";
postString+=apiKey;
postString+="\r\n";
postString+="Host:api.heclouds.com\r\n";
postString+="Connection:close\r\n";
postString+="Content-Length:";
postString+=jsonToSend.length();
postString+="\r\n";
postString+="\r\n";
postString+=jsonToSend;
postString+="\r\n";
postString+="\r\n";
postString+="\r\n";//报文字符串的拼接，具体url的格式参考onenet文档要求

const char *postArray = postString.c_str(); //将str转化为char数组
Serial.println(postArray);
wifi.send((const uint8_t*)postArray, strlen(postArray)); //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
Serial.println("send success");
if (wifi.releaseTCP()) { //释放TCP连接
Serial.print("release tcp ok\r\n");
}
else {
Serial.print("release tcp err\r\n");
}
postArray = NULL; //清空数组，等待下次传输数据

} else {
Serial.print("create tcp err\r\n");
}

}



