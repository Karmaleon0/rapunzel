#include "HttpHandler.h"

HttpHandler::HttpHandler(uint16_t port)
  : _server(port) {
}

void HttpHandler::setup() {
  _server.begin();
  Serial.println("HTTP Server started.");
}

void HttpHandler::loop() {
  WiFiClient client = _server.available();
  if (client) {
    Serial.println("New client connected.");
    String request = "";
    while (client.connected() && client.available()) {
      char c = client.read();
      request += c;
      if (c == '\n') break;
    }
    Serial.println(request);

    if (request.indexOf("GET /H") >= 0) {
      if (_ledOnCallback) {
        _ledOnCallback();
      }
      Serial.println("LED turned on.");
    } 
    else if (request.indexOf("GET /O") >= 0) {
      if (_ledOffCallback) {
        _ledOffCallback();
      }
      Serial.println("LED turned off.");
    } 
    else if (request.indexOf("GET /L") >= 0) {
      if (_keyThrowCallback) {
        _keyThrowCallback();
      }
      Serial.println("Key throw activated.");
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("<a href=\"/H\">Turn LED On</a><br>");
    client.println("<a href=\"/O\">Turn LED Off</a><br>");
    client.println("<a href=\"/L\">Throw Key</a><br>");
    client.println();
    client.stop();
    Serial.println("Client disconnected.");
  }
}

void HttpHandler::onLedOn(Callback cb) {
  _ledOnCallback = cb;
}

void HttpHandler::onLedOff(Callback cb) {
  _ledOffCallback = cb;
}

void HttpHandler::onKeyThrow(Callback cb) {
  _keyThrowCallback = cb;
}