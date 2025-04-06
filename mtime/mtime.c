/**:COPYRIGHT
            _ /      _ / 
        _ /_ /_ /  _/_/
         _ /       _ / 
        _ /       _ / 
       _ / _ / _/_ / _ /
    
   t1soluciones � 2020
          PARA
       Grupo APM
    
 */
// * AUTOR: Victor Noguedad
// * ARCHIVO: mtime.c
// * RESUMEN: Programa principal del modulo de temporizadores
// ::[CABECERAS]::
#include <mtime.h>
#include <string.h>
#include <stdio.h>
void setup()
{
   output_a(0x00);
   output_b(0x00);
   output_c(0x00);
   output_d(0x00);
   output_e(0x00);
   
   delay_ms(DEFAULT_D);
   
   if(DEBUG)
   {
      fprintf(RS232,"********************************\r");
      fprintf(RS232,"       Grupo APM (c) 2020       \r");
      fprintf(RS232,"================================\r");
      fprintf(RS232,"Bluetooth Basketball Score Board\r");
      fprintf(RS232,"================================\r");
      fprintf(RS232, "Version %s - rev %s \r", FW_VERSION, FW_REVISION);
      fprintf(RS232,"********************************\r");
      fprintf(RS232,"      DEPURACION :: TIEMPO      \r");
   }
   
   if(DEBUG) {fputs("> Habilitacion de I2C",RS232);}
   i2c_slaveaddr(I2C_TIMER);
   
   delay_ms(DEFAULT_D);
   
   if(DEBUG) {fputs("> Configuracion de Puertos",RS232);}
   //TODO: Activar puertos
   pTiroSU.a = PIN_B0;
   pTiroSU.b = PIN_B1;
   pTiroSU.c = PIN_B2;
   pTiroSU.d = PIN_B3;
   
   pTiroSD.a = PIN_B4;
   pTiroSD.b = PIN_B5;
   pTiroSD.c = PIN_B6;
   pTiroSD.d = PIN_B7;
   
   pTiempoSU.a = PIN_D0;
   pTiempoSU.b = PIN_D1;
   pTiempoSU.c = PIN_D2;
   pTiempoSU.d = PIN_D3;
   
   pTiempoSD.a = PIN_D4;
   pTiempoSD.b = PIN_D5;
   pTiempoSD.c = PIN_D6;
   pTiempoSD.d = PIN_D7;
   
   pTiempoMU.a = PIN_A4;
   pTiempoMU.b = PIN_A5;
   pTiempoMU.c = PIN_A6;
   pTiempoMU.d = PIN_A7;
   
   delay_ms(DEFAULT_D);
   
   if(DEBUG) {fputs("> Habilitacion de displays",RS232);}
   output_bit(DISPLAY, ON); 
   output_bit(MINUTOD, ON); 
   output_bit(PERIODO1, ON);
   output_bit(PERIODO2, ON);
   output_bit(PERIODO3, ON);
   output_bit(PERIODO4, ON);
   delay_ms(DEFAULT_D);
   output_bit(DISPLAY, OFF);
   output_bit(MINUTOD, OFF);
   output_bit(PERIODO1, OFF);
   output_bit(PERIODO2, OFF);
   output_bit(PERIODO3, OFF);
   output_bit(PERIODO4, OFF);
   delay_ms(DEFAULT_D);
   output_bit(DISPLAY, ON);
   output_bit(MINUTOD, ON);
   output_bit(PERIODO1, ON);
   output_bit(PERIODO2, ON);
   output_bit(PERIODO3, ON);
   output_bit(PERIODO4, ON);
   delay_ms(DEFAULT_D);
   output_bit(MINUTOD, OFF);
   output_bit(PERIODO1, OFF);
   output_bit(PERIODO2, OFF);
   output_bit(PERIODO3, OFF);
   output_bit(PERIODO4, OFF);
   
   if(DEBUG) {fputs("> Habilitacion de interrupciones",RS232);}
   enable_interrupts(INT_RDA);
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
}

void main()
{
   setup();

   while(TRUE)
   {

      //TODO: User Code
   }

}

#INT_RDA
void  RDA_isr(void) 
{
   char c;
   c = fgetc(RS232);
if(cmdGet)
{
   if(c == '#' || c == '\r')
   {
      cmdGet = false;
      strcpy(cmd, rcmd);
      parseCommand();
   }
   else
   {
   if(cmdIndex < 9)
      {
         rcmd[cmdIndex] = c;
         cmdIndex++;
      }
   }
}
      if(c == '-')
   {
       cmdGet = true; 
       rcmd = "\0\0\0\0\0\0\0\0\0";
       cmdIndex = 0;
   }
}

