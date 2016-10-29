# ESP8266 MeteoMonitoring Module with web-interface

![alt tag](https://habrastorage.org/files/534/1b8/ec6/5341b8ec6aca4be5a98198a8218210de.PNG)
![alt tag](https://habrastorage.org/files/5af/e07/81f/5afe0781fa8245e7bd4ed50a54437ec5.png) 

Реалізована підтримка експорту даних на такі хмарні сервіси як: Narodmon.ru та ThingSpeak. Підтримуються такі давачі даних: DS18B20, DHT11/22, BMP180 та дані з аналогового вивиоду ADC.

Для виконання початкових налаштувань, модуль створює точку доступу MonitoringSystem з паролем admin123 (в подальшому це можна змінити в веб-інтерфейсі) і стає доступним за ip адресою 192.168.4.1, після конфігуварання мережі і перезавантаження, ip адресу модуля можна дізнатись в налаштуваннях.

Номери виводів до яких підключаються давачі даних можна змінити в коді.
По замовчуванню для давачів визначені такі виводи:

1. DS18B20 - 0

2. DHT11/22 - 2

3. BMP180 - залежно від версії (опис нижче)

4. Аналогові дані - вивід ADC.

в ESP-01 є можливість нормально використовувати лише 2 виводи: GPIO0 і GPIO2. Тому на цій версії можливо підключити давачі даних двома варіантами:

1. DS18B20 на 0 вивід та
   DHT11/22 на 2 вивід

2. BMP180 на 2 і 0, розкоментувавши функцію Wire.begin(2, 0);

3. (Аналогові дані на ESP-01 неможливо приймати, так як вивід ADC відсутній)

в ESP-07, ESP-12(E) та інших версіях кількість виводів, які можна використовувати для вводу/виводу значно більша, тому можна підключити всі давачі даних відразу. Для давача BMP180 виводи, встановлюються через функцію Wire.begin([SDA], [SCL]).


Для завантаження прошивки в пам'ять ESP8266 потрібно використовувати Arduino IDE останньої версії, налаштовану для роботи з ESP8266.
Інструкція, як це зробити доступна за посиланнями: https://github.com/esp8266/Arduino або https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/installing-the-esp8266-arduino-addon
