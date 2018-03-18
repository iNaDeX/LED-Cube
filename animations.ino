//////////////////////////////////////////////////////////////////////////////////// Animations

void allRed()
{
  while (1) //infinite loop
  {
    for (byte x = 0; x < size_of_cube; x++)
    {
      for (byte y = 0; y < size_of_cube; y++)
      {
        for (byte z = 0; z < size_of_cube; z++)
        {
          LED_Set(x, y, z, potentiometre, 0, 0);
        }
      }
    }
  }
}

void allGreen()
{
  while (1) //infinite loop
  {
    for (byte x = 0; x < size_of_cube; x++)
    {
      for (byte y = 0; y < size_of_cube; y++)
      {
        for (byte z = 0; z < size_of_cube; z++)
        {
          LED_Set(x, y, z, 0, 255, 0);
        }
      }
    }
  }
}

void allBlue()
{
  while (1) //infinite loop
  {
    for (byte x = 0; x < size_of_cube; x++)
    {
      for (byte y = 0; y < size_of_cube; y++)
      {
        for (byte z = 0; z < size_of_cube; z++)
        {
          LED_Set(x, y, z, 0, 0, 255);
        }
      }
    }
  }
}

void test_led_individuelle()
{
  while (1) //infinite loop
  {
    for (byte z = 0; z < size_of_cube; z++)
    {
      for (byte y = 0; y < size_of_cube; y++)
      {
        for (byte x = 0; x < size_of_cube; x++)
        {
          LED_Set(x, y, z, 255, 0, 0); // allume
          delay(1);
          LED_Set(x, y, z, 0, 0, 0); // eteint

          LED_Set(x, y, z, 0, 255, 0); // allume
          delay(1);
          LED_Set(x, y, z, 0, 0, 0); // eteint

          LED_Set(x, y, z, 0, 0, 255); // allume
          delay(1);
          LED_Set(x, y, z, 0, 0, 0); // eteint
        }
      }
    }
  }
}

void test_2_rainbow()
{
  byte r = 255;
  byte g = 0;
  byte b = 255;
  byte stage = 1;
  byte rapidity = 1;
  int c = 0;

  while (1) //infinite loop
  {
    if (c == 10)
    {
      c = 0;
      for (byte x = 0; x < size_of_cube; x++)
      {
        for (byte y = 0; y < size_of_cube; y++)
        {
          for (byte z = 0; z < size_of_cube; z++)
          {
            LED_Set(x, y, z, r, g, b);
          }
        }
      }
      rainbow_color(&r, &g, &b, &stage, rapidity);
    }
    c++;
  }
}

void rainbow_color(byte* r, byte*g, byte*b, byte*stage, byte rapidity)
{
  switch (*stage)
  {
    case 1:
      *r = *r - rapidity;
      if (*r <= 0)
      {
        *r = 0;
        *stage = 2;
      }
      break;
    case 2:
      *g = *g + rapidity;
      if (*g >= 255)
      {
        *g = 255;
        *stage = 3;
      }
      break;
    case 3:
      *b = *b - rapidity;
      if (*b <= 0)
      {
        *b = 0;
        *stage = 4;
      }
      break;
    case 4:
      *r = *r + rapidity;
      if (*r >= 255)
      {
        *r = 255;
        *stage = 5;
      }
      break;
    case 5:
      *g = *g - rapidity;
      if (*g <= 0)
      {
        *g = 0;
        *stage = 6;
      }
      break;
    case 6:
      *b = *b + rapidity;
      if (*b >= 255)
      {
        *b = 255;
        *stage = 1;
      }
      break;
  }
}

void planchemontante()
{
  int z = 0;
  int x = 0;
  int y = 0;
  int montee;
  int cpt = 5;

  while (1)
  {
    cpt--;

    if (z == size_of_cube - 1)
      montee = -1;

    if (z == 0)
      montee = 1;


    for (x = 0; x < size_of_cube; x++)
    {
      for (y = 0; y < size_of_cube; y++)
      {
        LED_Set(x, y, z, 0, 0, 255);
        if (cpt == 0)
        {
          for (x = 0; x < size_of_cube; x++)
            for (y = 0; y < size_of_cube; y++)
            {
              LED_Set(x, y, z, 0, 0, 0);
            }
          z += montee;
          cpt = 5;
        }
      }
    }
  }
}

typedef struct goutte
{
  int x;
  int y;
  int z;
  int cpt;
}
t_goutte;

void pluie()
{
  //Déclaration des variables
  const int nb_gouttes = 1; // equivalent #define CE NOMBRE DOIT ETRE INFERIEUR A SIZE_OF_CUBE * SIZE_OF_CUBE car le code empêche d'avoir 2 gouttes au même endroit
  const int duree = 1; // equivalent #define

  t_goutte tab_gouttes[nb_gouttes];
  int posXY_occupe[size_of_cube][size_of_cube];

  // Initialisation
  for (int i = 0; i < nb_gouttes; i++)
  {
    tab_gouttes[i].x = 0;
    tab_gouttes[i].y = 0;
    tab_gouttes[i].z = 0;
    tab_gouttes[i].cpt = 0;
  }

  for (int i = 0; i < size_of_cube; i++)
  {
    for (int j = 0; j < size_of_cube; j++)
    {
      posXY_occupe[i][j] = 0;
    }
  }

  // Boucle de jeu
  while (1)
  {
    //Serial.println("Debut while");
    // On parcourt toutes les gouttes
    for (int i = 0; i < nb_gouttes; i++)
    {
      //Serial.print("Goutte:");Serial.println(i);
      //Serial.print("x,y,z,cpt:");Serial.print(tab_gouttes[i].x);Serial.print(tab_gouttes[i].y);Serial.print(tab_gouttes[i].z);Serial.println(tab_gouttes[i].cpt);

      if (tab_gouttes[i].z == 0 && tab_gouttes[i].cpt == 0) // si on trouve une goutte morte
      {
        //Serial.println("Creation");
        posXY_occupe[tab_gouttes[i].x][tab_gouttes[i].y] = 0; // On enregistre que les emplacements (x,y) précédemment pris par cette goutte sont maintenant libres
        LED_Set(tab_gouttes[i].x, tab_gouttes[i].y, tab_gouttes[i].z, 0, 0, 0); // On éteint la LED de la goutte qu'on va détruire

        do
        {
          //Serial.println("Cherche une place");
          tab_gouttes[i].x = getRandom(size_of_cube);
          tab_gouttes[i].y = getRandom(size_of_cube);
        }
        while ( posXY_occupe[tab_gouttes[i].x][tab_gouttes[i].y] == 1 ); // on cherche une position X non déja  prise par une autre goutte

        // on enregistre qu'on a pris cette position
        posXY_occupe[tab_gouttes[i].x][tab_gouttes[i].y] = 1;

        tab_gouttes[i].z = size_of_cube - 1;
        tab_gouttes[i].cpt = duree;
      }

      LED_Set(tab_gouttes[i].x, tab_gouttes[i].y, tab_gouttes[i].z, 0, 0, 255);

      tab_gouttes[i].cpt = tab_gouttes[i].cpt - 1; // on diminue le compteur de durée (qui indique pendant combien de tours de boucle une goutte reste à  la même hauteur

      if (tab_gouttes[i].cpt == 0 && tab_gouttes[i].z > 0) // si une goutte doit descendre (elle a épuisé sa durée sur l'étage actuel et n'est pas déja au RDC)
      {
        //Serial.println("Descente de la goutte");
        LED_Set(tab_gouttes[i].x, tab_gouttes[i].y, tab_gouttes[i].z, 0, 0, 0);
        tab_gouttes[i].z = tab_gouttes[i].z - 1; // on la fait descendre
        tab_gouttes[i].cpt = duree; // on remet au max sa durée de vie pour le nouvel étage
      }

      //LED_Set(tab_gouttes[i].x, tab_gouttes[i].y, tab_gouttes[i].z, 0, 0, 255); // remonté pour allumer le dernier étage autant que les autres (bug visible quand duree =1)
    }
  }

}

