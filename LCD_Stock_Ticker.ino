#include <LiquidCrystal_I2C.h> //Easy To Use 3rd party LCD library
#include <WiFiClientSecure.h> //HTTPS protocol library


const char* ssid = "Your SSID Here";
const char* password = "Your SSID Password Here";

const float BuyPrice = 0; //Insert Your Initial Buy Price Here

const char* rootCA = "Your SSL Root Certificate Here"
                           
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup() {
  //LCD Setup
  lcd.init();
  lcd.backlight();
  //Serial Setup (for debugging)
  Serial.begin(115200); //Set Baud Rate
  delay(100);
  Serial.println();
  Serial.println();
}

void loop() {
  lcd.clear();
  int disconnectedCounter = 0;
  if (WiFi.status() != WL_CONNECTED) //Connects to Wi-Fi network (troubleshoots itself, see serial output for debugging)
  {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    lcd.setCursor(0,0);
    lcd.print("Connecting to: ");
    lcd.setCursor(0,1);
    lcd.print(ssid);
    lcd.autoscroll();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
      Serial.print(".");
      lcd.print(".");
      disconnectedCounter++;
      if (disconnectedCounter == 5)
      ESP.restart();
    }
  }
  lcd.noAutoscroll();
  lcd.clear();
  Serial.println("");
  Serial.println("WiFi connected");
  lcd.setCursor(0,0);
  lcd.print("WiFi connected");
  delay(1000);
  Serial.println("connecting to ca.finance.yahoo.com");
  lcd.clear();
  lcd.print("connecting to");
  lcd.setCursor(0,1);
  lcd.print("Yahoo finance");
  WiFiClientSecure client;
  client.setCACert(rootCA);
  const int httpPort = 443;
  if (!client.connect("ca.finance.yahoo.com", httpPort))
  {
    Serial.println("Connection failed!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("connection");
    lcd.setCursor(0,1);
    lcd.print("failed.");
    delay(1000);
    return;
  }
  else {
    Serial.println("Requesting URL: /YourStockUrlHere");
    client.println("GET https://ca.finance.yahoo.com/YourStockUrlHere");
    client.println("Host: ca.finance.yahoo.com");
    client.println("Connection: close");
    client.println();
  }
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("|Your Stock Ticker Here|");
      lcd.setCursor(0,1);
      lcd.print("headers recieved");
      break;
    }
  }
  float YourStockPrice = 0.00;
  while (client.available()) {
    String line = client.readStringUntil('<');
    if (line.startsWith("|Your Javascript Identifier Here|"))
    {
      line.remove(0, 73);
      YourStockPrice = line.toFloat();
      break;
    }
  }
  if (YourStockPrice == 0.00)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Retrying |Your Ticker Here|");
    delay(1000);
    return;
  }
  client.stop();
  delay(1000);

ifNotConnected:
  disconnectedCounter = 0;
  if(WiFi.status() != WL_CONNECTED)
  {
  lcd.clear();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  lcd.setCursor(0,0);
  lcd.print("Connecting to: ");
  lcd.setCursor(0,1);
  lcd.print(ssid);
  lcd.autoscroll();
  WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
    lcd.print(".");
    disconnectedCounter++;
    if (disconnectedCounter == 5)
    ESP.restart();
  }
  
  client.stop();
  Serial.println((String)"Your Stock Ticker: " + YourStockPrice);
  for(int j=0; j<=1; j++) //Adjust this loop based on how many stocks you want to display
  {
  lcd.clear();
  lcd.setCursor(0, 0);s
  lcd.print(String("SU.TO: $") + YourStockPrice);
  lcd.setCursor(0, 1);
  lcd.print((String)(((YourStockPrice - BuyPrice) / BuyPrice) * 100)+"%");
  delay(2000);
  }
}
