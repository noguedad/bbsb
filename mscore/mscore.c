#include <mscore.h>
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
      fprintf(RS232,"      DEPURACION :: PUNTOS      \r");
   }

   
   if(DEBUG) {fputs("> Habilitacion de I2C",RS232);}
   i2c_slaveaddr(I2C_SCORE);
   
   delay_ms(DEFAULT_D);
   
   if(DEBUG) {fputs("> Configuracion de Puertos",RS232);}
   //TODO: Activar puertos
   pLocalU.a = PIN_B0;
   pLocalU.b = PIN_B1;
   pLocalU.c = PIN_B2;
   pLocalU.d = PIN_B3;
   
   pLocalD.a = PIN_B4;
   pLocalD.b = PIN_B5;
   pLocalD.c = PIN_B6;
   pLocalD.d = PIN_B7;
   
   pVisitaU.a = PIN_D0;
   pVisitaU.b = PIN_D1;
   pVisitaU.c = PIN_D2;
   pVisitaU.d = PIN_D3;
   
   pVisitaD.a = PIN_D4;
   pVisitaD.b = PIN_D5;
   pVisitaD.c = PIN_D6;
   pVisitaD.d = PIN_D7;
   
   pFLocal.a = PIN_A4;
   pFLocal.b = PIN_A5;
   pFLocal.c = PIN_A6;
   pFLocal.d = PIN_A7;
   
   pFVisita.a = PIN_A0;
   pFVisita.b = PIN_A1;
   pFVisita.c = PIN_A2;
   pFVisita.d = PIN_C0;
   
   
   delay_ms(DEFAULT_D);
   
   if(DEBUG) {fputs("> Habilitacion de displays",RS232);}
   output_bit(DISPLAY, ON); 
   output_bit(CENLOCAL, ON); 
   output_bit(CENVISITA, ON);
   output_bit(POSLOCAL, ON);
   output_bit(POSVISITA, ON);
   delay_ms(DEFAULT_D);
   output_bit(DISPLAY, OFF);
   output_bit(CENLOCAL, OFF); 
   output_bit(CENVISITA, OFF);
   output_bit(POSLOCAL, OFF);
   output_bit(POSVISITA, OFF);
   delay_ms(DEFAULT_D);
   output_bit(DISPLAY, ON);
   output_bit(CENLOCAL, ON); 
   output_bit(CENVISITA, ON);
   output_bit(POSLOCAL, ON);
   output_bit(POSVISITA, ON);
   delay_ms(DEFAULT_D);
   output_bit(CENLOCAL, OFF); 
   output_bit(CENVISITA, OFF);
   output_bit(POSLOCAL, OFF);
   output_bit(POSVISITA, OFF);
   
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
      case I2C_CMD_POS0:   if(DEBUG) {fputs("> Apagar Posicion",RS232);}
                           output_bit(POSLOCAL, OFF); output_bit(POSVISITA, OFF);
                           break;
      case I2C_CMD_POSL:   if(DEBUG) {fputs("> Activar Posicion Local",RS232);}
                           output_bit(POSLOCAL, ON); output_bit(POSVISITA, OFF);
                           break;
      case I2C_CMD_POSV:   if(DEBUG) {fputs("> Activar Posicion Visita",RS232);}
                           output_bit(POSLOCAL, OFF); output_bit(POSVISITA, ON);
                           break;
      case I2C_CMD_CLON:   if(DEBUG) {fputs("> Activar Centena Local",RS232);}
                           output_bit(CENLOCAL, ON);
                           break;
      case I2C_CMD_CLOF:   if(DEBUG) {fputs("> Desactivar Centena Local",RS232);}
                           output_bit(CENLOCAL, OFF);
                           break;
      case I2C_CMD_CVON:   if(DEBUG) {fputs("> Activar Centena Visita",RS232);}
                           output_bit(CENVISITA, ON);;
                           break;
      case I2C_CMD_CVOF:   if(DEBUG) {fputs("> Desactivar Centena Visita",RS232);}
                           output_bit(CENVISITA, OFF);
                           break;
      case I2C_CMD_TEST:   doTest();   
                           break;
      case I2C_CMD_RESET:  doReset();
                           break;
   }
   
   if(c > 0x0F && c < 0x1A) //0x10 - 0x19
   {
      if(DEBUG) {fputs("> Asignacion Puerto 1",RS232);}
      showNumber(c - 0x10, pLocalU);   
   }
   if(c > 0x1F && c < 0x2A) //0x20 - 0x29
   {
      if(DEBUG) {fputs("> Asignacion Puerto 2",RS232);}
      showNumber(c - 0x20, pLocalD);   
   }
   if(c > 0x2F && c < 0x3A) //0x30 - 0x39
   {
   if(DEBUG) {fputs("> Asignacion Puerto 3",RS232);}
      showNumber(c - 0x30, pVisitaD);   
   }
   if(c > 0x3F && c < 0x4A) //0x40 - 0x49
   {
      if(DEBUG) {fputs("> Asignacion Puerto 4",RS232);}
      showNumber(c - 0x40, pVisitaU);   
   }
   if(c > 0x4F && c < 0x5A) //0x50 - 0x59
   {
      if(DEBUG) {fputs("> Asignacion Puerto 5",RS232);}
      showNumber(c - 0x50, pFLocal);   
   }
   if(c > 0x5F && c < 0x6A) //0x60 - 0x69
   {
      if(DEBUG) {fputs("> Asignacion Puerto 6",RS232);}
      showNumber(c - 0x60, pFVisita);   
   }
}

