void conectarWiFi() {
  WiFi.mode(WIFI_STA);  // Indica modo Station (conexión a outro dispositivo)
  WiFi.disconnect();    // Desconecta unha posible conexión previa
  WiFi.begin(WIFI_SSID, WIFI_PASS); // Inicia a conexión 
  Serial.print("\n\nAgardando pola WiFi ");
  int contador = 0;  // Comproba estado da conexión e fai varias tentativas
  while(WiFi.status() != WL_CONNECTED and contador < MAX_INTENTOS) {
    contador++;
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  // Informa do estado da conexión e IP en caso de éxito
  if(contador < MAX_INTENTOS) {
    Serial.print("Conectado á WiFi coa IP: "); Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Non se puido conectar á WiFi");
  }
  Serial.print("\n\n\n");
}
