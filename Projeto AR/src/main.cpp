#include <WiFi.h>
#include <FirebaseESP32.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "time.h"
//--------------------------------config Temporizador----------------------------------

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = -3600*3;
String datta = "";
String hora = "";
unsigned long tempoMillis = 0; 
const int intervalo = 40000;
String horaLiga = "",horaDesliga = "";
String horaLiga2 ="",horaDesliga2 = "";


//--------------------------------pinagem de cada sala----------------------------------

const uint16_t sala1 = 12;  
const uint16_t sala2 = 33;
const uint16_t sala3 = 25;
const uint16_t sala4 = 14;
const uint16_t sala5 = 32;
const uint16_t sala6 = 27;
const uint16_t sala7 = 21;
const uint16_t sala8 = 22;
const uint16_t sala9 = 23;
#define pinoResetar 26

IRsend irsend1(sala1);  
IRsend irsend2(sala2);
IRsend irsend3(sala3);
IRsend irsend4(sala4);
IRsend irsend5(sala5);
IRsend irsend6(sala6);
IRsend irsend7(sala7);
IRsend irsend8(sala8);
IRsend irsend9(sala9);

//--------------------------------watch dog ----------------------------------

hw_timer_t *timer = NULL;

//--------------------------------Codigos IR ----------------------------------

uint16_t electraon[211] = {8932, 4542,  492, 1724,  518, 1748,  494, 612,  494, 588,  544, 588,  492, 614,  494, 1748,  494, 1748,  494, 1748,  492, 1750,  492, 1692,  552, 1746,  492, 1748,  494, 1748,  492, 1750,  492, 612,  494, 614,  492, 614,  492, 614,  494, 612,  494, 614,  518, 1724,  518, 1726,  540, 1672,  546, 588,  518, 588,  520, 588,  518, 588,  520, 588,  518, 588,  520, 588,  518, 588,  518, 564,  568, 564,  518, 564,  568, 538,  570, 538,  568, 1700,  542, 1698,  544, 562,  544, 562,  546, 560,  546, 562,  546, 562,  546, 562,  546, 562,  544, 562,  546, 562,  546, 558,  548, 534,  572, 560,  548, 560,  546, 560,  548, 1696,  544, 560,  548, 560,  546, 560,  546, 560,  546, 560,  546, 560,  546, 562,  542, 560,  548, 560,  548, 558,  548, 560,  548, 558,  548, 560,  546, 560,  546, 562,  544, 564,  518, 590,  492, 614,  494, 612,  520, 588,  518, 588,  522, 586,  522, 586,  520, 1720,  522, 584,  524, 582,  524, 582,  524, 586,  520, 584,  524, 582,  524, 582,  524, 584,  522, 586,  520, 586   ,  520, 1726,  540, 564,  546, 1694,  548, 560,  546, 560,  520, 586,  494, 612,  492, 614,  518, 1722,  568, 1672,  518, 1724,  494, 612,  520, 586,  546, 560,  548, 1694,  548, 1694,  546};  // ELECTRA_AC
uint16_t electraoff[211] = {8986, 4486,  550, 1690,  552, 1718,  524, 582,  522, 584,  522, 584,  522, 584,  522, 1720,  524, 1718,  522, 1718,  524, 1718,  524, 1718,  522, 1718,  522, 1718,  526, 1718,  522, 1720,  522, 584,  522, 582,  524, 584,  524, 584,  522, 584,  524, 584,  524, 1718,  524, 1718,  522, 1718,  524, 584,  520, 584,  524, 584,  522, 586,  520, 586,  522, 584,  522, 610,  496, 586,  522, 584,  524, 584,  520, 586,  518, 586,  522, 586,  518, 1720,  522, 1720,  522, 586,  520, 588,  520, 560,  546, 588,  518, 586,  522, 586,  520, 588,  518, 562,  546, 584,  522, 586,  522, 586,  518, 586,  522, 586,  522, 586,  518, 1722,  520, 586,  522, 586,  520, 586,  522, 586,  522, 586,  520, 586,  520, 586,  522, 584,  522, 586,  520, 586,  520, 586,  522, 584,  522, 584,  522, 584,  522, 586,  522, 560,  546, 584,  522, 586,  522, 586,  520, 584,  522, 586,  522, 584,  520, 586,  522, 584,  524, 584,  522, 584,  522, 584,  522, 584,  522, 584,  522, 586,  522, 584,  522, 584,  522, 584,  522, 584,    524, 1718,  520, 586,  520, 1720,  522, 584,  522, 584,  522, 586,  522, 584,  522, 588,  520, 1718,  522, 1718,  524, 1718,  522, 584,  522, 584,  522, 1718,  522, 584,  524, 1720,  522};
uint16_t fujitsuon[259] = {3300, 1578,  440, 372,  440, 370,  440, 1186,  438, 372,  440, 1184,  440, 372,  440, 372,  440, 372,  440, 1184,  440, 1184,  440, 374,  440, 372,  440, 374,  436, 1186,  440, 1184,  440, 372,  440, 370,  440, 372,  438, 372,  440, 372,  440, 410,  402, 372,  440, 372,  442, 370,  440, 372,  440, 370,  440, 372,  440, 372,  438, 1186,  438, 372,  438, 374,  440, 372,  440, 372,  438, 372,  440, 370,  440, 374,  440, 1184,  440, 372,  440, 372,  440, 372,  440, 372,  412, 1248,  402, 1184,  440, 1184,  440, 1186,  440, 1182,  442, 1182,  442, 1184,  440, 1184,  440, 370,  442, 372,  440, 1186,  438, 374,  438, 372,  442, 372,  438, 372,  440, 372,  440, 372,  438, 372,  440, 410,  400, 1186,  442, 1184,  440, 370,  440, 372,  442, 1184,  440, 372,  440, 372,  440, 374,  438, 1186,  440, 1184,  440, 1184,  440, 372,  440, 372,  440, 372,  440, 374,  440, 372,  440, 372,  438, 372,  440, 372,  440, 372,  440, 372,  438, 372,  438, 374,  440, 372,  440, 372,  440, 372,  440, 372,  440, 372,  440, 372,  440, 372,  440, 372,  440, 374,  438, 372,  442, 372,  438, 412,  400, 372,  442, 370,  440, 372,  440, 372,  440, 372,  440, 372,  440, 372,  440, 372,  440, 374,  412, 398,  440, 372,  438, 374,  440, 372,  440, 372,  442, 372,  438, 372,  440, 372,  440, 372,  440, 374,  438, 374,  438, 372,  440, 372,  440, 1184,  440, 372,  438, 372,  440, 1184,  442, 1184,  440, 1184,  440, 1184,  440, 1186,  442, 1182,  440, 372,  440, 372,  438};
uint16_t fujitsuoff[115] = {3320, 1576,  440, 374,  438, 410,  402, 1188,  438, 372,  438, 1186,  438, 374,  438, 374,  438, 374,  438, 1186,  438, 1186,  440, 372,  440, 372,  440, 372,  442, 1182,  438, 1186,  440, 372,  440, 374,  438, 372,  440, 408,  384, 392,  440, 372,  440, 372,  442, 370,  440, 374,  436, 374,  438, 374,  438, 374,  438, 372,  440, 1184,  440, 372,  440, 374,  438, 372,  440, 372,  438, 372,  442, 370,  440, 372,  440, 1184,  438, 374,  440, 372,  438, 374,  440, 372,  440, 1184,  440, 372,  440, 372,  438, 374,  438, 374,  440, 372,  438, 374,  438, 1186,  440, 372,  440, 1186,  438, 1186,  438, 1184,  440, 1184,  438, 1188,  438, 1186,  438};  // FUJITSU_AC

