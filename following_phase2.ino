#include<SoftwareSerial.h>
#include<TimerOne.h>

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
int data;
int dir = N;
void setup() {

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
  Timer1.initialize(1000);
  Timer1.attachInterrupt(check);
}

void loop()
{
  
   
  if(Bluetooth.available()>1)
  {
    data=Bluetooth.parseInt();
    int buff=Bluetooth.parseInt();
    Serial.println(data);
    nodex=data/10;
    nodey=data%10;
    Serial.println(nodex);
    Serial.println(nodey);
    if(data==5)
     {flag=1;
      go(2,-1);
     
     }
    else{
      
    nodex=data/10;
    nodey=data%10;
    flag=1;
    go(nodex,nodey);}
    
  }
  
 
flag=o;
}


void check()
{
  r1 = analogRead(r);
  c1 = analogRead(c);
  l1 = analogRead(l);
  int cs = 1011, ls = 1010, rs = 1010;
  if (r1 < rs) rv = 1; else if (r1 > rs) rv = 0;
  if (c1 < cs) cv = 1; else if (c1 > cs) cv = 0;
  if (l1 < ls) lv = 1; else if (l1 > ls) lv = 0;
  //if(flag==1)
  {
  if ((rv == 0) && (cv == 1) && (lv == 0))
    forward();
  else if ((rv == 1) && (cv == 1) && (lv == 0))
    left(); 
  else if ((rv == 0) && (cv == 1) && (lv == 1))
    right();
  }

  
}

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
  delay(500);
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
  delay(500);
  stop();
  delay(500);
  left();
  delay(500);
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
int count=0;
int u=0;
void uturn()
{
  forward();
  delay(100);
  stop();
  while(u<2)
  {right();
  delay(100);
  while (cv == 0)
  {
    right();
   
  }
  }
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

void node()
{
 {
  Serial.println("NODE DETECTED");
  delay(10);
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

}

void gostart()
{
  go(2, -1);
  uturn();
  stop();
  delay(50000);
}
void ble()
{
   if(Bluetooth.available()>0)
  {
    data=Bluetooth.parseInt();
    nodex=data/10;
    nodey=data%10;
    flag=1;
    go(nodex,nodey);
  }
  
 flag=0;
}

void go(int nodex, int nodey)
{

  Serial.println("BOT");
  Serial.println(nodex);
  Serial.println(nodey);
  while (((botx - nodex) != 0 || (boty - nodey) != 0 ) && flag==1)
  {
    while (boty < nodey)
    {
      if (dir == S)
        uturn();
      else if (dir == E)
        left90();
      else if (dir == W)
        right90();

      
      if(rv==1 && cv==1 && lv==1)
      node();
     
    }
    while (botx < nodex)
    {
      if (dir == N)
        right90();
      else if (dir == W)
        uturn();
      else if (dir == S)
        left90();
      
      if(rv==1 && cv==1 && lv==1)
      node();
    }

    while (boty > nodey)
    {
      if (dir == N)
        uturn();
      else if (dir == E)
        right90();
      else if (dir == W)
        left90();

      
      if(rv==1 && cv==1 && lv==1)
      node();
    }

    while (botx > nodex)
    {
      if (dir == S)
        right90();
      else if (dir == N)
        left90();
      else if (dir == W)
        uturn();
      
      if(rv==1 && cv==1 && lv==1)
      node();
  
    }
  }
  stop();
  Serial.println("DONEEEEEEEEEEEEEEEEEEEEEEEEEE");
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  ble();
  go(nodex,nodey);
  if(nodex==2 && nodey==-1)
    uturn();
    exit(0);

  //phasse 2 go(nodex,nodey);
}
