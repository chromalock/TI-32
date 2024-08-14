// Project: TI-32 v0.1
// Author:  ChromaLock
// Date:    2024

#include "./passwords.h"
#include "./launcher.h"
#include <TICL.h>
#include <CBL2.h>
#include <TIVar.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <Preferences.h>

constexpr auto TIP = D1;
constexpr auto RING = D10;
constexpr auto MAXHDRLEN = 16;
constexpr auto MAXDATALEN = 4096;
constexpr auto MAXARGS = 5;
constexpr auto MAXSTRARGLEN = 256;
constexpr auto PICSIZE = 756;
constexpr auto PICVARSIZE = PICSIZE + 2;
constexpr auto SERVER = "http://192.168.0.4:8080";
constexpr auto PASSWORD = 42069;

CBL2 cbl;
Preferences prefs;

// whether or not the user has entered the password
bool unlocked = true;

// Arguments
int currentArg = 0;
char strArgs[MAXARGS][MAXSTRARGLEN];
double realArgs[MAXARGS];

// the command to execute
int command = -1;
// whether or not the operation has completed
bool status = 0;
// whether or not the operation failed
bool error = 0;
// error or success message
char message[MAXSTRARGLEN];
// http response
char response[MAXSTRARGLEN];
// image variable (96x63)
uint8_t frame[PICVARSIZE] = {
  PICSIZE & 0xff, PICSIZE >> 8,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x7f, 0x30, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x0f, 0xff, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xff, 0x83, 0xf8, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xf0, 0x01, 0xfc, 0x00, 0xfc, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xf0, 0x07, 0xf0, 0x00, 0x3e, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0f, 0xe0, 0x1f, 0xe0, 0x00, 0x1e, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3f, 0xe0, 0x7f, 0x80, 0x00, 0x0f, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x7f, 0xe0, 0xfe, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xfd, 0xe0, 0xf8, 0x01, 0xe0, 0x07, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xf1, 0xe0, 0xf0, 0x07, 0xf8, 0x07, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xe1, 0xe0, 0xc0, 0x0f, 0xfc, 0x07, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xc1, 0xe0, 0xc0, 0x3e, 0x7f, 0x03, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x07, 0xc1, 0xe0, 0xc0, 0xf8, 0x3f, 0xc3, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x81, 0xe0, 0xc3, 0xf0, 0x0f, 0xf3, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x81, 0xe0, 0xc7, 0xf0, 0x03, 0xfb, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x01, 0xe0, 0xdf, 0x7c, 0x00, 0xff, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0x01, 0xe0, 0xfc, 0x3f, 0x00, 0x7f, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0x01, 0xe0, 0xf8, 0x0f, 0x80, 0x1f, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0x01, 0xe0, 0xe0, 0x03, 0xe0, 0x07, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0x01, 0xe0, 0xc0, 0x01, 0xf8, 0x03, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x01, 0xe0, 0xc0, 0x01, 0xfe, 0x01, 0xe0, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x81, 0xe0, 0xc0, 0x01, 0x9f, 0x01, 0xe0, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x81, 0xf0, 0xc0, 0x01, 0x87, 0xc0, 0xf0, 0x00, 0x00,
  0x00, 0x00, 0x07, 0xc0, 0x7c, 0xc0, 0x01, 0x83, 0xc0, 0xf0, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xc0, 0x3f, 0xc0, 0x01, 0x83, 0xc0, 0xf0, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xe0, 0x0f, 0xc0, 0x01, 0x83, 0xc0, 0x70, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xf0, 0x03, 0xe0, 0x03, 0x83, 0xc0, 0x78, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xfc, 0x01, 0xf8, 0x0f, 0x83, 0xc0, 0x78, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0x00, 0x7c, 0x3f, 0x83, 0xc0, 0x70, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0x80, 0x1f, 0x7d, 0x83, 0xc0, 0xf0, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xef, 0xe0, 0x07, 0xf1, 0x83, 0xc0, 0xf0, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xe7, 0xf8, 0x07, 0xc1, 0x83, 0xc0, 0xf0, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xe1, 0xfc, 0x1f, 0x81, 0x83, 0xc1, 0xe0, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xe0, 0x7f, 0x3e, 0x01, 0x83, 0xc1, 0xe0, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xe0, 0x3f, 0xf8, 0x01, 0x83, 0xc3, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xf0, 0x0f, 0xe0, 0x07, 0x83, 0xcf, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xf0, 0x03, 0xc0, 0x1f, 0x83, 0xdf, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x3f, 0x83, 0xff, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0xfe, 0x03, 0xfe, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x7c, 0x00, 0x03, 0xfc, 0x03, 0xf8, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3e, 0x00, 0x0f, 0xf0, 0x07, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1f, 0x80, 0x1f, 0xc0, 0x07, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0f, 0xe0, 0xff, 0x80, 0x0f, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xff, 0xf8, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xfc, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x06, 0x7e, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void connect();
void disconnect();
void gpt();
void notes();
void answer();
void send();
void launcher();
void snap();

struct Command {
  int id;
  const char* name;
  int nargs;
  void (*command_fp)();
};

struct Command commands[] = {
  { 0, "connect", 0, connect },
  { 1, "disconnect", 0, disconnect },
  { 2, "gpt", 1, gpt },
  { 3, "notes", 1, notes },
  { 4, "send", 2, send },
  { 5, "launcher", 0, launcher },
  { 6, "answer", 1, answer },
  { 7, "snap", 0, snap }
};

constexpr int NUMCOMMANDS = sizeof(commands) / sizeof(struct Command);

uint8_t header[MAXHDRLEN];
uint8_t data[MAXDATALEN];

// lowercase letters make strings weird,
// so we have to truncate the string
void fixStrVar(char* str) {
  int end = strlen(str);
  for (int i = 0; i < end; ++i) {
    if (isLowerCase(str[i])) {
      --end;
    }
  }
  str[end] = '\0';
}

int onReceived(uint8_t type, enum Endpoint model, int datalen);
int onRequest(uint8_t type, enum Endpoint model, int* headerlen,
              int* datalen, data_callback* data_callback);

void startCommand(int cmd) {
  command = cmd;
  status = 0;
  error = 0;
  currentArg = 0;
  for (int i = 0; i < MAXARGS; ++i) {
    memset(&strArgs[i], 0, MAXSTRARGLEN);
    realArgs[i] = 0;
  }
  strncpy(message, "no command", MAXSTRARGLEN);
}

void setError(const char* err) {
  Serial.print("ERROR: ");
  Serial.println(err);
  error = 1;
  status = 1;
  command = -1;
  strncpy(message, err, MAXSTRARGLEN);
}

void setSuccess(const char* success) {
  Serial.print("SUCCESS: ");
  Serial.println(success);
  error = 0;
  status = 1;
  command = -1;
  strncpy(message, success, MAXSTRARGLEN);
}

int sendProgramVariable(const char* name, uint8_t* program, size_t variableSize);

void setup() {
  Serial.begin(115200);
  Serial.println("[CBL]");

  cbl.setLines(TIP, RING);
  cbl.resetLines();
  cbl.setupCallbacks(header, data, MAXDATALEN, onReceived, onRequest);
  // cbl.setVerbosity(true, (HardwareSerial *)&Serial);

  pinMode(TIP, INPUT);
  pinMode(RING, INPUT);

  Serial.println("[preferences]");
  prefs.begin("ccalc", false);
  auto reboots = prefs.getUInt("boots", 0);
  Serial.print("reboots: ");
  Serial.println(reboots);
  prefs.putUInt("boots", reboots + 1);
  prefs.end();

  strncpy(message, "default message", MAXSTRARGLEN);
  delay(100);
  memset(data, 0, MAXDATALEN);
  memset(header, 0, 16);
  Serial.println("[ready]");
}

void (*queued_action)() = NULL;

void loop() {
  if (queued_action) {
    // dont ask me why you need this, but it fails otherwise.
    // probably relates to a CBL2 timeout thing?
    delay(1000);
    Serial.println("executing queued actions");
    void (*tmp)() = queued_action;
    queued_action = NULL;
    tmp();
  }
  if (command >= 0 && command < NUMCOMMANDS) {
    for (int i = 0; i < NUMCOMMANDS; ++i) {
      if (commands[i].id == command && commands[i].nargs == currentArg) {
        Serial.print("processing command: ");
        Serial.println(commands[i].name);
        commands[i].command_fp();
      }
    }
  }
  cbl.eventLoopTick();
}

int onReceived(uint8_t type, enum Endpoint model, int datalen) {
  char varName = header[3];

  Serial.print("unlocked: ");
  Serial.println(unlocked);

  // check for password
  if (!unlocked && varName == 'P') {
    auto password = TIVar::realToLong8x(data, model);
    if (password == PASSWORD) {
      Serial.println("successful unlock");
      unlocked = true;
      return 0;
    } else {
      Serial.println("failed unlock");
    }
  }

  if (!unlocked) {
    return -1;
  }

  // check for command
  if (varName == 'C') {
    if (type != VarTypes82::VarReal) {
      return -1;
    }
    int cmd = TIVar::realToLong8x(data, model);
    if (cmd >= 0 && cmd < NUMCOMMANDS) {
      Serial.print("command: ");
      Serial.println(cmd);
      startCommand(cmd);
      return 0;
    } else {
      Serial.print("invalid command: ");
      Serial.println(cmd);
      return -1;
    }
  }

  if (currentArg >= MAXARGS) {
    Serial.println("argument overflow");
    setError("argument overflow");
    return -1;
  }
  switch (type) {
    case VarTypes82::VarString:
      Serial.print("len: ");
      strncpy(strArgs[currentArg++], TIVar::strVarToString8x(data, model).c_str(), MAXSTRARGLEN);
      fixStrVar(strArgs[currentArg - 1]);
      Serial.print("Str");
      Serial.print(currentArg - 1);
      Serial.print(" ");
      Serial.println(strArgs[currentArg - 1]);
      break;
    case VarTypes82::VarReal:
      realArgs[currentArg++] = TIVar::realToFloat8x(data, model);
      Serial.print("Real");
      Serial.print(currentArg - 1);
      Serial.print(" ");
      Serial.println(realArgs[currentArg - 1]);
      break;
    default:
      // maybe set error here?
      return -1;
  }
  return 0;
}

uint8_t frameCallback(int idx) {
  return frame[idx];
}

char varIndex(int idx) {
  return '0' + (idx == 9 ? 0 : (idx + 1));
}

int onRequest(uint8_t type, enum Endpoint model, int* headerlen, int* datalen, data_callback* data_callback) {
  char varName = header[3];
  char strIndex = header[4];
  char strname[5] = { 'S', 't', 'r', varIndex(strIndex), 0x00 };
  char picname[5] = { 'P', 'i', 'c', varIndex(strIndex), 0x00 };
  Serial.print("request for ");
  Serial.println(varName == 0xaa ? strname : varName == 0x60 ? picname
                                                             : (const char*)&header[3]);
  memset(header, 0, sizeof(header));
  switch (varName) {
    case 0x60:
      if (type != VarTypes82::VarPic) {
        return -1;
      }
      *datalen = PICVARSIZE;
      TIVar::intToSizeWord(*datalen, &header[0]);
      header[2] = VarTypes82::VarPic;
      header[3] = 0x60;
      header[4] = strIndex;
      *data_callback = frameCallback;
      break;
    case 0xAA:
      if (type != VarTypes82::VarString) {
        return -1;
      }
      // TODO right now, the only string variable will be the message, but ill need to allow for other vars later
      *datalen = TIVar::stringToStrVar8x(String(message), data, model);
      TIVar::intToSizeWord(*datalen, header);
      header[2] = VarTypes82::VarString;
      header[3] = 0xAA;
      // send back as same variable that was requested
      header[4] = strIndex;
      *headerlen = 13;
      break;
    case 'E':
      if (type != VarTypes82::VarReal) {
        return -1;
      }
      *datalen = TIVar::longToReal8x(error, data, model);
      TIVar::intToSizeWord(*datalen, header);
      header[2] = VarTypes82::VarReal;
      header[3] = 'E';
      header[4] = '\0';
      *headerlen = 13;
      break;
    case 'S':
      if (type != VarTypes82::VarReal) {
        return -1;
      }
      *datalen = TIVar::longToReal8x(status, data, model);
      TIVar::intToSizeWord(*datalen, header);
      header[2] = VarTypes82::VarReal;
      header[3] = 'S';
      header[4] = '\0';
      *headerlen = 13;
      break;
    default:
      return -1;
  }
  return 0;
}

int makeRequest(String url, char* result, int resultLen) {
  memset(result, 0, resultLen);
  WiFiClient client;
  HTTPClient http;

  Serial.println(url);

  http.begin(client, url.c_str());

  // Send HTTP GET request
  int httpResponseCode = http.GET();
  auto httpResult = http.getString();
  Serial.print("http response: ");
  Serial.println(httpResult);

  if (httpResponseCode > 0) {
    strncpy((char*)result, httpResult.c_str(), MAXSTRARGLEN);
    http.end();
    return 0;
  }
  http.end();

  return httpResponseCode;
}

void connect() {
  const char* ssid = _SSID;
  const char* pass = _PASS;
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("PASS: ");
  Serial.println("<hidden>");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    if (WiFi.status() == WL_CONNECT_FAILED) {
      setError("failed to connect");
      return;
    }
  }
  setSuccess("connected");
}

