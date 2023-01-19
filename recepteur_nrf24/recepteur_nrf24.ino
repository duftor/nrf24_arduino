#include <SPI.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>
#include <AESLib.h>

#define pinCE   7
#define pinCSN  8             // On associe la 
#define tunnel  "PIPE1" 

RF24 radio(pinCE, pinCSN);    // Instanciation du NRF24L01

byte key[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
char data3[16];

const byte adresse[6] = tunnel;       // Mise au format "byte array" du nom du tunnel
char message[32];                     // Avec cette librairie, on est "limité" à 32 caractères par message

void setup() {
  // Initialisation du port série (pour afficher les infos reçues, sur le "Moniteur Série" de l'IDE Arduino)
  Serial.begin(9600);
  Serial.println("Récepteur NRF24L01");
  Serial.println("");

  // Partie NRF24
  radio.begin();                      // Initialisation du module NRF24
  radio.openReadingPipe(0, adresse);  // Ouverture du tunnel en LECTURE, avec le "nom" qu'on lui a donné
  radio.setPALevel(RF24_PA_MIN);      // Sélection d'un niveau "MINIMAL" pour communiquer (pas besoin d'une forte puissance, pour nos essais)
  radio.startListening();             // Démarrage de l'écoute du NRF24 (signifiant qu'on va recevoir, et non émettre quoi que ce soit, ici)
}

void loop() {
  // On vérifie à chaque boucle si un message est arrivé
  if (radio.available()) {
    //radio.read(&message, sizeof(message));                        // Si un message vient d'arriver, on le charge dans la variable "message"
    //Serial.print("Message reçu : "); Serial.println(message);     // … et on l'affiche sur le port série !
    radio.read(data3, sizeof(data3));
    aes128_dec_single(key, data3);

    Serial.print("Decrypted : ");
    for (int i = 0; i < 16; i++) {
      Serial.print(data3[i]);
    }
    Serial.println("");
  }
}
