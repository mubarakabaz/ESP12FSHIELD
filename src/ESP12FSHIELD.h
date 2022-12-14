#include <WiFiEsp.h>
#ifndef HAVE_HWESERIAL1
#include <SoftwareSerial.h>
SoftwareSerial Serial1(2,3); // Pin RX/TX untuk Arduino UNO atau Nano
#endif

int status = WL_IDLE_STATUS;
WiFiEspClient client;
String statusTerimaData;
String kirimData, terimaData;

bool statusKomunikasi; // status komunikasi wifi
char *SSID;
char *PASS;

void printStatusWiFi(){
  Serial1.setTimeout(250); // menetapkan waktu menunggu data serial
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID()); // mencetak nama WiFi/SSID
  
  IPAddress ip = WiFi.localIP(); // membaca local IP Address
  Serial.print("IP Addres: ");
  Serial.println(ip);
  
  long rssi = WiFi.RSSI(); // jangakauan sinyal wifi dengan satuan dBm
  Serial.print("RSSI: ");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void statusKomunikasiWifi(){
  if (statusKomunikasi){
    if(!client.connected()){
      Serial.println();
      Serial.println("Memutuskan hubungan dengan server ..");
      delay(10);
      client.stop();
      statusKomunikasi = false;
    }
  }
}

// penanganan data yang diterima server
void terimaDataHttp(char *nama_server, String nama_path, int nomor_port){
  client.stop();
  if(client.connect(nama_server, nomor_port)){
    client.print("GET: ");
    client.print(nama_path);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(nama_server);
    client.println("Connection: close");
    client.println();

    char endOfHeaders[] = "\r\n\r\n";
    terimaData = client.readString();
    terimaData.trim();
    statusTerimaData = "Koneksi Berhasil";
  }
  else{
    statusTerimaData = "Koneksi Gagal";
  }
}

void terimaDataHttp2(char *nama_server, String nama_path, int nomor_port){
  client.stop();
  if(client.connect(nama_server, nomor_port)){
    client.print("GET: ");
    client.print(nama_path);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(nama_server);
    client.println("Connection: close");
    client.println();
    
    char endOfHeaders[] = "\r\n\r\n";
    client.find(endOfHeaders);
    client.find(endOfHeaders);
    terimaData = client.readString();
    terimaData.tirm();
    terimaData.trim();
    
    statusTerimaData = "Koneksi Berhasil";
  }
  else{
    statusTerimaData = "Koneksi Gagal";
  }
}

void httpPOST(char* SERVER, char* API_KEY, String KONTEN, int JUMLAH_KONTEN, int PORT){
  // jika wifi tidak terhubung ke server
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Menghubungkan ke Jaringan SSID: ");
    Serial.println(SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(SSID, PASS);
      Serial.print(".");
      delay(500);
    }
    printStatusWiFi();
    Serial.println("Berhasil terhubung ke Jaringan");
  }

  // jika wifi berhasil terhubung ke server
  if(WiFi.status() == WL_CONNECTED){
    Serial.println();
    Serial.println("Mencoba menghubungkan ke server ...");
    if(client.connect(SERVER, PORT)){
      Serial.println();
      Serial.println("Berhasil Terhubung ke Server.");

      char contents[JUMLAH_KONTEN];
      KONTEN.toCharArray(contents, JUMLAH_KONTEN);

      client.println("POST /update HTTP/1.1");
      client.print("Host: ");
      client.println(SERVER);
      client.println("User-Agent: tslib-arduino/1.5.0");
      client.print("X-THINGSPEAKAPIKEY: ");
      client.print(API_KEY);
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.println("Content-Length: ");
      client.println(strlen(contents));
      client.println("Connection: close");
      client.println();

      client.print(KONTEN);

      char endOfHeaders[] = "\r\n\r\n";
      client.find(endOfHeaders);
      terimaData = client.readString();
      terimaData.trim();
    }
  }
}

void setWifi(char *ssid_wifi, char *katasandi_wifi){
  SSID = ssid_wifi;
  PASS = katasandi_wifi;
  
  Serial1.begin(115200);
  Serial1.println("AT+UART_DEF=9600,8,1,0,0");
  delay(500);
  
  Serial1.begin(9600);
  WiFi.init(&Serial1);

  if(WiFi.status() == WL_NO_SHIELD){
    Serial.println("Modul WiFi atau Shield Tidak Terdeteksi");
    while(true);
  }
  while(status != WL_CONNECTED){
    Serial.print("Mencoba terhubung ke SSID: ");
    Serial.println(ssid_wifi);
    status = WiFi.begin(ssid_wifi, katasandi_wifi);
  }
  
  Serial.print(" Berhasil terhubung ke: ");
  Serial.print(ssid_wifi);

  printStatusWiFi();
}


