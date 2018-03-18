////////////////////////////////////////////////////////// shift register mini library

//set all register pins to LOW
void clearRegisters()
{
  for(int i =0; i <number_of_74hc595 ; i++)
  {
    registers[i] = LOW;
  }
}

void setRegisters(boolean value) 
{
  if(value==LOW)
  {
    for(int i =0; i <number_of_74hc595 ; i++)
    {
      registers[i] = 0;
    }
  }
  else if(value==HIGH)
  {
    for(int i =0; i <number_of_74hc595 ; i++)
    {
      registers[i] = 255;
    }
  }
}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, boolean value)
{
  index=numOfRegisterPins-index-1;
  if(value==LOW)
  {
    registers[index>>3] &= ~( 1 << (7-(index%8)) );
  }
  else if(value==HIGH)
  {
    registers[index>>3] |=  ( 1 << (7-(index%8)) ) ;
  }
}

//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
void writeRegisters()
{  
  //digitalWrite(PIN_SLAVE_SELECT_SHIFT_REGISTER, LOW);
  PORTB &= B11111011;
  for(int i = 0 ; i< number_of_74hc595 ; i++)
  {
    SPI.transfer(registers[i]);
  }
  //digitalWrite(PIN_SLAVE_SELECT_SHIFT_REGISTER, HIGH);
  PORTB |= B00000100;
}