void disconnect() {
  WiFi.disconnect(true);
  setSuccess("disconnected");
}

void gpt() {
  const char* prompt = strArgs[0];
  Serial.print("prompt: ");
  Serial.println(prompt);

  auto url = String(SERVER) + String("/gpt/ask?question=") + urlEncode(String(prompt));

  if (makeRequest(url, response, MAXSTRARGLEN)) {
    setError("error making request");
    return;
  }

  Serial.print("response: ");
  Serial.println(response);

  setSuccess(response);
}

void notes() {
  const char* note = strArgs[0];

  auto url = String(SERVER) + String("/notes?name=") + urlEncode(String(note));

  if (makeRequest(url, response, MAXSTRARGLEN)) {
    setError("error making request");
    return;
  }

  Serial.print("response: ");
  Serial.println(response);

  setSuccess(response);
}

void send() {
  const char* recipient = strArgs[0];
  const char* message = strArgs[1];
  Serial.print("sending \"");
  Serial.print(message);
  Serial.print("\" to \"");
  Serial.print(recipient);
  Serial.println("\"");
  setSuccess("OK: sent");
}

void _sendLauncher() {
  sendProgramVariable("LAUNCHER", __launcher_var, __launcher_var_len);
}

void launcher() {
  // we have to queue this action, since otherwise the transfer fails
  // due to the CBL2 library still using the lines
  queued_action = _sendLauncher;
  setSuccess("queued transfer");
}

