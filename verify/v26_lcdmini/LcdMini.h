// LcdMini.h … LCD 1602（I2C）を Wire なしで動かす
const int SDA_PIN = 3, SCL_PIN = 4;  // SDA＝3番・SCL＝4番
const byte LCD_ADDR = 0x3F;          // LCD のアドレス

void sen(int pin, bool up) {         // 線を上げる（はなす）か下げる
  pinMode(pin, up ? INPUT : OUTPUT); // はなすと、基板の抵抗で HIGH
  if (!up) digitalWrite(pin, LOW);
}

void i2cByte(byte b) {               // 1バイトを 1ビットずつ送る
  for (int i = 0; i < 8; i++) {
    sen(SDA_PIN, b & 0x80);          // いちばん上のビット
    sen(SCL_PIN, true);              // SCL を上げ下げすると
    sen(SCL_PIN, false);             //   1ビット読んでもらえる
    b = b << 1;
  }
  sen(SDA_PIN, true);                // 返事の1ビット（読まない）
  sen(SCL_PIN, true); sen(SCL_PIN, false);
}

void i2cSend(byte v) {               // LCD の基板に 1バイトとどける
  sen(SDA_PIN, true);  sen(SCL_PIN, true);
  sen(SDA_PIN, false); sen(SCL_PIN, false);  // はじめの合図
  i2cByte(LCD_ADDR << 1);                    // あて先
  i2cByte(v);                                // 中身
  sen(SDA_PIN, false); sen(SCL_PIN, true);
  sen(SDA_PIN, true);                        // おわりの合図
}

void lcdHalf(byte v) {               // 4ビットぶん送る
  i2cSend(v | 0x0C);                 // 0x08＝明かり・0x04＝読んで！
  i2cSend((v & ~0x04) | 0x08);
}

void lcdSend(byte v, byte rs) {      // 1バイトを 2回に分けて送る
  lcdHalf((v & 0xF0) | rs);          // 上の4ビット
  lcdHalf((v << 4) | rs);            // 下の4ビット
}

void lcdCmd(byte c) {                // 命令を送る
  lcdSend(c, 0);
  delay(2);
}

void lcdInit() {                     // LCD を使いはじめる
  sen(SDA_PIN, true); sen(SCL_PIN, true);
  delay(50);
  lcdHalf(0x30); delay(5);           // 決まった手順で
  lcdHalf(0x30); delay(1);           //   目をさまさせて
  lcdHalf(0x30); lcdHalf(0x20);      // 4ビットで話すと決める
  lcdCmd(0x28); lcdCmd(0x0C);        // 2行・画面オン
  lcdCmd(0x01); lcdCmd(0x06);        // 消す・右へ進む
}

void lcdAt(byte col, byte row) {     // 何文字目・何行目から書くか
  lcdCmd(0x80 | (col + row * 0x40)); // 2行目は 0x40 から
}

void lcdStr(const char *s) {         // 文字を出す
  while (*s) lcdSend(*s++, 1);       // 1文字ずつ送る
}

void lcdNum(int n) {                 // 数を出す
  char b[7];
  int i = 0;
  if (n < 0) { lcdSend('-', 1); n = -n; }
  do {
    b[i++] = '0' + n % 10;           // 下の桁から取り出して
    n = n / 10;
  } while (n);
  while (i) lcdSend(b[--i], 1);      // 上の桁から出す
}
