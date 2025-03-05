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
    client.println("<!DOCTYPE html>");
    client.println("<html>");
    client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1'>");
    client.println("<style>");
    client.println("body { font-family: Arial, sans-serif; text-align: center; }");
    client.println(".button { display: block; width: 80%; max-width: 300px; padding: 15px; margin: 10px auto; font-size: 20px; text-decoration: none; color: white; background: #007BFF; border-radius: 5px; }");
    client.println(".button:hover { background: #0056b3; }");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<h1>Locksmith</h1>");
    client.println("<a class='button' href='/H'>Turn LED On</a>");
    client.println("<a class='button' href='/O'>Turn LED Off</a>");
    client.println("<a class='button' href='/L'>Throw Key</a>");
    client.println("</body>");
    client.println("</html>");
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