//--------------------------------WIFI E FIREBASE----------------------------------


const char* usuario = "ifce-professores";
const char* senha=  "1fc3pr0f2019";
bool statusConexao= false;

const char* host = "https://esp32-neo-default-rtdb.firebaseio.com/";
const char* token = "F9NYqbYo54CTpLVq31aCZIc3ySmseVgvv9RPc4gW";

FirebaseData comandoDB;

//--------------------------------reset module----------------------------------

void IRAM_ATTR resetModule(){
  Serial.println("interrupção");
  ets_printf("(watchdog) reiniciar\n"); //imprime no log
   if(digitalRead(pinoResetar)==HIGH)
   {
    digitalWrite(pinoResetar,LOW);
   }
   else
   {
    digitalWrite(pinoResetar,HIGH);
   }
    
   
  

}
//--------------------------------temporizador----------------------------------
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  String dia= (String(timeinfo.tm_mday).length()>1) ? timeinfo.tm_mday : "0"+String(timeinfo.tm_mday);
  String mes= (String(timeinfo.tm_mon).length()>1) ? timeinfo.tm_mon+1 : "0"+String(timeinfo.tm_mon+1);
  datta= dia+"/"+mes+"/"+String(timeinfo.tm_year+1900);
  String seg= (String(timeinfo.tm_sec).length()>1) ? timeinfo.tm_sec : "0"+String(timeinfo.tm_sec);
  String min_= (String(timeinfo.tm_min).length()>1) ? timeinfo.tm_min : "0"+String(timeinfo.tm_min);
  String hour= (String(timeinfo.tm_hour).length()>1) ? timeinfo.tm_hour : "0"+String(timeinfo.tm_hour);
  hora= hour+":"+min_;
  Serial.println("Data: "+datta+"\t Hora: "+hora);
  }