#INT_SSP
void  SSP_isr(void) 
{
   int state = i2c_isr_state();
      if(state== 0 ) 
         i2c_read();
      if(state == 0x80)
         i2c_read(2);
      if(state >= 0x80)
         i2c_write(0xFF);
      else if(state > 0)
      {
         ircmd[state - 1] = i2c_read();
         if(DEBUG) {fprintf(RS232, "> Commando: %X - %X\r",state, ircmd[state - 1]);}
         icmd = ircmd[state - 1];
         parseI2CCommand();
      }
}

void parseI2CCommand(void)
{
   int c = icmd;
   switch(c)
   {
      case I2C_TIMER:      if(DEBUG) {fputs("> Direccion TIMER",RS232);}
                           break;
      case I2C_SCORE:      if(DEBUG) {fputs("> Direccion SCORE",RS232);}
                           break;
      case I2C_CMD_PER0:   if(DEBUG) {fputs("> Apagar Periodos",RS232);}
                           showQuarter(0);
                           break;
      case I2C_CMD_PER1:   if(DEBUG) {fputs("> Activar Periodo 1",RS232);}
                           showQuarter(1);
                           break;
      case I2C_CMD_PER2:   if(DEBUG) {fputs("> Activar Periodo 2",RS232);}
                           showQuarter(2);
                           break;
      case I2C_CMD_PER3:   if(DEBUG) {fputs("> Activar Periodo 3",RS232);}
                           showQuarter(3);
                           break;
      case I2C_CMD_PER4:   if(DEBUG) {fputs("> Activar Periodo 4",RS232);}
                           showQuarter(4);
                           break;
      case I2C_CMD_MDON:   if(DEBUG) {fputs("> Activar Minuto Decena",RS232);}
                           output_bit(MINUTOD, ON);
                           break;
      case I2C_CMD_MDOF:   if(DEBUG) {fputs("> Desactivar Minuto Decena",RS232);}
                           output_bit(MINUTOD, OFF);
                           break;
      case I2C_CMD_BUZ1:   if(DEBUG) {fputs("> Toque Corto",RS232);}
                           buzz(1);
                           break;
      case I2C_CMD_BUZ2:   if(DEBUG) {fputs("> Toque Largo",RS232);}
                           buzz(2);
                           break;
      case I2C_CMD_BUZ3:   if(DEBUG) {fputs("> Toque Doble",RS232);}
                           buzz(3);
                           break;
      case I2C_CMD_TEST:   doTest();
                           break;
      case I2C_CMD_RESET:  doReset();
                           break;
   }
   
   if(c > 0x0F && c < 0x1A) //0x10 - 0x19
   {
      if(DEBUG) {fputs("> Asignacion Puerto 1",RS232);}
      showNumber(c - 0x10, pTiroSU);   
   }
   if(c > 0x1F && c < 0x2A) //0x20 - 0x29
   {
      if(DEBUG) {fputs("> Asignacion Puerto 2",RS232);}
      showNumber(c - 0x20, pTiroSD);   
   }
   if(c > 0x2F && c < 0x3A) //0x30 - 0x39
   {
      if(DEBUG) {fputs("> Asignacion Puerto 3",RS232);}
      showNumber(c - 0x30, pTiempoSD);   
   }
   if(c > 0x3F && c < 0x4A) //0x40 - 0x49
   {
      if(DEBUG) {fputs("> Asignacion Puerto 4",RS232);}
      showNumber(c - 0x40, pTiempoSU);   
   }
   if(c > 0x4F && c < 0x5A) //0x50 - 0x59
   {
      if(DEBUG) {fputs("> Asignacion Puerto 5",RS232);}
      showNumber(c - 0x50, pTiempoMU);   
   }
}

void doTest(void)
{
   if(DEBUG) {fputs("> Rutina de prueba",RS232);}
   output_bit(MINUTOD, OFF);
   output_bit(PERIODO1, OFF);
   output_bit(PERIODO2, OFF);
   output_bit(PERIODO3, OFF);
   output_bit(PERIODO4, OFF);
   for(int i=0; i<10; i++)
   {
      showNumber(i, pTiroSD);
      showNumber(i, pTiroSU);
      showNumber(i, pTiempoSD);
      showNumber(i, pTiempoSU);
      showNumber(i, pTiempoMU);
      output_toggle(MINUTOD);
      showQuarter(i);
      delay_ms(DEFAULT_D);
   }
   buzz(1);
}

void buzz(int t)
{
   switch(t)
   {
      case 1:  //TOOT
               output_bit(BUZZER, ON);
               delay_ms(1000);
               output_bit(BUZZER, OFF);
               break;
      case 2:  //LONG BUZZ
               output_bit(BUZZER, ON);
               delay_ms(3000);
               output_bit(BUZZER, OFF);
               break;
      case 3:  //DOUBLE
               output_bit(BUZZER, ON);
               delay_ms(1000);
               output_bit(BUZZER, OFF);
               delay_ms(250);
               output_bit(BUZZER, ON);
               delay_ms(1000);
               output_bit(BUZZER, OFF);
               break;
   }
}

