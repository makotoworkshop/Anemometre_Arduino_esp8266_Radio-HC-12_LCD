/*    esp8266 Anemometer Radio Receiver using HC-12 -> Web page port 8080 */

#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
#include <FS.h>

/****************/
/* DÉCLARATIONS */
/****************/
ESP8266WebServer server ( 8080 );   // on instancie un serveur ecoutant sur le port 80
#define ssid      "42"      // WiFi SSID
#define password  "xxxx"      // WiFi password
int incomingChar;

SoftwareSerial HC12(D5, D6); // HC-12 TX Pin, HC-12 RX Pin

/*********/
/* SETUP */
/*********/  
void setup() {
   Serial.begin ( 115200 );    // init du mode débug
   HC12.begin(9600);               // Serial port to HC12

  WiFi.begin ( ssid, password );
  // Attente de la connexion au réseau WiFi / Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); 
    Serial.print ( "." );
  }
  // Connexion WiFi établie
  Serial.println ( "" );
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

  // Montage de la zone mémoire SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS Mount failed");
  } 
  else {
    Serial.println("SPIFFS Mount succesfull");
  }
  delay(50);

  // Pages web du serveur
  server.serveStatic("/js", SPIFFS, "/js");       // dossier js qui contient les fichiers JavaScripts
  server.serveStatic("/css", SPIFFS, "/css");     // dossier css qui contient les fichiers css
  server.serveStatic("/", SPIFFS, "/index.html"); // racine du serveur, pointe l'index.html
  server.on("/mesures.json", sendMesures);        // écrit le fichier json à l'appel de la fonction
  server.begin();                                 // démarre le serveur
  Serial.println ( "HTTP server started" );  
}

/*************/
/* FONCTIONS */
/*************/
void sendMesures()    // appelée par le serveur web
{  
  String json = "{\"rpm\":\"" + String(incomingChar) + "\"}";
  server.send(200, "application/json", json);   // envoie dans le valeur dans le fichier json qui tourne en mémoire
  Serial.println("Mesures envoyees");
}

/*************/
/* PROGRAMME */
/*************/
void loop() {
  server.handleClient();  // à chaque iteration, on appelle handleClient pour que les requetes soient traitees
    while (HC12.available()) {      // If HC-12 has data
    incomingChar = HC12.read();
    sendMesures();
    Serial.print ( "buffer : " );
    Serial.println (incomingChar); 
    }
  delay(100); // la boucle fait tourner sendMesures(), via handleClient, régler delais si besoin de mettre à jour le JSON qu'à une fréquence voulue plutôt qu'instantanément 
}
