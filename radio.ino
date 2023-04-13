#include <SPI.h>                                          // Подключаем библиотеку для работы с шиной SPI
#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>  // Подключаем библиотеку для работы с nRF24L01+

#define X A1
#define Y A2
#define Z A3
RF24           radio(9, 10);                              // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN)
int            data[4]; 

const int buttonPin = 2;

int buttonState = 0;
int speed = 0;
int direction = 0;
void setup(){
    radio.begin();                                        // Инициируем работу nRF24L01+
    radio.setChannel(6);                                  // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
    radio.setDataRate     (RF24_1MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
    radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    radio.openWritingPipe (0x1234567890LL);
    Serial.begin(9600);
     pinMode(buttonPin, INPUT_PULLUP);// Открываем трубу с идентификатором 0x1234567890 для передачи данных (на одном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
}
void loop(){
  int x, y, z;
  x = analogRead(X);
  y = analogRead(Y);
  z = analogRead(Z);
    data[0] = 0;
    
    buttonState = digitalRead(buttonPin);// считываем показания Trema слайдера с вывода A1 и записываем их в 0 элемент массива data
         
    data[0] = buttonState;            // считываем показания Trema потенциометра с вывода A2 и записываем их в 1 элемент массива data
  
    
    //Serial.println(data[0]); // отправляем данные из массива data указывая сколько байт массива мы хотим отправить. Отправить данные можно с проверкой их доставки: if( radio.write(&data, sizeof(data)) ){данные приняты приёмником;}else{данные не приняты приёмником;}
    

     if (x > 600) {
      // go forward
      direction = 5; // 3 - Forward, 2 - backward, 1 - right, 0 - left
      speed = map(x, 520, 1023, 5, 255);
        
    }
    else if (x < 500) {
      // go backward
      direction = 1;
      speed = map(x, 520, 0, 5, 255);
    }
    else if (y > 600) {
      // go right
      direction = 2; // 0 - Forward, 1 - backward, 2 - right, 3 - left
      speed = 255; //map(xPosition, 520, 1023, 5, 255);
    }
    else if (y < 450) {
      // go left
      direction = 3; // 0 - Forward, 1 - backward, 2 - right, 3 - left
      speed = map(x, 520, 0, 5, 255);
    }
    else {
      direction = 4;
      speed = 0;
    }

    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(", ");
    Serial.print(z);
    Serial.print(", ");
     Serial.println(direction);
    data[0] = direction;
    data[1] = speed;
    data[2] = !buttonState;
    data[3] = z;
    radio.write(&data, sizeof(data)); 
    delay(100);
    
}