void parseCommand(void)
{
   char c[10];
   strcpy(c, cmd);
   switch(c[0])
   {
      case '0':   if(DEBUG) {fputs("\r> Activacion manual de puerto",RS232);}
                  switch(c[2])
                  {
                     case '1':   showNumber(char2int(c[3]), pTiroSU);
                                 break;
                     case '2':   showNumber(char2int(c[3]), pTiroSD);
                                 break;
                     case '3':   showNumber(char2int(c[3]), pTiempoSD);
                                 break;
                     case '4':   showNumber(char2int(c[3]), pTiempoSU);
                                 break;
                     case '5':   showNumber(char2int(c[3]), pTiempoMU);
                                 break;
                  }
                  break;
      case 'A':   if(DEBUG) {fputs("> Apagar Periodos",RS232);}
                           showQuarter(0);
                           break;
      case 'B':   if(DEBUG) {fputs("> Activar Periodo 1",RS232);}
                           showQuarter(1);
                           break;
      case 'C':   if(DEBUG) {fputs("> Activar Periodo 2",RS232);}
                           showQuarter(2);
                           break;
      case 'D':   if(DEBUG) {fputs("> Activar Periodo 3",RS232);}
                           showQuarter(3);
                           break;
      case 'E':   if(DEBUG) {fputs("> Activar Periodo 4",RS232);}
                           showQuarter(4);
                           break;
      case 'F':   if(DEBUG) {fputs("> Activar Minuto Decena",RS232);}
                           output_bit(MINUTOD, ON);
                           break;
      case 'G':   if(DEBUG) {fputs("> Desactivar Minuto Decena",RS232);}
                           output_bit(MINUTOD, OFF);
                           break;            
      case 'H':   if(DEBUG) {fputs("> Toque Corto",RS232);}
                           buzz(1);
                           break;
      case 'I':   if(DEBUG) {fputs("> Toque Largo",RS232);}
                           buzz(2);
                           break;
      case 'J':   if(DEBUG) {fputs("> Toque Doble",RS232);}
                           buzz(3);
                           break;
      case 'X':   doTest(); //Rutina de prueba
                  break;
      case 'Z':   if(DEBUG) {fputs("\r> Reseteo general...!",RS232);}
                  delay_ms(100);
                  if(DEBUG) {fputs("\r> (5)",RS232);}
                  delay_ms(100);
                  if(DEBUG) {fputs("\r> (4)",RS232);}
                  delay_ms(100);
                  if(DEBUG) {fputs("\r> (3)",RS232);}
                  delay_ms(100);
                  if(DEBUG) {fputs("\r> (2)",RS232);}
                  delay_ms(100);
                  if(DEBUG) {fputs("\r> (1)",RS232);}
                  delay_ms(100);
                  if(DEBUG) {fputs("\r> (0)",RS232);}
                  doReset();
                  
   }
}

void showQuarter(int8 n)
{
   output_bit(PERIODO1, OFF);
   output_bit(PERIODO2, OFF);
   output_bit(PERIODO3, OFF);
   output_bit(PERIODO4, OFF);
   
   switch(n)
   {
      case 1: output_bit(PERIODO1, ON); break;
      case 2: output_bit(PERIODO2, ON); break;
      case 3: output_bit(PERIODO3, ON); break;
      case 4: output_bit(PERIODO4, ON); break;
   }
}

void showNumber(int8 n, tPort p)
{
   int1 a=0, b=0, c=0, d=0;
   switch(n)
   {
      case 1:  a=1;
               break;
      case 2:  b=1;
               break;
      case 3:  a=1; b=1;
               break;
      case 4:  c=1;
               break;
      case 5:  c=1; a=1;
               break;
      case 6:  c=1; b=1;
               break;
      case 7:  c=1; b=1; a=1;
               break;
      case 8:  d=1;
               break;
      case 9:  d=1; a=1;
               break;
   }
   output_bit(p.a, a);
   output_bit(p.b, b);
   output_bit(p.c, c);
   output_bit(p.d, d);
}

int8 char2int(char c)
{
   switch(c)
   {
      case '0':   return 0;
                  break;
      case '1':   return 1;
                  break;
      case '2':   return 2;
                  break;
      case '3':   return 3;
                  break;
      case '4':   return 4;
                  break;
      case '5':   return 5;
                  break;
      case '6':   return 6;
                  break;
      case '7':   return 7;
                  break;
      case '8':   return 8;
                  break;
      case '9':   return 9;
                  break;
   }
   return 0;
}

void doReset(void)
{
   if(DEBUG) {fputs("> Reseteando...",RS232);}
   reset_cpu();
}

