/////////////////////////////////////// Mini bibliothèque de gestion des LED

// set a precise LED in the cube. Does not update it on the cube.
void LED_Set(byte x, byte y, byte z, byte r, byte g, byte b)
{
  led_state[x][y][z][RED_COMPONENT] = r;
  led_state[x][y][z][GREEN_COMPONENT] = g;
  led_state[x][y][z][BLUE_COMPONENT] = b;
}

// set all LEDs off
void LED_All_Clear()
{
  for(int x=0; x< size_of_cube; x++)
  {
    for(int y=0; y< size_of_cube; y++)
    {
      for(int z=0; z< size_of_cube; z++)
      {
        led_state[x][y][z][RED_COMPONENT] = 0;
        led_state[x][y][z][GREEN_COMPONENT] = 0;
        led_state[x][y][z][BLUE_COMPONENT] = 0;
      }
    }
  }  
}


