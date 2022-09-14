#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>//Librería utilizada para NFC
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
RTC_DS1307 rtc;//Variable que asignamos al modulo RTC
LiquidCrystal_I2C lcd(0x27, 20, 4);
#define PN532_IRQ   (2)//Puerto IRQ a utilizar en arduino
#define PN532_RESET (3)//Puerto RESET a utilizar en arduino
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);//Conexión IRQ y RESET a utiizar
String mensaje;
String Mensaje = "";
void setup(void) {
  //Serial.begin(115200);
  Serial.begin(9600);//Velocidad de conecion con puerto serie
  nfc.begin();//Inicializamos NFC
  // Configurar para leer etiquetas RFID
  nfc.setPassiveActivationRetries(0xFF);//Activa NFC modo pasivo
  nfc.SAMConfig();//Inicializa NFC
  // Serial.println("ACERQUE SU TARJETA");//Muestra en Pantalla
  lcd.init();//initializa LCD
  lcd.begin(20, 4); //Inicializa LCD 20x4
  lcd.backlight();//Enciende la luz de LCD
  lcd.clear(); //Limpia el contenido de la LCD
  lcd.setCursor(5, 1);//Se configura cursor en LCD
  lcd.print("BIENVENIDO");
  delay(2000);//Tiempo de visualizacion
  lcd.clear();//Limpia el contenido de la LCD

  if (! rtc.begin()) //Verifica si MODULO RTC este conectado
  {
    lcd.print("Modulo RTC Desconectado");
    while (1);
  }
  if (! rtc.isrunning()) //Verifica si MODULO RTC este funcionando
  {
    lcd.print("Modulo RTC no funcionando");

  }
 // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));// Configura la Hora y Fecha actual de la PC solo se realiza 1 vez 
}
// Funcion auxiliar para mostrar el buffer = Memoria
void printArray(byte *buffer, byte tambuffer) {
  for (byte i = 0; i < tambuffer; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : "");//Muestra en Pantalla en el ID de la Tarjeta
    Serial.print(buffer[i], HEX);//Muestra en Pantalla en HEXADECIMAL
  }
  return;
}
void formatoFecha (char Fecha[10], int Dia, int Mes, int A) { //Funcion FormatoHora con Variables a utilizar
  sprintf(Fecha, "%02d/%02d/%02d", A, Mes, Dia); //Configura el Formato de fecha deseable A-M-D Ej. 2000-09-25
}
void Hora() { //Funcion Hora
  DateTime now = rtc.now(); // Se utiliza una variable now de tipo DateTime
  char _Hora[10]; // Se utiliza con una variable Hora de vector 10
  formatoHora(_Hora, now.hour(), now.minute(), now.second()); // Se llama a la fucnion formatoHora
  lcd.setCursor(6, 2);//Lleva el cursor en posicion (6,2)
  lcd.print(_Hora); //Se imprime la Hora en LCD
  // Serial.print(_Hora); //Se imprime la Hora en Ventana Serial
}
void formatoHora (char Hora[10], int hora, int minu, int seg) { //Funcion FormatoHora con Variables a utilizar
  sprintf(Hora, "%02d:%02d:%02d", hora, minu, seg); //Configura el Formato de hora deseable HH:MM:SS 23:-54:12
}
void Fecha() { //Funcion Fecha
  DateTime now = rtc.now(); // Se utiliza una variable now de tipo DateTime
  char _Fecha[10];// Se utiliza con una variable Fecha de vector 10
  formatoFecha(_Fecha, now.year(), now.month(), now.day()); // Se llama a la funcion formatoFecha
  lcd.setCursor(5, 3);
  lcd.print(_Fecha); //Se imprime la Fecha en LCD
  
}
void lecturaNFC() {

  return;
}

void loop(void) {
  DateTime now = rtc.now();//Variable para Tiempo
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("ACERQUE SU");
  lcd.setCursor(6, 1);
  lcd.print("TARJETA");
  Hora();//Llama a la Funcion Hora
  Fecha();//Llama a la Funcion Fecha
  boolean lectura;//Variable tipo Boolena
  uint8_t id[] = { 0, 0, 0, 0, 0, 0, 0 };//Variable tipo uint8 de vector de 8
  uint8_t idtamano;//Variable tipo uint8
  lectura = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &id[0], &idtamano);//Realiza la lectura de la tarjeta NFC
  if (lectura) {//Condicion para verificar si hay tarjeta en el lector NFC
    printArray(id, idtamano);//Imprime el ID de la tarjeta NFC
    Serial.println("");//Lo muesta en modo serial
    delay(500);
    recibido();//Llama la funcion recbido (recepcion de aplicacion )
  }
  else {
    Serial.println("Tarjeta no encontrada");//Imprime el comentario en caso de no existir una tarjeta en el lector NFC
  }
  //MlecturaNFC();//Llama a la Funcion LecturaNFC
  //  return;
}