void feuartifice(int r, int g, int b)
{
  int x = 0;
  int y = 0;
  int z = 0;
  int d;
  int x1, x2, y1, y2;
  x = 1;
  y = 1;
  z = 0;
  while (1)
  {
    //delay(1000);
    for (z = 0; z < size_of_cube; z++)
    {
      LED_Set(x, y, z, r, g, b);
      delay(3);
      LED_Set(x, y, z, 0, 0, 0);

    }
    z = size_of_cube - 1;
    x1 = x + 1;
    x2 = x - 1;
    y1 = y - 1;
    y2 = y + 1;
    r = getRandom(255);
    g = getRandom(255);
    b = getRandom(155);
    d = getRandom(3);

    if (x1 < size_of_cube )
    {
      LED_Set(x1, y, z, r, g, b);
    }
    if (x2 > -1)
      LED_Set(x2, y, z, r, g, b);
    if (y1 > -1)
    {
      LED_Set(x, y1, z, r, g, b);
    }
    if (y2 < size_of_cube)
    {
      LED_Set(x, y2, z, r, g, b);
    }
    delay(1);
    if (x1 < size_of_cube )
      LED_Set(x1, y, z, 0, 0, 0);
    if (x2 > -1)
      LED_Set(x2, y, z, 0, 0, 0);
    if (y1 > -1)
      LED_Set(x, y1, z , 0, 0, 0);
    if (y2 < size_of_cube)
      LED_Set(x, y2, z, 0, 0, 0);
    while (z > -1)
    {
      z--;

      if (x1 < size_of_cube )
      {
        LED_Set(x1, y, z, r, g, b);
      }
      if (x2 > -1)
        LED_Set(x2, y, z, r, g, b);
      if (y1 > -1)
      {
        LED_Set(x, y1, z, r, g, b);
      }
      if (y2 < size_of_cube)
      {
        LED_Set(x, y2, z, r, g, b);
      }
      delay(size_of_cube - z);
      if (x1 < size_of_cube )
        LED_Set(x1, y, z, 0, 0, 0);
      if (x2 > -1)
        LED_Set(x2, y, z, 0, 0, 0);
      if (y1 > -1)
        LED_Set(x, y1, z, 0, 0, 0);
      if (y2 < size_of_cube)
        LED_Set(x, y2, z, 0, 0, 0);

    }
    r = getRandom(255);
    g = getRandom(255);
    b = getRandom(255);
    if (d == 0)
      r = 255;
    if (d == 1)
      g = 255;
    if (d == 2)
      b = 155;
  }
}

void epidemie(int r, int g, int b)
{
  int x = 0;
  int y = 0;
  int d = 0;
  int z = size_of_cube - 1;
  delay(100);
  while (1)
  {
    for (z = 0; z < size_of_cube; z++)
    {
      r = getRandom(255);
      g = getRandom(255);
      b = getRandom(255);
      d = getRandom(3);
      for (x = 0; x < size_of_cube; x++)
        for (y = 0; y < size_of_cube; y++)
        {
          LED_Set(x, y, z, r, g, b);

        }

      if (d == 0)
        r = 255;
      if (d == 1)
        g = 255;
      if (d == 2)
        b = 255;
    }

    y = 0;
    x = 0;
    for (z = 0; z < size_of_cube; z++)
    {
      if ((z % 2) == 1)
      {
        for (x = 0; x < size_of_cube; x++)
          for (y = 0; y < size_of_cube; y++)
          {
            LED_Set(x, y, z, 0, 0, 0);
            delay(1);
          }
      }
      if (z % 2 == 0)
      {
        for (x = size_of_cube; x > -1; x--)
          for (y = size_of_cube; y > -1; y--)
          {
            LED_Set(x, y, z, 0, 0, 0);
            delay(1);
          }

      }
    }
  }
}

