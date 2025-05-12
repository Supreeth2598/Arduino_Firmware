/* LLD SiPM FE VER 3 API */
/* Author: Lakshmi Narayan PhD */
/* NBio LLC */
/* Date created: 4/15/2020 */
/* Date last modified: 11/11/2024 */

#include <SPI.h>


/* Teensy 3.2 PORT Map */
# define CS 10
# define DOUT 11
# define DIN 12
# define SCLK 13
# define EXCT 15

String cmd, data;
String output = "";
int prvState, crState, count, sCount;
long pd[8], intTime, wTime;
word val1, val2, val3;

void setup()
{
  /* Serial port configuration */
  //Serial.setTimeout(3000);
  //Serial.begin(115200);
  Serial1.setTimeout(1000);
  Serial1.begin(115200);

  /* SPI Pin settings */
  pinMode(CS, OUTPUT);
  pinMode(DOUT, OUTPUT);
  pinMode(DIN, INPUT);
  pinMode(SCLK, OUTPUT);
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE1));
  digitalWrite(CS, LOW);
  SPI.transfer(0x42);
  SPI.transfer(0xEB);
  digitalWrite(CS, HIGH);

  /* Excitation ON/OFF Pin settings */
  pinMode(EXCT, OUTPUT);
  digitalWrite(EXCT, LOW);
}

void readADS(void)
{
  /* Read Channel 0 */
  digitalWrite(CS, LOW);
  SPI.transfer(0x42);
  SPI.transfer(0xEB);
  digitalWrite(CS, HIGH);
  delay(30); //wait for ADC conversion to complete
  digitalWrite(CS, LOW);
  val1 = SPI.transfer(0) * 256;
  val1 = val1 + SPI.transfer(0);
  digitalWrite(CS, HIGH);
  val1 = val1 + 100000;
  output +="#";
  output += val1;
  output += ":";
  //Serial1.print("#");
  //Serial1.print(val1);
  //Serial1.print(":");
  //Serial.print("#");
  //Serial.print(val1);
  //Serial.print(":");

  /* Read Channel 1 */
  digitalWrite(CS, LOW);
  SPI.transfer(0x52);
  SPI.transfer(0xEB);
  digitalWrite(CS, HIGH);
  delay(30); //wait for ADC conversion to complete
  digitalWrite(CS, LOW);
  val2 = SPI.transfer(0) * 256;
  val2 = val2 + SPI.transfer(0);
  digitalWrite(CS, HIGH);
  val2 = val2 + 200000;
  output += val2;
  output += ":";
  //Serial1.print(val2);
  //Serial1.print(":");
  //Serial.print(val2);
  //Serial.print(":");

  /* Read Channel 2 */
  digitalWrite(CS, LOW);
  SPI.transfer(0x62);
  SPI.transfer(0xEB);
  digitalWrite(CS, HIGH);
  delay(30); //wait for ADC conversion to complete
  digitalWrite(CS, LOW);
  val3 = SPI.transfer(0) * 256;
  val3 = val3 + SPI.transfer(0);
  digitalWrite(CS, HIGH);
  val3 = val3 + 300000;
  output += val3;
  output += "p";
  //Serial1.print(val3);
  //Serial1.print("p");
  //Serial.print(val3);
  //Serial.print("p");
  Serial1.print(output);//#1xxxxx:2xxxxx:3xxxxxp
  output = "";
}

/*void readTemp(void)
{
  Read Temperature 
  Read out is 32-bit transimission format
  digitalWrite(CS, LOW);
  SPI.transfer(0x42);
  SPI.transfer(0xFB);
  //digitalWrite(CS, HIGH);
  delay(100); //wait for ADC conversion to complete
  //digitalWrite(CS, LOW);
  val1 = SPI.transfer(0) * 256;
  val1 = val1 + SPI.transfer(0);
  digitalWrite(CS, HIGH);
  Convert 16-bit to 14-bit number
  val1 = val1 / 4;
  if (val1 > 8191)
  {
    val1 = val1 - 1;
    val1 = !val1;
    Serial.println(val1 * (-0.03125));
    Serial1.println(val1 * (-0.03125));
  }
  else
  {
    Serial.println(val1 * (0.03125));
    Serial1.println(val1 * (0.03125));
  }
}
*/

void loop()
{
  /* Waits indefinetly, receives the command from the PC and executes the command. */
 /* if (Serial.available())
  {
    cmd = Serial.readStringUntil('\r');

    if (cmd == "ID")
    {
      Serial.println("LLD SiPM FE VER 1");
    }

    if (cmd == "0")
    {
      readADS();
    }

    if (cmd == "readT")
    {
      readTemp();
    }

    if (cmd == "exctON")
    {
      digitalWrite(EXCT, HIGH);
    }

    if (cmd == "exctOFF")
    {
      digitalWrite(EXCT, LOW);
    }
  }
  delay(1);*/

 //Receives commands via bluetooth module
 if (Serial1.available())
  {
    cmd = Serial1.readStringUntil('\r');
    if (cmd == "0")
    {
      readADS();
    }

    /*if (cmd == "readT")
    {
      readTemp();
    }

    if (cmd == "exctON")
    {
      digitalWrite(EXCT, HIGH);
    }

    if (cmd == "exctOFF")
    {
      digitalWrite(EXCT, LOW);
    }*/
  }
}
