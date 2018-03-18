#include <SPI.h>//Serial Peripheral Interface Library
#include <PS2X_lib.h>  //for v1.6

// credits to http://www.kevindarrah.com/ for his amazing work and explanations on how he built a LED Cube

/////////////////////////////////////////////////////////////////////////////// CHANGE THESE

#define size_of_cube 5 // size of the cube (if 3x3x3 put a 3)
#define number_of_74hc595 2 // ((size_of_cube * 3) / 8) with superior round number ...
#define number_of_TLC5940 2 // ((size_of_cube * size_of_cube) / 16) with superior round number ...

#define RANDOM_TABLE_SIZE 250 // "grand" nombre pour avoir davantage de variété dans nos nombres aléatoires

#define PIN_SLAVE_SELECT_SHIFT_REGISTER 10 // (SS pour SPI) permet de dire qu'on veut parler ou non aux shift registers via la SPI, selon l'état du pin (HIGH/LOW)
#define PIN_STROBE_MSGEQ7 4 // strobe pins on digital 4
#define PIN_RES_MSGEQ7 9 // pin ?
#define PIN_MSG_MSGEQ7 A5// reset pins on digital 5 ??
#define ANALOG_PIN_LIBRE_POUR_ALEATOIRE 0
#define PIN_XLAT_TLC5940 2 // il ne suffit pas de les changer, car on utilise des PORTB en dur
#define PIN_GSCLK_TLC5940 3 // il ne suffit pas de les changer, car on utilise des PORTB en dur
#define PIN_BLANK_TLC5940 5 // il ne suffit pas de les changer, car on utilise des PORTB en dur
#define PIN_SPI_MOSI 11 // MOSI SPI
#define PIN_SPI_CLOCK 13 // CLOCK SPI
#define PIN_POTENTIOMETRE A4

//////////////////////////////////////////////////////////////////////////// do NOT touch the rest

#define TRANSISTOR_ON 0
#define TRANSISTOR_OFF 1

#define numOfRegisterPins number_of_74hc595 * 8
#define numOfTlcPins number_of_TLC5940 * 16

#define NB_TRANSFERT_BYTE (16 * 12 * number_of_TLC5940)/8

#define NB_TRANSISTORS 3 * size_of_cube

#define NB_BAND_MSGEQ7 7

#define MODE_ANIMATIONS 0
#define MODE_JEU 1
#define MODE_ZEN 2
#define MODE_MUSIQUE 4

//#define number_of_register_pins_used size_of_cube * 3 // NOT NEEDED
//#define number_of_tlc5940_pins_used size_of_cube * size_of_cube // NOT NEEDED

#define RED_COMPONENT 0
#define GREEN_COMPONENT 1
#define BLUE_COMPONENT 2

PS2X ps2x; // create PS2 Controller Class
byte chbit=0, spibit=0, spibyte=0;// variables used by tlc sub routine
int SINData;//variable used to shift data to the TLC
byte transferbyte[NB_TRANSFERT_BYTE];// bytes that are sent out to the tlc5940 via SPI
// 48 because 16 channels @ 12bits gives 384bits, 384/8 = 48 bytes, 12 bit to 8 bit conversion
float left[NB_BAND_MSGEQ7]; // store band values in these arrays // SEULES LES 5 premières cases sont utiles au final (on transforme 7 frequences en 5)

//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int manette_error_code = 0;

byte registers[number_of_74hc595]; // array containing the values of the shift registers outputs
byte led_state[size_of_cube][size_of_cube][size_of_cube][3]; // 3D array containing the R,G,B values of each LED in the cube

byte random_table[RANDOM_TABLE_SIZE]; // tableau de valeurs aléatoires pré-calculées
int random_table_index=0; // index actuel dans le tableau des variables aléatoires

unsigned long m_w; // variable pour la générationaléatoire
unsigned long m_z; // variable pour l'aléatoire

int mode=MODE_ANIMATIONS; // mode actuel du cube

volatile byte potentiometre=0;

volatile int transistor_channel = 0; // 3 composantes* 5 étages = 15, de 0 à  14

