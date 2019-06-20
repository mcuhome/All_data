//GY-25  ARDUINO  测试代码 IICLCD2004显示角度
//注意1：下载程序时候，请先断开GY25的连线!否则将导致下载不成功
//注意2：GY25模块使用时，上电自校正,建议不要用手拿着模块,
//       保持3秒以上静止状态，才能发送命令开始工作
//   GY25                  arduino pro mini
//   VCC----------------------VCC
//   RX-----------------------TX
//   TX-----------------------RX
//   GND----------------------GND
//-----------------------------------------------
//   IICLCD2004            arduino pro mini
//   VCC----------------------VCC
//   SCL----------------------A5
//   SDA----------------------A4
//   GND----------------------GND
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
int YPR[3];
unsigned char Re_buf[8],counter=0;
unsigned char sign=0;
int led = 13;
LiquidCrystal_I2C lcd(0x20,20,4);  // set the LCD address to 0x20 for a 20 chars and 4 line display

//-----------------------------------------------------------
void setup()
{
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.    
  Serial.begin(115200);  
  delay(2000);    
  Serial.write(0XA5); 
  Serial.write(0X52);    //初始化GY25,连续输出模式
  lcd.backlight();
  lcd.setCursor(0,0);     //I2C接口LCD2004显示初始值
  lcd.print("Yaw:");
  lcd.setCursor(0,1);
  lcd.print("Pitch:");
  lcd.setCursor(0,2);
  lcd.print("Roll:");
}
//-------------------------------------------------------------
void loop() {
  if(sign)
  {  
     sign=0;
     if(Re_buf[0]==0xAA && Re_buf[7]==0x55)        //检查帧头，帧尾
     {  	       
            YPR[0]=(Re_buf[1]<<8|Re_buf[2])/100;   //合成数据，去掉小数点后2位
            YPR[1]=(Re_buf[3]<<8|Re_buf[4])/100;
            YPR[2]=(Re_buf[5]<<8|Re_buf[6])/100;
            
            lcd.setCursor(4,0);     
            lcd.print("    ");
            lcd.setCursor(4,0);
	    lcd.print(YPR[0]);      //显示航向

            lcd.setCursor(6,1);    
            lcd.print("    ");                    
            lcd.setCursor(6,1);    //显示俯仰角
	    lcd.print(YPR[1]);     

            lcd.setCursor(5,2);   
            lcd.print("    ");  
            lcd.setCursor(5,2);     //显示横滚角
	    lcd.print(YPR[2]);       
            delay(100);           
   }
  } 
} 
//---------------------------------------------------------------
void serialEvent() {
  while (Serial.available()) {   
    Re_buf[counter]=(unsigned char)Serial.read();
    if(counter==0&&Re_buf[0]!=0xAA) return;      // 检查帧头         
    counter++;       
    if(counter==8)                //接收到数据
    {    
       counter=0;                 //重新赋值，准备下一帧数据的接收 
       sign=1;
    }      
  }
}

