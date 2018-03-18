void readMSGEQ7() // Function to read 7 band equalizers
{
  int band;
  digitalWrite(PIN_RES_MSGEQ7, HIGH);
  digitalWrite(PIN_RES_MSGEQ7, LOW);
  for (band = 0; band < 7; band++)
  {
    digitalWrite(PIN_STROBE_MSGEQ7, LOW); // strobe pin on the shield - kicks the IC up to the next band
    delayMicroseconds(300); //
    left[band] = analogRead(PIN_MSG_MSGEQ7); // store left band reading

    digitalWrite(PIN_STROBE_MSGEQ7, HIGH);
  }

  //Décaler les LED pour enlever celle de gauche
  for (band = 0; band < 6; band++)
  {
    left[band] = left[band + 1];
  }
  left[3] = (left[3] + left[4]) / 2;
  left[4] = (left[5] + left[6]) / 2;

  for (band = 0; band < 7; band++)
  {
    left[band] = left[band];
  }
}

void test_musique_volume()
{
  float moyenne = 0;
  for (int i = 0; i < size_of_cube; i++)
  {
    moyenne = moyenne + left[i];
  }
  moyenne = moyenne / size_of_cube;
  Serial.println(moyenne);
}

void anim_musique1()
{
  int x, y;
  float moyenne = 0;
  
  LED_All_Clear();
  
  for (int i = 0; i < size_of_cube; i++)
  {
    moyenne = moyenne + left[i];
  }
  moyenne = moyenne / size_of_cube;
  if (moyenne > 90)
  {
    for (x = 0; x < size_of_cube; x++)
    {
      for (y = 0; y < size_of_cube; y++)
      {
        LED_Set(x, y, 0, 0, 0, 255);
      }
    }
  }
  if (moyenne > 140)
  {
    for (x = 0; x < size_of_cube; x++)
    {
      for (y = 0; y < size_of_cube; y++)
      {
        LED_Set(x, y, 1, 80, 0, 180);
      }
    }
  }
  if (moyenne > 160)
  {
    for (x = 0; x < size_of_cube; x++)
    {
      for (y = 0; y < size_of_cube; y++)
      {
        LED_Set(x, y, 2, 120, 0, 120);
      }
    }
  }
  if (moyenne > 180)
  {
    for (x = 0; x < size_of_cube; x++)
    {
      for (y = 0; y < size_of_cube; y++)
      {
        LED_Set(x, y, 3, 180, 0, 80);
      }
    }
  }
  if (moyenne > 200)
  {
    for (x = 0; x < size_of_cube; x++)
    {
      for (y = 0; y < size_of_cube; y++)
      {
        LED_Set(x, y, 4, 255, 0, 0);
      }
    }
  }
}

void anim_musique2()
{
  int y;
  LED_All_Clear();
  for (int freq = 0; freq < size_of_cube; freq++)
  {
    if (left[freq] > 150)
    {
      for (y = 0; y < size_of_cube; y++)
      {
        LED_Set(freq, y, 0, 0, 255, 0);
      }
    }
    if (left[freq] > 300)
    {
      for (y = 0; y < size_of_cube; y++)
      {
        LED_Set(freq, y, 1, 255, 125, 0);
      }
    }
    if (left[freq] > 550)
    {
      for (y = 0; y < size_of_cube; y++)
      {
        LED_Set(freq, y, 2, 255, 0, 0);
      }
    }
    if (left[freq] > 700)
    {
      for (y = 0; y < size_of_cube; y++)
      {
        LED_Set(freq, y, 3, 255, 127, 0);
      }
    }
    if (left[freq] > 900)
    {
      for (y = 0; y < size_of_cube; y++)
      {
        LED_Set(freq, y, 4, 255, 0, 0);
      }
    }
  }
}