void answer() {
  const char* question = strArgs[0];
  setError("not implemented");
}

void snap() {
  // this is a camera thing
  setError("pictures not supported.");
}

/// OTHER FUNCTIONS

int sendProgramVariable(const char* name, uint8_t* program, size_t variableSize) {
  Serial.print("transferring: ");
  Serial.print(name);
  Serial.print("(");
  Serial.print(variableSize);
  Serial.println(")");

  int dataLength = 0;

  // IF THIS ISNT SET TO COMP83P, THIS DOESNT WORK
  // seems like ti-84s cant silent transfer to each other
  uint8_t msg_header[4] = { COMP83P, RTS, 13, 0 };

  uint8_t rtsdata[13] = { variableSize & 0xff, variableSize >> 8, VarTypes82::VarProgram, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  int nameSize = strlen(name);
  if (nameSize == 0) {
    return 1;
  }
  memcpy(&rtsdata[3], name, min(nameSize, 8));

  auto rtsVal = cbl.send(msg_header, rtsdata, 13);
  if (rtsVal) {
    Serial.print("rts return: ");
    Serial.println(rtsVal);
    return rtsVal;
  }

  cbl.resetLines();
  auto ackVal = cbl.get(msg_header, NULL, &dataLength, 0);
  if (ackVal || msg_header[1] != ACK) {
    Serial.print("ack return: ");
    Serial.println(ackVal);
    return ackVal;
  }

  auto ctsRet = cbl.get(msg_header, NULL, &dataLength, 0);
  if (ctsRet || msg_header[1] != CTS) {
    Serial.print("cts return: ");
    Serial.println(ctsRet);
    return ctsRet;
  }

  msg_header[1] = ACK;
  msg_header[2] = 0x00;
  msg_header[3] = 0x00;
  ackVal = cbl.send(msg_header, NULL, 0);
  if (ackVal || msg_header[1] != ACK) {
    Serial.print("ack cts return: ");
    Serial.println(ackVal);
    return ackVal;
  }

  msg_header[1] = DATA;
  msg_header[2] = variableSize & 0xff;
  msg_header[3] = (variableSize >> 8) & 0xff;
  auto dataRet = cbl.send(msg_header, program, variableSize);
  if (dataRet) {
    Serial.print("data return: ");
    Serial.println(dataRet);
    return dataRet;
  }

  ackVal = cbl.get(msg_header, NULL, &dataLength, 0);
  if (ackVal || msg_header[1] != ACK) {
    Serial.print("ack data: ");
    Serial.println(ackVal);
    return ackVal;
  }

  msg_header[1] = EOT;
  msg_header[2] = 0x00;
  msg_header[3] = 0x00;
  auto eotVal = cbl.send(msg_header, NULL, 0);
  if (eotVal) {
    Serial.print("eot return: ");
    Serial.println(eotVal);
    return eotVal;
  }

  Serial.print("transferred: ");
  Serial.println(name);
  return 0;
}