void doTest(void)
{
   if(DEBUG) {fputs("> Rutina de prueba",RS232);}
   output_bit(CENLOCAL, OFF);
   output_bit(CENVISITA, OFF);
   output_bit(POSLOCAL, OFF);
   output_bit(POSVISITA, OFF);
   
   for(int i=0; i<10; i++)
   {
      showNumber(i, pLocalD);
      showNumber(i, pLocalU);
      showNumber(i, pVisitaD);
      showNumber(i, pVisitaU);
      showNumber(i, pFLocal);
      showNumber(i, pFVisita);
      output_toggle(POSLOCAL);
      output_toggle(POSVISITA);
      output_toggle(CENLOCAL);
      output_toggle(CENVISITA);
      delay_ms(DEFAULT_D);
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
                     case '1':   showNumber(char2int(c[3]), pLocalU);
                                 break;
                     case '2':   showNumber(char2int(c[3]), pLocalD);
                                 break;
                     case '3':   showNumber(char2int(c[3]), pVisitaD);
                                 break;
                     case '4':   showNumber(char2int(c[3]), pVisitaU);
                                 break;
                     case '5':   showNumber(char2int(c[3]), pFLocal);
                                 break;
                     case '6':   showNumber(char2int(c[3]), pFVisita);
                                 break;
                  }
                  break;
      case 'A':   if(DEBUG) {fputs("> Apagar Posicion",RS232);}
                           output_bit(POSLOCAL, OFF); output_bit(POSVISITA, OFF);
                           break;
      case 'B':   if(DEBUG) {fputs("> Activar Posicion Local",RS232);}
                           output_bit(POSLOCAL, ON); output_bit(POSVISITA, OFF);
                           break;
      case 'C':   if(DEBUG) {fputs("> Activar Posicion Visita",RS232);}
                           output_bit(POSLOCAL, OFF); output_bit(POSVISITA, ON);
                           break;
      case 'D':   if(DEBUG) {fputs("> Activar Centena Local",RS232);}
                           output_bit(CENLOCAL, ON);
                           break;
      case 'E':   if(DEBUG) {fputs("> Desactivar Centena Local",RS232);}
                           output_bit(CENLOCAL, OFF);
                           break;
      case 'F':   if(DEBUG) {fputs("> Activar Centena Visita",RS232);}
                           output_bit(CENVISITA, ON);
                           break;
      case 'G':   if(DEBUG) {fputs("> Desactivar Centena Visita",RS232);}
                           output_bit(CENVISITA, OFF);
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


