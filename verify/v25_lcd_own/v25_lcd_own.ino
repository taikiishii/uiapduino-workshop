/*
 * v25_lcd_own  ---  検証㉕：Wire を使わない自作 LCD ドライバは動くか
 *
 * なぜ調べるか
 *   LiquidCrystal_I2C を使うと、「数をかぞえる」だけで Flash 88%（14524）。
 *   arduino-cli で内訳を測ったら、いちばん大きいのは LCD のライブラリではなく
 *   **I2C（Wire）で ＋5368 バイト**だった（2026-09-24）：
 *     Lチカ 6988 → Wire ＋5368 → LCD ライブラリと文字 ＋1920 → 数の表示 ＋248
 *   そこで I2C を自分で上げ下げする最小のドライバを書いたら **7416 バイト（45%）**。
 *   **約 7KB 節約**できる計算。ただし実機ではまだ動かしていない。
 *
 * つなぎかた（m1 と同じ）
 *   GND → GND ／ VCC → 5V ／ SDA → 3番 ／ SCL → 4番
 *
 * 見かた
 *   1行目に「count」、2行目に 1秒ごとに増える数 → ✅ 動く
 *   何も出ない → ACK を読んでいないので、アドレス（0x3F／0x27）と配線を確認
 *
 * しくみ
 *   I2C の線は「離す＝INPUT（基板のプルアップで HIGH）」「下げる＝OUTPUT で LOW」で動かす。
 *   HIGH を出力しないので、線どうしがぶつかっても壊れない（オープンドレインのまね）。
 *   PCF8574 のつなぎ：P0=RS／P2=EN／P3=明かり／P4〜P7=D4〜D7（FC-113 の標準）。
 */
//   離す＝INPUT（プルアップで HIGH）／下げる＝OUTPUT で LOW
const int SDA_PIN = 3, SCL_PIN = 4;
const byte ADDR = 0x3F;
const byte BL = 0x08, EN = 0x04, RS = 0x01;   // PCF8574：P3=明かり P2=EN P0=RS P4-7=D4-7

void lo(int p) { pinMode(p, OUTPUT); digitalWrite(p, LOW); }
void hi(int p) { pinMode(p, INPUT); }

void i2cByte(byte b) {
  for (int i = 0; i < 8; i++) {
    if (b & 0x80) hi(SDA_PIN); else lo(SDA_PIN);
    hi(SCL_PIN); lo(SCL_PIN);
    b <<= 1;
  }
  hi(SDA_PIN); hi(SCL_PIN); lo(SCL_PIN);   // ACK は読まない
}

void pcf(byte v) {                          // PCF8574 に1バイト
  hi(SDA_PIN); hi(SCL_PIN); lo(SDA_PIN); lo(SCL_PIN);   // START
  i2cByte(ADDR << 1);
  i2cByte(v);
  lo(SDA_PIN); hi(SCL_PIN); hi(SDA_PIN);    // STOP
}

void nib(byte v) { pcf(v | EN | BL); pcf((v & ~EN) | BL); }
void send(byte v, byte mode) { nib((v & 0xF0) | mode); nib((v << 4) | mode); }
void cmd(byte c) { send(c, 0); delay(2); }
void put(char c) { send(c, RS); }

void lcdInit() {
  hi(SDA_PIN); hi(SCL_PIN);
  delay(50);
  nib(0x30); delay(5); nib(0x30); delay(1); nib(0x30); nib(0x20);  // 4ビットにする
  cmd(0x28); cmd(0x0C); cmd(0x01); cmd(0x06);
}
void at(byte col, byte row) { cmd(0x80 | (col + row * 0x40)); }
void str(const char *s) { while (*s) put(*s++); }
void num(int n) {
  char b[7]; int i = 0;
  if (n < 0) { put('-'); n = -n; }
  do { b[i++] = '0' + n % 10; n /= 10; } while (n);
  while (i) put(b[--i]);
}

int n = 0;
void setup() { lcdInit(); str("count"); }
void loop() { at(0, 1); num(n); n = n + 1; delay(1000); }
