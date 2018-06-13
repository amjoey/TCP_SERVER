#include <ESP8266WiFi.h>
#include <EEPROM.h>

#define SERVER_PORT 21111    //กำหนด Port ใช้งาน
const char* ssid = "wifi2013";       //กำหนด SSID
const char* password = "the0815993598"; //กำหนด Password
WiFiServer server(SERVER_PORT); //สร้าง object server และกำหนด port

int pin = 2;
String line,selector;

int address = 0;

int value;

void setup() {
  EEPROM.begin(512);
  pinMode(pin, OUTPUT);

    value = EEPROM.read(address);

    if(value==1){
      digitalWrite(pin, LOW); // ให้ LED ติด
    }

    if(value==0){
      digitalWrite(pin, HIGH); // ให้ LED ติด
    }
  
  Serial.begin(115200); // เปิดใช้การ Debug ผ่าน Serial
     WiFi.begin(ssid, password); //เชื่อมต่อกับ AP
     while (WiFi.status() != WL_CONNECTED)  //รอการเชื่อมต่อ
    {
            delay(500);
            Serial.print(".");
    }
            Serial.println("WiFi connected");   //แสดงข้อความเชื่อมต่อสำเร็จ  
    Serial.println("IP address: "); 
    Serial.println(WiFi.localIP());     //แสดงหมายเลข IP
            server.begin();             //เริ่มต้นทำงาน TCP Server
}

void loop() {
  value = EEPROM.read(address);

  if(value==1){
    digitalWrite(pin, LOW); // ให้ LED ติด
  }

  if(value==0){
    digitalWrite(pin, HIGH); // ให้ LED ติด
  }
        
  WiFiClient client = server.available();
  if (!client) // ถ้าไม่มีการเชื่อมต่อมาใหม่
    return; // ส่งลับค่าว่าง ทำให้ลูปนี้ถูกยกเลิก

  Serial.println("New client"); // ส่งข้อความว่า New client ไปที่ Serial Monitor
  while (client.connected()) { // วนรอบไปเรื่อย ๆ หากยังมีการเชื่อมต่ออยู่
    if (client.available()) { // ถ้ามีการส่งข้อมูลเข้ามา
      char c = client.read(); // อ่านข้อมูลออกมา 1 ไบต์
      if (c == '\r') { // ถ้าเป็น \r (return)
        Serial.println(line); // แสดงตัวแปร line ไปที่ Serial Monitor
        if (line == "LEDON") { // ถ้าสงข้อความเข้ามาว่า LEDON
         // digitalWrite(pin, LOW); // ให้ LED ติด
          EEPROM.write(address, 1);
        } 

        if (line == "LEDOFF") { // ถ้าสงข้อความเข้ามาว่า LEDOFF
         // digitalWrite(pin, HIGH); // ให้ LED ดับ
          EEPROM.write(address, 0);
        }

        EEPROM.commit();


        selector = line;
        line = ""; // ล้างค่าตัวแปร line
        break; // ออกจากลูป
      } else if (c == '\n') { // ถ้าเป็น \n (new line)
        // Pass {new line}
   
      } else { // ถ้าไม่ใช่
        line += c; // เพิ่มข้อมูล 1 ไบต์ ไปต่อท้ายในตัวแปร line
      }

      
    }                    
  }
  //client.println("1");
  if(selector=="UPDATE"){
    client.println("%UPDATE%,"+String(value));
  }else{
    client.println("%OK%");
  }
  selector = "";
  delay(10);//หน่วยเวลาเพื่อให้ส่ง client.println("%OK%");
  client.stop(); // ปิดการเชื่อมต่อกับ Client
  Serial.println("Client disconnect"); // ส่งข้อความว่า Client disconnect ไปที่ Serial Monitor
}
