
---

# ESP32 Code Deployment and Server Setup Documentation

## 1. Downloading Code to the ESP32

1. **Open the Arduino IDE**:
   - Open the `.ino` file located in the `esp32` directory using the Arduino IDE.

2. **Configure Secrets**:
   - Navigate to the `secrets.h` file.
   - Fill in the required information (such as WiFi credentials, Hardware Pins, etc.) as specified by your project.

3. **Obtain Server IP Address**:
   - Refer to the [Server Setup](#how-to-run-the-server) section to get the IP address of your server.
   - Update the IP address in your `secrets.h` to match the server IP.

4. **Flash the ESP32**:
   - Follow the hardware documentation to upload the code to your ESP32.

## 2. Wiring Instructions

1. **Add Wiring Picture**:  
   _(Insert image here)_

2. **Wiring Configuration**:  
   - Wiring configurations may vary depending on the ESP32 model you're using.
   - Refer to your specific hardware's datasheet for pin configurations.

## 3. Running the Server

1. **Prerequisites**:
   - Ensure you have [Node.js](https://nodejs.org/) and npm (Node Package Manager) installed on your machine.

2. **Install Server Dependencies**:
   - Navigate to the `server` directory in your terminal or command prompt.
   - Run the following command to install the necessary dependencies:
     ```bash
     npm install
     ```

3. **Start the Server**:
   - To run the server, execute the following command:
     ```bash
     node index.mjs
     ```

## 4. Retrieving Your Server IP Address

To obtain the server's IP address (required for configuration in `secrets.h`):

### On Windows:

1. Open the Command Prompt by searching for "cmd" in the start menu or pressing `Win + R` and typing `cmd`.
2. Run the following command:
   ```bash
   ipconfig
   ```
3. Locate your specific network adapter (e.g., Wi-Fi or Ethernet).
4. Use the IPv4 address associated with that network adapter. This will be the server's IP address.

---

### Additional Notes:
- Ensure your ESP32 and server are connected to the same network.
- If you encounter issues with the server or ESP32 connectivity, verify firewall settings and network configurations.

