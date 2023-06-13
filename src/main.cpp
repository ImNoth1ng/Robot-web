#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <esp_camera.h>
#include <WiFi.h>
//Librerias necesarias para el robot

//Aquí coloca el nombre de tu red y la contraseña
const char* ssid = "tu_nombre_de_red";
const char* password = "tu_contraseña_de_red";

//Pines parael control de los motores
#define IN1 4
#define IN2 2

#define IN3 14
#define IN4 15

//Creamos la página web
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Carro Control</title>
  <style>
    body {
      background-color: #000;
      color: #0F0;
      font-family: "Courier New", monospace;
    }
    .container {
      width: 80%;
      margin: 0 auto;
      text-align: center;
      padding-top: 50px;
    }
    .button {
      background-color: #0F0;
      border: none;
      color: #000;
      padding: 20px 40px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 30px;
      margin: 10px 2px;
      cursor: pointer;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>Control del Carro</h1>
    <button class="button" onclick="sendData('forward')">Adelante</button><br>
    <button class="button" onclick="sendData('backward')">Atrás</button><br>
    <button class="button" onclick="sendData('left')">Izquierda</button>
    <button class="button" onclick="sendData('stop')">Detener</button>
    <button class="button" onclick="sendData('right')">Derecha</button>
    <script>
      async function sendData(command) {
        const response = await fetch(`control?cmd=${command}`);
        const data = await response.text();
        console.log(data);
      }
    </script>
  </div>
</body>
</html>
)rawliteral";

//Creamos el servidor web
AsyncWebServer server(80);


void setup() {
  //ponemos el serial a 115200 baudios
  Serial.begin(115200);

  //Configuramos los pines de los motores como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //Configuramos el wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando al WiFi..");
  }

  //Avisamos que estamos conectados
  Serial.println("Conectado a la red WiFi");

  //mostarmos la dirección IP
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  //Configuramos la cámara
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  //Inicializamos la cámara
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Error al inicializar la cámara! %d", err);
    return;
  }

  //Configuramos el servidor web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  //Configuramos la ruta para controlar el carro
  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request){
    String inputMessage;
    String cmd;
    //Obtenemos el comando
    if (request->hasParam("cmd")) {
      cmd = request->getParam("cmd")->value();
    } else {
      cmd = "No command sent";
    }
    //Ejecutamos el comando
    if (cmd == "forward") {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      inputMessage = "Adelante";
    } else if (cmd == "backward") {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      inputMessage = "Atrás";
    } else if (cmd == "left") {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      inputMessage = "Izquierda";
    } else if (cmd == "right") {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      inputMessage = "Derecha";
    } else if (cmd == "stop") {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      inputMessage = "Detener";
    } else {
      inputMessage = "No command sent";
    }
    //Enviamos la respuesta
    request->send(200, "text/plain", "Comando: " + inputMessage);
  });

  //Iniciamos el servidor web
  server.begin();


}

void loop() {

  
//noma

}
//posibles errores:
//1. No se puede conectar a la red WiFi
//2. No se puede inicializar la cámara
//3. No se puede inicializar el servidor web


//Mejoras posibles:
//1. Agregar un botón para tomar una foto
//2. Agregar un botón para grabar un video
//3. Agregar un botón para activar el modo de seguimiento de rostros
//4. Agregar un botón para activar el modo de seguimiento de objetos
//5. Agregar un botón para activar el modo de seguimiento de líneas
//6. Agregar un botón para activar el modo de seguimiento de colores
//7. Agregar un botón para activar el modo de seguimiento de movimiento
//8. Agregar un botón para activar el modo de seguimiento de distancia


//Ejemplos de proyectos:
//1. Carro que sigue rostros
//2. Carro que sigue objetos
//3. Carro que sigue líneas
//4. Carro que sigue colores