//*******************************************************************************************
//*******************************************************************************************
void setup() //   MAIN SETUP   MAIN SETUP   MAIN SETUP   MAIN SETUP   MAIN SETUP
{ 
  pinMode(PIN_XLAT_TLC5940, OUTPUT);
  pinMode(PIN_GSCLK_TLC5940, OUTPUT);
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_CLOCK, OUTPUT);
  pinMode(PIN_SLAVE_SELECT_SHIFT_REGISTER, OUTPUT);
  pinMode(PIN_RES_MSGEQ7, OUTPUT);
  pinMode(PIN_STROBE_MSGEQ7, OUTPUT);
  pinMode(PIN_MSG_MSGEQ7, INPUT);
  
  digitalWrite(PIN_RES_MSGEQ7,LOW); // reset low
  digitalWrite(PIN_STROBE_MSGEQ7,HIGH); //pin 5 is RESET on the shield

  digitalWrite(PIN_SLAVE_SELECT_SHIFT_REGISTER, HIGH); // on dit qu'on ne veut pas parler au(x) Shift register actuellement

  //Set up the SPI
  SPI.setBitOrder(MSBFIRST);//Most Significant Bit First (datasheet)
  SPI.setDataMode(SPI_MODE0);// Mode 0 Rising edge of data, keep clock low (datasheet)
  SPI.setClockDivider(SPI_CLOCK_DIV2);//Run the data in at 16MHz/2 = 8MHz
  //SPI.setClockDivider(1);//Run the data in at 16MHz/2 = 8MHz
  SPI.begin();//start up the SPI library

  for(int i=0; i<NB_TRANSFERT_BYTE; i++)//clear out Gray Scale Data
    transferbyte[i]=0; 
    
  for(int i=0; i<NB_BAND_MSGEQ7; i++)
    left[i]=0;
    
  for(int i=0; i<numOfTlcPins; i++)//wipe out the data in tlc
    tlc(i, 0);// This is how you update the LEDs, tlc is a subroutine with two inputs
  // tlc(Channel, Value)  Channel in this case is 0-32 and value is 0-4095 duty cycle
  // 4095 is 100% ON
    
  LED_All_Clear();

  setRegisters(TRANSISTOR_OFF);
  writeRegisters();
  
  randomSeed(analogRead(ANALOG_PIN_LIBRE_POUR_ALEATOIRE));
  m_w = random(255);
  m_z = random(255);
  random_table_index= random(RANDOM_TABLE_SIZE);
  random_table_create(random_table);
  //Serial.print("index:");Serial.println(random_table_index); // DEBUG ALEATOIRE
  //Serial.print("m_w:");Serial.println(m_w); // DEBUG ALEATOIRE
  //Serial.print("m_z:");Serial.println(m_z); // DEBUG ALEATOIRE
  //Serial.flush(); // DEBUG ALEATOIRE
  //random_test(); // DEBUG ALEATOIRE
  
  //Serial.print("freeRam=");Serial.println(freeRam());
  
  manette_initialiser();

  noInterrupts();// set up the counters, so don't go into interrupts
  TCCR2A=B00010010;//Timer 2 set to Compare Mode Toggling pin 5 @ 8MHz, Arduino Digital 3
  // TIMER 2 IS GSCLCK
  //Timer 2 prescaler set to 1, 16/1=16 MHz, but toggles pin 5 every other cycle, 8MHz
  TCCR2B=B00000001;

  TCCR1A=B00000000;//Timer 1 doesn't toggle anything, used for counting
  //Timer 1 prescaler set to Fclk/256
  //Why? We need to count 4096 pulses out of Timer 2 - pin 5
  //8 MHz = 1 pulse every 125ns - - 4096 pulses would need 512us
  //Timer 1 runs at 16MHz/256=62.5kHz, we need a match at every 512us
  //Basically, I can get an interrupt to get called every 512us, so...
  // I need to run Timer 2 @ 8MHz for 512us to get 4096 pulses
  // I can't count those pulses directy (too fast) , so
  // I'll count using Timer 1, which makes a count every 16us
  // The counter starts at 0, so we'll set it to 31 to get an interrupt after 512us
  TCCR1B=B00001100;//Mode=CTC with OSCR1A = TOP and 256 as the prescaler
  // Mask set up, will call ISR (Inerrupt Service Routine) for Compare match on A
  TIMSK1=B00000010;
  //These are the match values for the counters
  // 0 here means it will match on one cycle of the clock/prescaler
  OCR1A=300 ;//(31/100)to get our 512us Interrupt

  interrupts();// kick off the timers!

  pinMode(PIN_BLANK_TLC5940, OUTPUT);//BLANK  We set this pin up here, so it remains in a high impedance
  // state throughout the setup, otherwise the LEDs go crazy!  even if you write this HIGH  
}

