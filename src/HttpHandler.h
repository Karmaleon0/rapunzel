#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <Arduino.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <functional>

class HttpHandler {
public:
  using Callback = std::function<void()>;

  HttpHandler(uint16_t port = 80);

  void setup();

  void loop();

  void onLedOn(Callback cb);
  void onLedOff(Callback cb);
  void onKeyThrow(Callback cb);

private:
  WiFiServer _server;
  Callback _ledOnCallback;
  Callback _ledOffCallback;
  Callback _keyThrowCallback;
};

#endif