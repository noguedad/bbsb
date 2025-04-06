#include <16F887.h>

#define FW_VERSION   "2.0"
#define FW_REVISION  "F"

//-- CONFIGURACION DE PIC --
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOPUT                    //No Power Up Timer
#FUSES NOMCLR                   //Master Clear pin used for I/O
#FUSES NOPROTECT                //Code not protected from reading
#FUSES NOCPD                    //No EE protection
#FUSES NOBROWNOUT               //No brownout reset
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES BORV40                   //Brownout reset at 4.0V
#FUSES NOWRT                    //Program memory not write protected

#use delay(internal=8MHz)

//-- CONFIGURACION DE IO --
#use FIXED_IO( A_outputs=PIN_A7,PIN_A6,PIN_A5,PIN_A4,PIN_A3,PIN_A2,PIN_A1,PIN_A0 )
#use FIXED_IO( B_outputs=PIN_B7,PIN_B6,PIN_B5,PIN_B4,PIN_B3,PIN_B2,PIN_B1,PIN_B0 )
#use FIXED_IO( C_outputs=PIN_C5, PIN_C1, PIN_C0 )
#use FIXED_IO( D_outputs=PIN_D7,PIN_D6,PIN_D5,PIN_D4,PIN_D3,PIN_D2,PIN_D1,PIN_D0 )
#use FIXED_IO( E_outputs=PIN_E3,PIN_E2,PIN_E1,PIN_E0 )

//-- DEFINICION DE PINES --
#define I2C_SCL   PIN_C3
#define I2C_SDA   PIN_C4
#define RS232_TX  PIN_C6
#define RS232_RX  PIN_C7
#define DISPLAY   PIN_E0
#define DEBUG     input_state(PIN_C2)
#define PERIODO1  PIN_E1
#define PERIODO2  PIN_E2
#define PERIODO3  PIN_C0
#define PERIODO4  PIN_C1
#define MINUTOD   PIN_A0
#define BUZZER    PIN_A1

#define I2C_TIMER       0x0A
#define I2C_SCORE       0x0C

//-- CONFIGURACION DE PUERTOS Y TIMERS --
#use rs232  (baud=9600,parity=N,xmit=RS232_TX,rcv=RS232_RX,bits=8,stream=RS232)
#use i2c    (Slave,Fast,sda=I2C_SDA,scl=I2C_SCL, address = I2C_TIMER, FORCE_HW)

//-- DEFINICIONES --
#define I2C_CMD_INIT    0xF0
#define I2C_CMD_TEST    0xFF
#define I2C_CMD_RESET   0xFD

#define I2C_CMD_PER0    0x70
#define I2C_CMD_PER1    0x71
#define I2C_CMD_PER2    0x72
#define I2C_CMD_PER3    0x73
#define I2C_CMD_PER4    0x74
#define I2C_CMD_MDON    0x75
#define I2C_CMD_MDOF    0x76
#define I2C_CMD_BUZ1    0x81
#define I2C_CMD_BUZ2    0x82
#define I2C_CMD_BUZ3    0x83

#define ON              1
#define OFF             0
#define DEFAULT_D       200

#define   PuertoA    1
#define   PuertoB    2
#define   PuertoC    3
#define   PuertoD    4
#define   PuertoE    5
#define   PuertoF    6

//-- DECLARACIONES --
//Estructura para el puerto
typedef struct sPort    
{
   int a;
   int b;
   int c;
   int d;
} tPort;

//Datos para la captura de comandos del RS232
char  cmd[10]  = "\0\0\0\0\0\0\0\0\0";
char  rcmd[10] = "\0\0\0\0\0\0\0\0\0";
int8  cmdIndex = 0;
int1  cmdGet   = false;

int  icmd;
int  ircmd[5] = "\0\0\0\0";

//Puertos de Displays
tPort pTiroSU;
tPort pTiroSD;
tPort pTiempoSU;
tPort pTiempoSD;
tPort pTiempoMU;


//-- PROTOTIPOS --
void RDA_isr(void);
void SSP_isr(void);
void parseCommand(void);
void doTest(void);
void showNumber(int8, tPort);
int8 char2int(char); 
void doReset(void);
void parseI2CCommand(void);
void showQuarter(int8);
void buzz(int);