void coulee(int r, int g , int b)
{
  int x, y, z;
  int i = 0;
  while (1)
  {
    z = size_of_cube - 1;
    for (x = 0; x < size_of_cube; x++)
      for (y = 0; y < size_of_cube; y++)
      {
        LED_Set(x, y, z, r, g, b);
      }

    for (y = 0; y < size_of_cube; y++)
      for (x = 0; x < size_of_cube; x++)
        for (z = size_of_cube - 1; z > -1; z--)
        {
          LED_Set(x, y, z, r, g, b);
          delay(5);
          if (z != 0)
            LED_Set(x, y, z, 0, 0, 0);
        }

    for (y = 0; y < size_of_cube; y++)
      for (x = 0; x < size_of_cube; x++)
        for (z = 0; z < size_of_cube; z++)
        { 
          if (z != 0)
          {
            LED_Set(x, y, z, r, g, b);
            delay(5);
          }
          if (z != size_of_cube - 1)
            LED_Set(x, y, z, 0, 0, 0);
        }
  }
}
/*  
void afficher_lumos(int r,int g,int b)
{
  ////////////////////////////allumage du L face 0////////////////////////////
  LED_Set(0, 0, 4, r, g, b);   
  LED_Set(0, 0, 3, r, g, b);
  LED_Set(0, 0, 2, r, g, b);   
  LED_Set(0, 0, 1, r, g, b);
  LED_Set(0, 0, 0, r, g, b);
  LED_Set(1, 0, 0, r, g, b);
  LED_Set(2, 0, 0, r, g, b);
  LED_Set(3, 0, 0, r, g, b);
  LED_Set(4, 0, 0, r, g, b);
  delay(5);
  LED_Set(0, 0, 4, 0, 0, 0);
  LED_Set(0, 0, 3, 0, 0, 0);
  LED_Set(0, 0, 2, 0, 0, 0);
  LED_Set(0, 0, 1, 0, 0, 0);
  LED_Set(0, 0, 0, 0, 0, 0);
  LED_Set(1, 0, 0, 0, 0, 0);
  LED_Set(2, 0, 0, 0, 0, 0);
  LED_Set(3, 0, 0, 0, 0, 0);
  LED_Set(4, 0, 0, 0, 0, 0);

  //allumage du L face 1//
  LED_Set(0, 1, 4, r, g, b);
  LED_Set(0, 1, 3, r, g, b);
  LED_Set(0, 1, 2, r, g, b);
  LED_Set(0, 1, 1, r, g, b);
  LED_Set(0, 1, 0, r, g, b);
  LED_Set(1, 1, 0, r, g, b);
  LED_Set(2, 1, 0, r, g, b);
  LED_Set(3, 1, 0, r, g, b);
  LED_Set(4, 1, 0, r, g, b);
  delay(5);
  LED_Set(0, 1, 4, 0, 0, 0);
  LED_Set(0, 1, 3, 0, 0, 0);
  LED_Set(0, 1, 2, 0, 0, 0);
  LED_Set(0, 1, 1, 0, 0, 0);
  LED_Set(0, 1, 0, 0, 0, 0);
  LED_Set(1, 1, 0, 0, 0, 0);
  LED_Set(2, 1, 0, 0, 0, 0);  
  LED_Set(3, 1, 0, 0, 0, 0);
  LED_Set(4, 1, 0, 0, 0, 0);

  //allumage du L face 2//
  LED_Set(0, 2, 4, r, g, b);
  LED_Set(0, 2, 3, r, g, b);
  LED_Set(0, 2, 2, r, g, b);
  LED_Set(0, 2, 1, r, g, b);
  LED_Set(0, 2, 0, r, g, b);
  LED_Set(1, 2, 0, r, g, b);
  LED_Set(2, 2, 0, r, g, b);
  LED_Set(3, 2, 0, r, g, b);
  LED_Set(4, 2, 0, r, g, b);
  delay(5);
  LED_Set(0, 2, 4, 0, 0, 0);
  LED_Set(0, 2, 3, 0, 0, 0);
  LED_Set(0, 2, 2, 0, 0, 0);
  LED_Set(0, 2, 1, 0, 0, 0);
  LED_Set(0, 2, 0, 0, 0, 0);
  LED_Set(1, 2, 0, 0, 0, 0);
  LED_Set(2, 2, 0, 0, 0, 0);  
  LED_Set(3, 2, 0, 0, 0, 0);
  LED_Set(4, 2, 0, 0, 0, 0);

  //affichage du L face 3//
  LED_Set(0, 3, 4, r, g, b);
  LED_Set(0, 3, 3, r, g, b);
  LED_Set(0, 3, 2, r, g, b);
  LED_Set(0, 3, 1, r, g, b);
  LED_Set(0, 3, 0, r, g, b);
  LED_Set(1, 3, 0, r, g, b);
  LED_Set(2, 3, 0, r, g, b);
  LED_Set(3, 3, 0, r, g, b);
  LED_Set(4, 3, 0, r, g, b);
  delay(5);
  LED_Set(0, 3, 4, 0, 0, 0);
  LED_Set(0, 3, 3, 0, 0, 0);
  LED_Set(0, 3, 2, 0, 0, 0);
  LED_Set(0, 3, 1, 0, 0, 0);
  LED_Set(0, 3, 0, 0, 0, 0);
  LED_Set(1, 3, 0, 0, 0, 0);
  LED_Set(2, 3, 0, 0, 0, 0);  
  LED_Set(3, 3, 0, 0, 0, 0);
  LED_Set(4, 3, 0, 0, 0, 0);

  //affichage du L face 4//
  LED_Set(0, 4, 4, r, g, b);
  LED_Set(0, 4, 3, r, g, b);
  LED_Set(0, 4, 2, r, g, b);
  LED_Set(0, 4, 1, r, g, b);
  LED_Set(0, 4, 0, r, g, b);
  LED_Set(1, 4, 0, r, g, b);
  LED_Set(2, 4, 0, r, g, b);
  LED_Set(3, 4, 0, r, g, b);
  LED_Set(4, 4, 0, r, g, b);
  delay(5);
  LED_Set(0, 4, 4, 0, 0, 0);
  LED_Set(0, 4, 3, 0, 0, 0);
  LED_Set(0, 4, 2, 0, 0, 0);
  LED_Set(0, 4, 1, 0, 0, 0);
  LED_Set(0, 4, 0, 0, 0, 0);
  LED_Set(1, 4, 0, 0, 0, 0);
  LED_Set(2, 4, 0, 0, 0, 0);  
  LED_Set(3, 4, 0, 0, 0, 0);
  LED_Set(4, 4, 0, 0, 0, 0);






  ////////////////////////////allumage du U face 0////////////////////////////
  LED_Set(0, 0, 4, r, g, b);
  LED_Set(0, 0, 3, r, g, b);
  LED_Set(0, 0, 2, r, g, b);
  LED_Set(0, 0, 1, r, g, b);
  LED_Set(0, 0, 0, r, g, b);
  LED_Set(1, 0, 0, r, g, b);
  LED_Set(2, 0, 0, r, g, b);
  LED_Set(3, 0, 0, r, g, b);
  LED_Set(4, 0, 0, r, g, b);
  LED_Set(4, 0, 0, r, g, b);
  LED_Set(4, 0, 1, r, g, b);
  LED_Set(4, 0, 2, r, g, b);
  LED_Set(4, 0, 3, r, g, b);
  LED_Set(4, 0, 4, r, g, b);
  delay(5);
  LED_Set(0, 0, 4, 0, 0, 0);
  LED_Set(0, 0, 3, 0, 0, 0);
  LED_Set(0, 0, 2, 0, 0, 0);
  LED_Set(0, 0, 1, 0, 0, 0);
  LED_Set(0, 0, 0, 0, 0, 0);
  LED_Set(1, 0, 0, 0, 0, 0);
  LED_Set(2, 0, 0, 0, 0, 0);
  LED_Set(3, 0, 0, 0, 0, 0);
  LED_Set(4, 0, 0, 0, 0, 0);
  LED_Set(4, 0, 0, 0, 0, 0);
  LED_Set(4, 0, 1, 0, 0, 0);
  LED_Set(4, 0, 2, 0, 0, 0);
  LED_Set(4, 0, 3, 0, 0, 0);
  LED_Set(4, 0, 4, 0, 0, 0);

  //allumage du U face 1//
   LED_Set(0, 1, 4, r, g, b);
  LED_Set(0, 1, 3, r, g, b);
  LED_Set(0, 1, 2, r, g, b);
  LED_Set(0, 1, 1, r, g, b);
  LED_Set(0, 1, 0, r, g, b);
  LED_Set(1, 1, 0, r, g, b);
  LED_Set(2, 1, 0, r, g, b);
  LED_Set(3, 1, 0, r, g, b);
  LED_Set(4, 1, 0, r, g, b);
  LED_Set(4, 1, 0, r, g, b);
  LED_Set(4, 1, 1, r, g, b);
  LED_Set(4, 1, 2, r, g, b);
  LED_Set(4, 1, 3, r, g, b);
  LED_Set(4, 1, 4, r, g, b);
  delay(5);
  LED_Set(0, 1, 4, 0, 0, 0);
  LED_Set(0, 1, 3, 0, 0, 0);
  LED_Set(0, 1, 2, 0, 0, 0);
  LED_Set(0, 1, 1, 0, 0, 0);
  LED_Set(0, 1, 0, 0, 0, 0);
  LED_Set(1, 1, 0, 0, 0, 0);
  LED_Set(2, 1, 0, 0, 0, 0);
  LED_Set(3, 1, 0, 0, 0, 0);
  LED_Set(4, 1, 0, 0, 0, 0);
  LED_Set(4, 1, 0, 0, 0, 0);
  LED_Set(4, 1, 1, 0, 0, 0);
  LED_Set(4, 1, 2, 0, 0, 0);
  LED_Set(4, 1, 3, 0, 0, 0);
  LED_Set(4, 1, 4, 0, 0, 0);

  //allumage du U face 2//
  LED_Set(0, 2, 4, r, g, b);
  LED_Set(0, 2, 3, r, g, b);
  LED_Set(0, 2, 2, r, g, b);
  LED_Set(0, 2, 1, r, g, b);
  LED_Set(0, 2, 0, r, g, b);
  LED_Set(1, 2, 0, r, g, b);
  LED_Set(2, 2, 0, r, g, b);
  LED_Set(3, 2, 0, r, g, b);
  LED_Set(4, 2, 0, r, g, b);
  LED_Set(4, 2, 0, r, g, b);
  LED_Set(4, 2, 1, r, g, b);
  LED_Set(4, 2, 2, r, g, b);
  LED_Set(4, 2, 3, r, g, b);
  LED_Set(4, 2, 4, r, g, b);
  delay(5);
  LED_Set(0, 2, 4, 0, 0, 0);
  LED_Set(0, 2, 3, 0, 0, 0);
  LED_Set(0, 2, 2, 0, 0, 0);
  LED_Set(0, 2, 1, 0, 0, 0);
  LED_Set(0, 2, 0, 0, 0, 0);
  LED_Set(1, 2, 0, 0, 0, 0);
  LED_Set(2, 2, 0, 0, 0, 0);
  LED_Set(3, 2, 0, 0, 0, 0);
  LED_Set(4, 2, 0, 0, 0, 0);
  LED_Set(4, 2, 0, 0, 0, 0);
  LED_Set(4, 2, 1, 0, 0, 0);
  LED_Set(4, 2, 2, 0, 0, 0);
  LED_Set(4, 2, 3, 0, 0, 0);
  LED_Set(4, 2, 4, 0, 0, 0);

  //allumage du U face 3//
  LED_Set(0, 3, 4, r, g, b);
  LED_Set(0, 3, 3, r, g, b);
  LED_Set(0, 3, 2, r, g, b);
  LED_Set(0, 3, 1, r, g, b);
  LED_Set(0, 3, 0, r, g, b);
  LED_Set(1, 3, 0, r, g, b);
  LED_Set(2, 3, 0, r, g, b);
  LED_Set(3, 3, 0, r, g, b);
  LED_Set(4, 3, 0, r, g, b);
  LED_Set(4, 3, 0, r, g, b);
  LED_Set(4, 3, 1, r, g, b);
  LED_Set(4, 3, 2, r, g, b);
  LED_Set(4, 3, 3, r, g, b);
  LED_Set(4, 3, 4, r, g, b);
  delay(5);
  LED_Set(0, 3, 4, 0, 0, 0);
  LED_Set(0, 3, 3, 0, 0, 0);
  LED_Set(0, 3, 2, 0, 0, 0);
  LED_Set(0, 3, 1, 0, 0, 0);
  LED_Set(0, 3, 0, 0, 0, 0);
  LED_Set(1, 3, 0, 0, 0, 0);
  LED_Set(2, 3, 0, 0, 0, 0);
  LED_Set(3, 3, 0, 0, 0, 0);
  LED_Set(4, 3, 0, 0, 0, 0);
  LED_Set(4, 3, 0, 0, 0, 0);
  LED_Set(4, 3, 1, 0, 0, 0);
  LED_Set(4, 3, 2, 0, 0, 0);
  LED_Set(4, 3, 3, 0, 0, 0);
  LED_Set(4, 3, 4, 0, 0, 0);

  //allumage du U face 4//
  LED_Set(0, 4, 4, r, g, b);
  LED_Set(0, 4, 3, r, g, b);
  LED_Set(0, 4, 2, r, g, b);
  LED_Set(0, 4, 1, r, g, b);
  LED_Set(0, 4, 0, r, g, b);
  LED_Set(1, 4, 0, r, g, b);
  LED_Set(2, 4, 0, r, g, b);
  LED_Set(3, 4, 0, r, g, b);
  LED_Set(4, 4, 0, r, g, b);
  LED_Set(4, 4, 0, r, g, b);
  LED_Set(4, 4, 1, r, g, b);
  LED_Set(4, 4, 2, r, g, b);
  LED_Set(4, 4, 3, r, g, b);
  LED_Set(4, 4, 4, r, g, b);
  delay(5);
  LED_Set(0, 4, 4, 0, 0, 0);
  LED_Set(0, 4, 3, 0, 0, 0);
  LED_Set(0, 4, 2, 0, 0, 0);
  LED_Set(0, 4, 1, 0, 0, 0);
  LED_Set(0, 4, 0, 0, 0, 0);
  LED_Set(1, 4, 0, 0, 0, 0);
  LED_Set(2, 4, 0, 0, 0, 0);
  LED_Set(3, 4, 0, 0, 0, 0);
  LED_Set(4, 4, 0, 0, 0, 0);
  LED_Set(4, 4, 0, 0, 0, 0);
  LED_Set(4, 4, 1, 0, 0, 0);
  LED_Set(4, 4, 2, 0, 0, 0);
  LED_Set(4, 4, 3, 0, 0, 0);
  LED_Set(4, 4, 4, 0, 0, 0);


  ////////////////////////////allumage du M face 0////////////////////////////
  LED_Set(0, 0, 0, r, g, b);
  LED_Set(0, 0, 1, r, g, b);
  LED_Set(0, 0, 2, r, g, b);
  LED_Set(0, 0, 3, r, g, b);
  LED_Set(0, 0, 4, r, g, b);
  LED_Set(1, 0, 3, r, g, b);
  LED_Set(2, 0, 2, r, g, b);
  LED_Set(3, 0, 3, r, g, b);
  LED_Set(4, 0, 4, r, g, b);
  LED_Set(4, 0, 3, r, g, b);
  LED_Set(4, 0, 2, r, g, b);
  LED_Set(4, 0, 1, r, g, b);
  LED_Set(4, 0, 0, r, g, b);
  delay(5);
  LED_Set(0, 0, 0, 0, 0, 0);
  LED_Set(0, 0, 1, 0, 0, 0);
  LED_Set(0, 0, 2, 0, 0, 0);
  LED_Set(0, 0, 3, 0, 0, 0);
  LED_Set(0, 0, 4, 0, 0, 0);
  LED_Set(1, 0, 3, 0, 0, 0);
  LED_Set(2, 0, 2, 0, 0, 0);
  LED_Set(3, 0, 3, 0, 0, 0);
  LED_Set(4, 0, 4, 0, 0, 0);
  LED_Set(4, 0, 3, 0, 0, 0);
  LED_Set(4, 0, 2, 0, 0, 0);
  LED_Set(4, 0, 1, 0, 0, 0);
  LED_Set(4, 0, 0, 0, 0, 0);
  
  //allumage du M face 1//
  LED_Set(0, 1, 0, r, g, b);
  LED_Set(0, 1, 1, r, g, b);
  LED_Set(0, 1, 2, r, g, b);
  LED_Set(0, 1, 3, r, g, b);
  LED_Set(0, 1, 4, r, g, b);
  LED_Set(1, 1, 3, r, g, b);
  LED_Set(2, 1, 2, r, g, b);
  LED_Set(3, 1, 3, r, g, b);
  LED_Set(4, 1, 4, r, g, b);
  LED_Set(4, 1, 3, r, g, b);
  LED_Set(4, 1, 2, r, g, b);
  LED_Set(4, 1, 1, r, g, b);
  LED_Set(4, 1, 0, r, g, b);
  delay(5);
  LED_Set(0, 1, 0, 0, 0, 0);
  LED_Set(0, 1, 1, 0, 0, 0);
  LED_Set(0, 1, 2, 0, 0, 0);
  LED_Set(0, 1, 3, 0, 0, 0);
  LED_Set(0, 1, 4, 0, 0, 0);
  LED_Set(1, 1, 3, 0, 0, 0);
  LED_Set(2, 1, 2, 0, 0, 0);
  LED_Set(3, 1, 3, 0, 0, 0);
  LED_Set(4, 1, 4, 0, 0, 0);
  LED_Set(4, 1, 3, 0, 0, 0);
  LED_Set(4, 1, 2, 0, 0, 0);
  LED_Set(4, 1, 1, 0, 0, 0);
  LED_Set(4, 1, 0, 0, 0, 0);

  //allumage du M face 2//
  LED_Set(0, 2, 0, r, g, b);
  LED_Set(0, 2, 1, r, g, b);
  LED_Set(0, 2, 2, r, g, b);
  LED_Set(0, 2, 3, r, g, b);
  LED_Set(0, 2, 4, r, g, b);
  LED_Set(1, 2, 3, r, g, b);
  LED_Set(2, 2, 2, r, g, b);
  LED_Set(3, 2, 3, r, g, b);
  LED_Set(4, 2, 4, r, g, b);
  LED_Set(4, 2, 3, r, g, b);
  LED_Set(4, 2, 2, r, g, b);
  LED_Set(4, 2, 1, r, g, b);
  LED_Set(4, 2, 0, r, g, b);
  delay(5);
  LED_Set(0, 2, 0, 0, 0, 0);
  LED_Set(0, 2, 1, 0, 0, 0);
  LED_Set(0, 2, 2, 0, 0, 0);
  LED_Set(0, 2, 3, 0, 0, 0);
  LED_Set(0, 2, 4, 0, 0, 0);
  LED_Set(1, 2, 3, 0, 0, 0);
  LED_Set(2, 2, 2, 0, 0, 0);
  LED_Set(3, 2, 3, 0, 0, 0);
  LED_Set(4, 2, 4, 0, 0, 0);
  LED_Set(4, 2, 3, 0, 0, 0);
  LED_Set(4, 2, 2, 0, 0, 0);
  LED_Set(4, 2, 1, 0, 0, 0);
  LED_Set(4, 2, 0, 0, 0, 0);

  //allumage du M face 2//
  LED_Set(0, 2, 0, r, g, b);
  LED_Set(0, 2, 1, r, g, b);
  LED_Set(0, 2, 2, r, g, b);
  LED_Set(0, 2, 3, r, g, b);
  LED_Set(0, 2, 4, r, g, b);
  LED_Set(1, 2, 3, r, g, b);
  LED_Set(2, 2, 2, r, g, b);
  LED_Set(3, 2, 3, r, g, b);
  LED_Set(4, 2, 4, r, g, b);
  LED_Set(4, 2, 3, r, g, b);
  LED_Set(4, 2, 2, r, g, b);
  LED_Set(4, 2, 1, r, g, b);
  LED_Set(4, 2, 0, r, g, b);
  delay(5);
  LED_Set(0, 2, 0, 0, 0, 0);
  LED_Set(0, 2, 1, 0, 0, 0);
  LED_Set(0, 2, 2, 0, 0, 0);
  LED_Set(0, 2, 3, 0, 0, 0);
  LED_Set(0, 2, 4, 0, 0, 0);
  LED_Set(1, 2, 3, 0, 0, 0);
  LED_Set(2, 2, 2, 0, 0, 0);
  LED_Set(3, 2, 3, 0, 0, 0);
  LED_Set(4, 2, 4, 0, 0, 0);
  LED_Set(4, 2, 3, 0, 0, 0);
  LED_Set(4, 2, 2, 0, 0, 0);
  LED_Set(4, 2, 1, 0, 0, 0);
  LED_Set(4, 2, 0, 0, 0, 0);

  //allumage du M face 3//
  LED_Set(0, 3, 0, r, g, b);
  LED_Set(0, 3, 1, r, g, b);
  LED_Set(0, 3, 2, r, g, b);
  LED_Set(0, 3, 3, r, g, b);
  LED_Set(0, 3, 4, r, g, b);
  LED_Set(1, 3, 3, r, g, b);
  LED_Set(2, 3, 2, r, g, b);
  LED_Set(3, 3, 3, r, g, b);
  LED_Set(4, 3, 4, r, g, b);
  LED_Set(4, 3, 3, r, g, b);
  LED_Set(4, 3, 2, r, g, b);
  LED_Set(4, 3, 1, r, g, b);
  LED_Set(4, 3, 0, r, g, b);
  delay(5);
  LED_Set(0, 3, 0, 0, 0, 0);
  LED_Set(0, 3, 1, 0, 0, 0);
  LED_Set(0, 3, 2, 0, 0, 0);
  LED_Set(0, 3, 3, 0, 0, 0);
  LED_Set(0, 3, 4, 0, 0, 0);
  LED_Set(1, 3, 3, 0, 0, 0);
  LED_Set(2, 3, 2, 0, 0, 0);
  LED_Set(3, 3, 3, 0, 0, 0);
  LED_Set(4, 3, 4, 0, 0, 0);
  LED_Set(4, 3, 3, 0, 0, 0);
  LED_Set(4, 3, 2, 0, 0, 0);
  LED_Set(4, 3, 1, 0, 0, 0);
  LED_Set(4, 3, 0, 0, 0, 0);

  //allumage du M face 4//
  LED_Set(0, 4, 0, r, g, b);
  LED_Set(0, 4, 1, r, g, b);
  LED_Set(0, 4, 2, r, g, b);
  LED_Set(0, 4, 3, r, g, b);
  LED_Set(0, 4, 4, r, g, b);
  LED_Set(1, 4, 3, r, g, b);
  LED_Set(2, 4, 2, r, g, b);
  LED_Set(3, 4, 3, r, g, b);
  LED_Set(4, 4, 4, r, g, b);
  LED_Set(4, 4, 3, r, g, b);
  LED_Set(4, 4, 2, r, g, b);
  LED_Set(4, 4, 1, r, g, b);
  LED_Set(4, 4, 0, r, g, b);
  delay(5);
  LED_Set(0, 4, 0, 0, 0, 0);
  LED_Set(0, 4, 1, 0, 0, 0);
  LED_Set(0, 4, 2, 0, 0, 0);
  LED_Set(0, 4, 3, 0, 0, 0);
  LED_Set(0, 4, 4, 0, 0, 0);
  LED_Set(1, 4, 3, 0, 0, 0);
  LED_Set(2, 4, 2, 0, 0, 0);
  LED_Set(3, 4, 3, 0, 0, 0);
  LED_Set(4, 4, 4, 0, 0, 0);
  LED_Set(4, 4, 3, 0, 0, 0);
  LED_Set(4, 4, 2, 0, 0, 0);
  LED_Set(4, 4, 1, 0, 0, 0);
  LED_Set(4, 4, 0, 0, 0, 0);


  ////////////////////////////allumage du O face 0////////////////////////////
  LED_Set(0, 0, 4, r, g, b);
  LED_Set(0, 0, 3, r, g, b);
  LED_Set(0, 0, 2, r, g, b);
  LED_Set(0, 0, 1, r, g, b);
  LED_Set(0, 0, 0, r, g, b);
  LED_Set(1, 0, 0, r, g, b);
  LED_Set(2, 0, 0, r, g, b);
  LED_Set(3, 0, 0, r, g, b);
  LED_Set(1, 0, 4, r, g, b);
  LED_Set(2, 0, 4, r, g, b);
  LED_Set(3, 0, 4, r, g, b);
  LED_Set(4, 0, 0, r, g, b);
  LED_Set(4, 0, 1, r, g, b);
  LED_Set(4, 0, 2, r, g, b);
  LED_Set(4, 0, 3, r, g, b);
  LED_Set(4, 0, 4, r, g, b);
  delay(5);
  LED_Set(0, 0, 4, 0, 0, 0);
  LED_Set(0, 0, 3, 0, 0, 0);
  LED_Set(0, 0, 2, 0, 0, 0);
  LED_Set(0, 0, 1, 0, 0, 0);
  LED_Set(0, 0, 0, 0, 0, 0);
  LED_Set(1, 0, 0, 0, 0, 0);
  LED_Set(2, 0, 0, 0, 0, 0);
  LED_Set(3, 0, 0, 0, 0, 0);
  LED_Set(1, 0, 4, 0, 0, 0);
  LED_Set(2, 0, 4, 0, 0, 0);
  LED_Set(3, 0, 4, 0, 0, 0);
  LED_Set(4, 0, 0, 0, 0, 0);
  LED_Set(4, 0, 1, 0, 0, 0);
  LED_Set(4, 0, 2, 0, 0, 0);
  LED_Set(4, 0, 3, 0, 0, 0);
  LED_Set(4, 0, 4, 0, 0, 0);

  //allumage du O face 1//
  LED_Set(0, 1, 4, r, g, b);
  LED_Set(0, 1, 3, r, g, b);
  LED_Set(0, 1, 2, r, g, b);
  LED_Set(0, 1, 1, r, g, b);
  LED_Set(0, 1, 0, r, g, b);
  LED_Set(1, 1, 0, r, g, b);
  LED_Set(2, 1, 0, r, g, b);
  LED_Set(3, 1, 0, r, g, b);
  LED_Set(1, 1, 4, r, g, b);
  LED_Set(2, 1, 4, r, g, b);
  LED_Set(3, 1, 4, r, g, b);
  LED_Set(4, 1, 0, r, g, b);
  LED_Set(4, 1, 1, r, g, b);
  LED_Set(4, 1, 2, r, g, b);
  LED_Set(4, 1, 3, r, g, b);
  LED_Set(4, 1, 4, r, g, b);
  delay(5);
  LED_Set(0, 1, 4, 0, 0, 0);
  LED_Set(0, 1, 3, 0, 0, 0);
  LED_Set(0, 1, 2, 0, 0, 0);
  LED_Set(0, 1, 1, 0, 0, 0);
  LED_Set(0, 1, 0, 0, 0, 0);
  LED_Set(1, 1, 0, 0, 0, 0);
  LED_Set(2, 1, 0, 0, 0, 0);
  LED_Set(3, 1, 0, 0, 0, 0);
  LED_Set(1, 1, 4, 0, 0, 0);
  LED_Set(2, 1, 4, 0, 0, 0);
  LED_Set(3, 1, 4, 0, 0, 0);
  LED_Set(4, 1, 0, 0, 0, 0);
  LED_Set(4, 1, 1, 0, 0, 0);
  LED_Set(4, 1, 2, 0, 0, 0);
  LED_Set(4, 1, 3, 0, 0, 0);
  LED_Set(4, 1, 4, 0, 0, 0);

  //allumage du O face 2//
  LED_Set(0, 2, 4, r, g, b);
  LED_Set(0, 2, 3, r, g, b);
  LED_Set(0, 2, 2, r, g, b);
  LED_Set(0, 2, 1, r, g, b);
  LED_Set(0, 2, 0, r, g, b);
  LED_Set(1, 2, 0, r, g, b);
  LED_Set(2, 2, 0, r, g, b);
  LED_Set(3, 2, 0, r, g, b);
  LED_Set(1, 2, 4, r, g, b);
  LED_Set(2, 2, 4, r, g, b);
  LED_Set(3, 2, 4, r, g, b);
  LED_Set(4, 2, 0, r, g, b);
  LED_Set(4, 2, 1, r, g, b);
  LED_Set(4, 2, 2, r, g, b);
  LED_Set(4, 2, 3, r, g, b);
  LED_Set(4, 2, 4, r, g, b);
  delay(5);
  LED_Set(0, 2, 4, 0, 0, 0);
  LED_Set(0, 2, 3, 0, 0, 0);
  LED_Set(0, 2, 2, 0, 0, 0);
  LED_Set(0, 2, 1, 0, 0, 0);
  LED_Set(0, 2, 0, 0, 0, 0);
  LED_Set(1, 2, 0, 0, 0, 0);
  LED_Set(2, 2, 0, 0, 0, 0);
  LED_Set(3, 2, 0, 0, 0, 0);
  LED_Set(1, 2, 4, 0, 0, 0);
  LED_Set(2, 2, 4, 0, 0, 0);
  LED_Set(3, 2, 4, 0, 0, 0);
  LED_Set(4, 2, 0, 0, 0, 0);
  LED_Set(4, 2, 1, 0, 0, 0);
  LED_Set(4, 2, 2, 0, 0, 0);
  LED_Set(4, 2, 3, 0, 0, 0);
  LED_Set(4, 2, 4, 0, 0, 0);

  //allumage du O face 3//
  LED_Set(0, 3, 4, r, g, b);
  LED_Set(0, 3, 3, r, g, b);
  LED_Set(0, 3, 2, r, g, b);
  LED_Set(0, 3, 1, r, g, b);
  LED_Set(0, 3, 0, r, g, b);
  LED_Set(1, 3, 0, r, g, b);
  LED_Set(2, 3, 0, r, g, b);
  LED_Set(3, 3, 0, r, g, b);
  LED_Set(1, 3, 4, r, g, b);
  LED_Set(2, 3, 4, r, g, b);
  LED_Set(3, 3, 4, r, g, b);
  LED_Set(4, 3, 0, r, g, b);
  LED_Set(4, 3, 1, r, g, b);
  LED_Set(4, 3, 2, r, g, b);
  LED_Set(4, 3, 3, r, g, b);
  LED_Set(4, 3, 4, r, g, b);
  delay(5);
  LED_Set(0, 3, 4, 0, 0, 0);
  LED_Set(0, 3, 3, 0, 0, 0);
  LED_Set(0, 3, 2, 0, 0, 0);
  LED_Set(0, 3, 1, 0, 0, 0);
  LED_Set(0, 3, 0, 0, 0, 0);
  LED_Set(1, 3, 0, 0, 0, 0);
  LED_Set(2, 3, 0, 0, 0, 0);
  LED_Set(3, 3, 0, 0, 0, 0);
  LED_Set(1, 3, 4, 0, 0, 0);
  LED_Set(2, 3, 4, 0, 0, 0);
  LED_Set(3, 3, 4, 0, 0, 0);
  LED_Set(4, 3, 0, 0, 0, 0);
  LED_Set(4, 3, 1, 0, 0, 0);
  LED_Set(4, 3, 2, 0, 0, 0);
  LED_Set(4, 3, 3, 0, 0, 0);
  LED_Set(4, 3, 4, 0, 0, 0);

  //allumage du O face 4//
  LED_Set(0, 4, 4, r, g, b);
  LED_Set(0, 4, 3, r, g, b);
  LED_Set(0, 4, 2, r, g, b);
  LED_Set(0, 4, 1, r, g, b);
  LED_Set(0, 4, 0, r, g, b);
  LED_Set(1, 4, 0, r, g, b);
  LED_Set(2, 4, 0, r, g, b);
  LED_Set(3, 4, 0, r, g, b);
  LED_Set(1, 4, 4, r, g, b);
  LED_Set(2, 4, 4, r, g, b);
  LED_Set(3, 4, 4, r, g, b);
  LED_Set(4, 4, 0, r, g, b);
  LED_Set(4, 4, 1, r, g, b);
  LED_Set(4, 4, 2, r, g, b);
  LED_Set(4, 4, 3, r, g, b);
  LED_Set(4, 4, 4, r, g, b);
  delay(5);
  LED_Set(0, 4, 4, 0, 0, 0);
  LED_Set(0, 4, 3, 0, 0, 0);
  LED_Set(0, 4, 2, 0, 0, 0);
  LED_Set(0, 4, 1, 0, 0, 0);
  LED_Set(0, 4, 0, 0, 0, 0);
  LED_Set(1, 4, 0, 0, 0, 0);
  LED_Set(2, 4, 0, 0, 0, 0);
  LED_Set(3, 4, 0, 0, 0, 0);
  LED_Set(1, 4, 4, 0, 0, 0);
  LED_Set(2, 4, 4, 0, 0, 0);
  LED_Set(3, 4, 4, 0, 0, 0);
  LED_Set(4, 4, 0, 0, 0, 0);
  LED_Set(4, 4, 1, 0, 0, 0);
  LED_Set(4, 4, 2, 0, 0, 0);
  LED_Set(4, 4, 3, 0, 0, 0);
  LED_Set(4, 4, 4, 0, 0, 0);


 ////////////////////////////allumage du S face 0////////////////////////////
 LED_Set(0, 0, 0, r, g, b);
 LED_Set(1, 0, 0, r, g, b);
 LED_Set(2, 0, 0, r, g, b);
 LED_Set(3, 0, 0, r, g, b);
 LED_Set(4, 0, 0, r, g, b);
 LED_Set(4, 0, 1, r, g, b);
 LED_Set(4, 0, 2, r, g, b);
 LED_Set(3, 0, 2, r, g, b);
 LED_Set(2, 0, 2, r, g, b);
 LED_Set(1, 0, 2, r, g, b);
 LED_Set(0, 0, 2, r, g, b);
 LED_Set(0, 0, 3 ,r ,g ,b);
 LED_Set(0, 0, 4, r, g, b);
 LED_Set(1, 0, 4, r, g, b);
 LED_Set(2, 0, 4, r, g, b);
 LED_Set(3, 0, 4, r, g, b);
 LED_Set(4, 0, 4, r, g, b);
 delay(5);
 LED_Set(0, 0, 0, 0, 0, 0);
 LED_Set(1, 0, 0, 0, 0, 0);
 LED_Set(2, 0, 0, 0, 0, 0);
 LED_Set(3, 0, 0, 0, 0, 0);
 LED_Set(4, 0, 0, 0, 0, 0);
 LED_Set(4, 0, 1, 0, 0, 0);
 LED_Set(4, 0, 2, 0, 0, 0);
 LED_Set(3, 0, 2, 0, 0, 0);
 LED_Set(2, 0, 2, 0, 0, 0);
 LED_Set(1, 0, 2, 0, 0, 0);
 LED_Set(0, 0, 2, 0, 0, 0);
 LED_Set(0, 0, 3, 0, 0, 0);
 LED_Set(0, 0, 4, 0, 0, 0);
 LED_Set(1, 0, 4, 0, 0, 0);
 LED_Set(2, 0, 4, 0, 0, 0);
 LED_Set(3, 0, 4, 0, 0, 0);
 LED_Set(4, 0, 4, 0, 0, 0);

 //allumage du S face 1//
 LED_Set(0, 1, 0, r, g, b);
 LED_Set(1, 1, 0, r, g, b);
 LED_Set(2, 1, 0, r, g, b);
 LED_Set(3, 1, 0, r, g, b);
 LED_Set(4, 1, 0, r, g, b);
 LED_Set(4, 1, 1, r, g, b);
 LED_Set(4, 1, 2, r, g, b);
 LED_Set(3, 1, 2, r, g, b);
 LED_Set(2, 1, 2, r, g, b);
 LED_Set(1, 1, 2, r, g, b);
 LED_Set(0, 1, 2, r, g, b);
 LED_Set(0, 1, 3 ,r ,g ,b);
 LED_Set(0, 1, 4, r, g, b);
 LED_Set(1, 1, 4, r, g, b);
 LED_Set(2, 1, 4, r, g, b);
 LED_Set(3, 1, 4, r, g, b);
 LED_Set(4, 1, 4, r, g, b);
 delay(5);
 LED_Set(0, 1, 0, 0, 0, 0);
 LED_Set(1, 1, 0, 0, 0, 0);
 LED_Set(2, 1, 0, 0, 0, 0);
 LED_Set(3, 1, 0, 0, 0, 0);
 LED_Set(4, 1, 0, 0, 0, 0);
 LED_Set(4, 1, 1, 0, 0, 0);
 LED_Set(4, 1, 2, 0, 0, 0);
 LED_Set(3, 1, 2, 0, 0, 0);
 LED_Set(2, 1, 2, 0, 0, 0);
 LED_Set(1, 1, 2, 0, 0, 0);
 LED_Set(0, 1, 2, 0, 0, 0);
 LED_Set(0, 1, 3, 0, 0, 0);
 LED_Set(0, 1, 4, 0, 0, 0);
 LED_Set(1, 1, 4, 0, 0, 0);
 LED_Set(2, 1, 4, 0, 0, 0);
 LED_Set(3, 1, 4, 0, 0, 0);
 LED_Set(4, 1, 4, 0, 0, 0);

 //allumage du S face 2//
 LED_Set(0, 2, 0, r, g, b);
 LED_Set(1, 2, 0, r, g, b);
 LED_Set(2, 2, 0, r, g, b);
 LED_Set(3, 2, 0, r, g, b);
 LED_Set(4, 2, 0, r, g, b);
 LED_Set(4, 2, 1, r, g, b);
 LED_Set(4, 2, 2, r, g, b);
 LED_Set(3, 2, 2, r, g, b);
 LED_Set(2, 2, 2, r, g, b);
 LED_Set(1, 2, 2, r, g, b);
 LED_Set(0, 2, 2, r, g, b);
 LED_Set(0, 2, 3 ,r ,g ,b);
 LED_Set(0, 2, 4, r, g, b);
 LED_Set(1, 2, 4, r, g, b);
 LED_Set(2, 2, 4, r, g, b);
 LED_Set(3, 2, 4, r, g, b);
 LED_Set(4, 2, 4, r, g, b);
 delay(5);
 LED_Set(0, 2, 0, 0, 0, 0);
 LED_Set(1, 2, 0, 0, 0, 0);
 LED_Set(2, 2, 0, 0, 0, 0);
 LED_Set(3, 2, 0, 0, 0, 0);
 LED_Set(4, 2, 0, 0, 0, 0);
 LED_Set(4, 2, 1, 0, 0, 0);
 LED_Set(4, 2, 2, 0, 0, 0);
 LED_Set(3, 2, 2, 0, 0, 0);
 LED_Set(2, 2, 2, 0, 0, 0);
 LED_Set(1, 2, 2, 0, 0, 0);
 LED_Set(0, 2, 2, 0, 0, 0);
 LED_Set(0, 2, 3, 0, 0, 0);
 LED_Set(0, 2, 4, 0, 0, 0);
 LED_Set(1, 2, 4, 0, 0, 0);
 LED_Set(2, 2, 4, 0, 0, 0);
 LED_Set(3, 2, 4, 0, 0, 0);
 LED_Set(4, 2, 4, 0, 0, 0);

 //allumage du S face 3//
 LED_Set(0, 3, 0, r, g, b);
 LED_Set(1, 3, 0, r, g, b);
 LED_Set(2, 3, 0, r, g, b);
 LED_Set(3, 3, 0, r, g, b);
 LED_Set(4, 3, 0, r, g, b);
 LED_Set(4, 3, 1, r, g, b);
 LED_Set(4, 3, 2, r, g, b);
 LED_Set(3, 3, 2, r, g, b);
 LED_Set(2, 3, 2, r, g, b);
 LED_Set(1, 3, 2, r, g, b);
 LED_Set(0, 3, 2, r, g, b);
 LED_Set(0, 3, 3 ,r ,g ,b);
 LED_Set(0, 3, 4, r, g, b);
 LED_Set(1, 3, 4, r, g, b);
 LED_Set(2, 3, 4, r, g, b);
 LED_Set(3, 3, 4, r, g, b);
 LED_Set(4, 3, 4, r, g, b);
 delay(5);
 LED_Set(0, 3, 0, 0, 0, 0);
 LED_Set(1, 3, 0, 0, 0, 0);
 LED_Set(2, 3, 0, 0, 0, 0);
 LED_Set(3, 3, 0, 0, 0, 0);
 LED_Set(4, 3, 0, 0, 0, 0);
 LED_Set(4, 3, 1, 0, 0, 0);
 LED_Set(4, 3, 2, 0, 0, 0);
 LED_Set(3, 3, 2, 0, 0, 0);
 LED_Set(2, 3, 2, 0, 0, 0);
 LED_Set(1, 3, 2, 0, 0, 0);
 LED_Set(0, 3, 2, 0, 0, 0);
 LED_Set(0, 3, 3, 0, 0, 0);
 LED_Set(0, 3, 4, 0, 0, 0);
 LED_Set(1, 3, 4, 0, 0, 0);
 LED_Set(2, 3, 4, 0, 0, 0);
 LED_Set(3, 3, 4, 0, 0, 0);
 LED_Set(4, 3, 4, 0, 0, 0);

 //allumage du S face 4//
 LED_Set(0, 4, 0, r, g, b);
 LED_Set(1, 4, 0, r, g, b);
 LED_Set(2, 4, 0, r, g, b);
 LED_Set(3, 4, 0, r, g, b);
 LED_Set(4, 4, 0, r, g, b);
 LED_Set(4, 4, 1, r, g, b);
 LED_Set(4, 4, 2, r, g, b);
 LED_Set(3, 4, 2, r, g, b);
 LED_Set(2, 4, 2, r, g, b);
 LED_Set(1, 4, 2, r, g, b);
 LED_Set(0, 4, 2, r, g, b);
 LED_Set(0, 4, 3 ,r ,g ,b);
 LED_Set(0, 4, 4, r, g, b);
 LED_Set(1, 4, 4, r, g, b);
 LED_Set(2, 4, 4, r, g, b);
 LED_Set(3, 4, 4, r, g, b);
 LED_Set(4, 4, 4, r, g, b);
 delay(5);
 LED_Set(0, 4, 0, 0, 0, 0);
 LED_Set(1, 4, 0, 0, 0, 0);
 LED_Set(2, 4, 0, 0, 0, 0);
 LED_Set(3, 4, 0, 0, 0, 0);
 LED_Set(4, 4, 0, 0, 0, 0);
 LED_Set(4, 4, 1, 0, 0, 0);
 LED_Set(4, 4, 2, 0, 0, 0);
 LED_Set(3, 4, 2, 0, 0, 0);
 LED_Set(2, 4, 2, 0, 0, 0);
 LED_Set(1, 4, 2, 0, 0, 0);
 LED_Set(0, 4, 2, 0, 0, 0);
 LED_Set(0, 4, 3, 0, 0, 0);
 LED_Set(0, 4, 4, 0, 0, 0);
 LED_Set(1, 4, 4, 0, 0, 0);
 LED_Set(2, 4, 4, 0, 0, 0);
 LED_Set(3, 4, 4, 0, 0, 0);
 LED_Set(4, 4, 4, 0, 0, 0);
}
*/


