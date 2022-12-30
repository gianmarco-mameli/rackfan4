// the program works with fans connected on normally closed ports, so to turn off the fan the realy must be on

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <sstream>
#include <private.h>

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

const char *mqtt_broker = "192.168.1.1";
const int mqtt_port = 1883;

const char *client_id = "rackfan4";
const char *fan1_status_topic = "rackfan4/fan1/status";
const char *fan2_status_topic = "rackfan4/fan2/status";
const char *fan3_status_topic = "rackfan4/fan3/status";
const char *fan4_status_topic = "rackfan4/fan4/status";
const char *fan1_topic = "rackfan4/fan1/state";
const char *fan2_topic = "rackfan4/fan2/state";
const char *fan3_topic = "rackfan4/fan3/state";
const char *fan4_topic = "rackfan4/fan4/state";

int fan1_status = 0;
int fan2_status = 0;
int fan3_status = 0;
int fan4_status = 0;
char in_message[100];

#define fan1 5 //D1
#define fan2 4 //D2
#define fan3 12 //D6
#define fan4 13 //D7

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
  unsigned int i = 0;
  for (; i < length; i++)
  {
    in_message[i] = char(payload[i]);
  }
  in_message[i] = '\0';
  if (strcmp(topic, fan1_topic) == 0)
  {
    fan1_status = atoi(in_message);
  }
  if (strcmp(topic, fan2_topic) == 0)
  {
    fan2_status = atoi(in_message);
  }
  if (strcmp(topic, fan3_topic) == 0)
  {
    fan3_status = atoi(in_message);
  }
  if (strcmp(topic, fan4_topic) == 0)
  {
    fan4_status = atoi(in_message);
  }
}

void InitWiFi()
{
  if (WiFi.status() == WL_NO_SHIELD)
  {
    while (true)
      ;
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, pass);
  }
}

void InitMqtt()
{
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
}

void reconnect()
{
  while (!client.connected())
  {
    if (client.connect(client_id))
    {
      client.publish(fan1_status_topic, "connected");
      client.publish(fan2_status_topic, "connected");
      client.publish(fan3_status_topic, "connected");
      client.publish(fan4_status_topic, "connected");
      delay(500);
      client.publish(fan1_topic, "1", true);
      client.publish(fan2_topic, "1", true);
      client.publish(fan3_topic, "1", true);
      client.publish(fan4_topic, "1", true);
      delay(500);
      client.subscribe(fan1_topic, 1);
      client.subscribe(fan2_topic, 1);
      client.subscribe(fan3_topic, 1);
      client.subscribe(fan4_topic, 1);
    }
    else
    {
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(fan1, OUTPUT);
  pinMode(fan2, OUTPUT);
  pinMode(fan3, OUTPUT);
  pinMode(fan4, OUTPUT);
  delay(1000);
  digitalWrite(fan1, HIGH);
  digitalWrite(fan2, HIGH);
  digitalWrite(fan3, HIGH);
  digitalWrite(fan4, HIGH);
  delay(1000);
  InitWiFi();
  delay(1000);
  InitMqtt();
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  if (fan1_status == 0)
  {
    digitalWrite(fan1, LOW);
    client.publish(fan1_status_topic, "fan1 off");
  }
  else
  {
    digitalWrite(fan1, HIGH);
    client.publish(fan1_status_topic, "fan1 on");
  }

  if (fan2_status == 0)
  {
    digitalWrite(fan2, LOW);
    client.publish(fan2_status_topic, "fan2 off");
  }
  else
  {
    digitalWrite(fan2, HIGH);
    client.publish(fan2_status_topic, "fan2 on");
  }

  if (fan3_status == 0)
  {
    digitalWrite(fan3, LOW);
    client.publish(fan3_status_topic, "fan3 off");
  }
  else
  {
    digitalWrite(fan3, HIGH);
    client.publish(fan3_status_topic, "fan3 on");
  }

  if (fan4_status == 0)
  {
    digitalWrite(fan4, LOW);
    client.publish(fan4_status_topic, "fan4 off");
  }
  else
  {
    digitalWrite(fan4, HIGH);
    client.publish(fan4_status_topic, "fan4 on");
  }
  delay(1000);
}