# 🚀 PlatformIO Setup for NodeMCU v2 with WiFi SoftAP and OTA

This project provides a **PlatformIO setup** for the **NodeMCU v2 (ESP8266)**, featuring:
- **WiFi SoftAP (Access Point Mode)**
- **Over-the-Air (OTA) Updates** via `ArduinoOTA`
- **DNS Server for local access**

## 📜 Features
- **WiFi SoftAP:** The ESP8266 creates a local WiFi network (`Rapunzel`)
- **DNS Server:** Redirects all DNS requests to the ESP8266
- **OTA Updates:** Supports firmware updates via WiFi
- **PlatformIO Support:** Easily configurable via `platformio.ini`

## 📂 Project Structure

📦 your_project_directory
┣ 📜 platformio.ini      # PlatformIO configuration file
┣ 📜 main.cpp            # Main application entry point
┣ 📂 src
┃ ┣ 📜 OTAHandler.h      # OTA update handler (header)
┃ ┣ 📜 OTAHandler.cpp    # OTA update handler (implementation)
┃ ┣ 📜 Wifi.h            # WiFi and SoftAP handler (header)
┃ ┗ 📜 Wifi.cpp          # WiFi and SoftAP handler (implementation)
┗ 📜 README.md           # This file

## 🔧 Setup & Installation

### 📥 Prerequisites
- Install [PlatformIO](https://platformio.org/)
- A **NodeMCU v2 (ESP8266)** module
- A **USB cable** to flash the initial firmware

### ⚙️ Clone this repository
```sh
git clone https://github.com/TobiFessler/rapunzel.git
cd rapunzel
```

🛠 Configure WiFi Credentials

Update the Wifi.cpp file with your preferred SSID and password:

const char *Ssid = "Rapunzel";
const char *Password = "rapunzel";

🚀 Flash the Code

Connect your ESP8266 via USB and run:

pio run --target upload

Once flashed, the ESP8266 will start the WiFi SoftAP and allow OTA updates.

📡 OTA Updates

After the first flash, you can update the firmware over WiFi using:

pio run --target upload --upload-port 192.168.4.1

🖥 Usage
	1.	Connect to the WiFi “Rapunzel” (password: rapunzel).
	2.	The ESP8266 will be accessible at 192.168.4.1.
	3.	OTA updates can be performed remotely.

🛠 Troubleshooting

🔴 OTA Upload Fails
	•	Ensure you are connected to the correct WiFi network.
	•	Check if the ESP8266 is powered on.
	•	Verify that the upload port (192.168.4.1) is correct.

🔴 Serial Monitor Output

To debug via the serial monitor:

pio device monitor

💡 Contributing

Pull requests and suggestions are welcome! Feel free to open an issue if you find any bugs or have feature requests.

🔗 Resources
	•	ESP8266 Arduino OTA Docs
	•	PlatformIO Documentation
