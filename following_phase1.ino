#include<SoftwareSerial.h>

SoftwareSerial Bluetooth(0, 1);
#define N 1
#define W 2
#define S 3
#define E 4


int r = A0, c = A3, l = A5;//wrt bot
int m1r = 5, m2r = 6, m3 = 7, m4 = 8;
int rv, cv, lv, t;

int botx = 2, boty = -1, nodex, nodey;
void go(int nodex, int nodey);
void forward();
void stop();
void line(int rv, int cv, int lv);
int led = 9;
int r1, c1, l1;
int flag = 0;
String data;
int dir = N;
void setup() {
 // setting up the motor pin modes anfd the bluetooth communication 
 //Get the x and y coordinate from the user in the serail monitor and pass it to the robot from the laptop
 //via bluetooth
  pinMode(m1r, OUTPUT);
  pinMode(m2r, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
  pinMode(r, INPUT);//s1
  pinMode(c, INPUT);//s2
  pinMode(l, INPUT);//s3
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Bluetooth.begin(9600);
  while (Bluetooth.available())//checks if bluetooth module is connected
  {

    data = Bluetooth.read();//reads the serail port if any input has been received
   
    if (flag % 2 == 0)
    {
      Serial.println("X:");
      nodex = data.toInt();
    }
    else if (flag % 2 == 1)
    {
      Serial.println("Y:");
      nodey = data.toInt();
    }
    else if (flag % 3 == 0)
    {
      Serial.println("Pass it(y/n):");
      char choice = Serial.read();
      if (choice == 'y')
        go(nodex, nodey);
    }
  }

}
void loop()
{
  //the sensor 
  r1 = analogRead(r);
  c1 = analogRead(c);
  l1 = analogRead(l);
  // change code to take analog values after trials

  Serial.println(r1);
  Serial.println(c1);
  Serial.println(l1);
  int cs = 990, ls = 990, rs = 983; // values for sensor output after calibrating it with the surface
  if (r1 < rs) rv = 1; else if (r1 > rs) rv = 0;
  Serial.println("right value:");
  Serial.println(rv);
  if (c1 < cs) cv = 1; else if (c1 > cs) cv = 0;
  Serial.println("center value:");
  Serial.println(cv);
  if (l1 < ls) lv = 1; else if (l1 > ls) lv = 0;
  Serial.println("left value:");
  Serial.println(lv );
  Serial.println("HI2");
  delay(500);
  if ((rv == 0) && (cv == 1) && (lv == 0)) // different conditions of sensor and their outputs
    forward();
  else if ((rv == 1) && (cv == 1) && (lv == 0))
    right();
  else if ((rv == 0) && (cv == 1) && (lv == 1))
    left(); 
  else if ((rv == 1) && (cv == 1) && (lv == 1))
    node();
  
  go(1, 3);
}

// functions for robot movements
void forward()
{
  digitalWrite(m1r, 1);
  digitalWrite(m2r, 0);//right forward
  digitalWrite(m3, 1);
  digitalWrite(m4, 0);
  Serial.println("FORWARD");

}
void right()
{
  digitalWrite(m1r, 1);
  digitalWrite(m2r, 0);
  digitalWrite(m3, 0);//left reverse
  digitalWrite(m4, 1);
  Serial.println("RIGHT");
}
void left()
{
  digitalWrite(m1r, 0);
  digitalWrite(m2r, 1);
  digitalWrite(m3, 1);
  digitalWrite(m4, 0);
  Serial.println("LEFT");
}
void stop()
{

  digitalWrite(m1r, 0);
  digitalWrite(m2r, 0);
  digitalWrite(m3, 0);
  digitalWrite(m4, 0);
  Serial.println("STOP");
}

void right90()
{
  forward();
  delay(10);
  stop();
  right();
  delay(10);
  while (cv == 0)
  { right();
    Serial.println("GOT");
  }

  if (dir == N)
    dir = E;
  else if (dir == W)
    dir = S;
  else if (dir == S)
    dir = E;
  else if (dir == E)
    dir = N;

  Serial.println("RIGHT90");
  Serial.println("current");
  Serial.println(dir);

}

void left90()
{
  forward();
  delay(10);
  stop();
  left();
  delay(10);
  while (cv == 0)
  {
    left();
  }
  if (dir == N)
    dir = W;
  else if (dir == W)
    dir = S;
  else if (dir == S)
    dir = E;
  else if (dir == E)
    dir = N;
  Serial.println("LEFT90");
}

void uturn()
{
  forward();
  delay(10);
  stop();
A: right();
  delay(10);
  while (cv == 0)
  {
    right();
  }
  goto A;
  if (dir == N)
    dir = S;
  else if (dir == W)
    dir = E;
  else if (dir == S)
    dir = N;
  else if (dir == E)
    dir = W;
  Serial.println("UTURN");
}

void node()  // if a node is detected in the track
{
  Serial.println("NODE DETECTED");
  Serial.println(dir);
  if (dir == N)
  {
    boty++;

  }
  else if (dir == W)
  {
    botx--;
  }
  else if (dir == S)
  {
    boty--;
  }
  else if (dir == E)
  {
    botx++;
  }
  delay(1000);
  Serial.println(dir);
  Serial.println("Y");
  Serial.println(boty);
  Serial.println("X");
  Serial.println(botx);
  delay(1000);
}


void travel()
{
  forward();
  delay(10);
  Serial.println("right value:");
  Serial.println(rv);
  Serial.println("center value:");
  Serial.println(cv);
  Serial.println("left value:");
  Serial.println(lv);
  if ((rv == 0) && (cv == 1) && (lv == 0))
    forward();
  else if ((rv == 1) && (cv == 1) && (lv == 0))
    right();
  else if ((rv == 0) && (cv == 1) && (lv == 1))
    left();
  else if ((rv == 1) && (cv == 1) && (lv == 1))
    node();
  delay(500);
}

void gostart()
{
  go(2, -1);
  uturn();
  stop();
  delay(50000);
}


void go(int nodex, int nodey) //given the final node this gives path instructions to follow 
{

  Serial.println("BOT");
  Serial.println(nodex);
  Serial.println(nodey);
  while ((botx - nodex) != 0 || (boty - nodey) != 0 )
  {
    while (boty < nodey)
    {
      if (dir == S)
        uturn();
      else if (dir == E)
        left90();
      else if (dir == W)
        right90();
      Serial.println("DFGDF");
      travel();
    }
    while (botx < nodex)
    {
      if (dir == N)
        right90();
      else if (dir == W)
        uturn();
      else if (dir == S)
        left90();

      travel();
    }

    while (boty > nodey)
    {
      if (dir == N)
        uturn();
      else if (dir == E)
        right90();
      else if (dir == W)
        left90();

      travel();
    }

    while (botx > nodex)
    {
      if (dir == S)
        right90();
      else if (dir == N)
        left90();
      else if (dir == W)
        uturn();

      travel();
    }
  }
  stop();
  Serial.println("DONE");
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  gostart();

  //phasse 2 go(nodex,nodey);
}