//--------------------------------SETUP----------------------------------
void setup() 
  {
  pinMode(pinoResetar,OUTPUT);
  digitalWrite(pinoResetar,HIGH);
  Serial.begin(115200);
   
 
  Serial.println("INICIANDO");
 
  WiFi.mode(WIFI_STA);  
  WiFi.begin(usuario,senha);
  
 

  while(WiFi.status()!= WL_CONNECTED){
    delay(1000);
  Serial.print(".");

  }
  Serial.println("");

  statusConexao = true;
  Firebase.begin(host,token);
  irsend1.begin();
  irsend2.begin();
  irsend3.begin();
  irsend4.begin();
  irsend5.begin();
  irsend6.begin();
  irsend7.begin();
  irsend8.begin();
  irsend9.begin(); 
  Serial.println("conectado");

 configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

   timer = timerBegin(0, 80, true); 
  timerAttachInterrupt(timer, &resetModule, true);
  timerAlarmWrite(timer, 30000000, true);
  timerAlarmEnable(timer);
    }
//--------------------------------OTA----------------------------------

  
//--------------------------------LOOP----------------------------------

void loop()
{
  
  timerWrite(timer, 0);
  long tme = millis();
  
  Firebase.get(comandoDB,"AR/comando");
  int comando = comandoDB.intData();
  Serial.println("Comando: "+String(comando));
  switch(comando)
  {
   //------------AR 1------------------ 
   case 10:
   Serial.println("Ligando Sala 1");
   irsend1.sendRaw(fujitsuon, 259, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 1",1);
   break;
   
   case 11:
   Serial.println("Desligando Sala 1");
   irsend1.sendRaw(fujitsuoff, 115, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 1",0);
   break;

   //------------AR 2------------------ 

   case 20:
   Serial.println("Ligando Sala 2");
   irsend2.sendRaw(fujitsuon, 259, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 2",1);
   break;
   
   case 21:
   Serial.println("Desligando Sala 2");
   irsend2.sendRaw(fujitsuoff, 115, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 2",0);
   break;

   //------------AR 3------------------ 

   case 30:
   Serial.println("Ligando Sala 3");
   irsend3.sendRaw(fujitsuon, 259, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 3",1);
   break;
   
   case 31:
   Serial.println("Desligando Sala 3");
   irsend3.sendRaw(fujitsuoff, 115, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 3",0);
   break;

   //------------AR 4------------------ 

   case 40:
   Serial.println("Ligando Sala 4");
   irsend4.sendRaw(fujitsuon, 259, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 4",1);
   break;
   
   case 41:
   Serial.println("Desligando Sala 4");
   irsend4.sendRaw(fujitsuoff, 115, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 4",0);
   break;

   //------------AR 5------------------ 

   case 50:
   Serial.println("Ligando Sala 5");
   irsend5.sendRaw(fujitsuon, 259, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 5",1);
   break;
   
   case 51:
   Serial.println("Desligando Sala 5");
   irsend5.sendRaw(fujitsuoff, 115, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 5",0);
   break;

   //------------AR 6------------------ 

   case 60:
   Serial.println("Ligando Sala 6");
   irsend6.sendRaw(fujitsuon, 259, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 6",1);
   break;
   
   case 61:
   Serial.println("Desligando Sala 6");
   irsend6.sendRaw(fujitsuoff, 115, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
    Firebase.set(comandoDB,"AR/sala 6",0);
   break;

   //------------AR 7------------------ 

   case 70:
   Serial.println("Ligando Sala 7");
   irsend7.sendRaw(electraon, 211, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 7",1);
   break;
   
   case 71:
   Serial.println("Desligando Sala 7");
   irsend7.sendRaw(electraoff, 211, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 7",0);
   break;
   //------------AR 8------------------ 

   case 80:
   Serial.println("Ligando Sala 8");
   irsend8.sendRaw(fujitsuon, 259, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 8",1);
   break;
   
   case 81:
   Serial.println("Desligando Sala 8");
   irsend8.sendRaw(fujitsuoff, 115, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 8",0);
   break;

   //------------AR 9------------------ 

   case 90:
   Serial.println("Ligando Sala 9");
   irsend9.sendRaw(fujitsuon, 259, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0); 
   Firebase.set(comandoDB,"AR/sala 9",1);
   break;
   
   case 91:
   Serial.println("Desligando Sala 9");
   irsend9.sendRaw(fujitsuoff, 115, 38);
   delay(1000);
   Firebase.set(comandoDB,"AR/comando",0);
   Firebase.set(comandoDB,"AR/sala 9",0);
   break;

   case 100:
   while(true)
   {
    Serial.println("travando");
    }
   break;

   case 1000:
   Firebase.getString(comandoDB,"horario/liga");
   horaLiga = comandoDB.to<const char *>();
   Serial.print("horaLiga: ");
   Serial.println(horaLiga);
   delay(500);
   Firebase.set(comandoDB,"AR/comando",0);
   break;

   case 1001:
   Firebase.getString(comandoDB,"horario/desliga");
   horaDesliga = comandoDB.to<const char *>();
   Serial.print("horaDesliga: ");
   Serial.println(horaDesliga);
   delay(500);
   Firebase.set(comandoDB,"AR/comando",0);
   break;

    case 1003:
   Firebase.getString(comandoDB,"horario/liga2");
   horaLiga2 = comandoDB.to<const char *>();
   Serial.print("horaLiga2: ");
   Serial.println(horaLiga2);
   delay(500);
   Firebase.set(comandoDB,"AR/comando",0);
   break;

   case 1004:
   Firebase.getString(comandoDB,"horario/desliga2");
   horaDesliga2 = comandoDB.to<const char *>();
   Serial.print("horaDesliga2: ");
   Serial.println(horaDesliga2);
   delay(500);
   Firebase.set(comandoDB,"AR/comando",0);
   break;

   case 666:
   while(true)
   {
    Serial.println("travando");
    delay(1000);
   }
   break;
  }
  
  
  if(millis()-tempoMillis > intervalo)
  {
   printLocalTime();
   tempoMillis=millis();
   if(horaLiga ==hora || horaLiga2 == hora)
   {
    Serial.println("ligando salas");
    irsend1.sendRaw(fujitsuon, 259, 38);
    Firebase.set(comandoDB,"AR/sala 1",1);
    delay(500);
    irsend2.sendRaw(fujitsuon, 259, 38);
    Firebase.set(comandoDB,"AR/sala 2",1);
    delay(500);
    irsend3.sendRaw(fujitsuon, 259, 38);
    Firebase.set(comandoDB,"AR/sala 3",1);
    delay(500);
    irsend4.sendRaw(fujitsuon, 259, 38);
    Firebase.set(comandoDB,"AR/sala 4",1);
    delay(500);
    irsend5.sendRaw(fujitsuon, 259, 38);
    Firebase.set(comandoDB,"AR/sala 5",1);
    delay(500);
    irsend6.sendRaw(fujitsuon, 259, 38);
    Firebase.set(comandoDB,"AR/sala 6",1);
    delay(500);
    irsend7.sendRaw(electraon, 211, 38);
    Firebase.set(comandoDB,"AR/sala 7",1);
    delay(500);
    irsend8.sendRaw(fujitsuon, 259, 38);
    Firebase.set(comandoDB,"AR/sala 8",1);
    delay(500);
    irsend9.sendRaw(fujitsuon, 259, 38);
    Firebase.set(comandoDB,"AR/sala 9",1);
    delay(500);
    
   }
   if(horaDesliga == hora || horaDesliga2 == hora)
   {
    
   Serial.println("desligando salas");
   irsend1.sendRaw(fujitsuoff, 115, 38);
   Firebase.set(comandoDB,"AR/sala 1",0);
   delay(500);
   irsend2.sendRaw(fujitsuoff, 115, 38);
   Firebase.set(comandoDB,"AR/sala 2",0);
   delay(500);
   irsend3.sendRaw(fujitsuoff, 115, 38);
   Firebase.set(comandoDB,"AR/sala 3",0);
   delay(500);
   irsend4.sendRaw(fujitsuoff, 115, 38);
   Firebase.set(comandoDB,"AR/sala 4",0);
   delay(500);
   irsend5.sendRaw(fujitsuoff, 115, 38);
   Firebase.set(comandoDB,"AR/sala 5",0);
   delay(500);
   irsend6.sendRaw(fujitsuoff, 115, 38);
   Firebase.set(comandoDB,"AR/sala 6",0);
   delay(500);
   irsend7.sendRaw(electraoff, 211, 38);
   Firebase.set(comandoDB,"AR/sala 7",0);
   delay(500);
   irsend8.sendRaw(fujitsuoff, 115, 38);
   Firebase.set(comandoDB,"AR/sala 8",0);
   delay(500);
   irsend9.sendRaw(fujitsuoff, 115, 38);
   Firebase.set(comandoDB,"AR/sala 9",0);
   delay(500);
   
   }
   
  }
   delay(500);
}