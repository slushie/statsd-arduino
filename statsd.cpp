#include "Arduino.h"
#include "Statsd.h"
#include "Ethernet.h"
#include "EthernetUDP.h"
#include "IPAddress.h"
#include "string.h"

EthernetUDP statsd;

StatsD::StatsD(IPAddress ip, int port, int localPort)
{
  _ip = ip;
  _port = port;
  statsd.begin(localPort);
}

void StatsD::increment(const char *metric) {
  StatsD::_send(metric, ":1|c");
}

void StatsD::decrement(const char *metric) {
  StatsD::_send(metric, ":-1|c");
}

void StatsD::timing(const char *metric, int ms) {
  char timing[sizeof(&metric) + 20];
  strcpy(timing, metric);
  strcat(timing, ":");
  strcat(timing, (char*)ms);
  StatsD::_send(timing, "|ms");
}

void StatsD::_send(const char *metric, const char *cmd) {
  int buffer_len = sizeof(&metric) + 4;
  char buffer[buffer_len];
  strcpy(buffer, metric);
  strcat(buffer, cmd);

  const uint8_t* msg = (uint8_t*)buffer;
  statsd.beginPacket(_ip, _port);
  statsd.write(msg, sizeof(&msg));
  statsd.endPacket();
}