void recibido() {
  int caracteres = 0; //Cantidad de caracters
  while (Serial.available() > 0) { //Ciclo que se ejecutará mientras halla caracteres que se puedan leer en el puerto Serie
    //En la comunicación serial se envían los mensajes caracter a caracter, por lo que tenemos que leerlos 1 a 1
    //Serial.available() indica la canidad de caracteres disponibles. //Este while no se romperá hasta que se halla leido todo.
    Mensaje = Mensaje + Decimal_to_ASCII(Serial.read()); //Se lee el caracter de entrada, se transforma desde ASCII
    //Se acumulan los caracteres en la variable mensaje
  }
  caracteres = Mensaje.length(); //Se lee la cantidad de //caracteres del mensaje que viene desde Java
  if (caracteres > 20) { //Si hay mas de 20 caracteres...
    if (Mensaje != "") { //Si la variable mensaje NO está vacia...
      lcd.clear(); //Se limpia el LCD
      lcd.setCursor(7, 0);
      lcd.print("ACCESO");
      lcd.setCursor(6, 2);
      lcd.print("CORRECTO");
      delay(1000);
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("BIENVENID@");
      lcd.setCursor(0, 2);
      lcd.print(Mensaje.substring(0, 20)); //Se imprime los //primeros 20 caracteres en el renglón 1
      lcd.setCursor(0, 3); //Se salta al rengón 2
      lcd.print(Mensaje.substring(20, caracteres)); //Se //imprime los caracteres que hallan sobrado del primer renglón
      delay(2000); //Se espera 2 segundo
    }
  }

  else
  { //Si la cantidad de caracteres no es mayor a 20...
    if (Mensaje != "") {
      lcd.clear();
      lcd.setCursor(7, 0);
      lcd.print("ACCESO");
      lcd.setCursor(6, 2);
      lcd.print("CORRECTO");
      delay(1000);
      lcd.clear();
      lcd.setCursor(5, 0);//Se configura cursor en LCD
      lcd.print("BIENVENID@");
      lcd.setCursor(0, 2);
      lcd.print(Mensaje); //Se imprime todo en el primer renglón
      delay(2000); //Se espera 2 segundo
    }  
    else{//Si no se tiene nada en la comunicacion serial
      lcd.clear(); //Se limpia el LCD
      lcd.setCursor(7, 0);
      lcd.print("ACCESO");
      lcd.setCursor(6, 2);
      lcd.print("CORRECTO");
      delay(1000);
      lcd.clear();
    }
  }

  Mensaje = ""; //Se limpia la variable mensaje. Esto evitará que //se imprima algo en el LCD si no se ha enviado algún mensaje.
}

char Decimal_to_ASCII(int entrada) {//Convierte los caracteres ASCII
  char salida = ' ';
  switch (entrada) {
    case 65:
      salida = 'A';
      break;
    case 66:
      salida = 'B';
      break;
    case 67:
      salida = 'C';
      break;
    case 68:
      salida = 'D';
      break;
    case 69:
      salida = 'E';
      break;
    case 70:
      salida = 'F';
      break;
    case 71:
      salida = 'G';
      break;
    case 72:
      salida = 'H';
      break;
    case 73:
      salida = 'I';
      break;
    case 74:
      salida = 'J';
      break;
    case 75:
      salida = 'K';
      break;
    case 76:
      salida = 'L';
      break;
    case 77:
      salida = 'M';
      break;
    case 78:
      salida = 'N';
      break;
    case 79:
      salida = 'O';
      break;
    case 80:
      salida = 'P';
      break;
    case 81:
      salida = 'Q';
      break;
    case 82:
      salida = 'R';
      break;
    case 83:
      salida = 'S';
      break;
    case 84:
      salida = 'T';
      break;
    case 85:
      salida = 'U';
      break;
    case 86:
      salida = 'V';
      break;
    case 87:
      salida = 'W';
      break;
    case 88:
      salida = 'X';
      break;
    case 89:
      salida = 'Y';
      break;
    case 90:
      salida = 'Z';
      break;
    case 97:
      salida = 'a';
      break;
    case 98:
      salida = 'b';
      break;
    case 99:
      salida = 'c';
      break;
    case 100:
      salida = 'd';
      break;
    case 101:
      salida = 'e';
      break;
    case 102:
      salida = 'f';
      break;
    case 103:
      salida = 'g';
      break;
    case 104:
      salida = 'h';
      break;
    case 105:
      salida = 'i';
      break;
    case 106:
      salida = 'j';
      break;
    case 107:
      salida = 'k';
      break;
    case 108:
      salida = 'l';
      break;
    case 109:
      salida = 'm';
      break;
    case 110:
      salida = 'n';
      break;
    case 111:
      salida = 'o';
      break;
    case 112:
      salida = 'p';
      break;
    case 113:
      salida = 'q';
      break;
    case 114:
      salida = 'r';
      break;
    case 115:
      salida = 's';
      break;
    case 116:
      salida = 't';
      break;
    case 117:
      salida = 'u';
      break;
    case 118:
      salida = 'v';
      break;
    case 119:
      salida = 'w';
      break;
    case 120:
      salida = 'x';
      break;
    case 121:
      salida = 'y';
      break;
    case 122:
      salida = 'z';
      break;
  }
  return salida;
}
