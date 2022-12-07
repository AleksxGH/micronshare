#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
#define id_size 4 // размер id в байтах
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte id_masbyte[id_size], id_flag;
unsigned long id_unlong, get_id, id;
int count, data;
void setup() {
  Serial.begin (9600);
   SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;
  byte blockAddr      = 4;
  byte trailerBlock   = 7;
  byte dataBlock[16];
  byte buffer[18];
  byte size = sizeof(buffer);


  mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));      //аутентификация ключа А

  mfrc522.MIFARE_Read(blockAddr, buffer, &size); // прочитать блок памяти и записать в буфер
  memcpy(dataBlock, buffer, 16);
  id=(long(buffer[3]) << 0) + (long (buffer[2]) << 8) + (long (buffer[1]) << 16 ) + (long (buffer[0]) << 24 );
  id=11223344;
  count=1;
  data=071222;
  dataBlock[0] = id>>24;
  dataBlock[1] = id>>16;
  dataBlock[2] = id>>8;
  dataBlock[3] = id;
  dataBlock[4] = data>>16;
  dataBlock[5] = data>>8;
  dataBlock[6] = data;
  dataBlock[7] = count>>8;
  dataBlock[8] = count;
  dump_byte_array(buffer, 16);
  dump_byte_array(dataBlock, 16);
  mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);                                                     // записать в блок памяти данные массива
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
//----------------------------вывод на экран буфера ---------------------------------------------------------------------------------------------------
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print (" ");
    Serial.print(buffer[i], HEX);
  }
  Serial.println();
}
