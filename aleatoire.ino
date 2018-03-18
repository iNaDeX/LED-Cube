////// Mini-bibliothèque d'aléatoire pour remplacer random() de l'arduino qui est trop lent

// http://forum.arduino.cc/index.php?topic=246809.0
byte getRand()
{
  m_z = 36969L * (m_z & 65535L) + (m_z >> 16);
  m_w = 18000L * (m_w & 65535L) + (m_w >> 16);
  return ((m_z << 16) + m_w)%256;
} 

void random_table_create(byte random_table[RANDOM_TABLE_SIZE])
{
  for(int i=0; i<RANDOM_TABLE_SIZE;i++)
  {
    random_table[i] = getRand() % 256; // on crée des nombres aléatoires entre 0 et 255 inclus
  } 
}

// utilise une table de nombres random pré-calculés
byte getRandom(int nb_max)
{
  random_table_index++;
  if(random_table_index > RANDOM_TABLE_SIZE)
    random_table_index=0;
  return  random_table[random_table_index]%nb_max;
}
