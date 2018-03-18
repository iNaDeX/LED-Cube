#define TAILLE_SNAKE 3
#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3
#define FORWARD 4
#define BACKWARD 5
#define X 0
#define Y 1
#define Z 2

int r_snake = 0, g_snake = 0, b_snake = 255;
int r_pomme = 0, g_pomme = 255, b_pomme = 0;
int snake_posx = 1;
int snake_posy = 0;
int snake_posz = 1;
int pomme_x;
int pomme_y;
int pomme_z;
int pomme_active=1;
int dir = -1;
int score=0;
int snake[TAILLE_SNAKE][3];
int cpt = 0;

void snakey()
{
  while (1)
  {
    cpt++;
    ps2x.read_gamepad(); //read controller

    if (cpt == 100)
    {
      cpt = 0;
      //LED_Set(snake_posx, snake_posy, snake_posz, 0, 0, 0);
      LED_Set(snake[TAILLE_SNAKE-1][X], snake[TAILLE_SNAKE-1][Y], snake[TAILLE_SNAKE-1][Z], 0, 0, 0);
      
      
      for(int i=TAILLE_SNAKE-1;i>0;i--)
      {
        for(int j=0;j<3;j++)
        {
          snake[i][j]=snake[i-1][j];
        }
      }
      
      switch (dir)
      {
      case UP :
        //snake_posz++;
        snake[0][Z]++;
        break;

      case DOWN :
       // snake_posz--;
       snake[0][Z]--;
        break;

      case LEFT :
        //snake_posx--;
        snake[0][X]--;
        break;

      case RIGHT:
        //snake_posx++;
        snake[0][X]++;
        break;

      case FORWARD :
        //snake_posy++;
        snake[0][Y]++;
        break;

      case BACKWARD :
        //snake_posy--;
        snake[0][Y]--;
        break;

      default:
        ;
      }

    }
/*
    if (snake_posy < 0)
      snake_posy = size_of_cube - 1;
    else if (snake_posy >= size_of_cube)
      snake_posy = 0;

    if (snake_posx < 0)
      snake_posx = size_of_cube - 1;
    else if (snake_posx >= size_of_cube)
      snake_posx = 0;

    if (snake_posz < 0)
      snake_posz = size_of_cube - 1;
    else if (snake_posz >= size_of_cube)
      snake_posz = 0;
*/


    if (snake[0][X] < 0)
      snake[0][X] = size_of_cube - 1;
    else if (snake[0][X] >= size_of_cube)
      snake[0][X] = 0;

    if (snake[0][Y] < 0)
      snake[0][Y] = size_of_cube - 1;
    else if (snake[0][Y] >= size_of_cube)
      snake[0][Y] = 0;

    if (snake[0][Z] < 0)
      snake[0][Z] = size_of_cube - 1;
    else if (snake[0][Z] >= size_of_cube)
      snake[0][Z] = 0;

    
    if (!pomme_active)
    {
      pomme_x = getRandom(size_of_cube);
      pomme_y = getRandom(size_of_cube);
      pomme_z = getRandom(size_of_cube);
      pomme_active=1;
    }
    else 
    {
      LED_Set(pomme_x,pomme_y,pomme_z,r_pomme,g_pomme,b_pomme);
      if (pomme_x==snake[0][X] && pomme_y==snake[0][Y] && pomme_z==snake[0][Z])
        {
          pomme_active=0;
          score++;
        }
    }


    if (ps2x.ButtonPressed(PSB_PAD_RIGHT))
    {
      if (dir!=LEFT)
        dir = RIGHT;
    }
    else if (ps2x.ButtonPressed(PSB_PAD_LEFT))
    {
      if (dir!=RIGHT)
        dir = LEFT;
    }
    else if (ps2x.ButtonPressed(PSB_PAD_UP))
    {
      if (dir!=BACKWARD)
        dir=FORWARD;
    }
    else if (ps2x.ButtonPressed(PSB_PAD_DOWN))
    {
      if (dir!=FORWARD)
        dir=BACKWARD;
    }
    else if (ps2x.ButtonPressed(PSB_GREEN))
    {
      if (dir!=DOWN)
        dir=UP;
    }
    else if (ps2x.ButtonPressed(PSB_BLUE))
    {
      if (dir!=UP)
        dir=DOWN;
    }
    LED_Set(snake[0][X], snake[0][Y], snake[0][Z], r_snake, g_snake, b_snake);
  }
}

void test_manette()
{
  while (1)
  {
    ps2x.read_gamepad(); //read controller

    if (ps2x.ButtonPressed(PSB_PAD_RIGHT))
    {
      LED_Set(2, 0, 1, 0, 0, 255);
      //Serial.println("right");
    }
    else if (ps2x.ButtonPressed(PSB_PAD_LEFT))
    {
      LED_Set(0, 0, 1, 0, 0, 255);
      //Serial.println("left");
    }
    else if (ps2x.ButtonPressed(PSB_PAD_UP))
    {
      LED_Set(1, 0, 2, 0, 0, 255);
      //Serial.println("up");
    }
    else if (ps2x.ButtonPressed(PSB_PAD_DOWN))
    {
      LED_Set(1, 0, 0, 0, 0, 255);
      //Serial.println("down");
    }
    LED_Set(1, 0, 1, 255, 0, 0);
  }
}

void manette_initialiser()
{
  byte type;
  manette_error_code = ps2x.config_gamepad(8, 6, 7, 12); //setup pins and settings:  GamePad(clock, command, attention, data). Check for error

  if (manette_error_code == 0)
  {
    Serial.println("Controller OK");
  }
  else if (manette_error_code == 1)
    Serial.println("No controller");

  else if (manette_error_code == 2)
    Serial.println("Controller bug1");

  else if (manette_error_code == 3)
    Serial.println("Controller bug2");

  type = ps2x.readType();
  switch (type)
  {
    case 1:
      Serial.println("DualShock Controller");
      break;
    default:
      Serial.println("Unknown Controller");
      break;
  }
}