void diagonaletournante()   /// A DEBUG
{
  int x = 0;
  int y = 0;
  int z = 0;
  int cpt = 5;
  while (1)
  {
    cpt--;
    for (z = 0; z < size_of_cube; z++)
    {
      LED_Set(0, y, z, 0, 0, 255);
      LED_Set(size_of_cube - x, size_of_cube - y, z, 0, 0, 255);
      LED_Set(x, 0, z, 0, 0, 255);
      if (cpt == 0)
      {
        LED_Set(0, y, z, 0, 0, 0);
        LED_Set(size_of_cube - x, size_of_cube - y, z, 0, 0, 0);
        LED_Set(x, 0, z, 0, 0, 0);
        cpt = 5;
      }
      if (x < size_of_cube - 1)
        x++;
      else
      {
        y++;
        if (y == size_of_cube - 1)
          x = 0;
      }
    }
  }
}


void gouttetombante()   /// Pas terminé
{
  while (1)
  {
    int x = 0;
    int y = 0;
    int i = 0;
    int j = 0;
    int z = size_of_cube;
    int cpt = 5;
    int pred[size_of_cube * size_of_cube];
    for (i = 0; i < size_of_cube * size_of_cube; i++)
    {
      pred[i] = 0;
    }
    i = 0;
    cpt--;
    for (x = 0; x < size_of_cube; x++)
    {
      for (y = 0; y < size_of_cube; y++)
      {
        LED_Set(x, y, z, 0, 0, 255);

      }
      while (i < size_of_cube * size_of_cube - 1)
      {
        if (pred[i] == x && pred[i + 1] == y)
        {
          x = getRandom(size_of_cube - 1);
          y = getRandom(size_of_cube - 1);
          i = 0;
        }
        i += 2;
      }
      if (j < size_of_cube * size_of_cube - 1)
      { pred[j] = x;
        pred[j + 1] = y;
        j += 2;
      }
      else
      {
        j = 0;
        for (i = 0; i < size_of_cube * size_of_cube; i++)
          pred[i] = 0;
      }

      while (z > 0)
      {
        if (cpt == 0)
        { LED_Set(x, y, z, 0, 0, 0);
          cpt = 5;
          z--;
        }
        LED_Set(x, y, z, 0, 0, 255);

      }
    }
  }
}

