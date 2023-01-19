#include <AESLib.h>
#include <SPI.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>

#define pinCE   7             // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN  8             // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino
#define tunnel  "PIPE1"       // On définit un "nom de tunnel" (5 caractères), pour pouvoir communiquer d'un NRF24 à l'autre

RF24 radio(pinCE, pinCSN);    // Instanciation du NRF24L01

byte key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
char data3[] = "0123456789012345"; //16 chars == 16 bytes

const byte adresse[6] = tunnel;               // Mise au format "byte array" du nom du tunnel
const char message[] = "Hello World !!!";     // Message à transmettre à l'autre NRF24 (32 caractères maxi, avec cette librairie)

void setup() {
  Serial.begin(9600);
  aes128_enc_single(key, data3);
  radio.begin();                      // Initialisation du module NRF24
  radio.openWritingPipe(adresse);     // Ouverture du tunnel en ÉCRITURE, avec le "nom" qu'on lui a donné
  radio.setPALevel(RF24_PA_MIN);      // Sélection d'un niveau "MINIMAL" pour communiquer (pas besoin d'une forte puissance, pour nos essais)
  radio.stopListening();              // Arrêt de l'écoute du NRF24 (signifiant qu'on va émettre, et non recevoir, ici)
}

void loop() {

  Serial.println("encrypted:");
  
  for(int i=0; i<16; i++) {
    Serial.print(data3[i]);
  }
  Serial.println("");
 
  radio.write(data3, sizeof(data3));
  
  //radio.write(&message, sizeof(message));     // Envoi de notre message
  delay(1000);                                // … toutes les secondes !
}