void loop() //   MAIN LOOP   MAIN LOOP   MAIN LOOP   MAIN LOOP   MAIN LOOP   MAIN LOOP
{
  /*if (manette_error_code == 1) //skip loop if no controller found
    return;*/
    
  //serial_test();
 
  //millis_test(); 
    
  //readMSGEQ7();
  //test_musique_volume();
  //anim_musique1();
  //anim_musique2();
  
  //test_manette();
  snakey();
  
  //LED_Set(0,0,0,255,0,0);
    
  /*LED_Set(1,1,2,255,255,255);
  LED_Set(1,1,1,255,255,255);
  LED_Set(1,1,0,255,255,255);*/
  //allRed();
  //allGreen();
  //allBlue();
  //test_2_rainbow();
  
  //test_led_individuelle();
  //test_2_rainbow();

  //pluie();
  //planchemontante();
  //diagonaletournante();
  //gouttetombante();
  //feuartifice(255, 00, 255);
  //epidemie(0, 0, 250);
  //coulee(0, 30, 100);
  //afficher_lumos(255,0,255);
  //truc_tournant_avecdemitour(255,50,50);
  //truc_style();
  //truc_tournant(255,255,0,1);
  //gouttetombante();
}

// INTERRUPTS   INTERRUPTS   INTERRUPTS   INTERRUPTS   INTERRUPTS   INTERRUPTS   INTERRUPTS 
ISR(TIMER1_COMPA_vect)// Interrupt to count 4096 Pulses on GSLCK
{ 
  potentiometre=analogRead(PIN_POTENTIOMETRE) / 1023 * 255;
  
  // Déclaration des variables
  int column_channel =0;

  // Préparation des TLC
  for(int x=0; x<size_of_cube; x++)
  {
    for(int y=0; y<size_of_cube; y++)
    {
      tlc(column_channel, 16*led_state[x][y][transistor_channel/3][transistor_channel%3]);
      column_channel++;
    }
  }

  // Désactivation du transistor associé à  la composante précédente
  setRegisters(TRANSISTOR_OFF); //sert à  éteindre la composante précédente, en fait on les éteint tous, c'est tout aussi efficace
  writeRegisters();

  SPI.end();//end the SPI so we can write to the clock pin
  PORTB |= 1<<5;// SPI Clock pin to give it the extra count
  PORTB &= 0<<5;// The data sheet says you need this for some reason?
  SPI.begin();// start the SPI back up
  for(SINData= NB_TRANSFERT_BYTE - 1; SINData>=0; SINData--)// send the data out!
    SPI.transfer(transferbyte[SINData]);// The SPI port only understands bytes-8 bits wide
  // The TLC needs 12 bits for each channel, so 12bits times 32 channels gives 384 bits
  // 384/8=48 bytes, 0-47

  PORTD |= 1<<5;// write blank HIGH to reset the 4096 counter in the TLC
  PORTD |= 1<<2;// write XLAT HIGH to latch in data from the last data stream
  PORTD &= 0<<2;//XLAT can go low now
  PORTD &= 0<<5;//Blank goes LOW to start the next cycle

  // Activation du transistor associé à  la composante
  setRegisterPin(transistor_channel, TRANSISTOR_ON);
  writeRegisters();

  // On passe au transistor suivant
  transistor_channel++;
  if(transistor_channel >= NB_TRANSISTORS)
  {
    transistor_channel= 0;
  }
}

void tlc(int channel, int value)
{
  /* DESACTIVE CAR PERTE DE TEMPS
  Limit check
  if(value>4095)
    value=4095;
  if(value<0)
    value=0;
  */

  // We need to convert the 12 bit value into an 8 bit BYTE, the SPI can't write 12bits

  //We figure out where in all of the bytes to write to, so we don't have to waste time
  // updating everything

  //12 bits into bytes, a start of 12 bits will either at 0 or 4 in a byte
  spibit=0;
  if(bitRead(channel, 0))//if the read of the value is ODD, the start is at a 4
    spibit=4;

  //This is a simplification of channel * 12 bits / 8 bits
  spibyte = int(channel*3/2);//this assignes which byte the 12 bit value starts in

  for(chbit=0; chbit<12; chbit++, spibit++){// start right at where the update will go
    if(spibit==8){//during the 12 bit cycle, the limit of byte will be reached
      spibyte++;//roll into the next byte
      spibit=0;//reset the bit count in the byte
    }
    if(bitRead(value, chbit))//check the value for 1's and 0's
      bitSet(transferbyte[spibyte], spibit);//transferbyte is what is written to the TLC
    else
      bitClear(transferbyte[spibyte], spibit);
  }//0-12 bit loop
}//  END OF TLC WRITE  END OF TLC WRITE  END OF TLC WRITE  END OF TLC WRITE  END OF TLC WRITE
//*******************************************************************************************

void serial_test()
{
  long i=0;
  while(1)
  {
    Serial.println(i);
    i++;
  }
}

void millis_test()
{
  long t=0;
  while(1)
  {
    Serial.println(millis());
    delay(10);
  }
}

void random_test()
{
  for(int i=0;i<200;i++)
  {
    Serial.println(getRandom(2));
  }
}

int freeRam () // Retourne la RAM disponible
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
