
#include <mcoms.h>
#include <string.h>
#include <stdio.h>

void setup()
{
   output_a(0x00);
   output_b(0x00);
   output_c(0x00);
   output_d(0x00);
   output_e(0x00);
   
   delay_ms(100);
   
   if(DEBUG)
   {
      fprintf(RS232,"********************************\r");
      fprintf(RS232,"       Grupo APM (c) 2020       \r");
      fprintf(RS232,"================================\r");
      fprintf(RS232,"Bluetooth Basketball Score Board\r");
      fprintf(RS232,"================================\r");
      fprintf(RS232, "Version %s - rev %s \r", FW_VERSION, FW_REVISION);
      fprintf(RS232,"********************************\r");
      fprintf(RS232,"      CONSOLA DE DEPURACION     \r");
   }
   else
   {
      fputs("Grupo APM (c) 2020 :: BBSB 1.0 rev A",RS232);
   }
   
   if(DEBUG) {fputs("> Configuracion de Puertos",RS232);}
   
   //Jugador Unidades
   puertoA.a = PIN_B0;
   puertoA.b = PIN_B1;
   puertoA.c = PIN_B2;
   puertoA.d = PIN_B3;
   
   //Jugador Decenas
   puertoB.a = PIN_B4;
   puertoB.b = PIN_B5;
   puertoB.c = PIN_B6;
   puertoB.d = PIN_B7;
   
   if(DEBUG) {fputs("> Inicializacion de I2C",RS232);}
   
   if(DEBUG) {fputs("> Inicializacion de modulo tiempo",RS232);}
   i2c_start();
   i2c_write(I2C_TIMER);
   i2c_write(I2C_CMD_INIT);
   i2c_stop();

   if(DEBUG) {fputs("> Inicializacion de modulo puntos",RS232);}
   i2c_start();
   i2c_write(I2C_SCORE);
   i2c_write(I2C_CMD_INIT);
   i2c_stop();
   
   if(DEBUG) {fputs("> Habilitacion de interrupciones",RS232);}
   enable_interrupts(INT_RDA);
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
}

void main()
{
   setup();
   
   TICK_TYPE CurrentTick,PreviousTick;

   CurrentTick = PreviousTick = get_ticks();

   while(TRUE)
   {
      CurrentTick = get_ticks();

      if(GetTickDifference(CurrentTick, PreviousTick) >= (TICK_TYPE)TICKS_PER_SECOND)
      {
         timeTick();
         PreviousTick = CurrentTick;
      }

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
       rcmd = "\0\0\0\0\0\0\0\0\0\0";
       cmdIndex = 0;
   }
}

#INT_SSP
void  SSP_isr(void) 
{

}

TICK_TYPE GetTickDifference(TICK_TYPE currTick, TICK_TYPE prevTick)
{
   return(currTick-prevTick);
}

void timeTick(void)
{
   //TODO: User Code
}

void doTest(void)
{
   if(DEBUG) {fputs("> Rutina de prueba",RS232);}
   
   if(DEBUG) {fputs("> Probando modulo tiempo",RS232);}
   i2c_start();
   i2c_write(I2C_TIMER);
   i2c_write(I2C_CMD_TEST);
   i2c_stop();

   if(DEBUG) {fputs("> Probando modulo puntos",RS232);}
   i2c_start();
   i2c_write(I2C_SCORE);
   i2c_write(I2C_CMD_TEST);
   i2c_stop();

   if(DEBUG) {fputs("> Probando modulo comunicaciones",RS232);}
}

void parseCommand(void)
{
   char c[10];
   strcpy(c, cmd);
   switch(c[0])
   {
      case 'L':   if(DEBUG) {fputs("\r> Comando local",RS232);}
                  switch(c[1])
                  {
                     case '0':   if(DEBUG) {fputs("\r> Resetea a 0",RS232);}
                                 break;
                     case '1':   if(DEBUG) {fputs("\r> Incrementar por 1",RS232);}
                                 break;
                     case '2':   if(DEBUG) {fputs("\r> Incrementar por 2",RS232);}
                                 break;
                     case '3':   if(DEBUG) {fputs("\r> Incrementar por 3",RS232);}
                                 break;
                     case 'D':   if(DEBUG) {fputs("\r> Decrementar por 1",RS232);}
                                 break;
                     case 'P':   if(DEBUG) {fputs("\r> Asignar posicion",RS232);}
                                 break;
                  }
                  break;
      case 'P':   if(DEBUG) {fputs("\r> Pausar tiempo",RS232);}
                  break;
      case 'Q':   if(DEBUG) {fputs("\r> Renudar tiempo",RS232);}
                  break;
      case 'T':   if(DEBUG) {fputs("\r> Configuracion de tiempo",RS232);}
                  break;
      case 'V':   if(DEBUG) {fputs("\r> Comando visitante",RS232);}
                  switch(c[1])
                  {
                     case '0':   if(DEBUG) {fputs("\r> Resetea a 0",RS232);}
                                 break;
                     case '1':   if(DEBUG) {fputs("\r> Incrementar por 1",RS232);}
                                 break;
                     case '2':   if(DEBUG) {fputs("\r> Incrementar por 2",RS232);}
                                 break;
                     case '3':   if(DEBUG) {fputs("\r> Incrementar por 3",RS232);}
                                 break;
                     case 'D':   if(DEBUG) {fputs("\r> Decrementar por 1",RS232);}
                                 break;
                     case 'P':   if(DEBUG) {fputs("\r> Asignar posicion",RS232);}
                                 break;
                  }
                  break;
      case 'X':   doTest(); //Rutina de prueba
                  break;
   }
}