void truc_tournant(int r,int g,int b,int sens)
{
  LED_Set(1, 1, 0, r, g, b);
  LED_Set(1, 1, 1, r, g, b);
  LED_Set(1, 1, 2, r, g, b);

  if(sens==0)
  {
  LED_Set(1, 0, 0, r, g, b);
  LED_Set(1, 0, 1, r, g, b);
  LED_Set(1, 0, 2, r, g, b);
  LED_Set(1, 2, 0, r, g, b);
  LED_Set(1, 2, 1, r, g, b);
  LED_Set(1, 2, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(1, 0, 0, 0, 0, 0);
  LED_Set(1, 0, 1, 0, 0, 0);
  LED_Set(1, 0, 2, 0, 0, 0);
  LED_Set(1, 2, 0, 0, 0, 0);
  LED_Set(1, 2, 1, 0, 0, 0);
  LED_Set(1, 2, 2, 0, 0, 0);


  LED_Set(2, 0, 0, r, g, b);
  LED_Set(2, 0, 1, r, g, b);
  LED_Set(2, 0, 2, r, g, b);
  LED_Set(0, 2, 0, r, g, b);
  LED_Set(0, 2, 1, r, g, b);
  LED_Set(0, 2, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(2, 0, 0, 0, 0, 0);
  LED_Set(2, 0, 1, 0, 0, 0);
  LED_Set(2, 0, 2, 0, 0, 0);
  LED_Set(0, 2, 0, 0, 0, 0);
  LED_Set(0, 2, 1, 0, 0, 0);
  LED_Set(0, 2, 2, 0, 0, 0);


  LED_Set(0, 1, 0, r, g, b);
  LED_Set(0, 1, 1, r, g, b);
  LED_Set(0, 1, 2, r, g, b);
  LED_Set(2, 1, 0, r, g, b);
  LED_Set(2, 1, 1, r, g, b);
  LED_Set(2, 1, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(0, 1, 0, 0, 0, 0);
  LED_Set(0, 1, 1, 0, 0, 0);
  LED_Set(0, 1, 2, 0, 0, 0);
  LED_Set(2, 1, 0, 0, 0, 0);
  LED_Set(2, 1, 1, 0, 0, 0);
  LED_Set(2, 1, 2, 0, 0, 0);


  LED_Set(0, 0, 0, r, g, b);
  LED_Set(0, 0, 1, r, g, b);
  LED_Set(0, 0, 2, r, g, b);
  LED_Set(2, 2, 0, r, g, b);
  LED_Set(2, 2, 1, r, g, b);
  LED_Set(2, 2, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(0, 0, 0, 0, 0, 0);
  LED_Set(0, 0, 1, 0, 0, 0);
  LED_Set(0, 0, 2, 0, 0, 0);
  LED_Set(2, 2, 0, 0, 0, 0);
  LED_Set(2, 2, 1, 0, 0, 0);
  LED_Set(2, 2, 2, 0, 0, 0); 
  }

  if(sens==1)
  {
  LED_Set(1, 0, 0, r, g, b);
  LED_Set(1, 0, 1, r, g, b);
  LED_Set(1, 0, 2, r, g, b);
  LED_Set(1, 2, 0, r, g, b);
  LED_Set(1, 2, 1, r, g, b);
  LED_Set(1, 2, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(1, 0, 0, 0, 0, 0);
  LED_Set(1, 0, 1, 0, 0, 0);
  LED_Set(1, 0, 2, 0, 0, 0);
  LED_Set(1, 2, 0, 0, 0, 0);
  LED_Set(1, 2, 1, 0, 0, 0);
  LED_Set(1, 2, 2, 0, 0, 0);

  LED_Set(0, 0, 0, r, g, b);
  LED_Set(0, 0, 1, r, g, b);
  LED_Set(0, 0, 2, r, g, b);
  LED_Set(2, 2, 0, r, g, b);
  LED_Set(2, 2, 1, r, g, b);
  LED_Set(2, 2, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(0, 0, 0, 0, 0, 0);
  LED_Set(0, 0, 1, 0, 0, 0);
  LED_Set(0, 0, 2, 0, 0, 0);
  LED_Set(2, 2, 0, 0, 0, 0);
  LED_Set(2, 2, 1, 0, 0, 0);
  LED_Set(2, 2, 2, 0, 0, 0); 

  LED_Set(0, 1, 0, r, g, b);
  LED_Set(0, 1, 1, r, g, b);
  LED_Set(0, 1, 2, r, g, b);
  LED_Set(2, 1, 0, r, g, b);
  LED_Set(2, 1, 1, r, g, b);
  LED_Set(2, 1, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(0, 1, 0, 0, 0, 0);
  LED_Set(0, 1, 1, 0, 0, 0);
  LED_Set(0, 1, 2, 0, 0, 0);
  LED_Set(2, 1, 0, 0, 0, 0);
  LED_Set(2, 1, 1, 0, 0, 0);
  LED_Set(2, 1, 2, 0, 0, 0);

  LED_Set(2, 0, 0, r, g, b);
  LED_Set(2, 0, 1, r, g, b);
  LED_Set(2, 0, 2, r, g, b);
  LED_Set(0, 2, 0, r, g, b);
  LED_Set(0, 2, 1, r, g, b);
  LED_Set(0, 2, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(2, 0, 0, 0, 0, 0);
  LED_Set(2, 0, 1, 0, 0, 0);
  LED_Set(2, 0, 2, 0, 0, 0);
  LED_Set(0, 2, 0, 0, 0, 0);
  LED_Set(0, 2, 1, 0, 0, 0);
  LED_Set(0, 2, 2, 0, 0, 0);  
  }
}

void truc_style()
{

  LED_Set(0, 0, 2, 0, 0, 255);
  LED_Set(2, 0, 2, 0, 0, 255);
  LED_Set(2, 2, 2, 0, 0, 255);
  LED_Set(0, 2, 2, 0, 0, 255);
  LED_Set(1, 0, 1, 0, 0, 255);
  LED_Set(2, 1, 1, 0, 0, 255);
  LED_Set(1, 2, 1, 0, 0, 255);
  LED_Set(0, 1, 1, 0, 0, 255);
  LED_Set(0, 0, 0, 0, 0, 255);
  LED_Set(2, 0, 0, 0, 0, 255);
  LED_Set(2, 2, 0, 0, 0, 255);
  LED_Set(0, 2, 0, 0, 0, 255);
  delayMicroseconds(25);
  LED_Set(0, 0, 2, 0, 0, 0);
  LED_Set(2, 0, 2, 0, 0, 0);
  LED_Set(2, 2, 2, 0, 0, 0);
  LED_Set(0, 2, 2, 0, 0, 0);
  LED_Set(1, 0, 1, 0, 0, 0);
  LED_Set(2, 1, 1, 0, 0, 0);
  LED_Set(1, 2, 1, 0, 0, 0);
  LED_Set(0, 1, 1, 0, 0, 0);
  LED_Set(0, 0, 0, 0, 0, 0);
  LED_Set(2, 0, 0, 0, 0, 0);
  LED_Set(2, 2, 0, 0, 0, 0);
  LED_Set(0, 2, 0, 0, 0, 0);

  LED_Set(1, 0, 2, 0, 0, 255);
  LED_Set(2, 1, 2, 0, 0, 255);
  LED_Set(1, 2, 2, 0, 0, 255);
  LED_Set(0, 1, 2, 0, 0, 255);
  LED_Set(1, 0, 0, 0, 0, 255);
  LED_Set(2, 1, 0, 0, 0, 255);
  LED_Set(1, 2, 0, 0, 0, 255);
  LED_Set(0, 1, 0, 0, 0, 255);
  delayMicroseconds(25);
  LED_Set(1, 0, 2, 0, 0, 0);
  LED_Set(2, 1, 2, 0, 0, 0);
  LED_Set(1, 2, 2, 0, 0, 0);
  LED_Set(0, 1, 2, 0, 0, 0);
  LED_Set(0, 0, 1, 0, 0, 0);
  LED_Set(2, 0, 1, 0, 0, 0);
  LED_Set(2, 2, 1, 0, 0, 0);
  LED_Set(0, 2, 1, 0, 0, 0);
  LED_Set(1, 0, 0, 0, 0, 0);
  LED_Set(2, 1, 0, 0, 0, 0);
  LED_Set(1, 2, 0, 0, 0, 0);
  LED_Set(0, 1, 0, 0, 0, 0);
  
}

void truc_tournant_avecdemitour(int r,int g,int b)
{
  LED_Set(1, 1, 0, r, g, b);
  LED_Set(1, 1, 1, r, g, b);
  LED_Set(1, 1, 2, r, g, b);
  int compteur=0;

  if(compteur%2==0)
  {
  LED_Set(1, 0, 0, r, g, b);
  LED_Set(1, 0, 1, r, g, b);
  LED_Set(1, 0, 2, r, g, b);
  LED_Set(1, 2, 0, r, g, b);
  LED_Set(1, 2, 1, r, g, b);
  LED_Set(1, 2, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(1, 0, 0, 0, 0, 0);
  LED_Set(1, 0, 1, 0, 0, 0);
  LED_Set(1, 0, 2, 0, 0, 0);
  LED_Set(1, 2, 0, 0, 0, 0);
  LED_Set(1, 2, 1, 0, 0, 0);
  LED_Set(1, 2, 2, 0, 0, 0);


  LED_Set(2, 0, 0, r, g, b);
  LED_Set(2, 0, 1, r, g, b);
  LED_Set(2, 0, 2, r, g, b);
  LED_Set(0, 2, 0, r, g, b);
  LED_Set(0, 2, 1, r, g, b);
  LED_Set(0, 2, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(2, 0, 0, 0, 0, 0);
  LED_Set(2, 0, 1, 0, 0, 0);
  LED_Set(2, 0, 2, 0, 0, 0);
  LED_Set(0, 2, 0, 0, 0, 0);
  LED_Set(0, 2, 1, 0, 0, 0);
  LED_Set(0, 2, 2, 0, 0, 0);


  LED_Set(0, 1, 0, r, g, b);
  LED_Set(0, 1, 1, r, g, b);
  LED_Set(0, 1, 2, r, g, b);
  LED_Set(2, 1, 0, r, g, b);
  LED_Set(2, 1, 1, r, g, b);
  LED_Set(2, 1, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(0, 1, 0, 0, 0, 0);
  LED_Set(0, 1, 1, 0, 0, 0);
  LED_Set(0, 1, 2, 0, 0, 0);
  LED_Set(2, 1, 0, 0, 0, 0);
  LED_Set(2, 1, 1, 0, 0, 0);
  LED_Set(2, 1, 2, 0, 0, 0);


  LED_Set(0, 0, 0, r, g, b);
  LED_Set(0, 0, 1, r, g, b);
  LED_Set(0, 0, 2, r, g, b);
  LED_Set(2, 2, 0, r, g, b);
  LED_Set(2, 2, 1, r, g, b);
  LED_Set(2, 2, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(0, 0, 0, 0, 0, 0);
  LED_Set(0, 0, 1, 0, 0, 0);
  LED_Set(0, 0, 2, 0, 0, 0);
  LED_Set(2, 2, 0, 0, 0, 0);
  LED_Set(2, 2, 1, 0, 0, 0);
  LED_Set(2, 2, 2, 0, 0, 0); 

  LED_Set(1, 0, 0, r, g, b);
  LED_Set(1, 0, 1, r, g, b);
  LED_Set(1, 0, 2, r, g, b);
  LED_Set(1, 2, 0, r, g, b);
  LED_Set(1, 2, 1, r, g, b);
  LED_Set(1, 2, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(1, 0, 0, 0, 0, 0);
  LED_Set(1, 0, 1, 0, 0, 0);
  LED_Set(1, 0, 2, 0, 0, 0);
  LED_Set(1, 2, 0, 0, 0, 0);
  LED_Set(1, 2, 1, 0, 0, 0);
  LED_Set(1, 2, 2, 0, 0, 0);

  compteur++;
  }

  if(compteur%2==1)
  {
  LED_Set(0, 0, 0, r, g, b);
  LED_Set(0, 0, 1, r, g, b);
  LED_Set(0, 0, 2, r, g, b);
  LED_Set(2, 2, 0, r, g, b);
  LED_Set(2, 2, 1, r, g, b);
  LED_Set(2, 2, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(0, 0, 0, 0, 0, 0);
  LED_Set(0, 0, 1, 0, 0, 0);
  LED_Set(0, 0, 2, 0, 0, 0);
  LED_Set(2, 2, 0, 0, 0, 0);
  LED_Set(2, 2, 1, 0, 0, 0);
  LED_Set(2, 2, 2, 0, 0, 0); 

  LED_Set(0, 1, 0, r, g, b);
  LED_Set(0, 1, 1, r, g, b);
  LED_Set(0, 1, 2, r, g, b);
  LED_Set(2, 1, 0, r, g, b);
  LED_Set(2, 1, 1, r, g, b);
  LED_Set(2, 1, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(0, 1, 0, 0, 0, 0);
  LED_Set(0, 1, 1, 0, 0, 0);
  LED_Set(0, 1, 2, 0, 0, 0);
  LED_Set(2, 1, 0, 0, 0, 0);
  LED_Set(2, 1, 1, 0, 0, 0);
  LED_Set(2, 1, 2, 0, 0, 0);

  LED_Set(2, 0, 0, r, g, b);
  LED_Set(2, 0, 1, r, g, b);
  LED_Set(2, 0, 2, r, g, b);
  LED_Set(0, 2, 0, r, g, b);
  LED_Set(0, 2, 1, r, g, b);
  LED_Set(0, 2, 2, r, g, b);
  delayMicroseconds(25);
  LED_Set(2, 0, 0, 0, 0, 0);
  LED_Set(2, 0, 1, 0, 0, 0);
  LED_Set(2, 0, 2, 0, 0, 0);
  LED_Set(0, 2, 0, 0, 0, 0);
  LED_Set(0, 2, 1, 0, 0, 0);
  LED_Set(0, 2, 2, 0, 0, 0);

  compteur++;
  }
}
