#include <EtherCard.h>

static byte mymac[] = {0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A };
static byte myip[] = {210, 10, 41, 104 };
bool flag, flag1;
byte Ethernet::buffer[900];
BufferFiller bfill;
int LedPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
boolean PinStatus[8];

const char http_OK[] PROGMEM =
  "HTTP/1.0 200 OK\r\n"
  "Content-Type: text/html\r\n"
  "Pragma: no-cache\r\n\r\n";

const char http_Found[] PROGMEM =
  "HTTP/1.0 302 Found\r\n"
  "Location: /\r\n\r\n";

void homePage()
{
  bfill.emit_p(PSTR("$F"
                    "<title>ArduinoPIN Webserver</title>"
                    "ArduinoPIN 2: <a href=\"?ArduinoPIN0=$F\">$F</a><br />"
                    "ArduinoPIN 3: <a href=\"?ArduinoPIN1=$F\">$F</a><br />"
                    "ArduinoPIN 4: <a href=\"?ArduinoPIN2=$F\">$F</a><br />"
                    "ArduinoPIN 5: <a href=\"?ArduinoPIN3=$F\">$F</a><br />"
                    "ArduinoPIN 6: <a href=\"?ArduinoPIN4=$F\">$F</a><br />"
                    "ArduinoPIN 7: <a href=\"?ArduinoPIN5=$F\">$F</a><br />"
                    "ArduinoPIN 8: <a href=\"?ArduinoPIN6=$F\">$F</a><br />"
                    "ArduinoPIN 9: <a href=\"?ArduinoPIN7=$F\">$F</a>"),
               http_OK,
               PinStatus[0] ? PSTR("off") : PSTR("on"),
               PinStatus[0] ? PSTR("<font color=\"green\"><b>ON</b></font>") : PSTR("<font color=\"red\">OFF</font>"),
               PinStatus[1] ? PSTR("off") : PSTR("on"),
               PinStatus[1] ? PSTR("<font color=\"green\"><b>ON</b></font>") : PSTR("<font color=\"red\">OFF</font>"),
               PinStatus[2] ? PSTR("off") : PSTR("on"),
               PinStatus[2] ? PSTR("<font color=\"green\"><b>ON</b></font>") : PSTR("<font color=\"red\">OFF</font>"),
               PinStatus[3] ? PSTR("off") : PSTR("on"),
               PinStatus[3] ? PSTR("<font color=\"green\"><b>ON</b></font>") : PSTR("<font color=\"red\">OFF</font>"),
               PinStatus[4] ? PSTR("off") : PSTR("on"),
               PinStatus[4] ? PSTR("<font color=\"green\"><b>ON</b></font>") : PSTR("<font color=\"red\">OFF</font>"),
               PinStatus[5] ? PSTR("off") : PSTR("on"),
               PinStatus[5] ? PSTR("<font color=\"green\"><b>ON</b></font>") : PSTR("<font color=\"red\">OFF</font>"),
               PinStatus[6] ? PSTR("off") : PSTR("on"),
               PinStatus[6] ? PSTR("<font color=\"green\"><b>ON</b></font>") : PSTR("<font color=\"red\">OFF</font>"),
               PinStatus[7] ? PSTR("off") : PSTR("on"),
               PinStatus[7] ? PSTR("<font color=\"green\"><b>ON</b></font>") : PSTR("<font color=\"red\">OFF</font>"));
}
//------------------------
void setup()
{
  Serial.begin(9600);
  ether.begin(sizeof Ethernet::buffer, mymac, 10);
  ether.staticSetup(myip);
  for (int i = 0; i < 8; i++) {
    pinMode(LedPins[i], OUTPUT);
    PinStatus[i] = false;
  }
}
// --------------------------------------
void loop() {
  char etalon[16];
  delay(1);
  word len = ether.packetReceive(); // check for ethernet packet / проверить ethernet пакеты.
  word pos = ether.packetLoop(len); // check for tcp packet / проверить TCP пакеты.
  if (pos) {
    bfill = ether.tcpOffset();
    char *data = (char *) Ethernet::buffer + pos;
    if (strncmp("GET /", data, 5) != 1) {
      data += 5;
     // Serial.println ("0");
      if (data[0] == ' ') {
        homePage(); // Return home page Если обнаружено изменения на станице, запускаем функцию.
        for (int i = 0; i <= 7; i++)
          digitalWrite(LedPins[i], PinStatus[i]);
      }
      else{
        for (int i = 0; i < 8; i++) {
          sprintf(etalon, "?ArduinoPIN%d=on", i);
         // Serial.println(etalon);
          //Serial.println(data);
          if ((strncmp(etalon, data, 15) == 0)) {
            PinStatus[i] = true;
            bfill.emit_p(http_Found);
            //Serial.println(1);
            break;
          }
        }
      //------------------------------------------------------
      for (int i = 0; i < 8; i++) {
        sprintf(etalon, "?ArduinoPIN%d=off", i);
        if ((strncmp(etalon, data, 16) == 0) ) {
          PinStatus[i] = false;
          bfill.emit_p(http_Found);
          break;
          Serial.print(2);
        }
      }
    }}
    ether.httpServerReply(bfill.position()); // send http response
  }
}
