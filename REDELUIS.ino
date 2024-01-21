#include <ESP8266WiFi.h>
const char* ssid = "ALUNOS"; //"brisa-2425476"; "TP-LINK_7880EC" "ALUNOS"
const char* password = "ifce.aluno"; //"wgpdtnk1"; "58818848" "aluno.ifce"
int ledPin = 2; // Arduino standard is GPIO13 but lolin nodeMCU is 2 http://www.esp8266.com/viewtopic.php?f=26&t=13410#p61332
int relay1 = 5;
WiFiServer server(80);
void setup() 
{
  Serial.begin(115200);
  delay(10);
  pinMode(ledPin, OUTPUT);
  pinMode(relay1, OUTPUT);
  digitalWrite(ledPin, HIGH);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

 Serial.println("");
 Serial.println("WiFi connected");

// Start the server
 server.begin();
 Serial.println("Server started");

// Print the IP address
 Serial.print("Use this URL to connect: ");
 Serial.print("http://");
 Serial.print(WiFi.localIP());
 Serial.println("/");

}

void loop() 
{
 // Check if a client has connected
 WiFiClient client = server.available();
 if (!client) 
  {
    return;
  }

// Wait until the client sends some data
 Serial.println("new client");
 while(!client.available())
  {
    delay(1);
  }

// Read the first line of the request
 String request = client.readStringUntil('r');
 Serial.println(request);
 client.flush();

// Match the request
 //Lolin nodeMCU has inverted the LED.
 //Low level turns on the led
 //while High level turns it off

  int value = HIGH; //initially off *Nota: Usar no ELIPSE
  if (request.indexOf("/LED=OFF") != -1) 
  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
 if (request.indexOf("/LED=ON") != -1) 
 {
    digitalWrite(ledPin, LOW);
    value = LOW;
 }
 //Criando a página para os relés
 int rv1 = HIGH; //Rele desativo no estado alto da porta
 if (request.indexOf("/RL1=OFF") != -1) //Relé Desligado
 {
    digitalWrite(relay1, HIGH);
    rv1 = HIGH;
 }
 if (request.indexOf("/RL1=ON") != -1)
 {
    digitalWrite(relay1, LOW);
    rv1 = LOW;
 }

// Set ledPin according to the request
 //digitalWrite(ledPin, value);

// Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<meta charset = 'UTF-8'>/");
  client.println("<title>Controlador de Relés - Luís</title>");
  client.println("<h1 style=\"text-align: center;font-family: 'Times New Roman', Times, serif;\"\>Teste SDCD N2 - Controle de relés.<br> Testando o CLI do Arduino");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">");
  client.println("<style>p.padding{padding-left: 0.4cm;}p{color: black;}cred{color: red}cgreen{color: green}</style>");

  client.print("<br><p class=\"padding\">Led On-Board está: ");
 //High=off
 //Low=on

  if(value == HIGH) 
    {
      client.print("<cred>Desligado</cred>");
    } 
  else 
    {
      client.print("<cgreen>Ligado<cgreen></p>");
    }
  client.println("<div class=\"w3-container\">");
  client.println("<br>");
  client.println("<a href=\"/LED=ON\"\"><button class=\"w3-btn w3-ripple w3-green\">Ligar </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button class=\"w3-btn w3-ripple w3-red\">Desligar </button></a><br/>");

  client.println("<a href=\"/RL1=ON\"\"><button class=\"w3-btn w3-ripple w3-green\">Relé - Ligar </button></a>");
  client.println("<a href=\"/RL1=OFF\"><button class=\"w3-btn w3-ripple w3-red\">Relé - Desligar </button></a><br/>");

  client.println("</h1>");
  client.println("</div>");
  client.println("</html>");

  delay(1);
  Serial.println("Cliente desconectado");
  Serial.println("");
}
