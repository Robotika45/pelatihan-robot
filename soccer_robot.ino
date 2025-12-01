#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>

AsyncWebServer server(80);
Servo kicker;

const char* ssid = "RobotSoccer2";
const char* password = "12345678";
IPAddress local_IP(192, 168, 20, 1);
IPAddress gateway(local_IP);
IPAddress subnet(255, 255, 255, 0);

#define ENA 0
#define IN1 2
#define IN2 14
#define IN3 12
#define IN4 13
#define ENB 15
#define SERVO 4

bool tendang = false;

void setMotor(float left, float right) {
  left = constrain(left, -1.0, 1.0);
  right = constrain(right, -1.0, 1.0);

  int lSpeed = abs(left * 255);
  int rSpeed = abs(right * 255);

  digitalWrite(IN1, left > 0);
  digitalWrite(IN2, left < 0);
  digitalWrite(IN3, right > 0);
  digitalWrite(IN4, right < 0);

  analogWrite(ENA, lSpeed);
  analogWrite(ENB, rSpeed);
}

void stopMotor() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  kicker.attach(SERVO);
  kicker.write(30);

  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println("=== SOCCER ROBOT READY ===");
  Serial.print("SSID: "); Serial.println(ssid);
  Serial.print("IP: "); Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "text/html", getWebPage());
  });

  server.on("/joystick", HTTP_GET, [](AsyncWebServerRequest *req) {
    if (req->hasParam("x") && req->hasParam("y")) {
      float x = req->getParam("x")->value().toFloat();
      float y = req->getParam("y")->value().toFloat();

      float turnFactor = 0.6;
      x *= turnFactor;

      float right = y + x;
      float left = y - x;

      left = constrain(left, -1.0, 1.0);
      right = constrain(right, -1.0, 1.0);
      setMotor(left, right);
    }
    req->send(200, "text/plain", "OK");
  });

  server.on("/kick", HTTP_GET, [](AsyncWebServerRequest *req) {
    kicker.write(120);
    delay(200);
    kicker.write(30);
    req->send(200, "text/plain", "kick");
  });

  server.begin();
}

void loop() {}

String getWebPage() {
  return R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
<title>SOCCER</title>
<style>
html, body {
  margin: 0; padding: 0; overflow: hidden;
  width: 100%; height: 100%;
  background: #0a0a0a; color: white;
  touch-action: none; user-select: none;
  font-family: sans-serif;
}
#container {
  display: flex; flex-direction: row; justify-content: space-between;
  align-items: center; width: 100vw; height: 100vh;
  position: fixed; top: 0; left: 0;
}
#joystick {
  position: relative;
  width: 220px; height: 220px;
  background: #1a1a1a;
  border: 2px solid #333; border-radius: 50%;
  margin-left: 5vw; margin-bottom: 5vh;
  align-self: flex-end;
}
#stick {
  width: 90px; height: 90px;
  background: #00bcd4;
  border-radius: 50%; position: absolute;
  left: 65px; top: 65px;
}
#kick, #emergency {
  width: 150px; height: 150px;
  border-radius: 50%;
  color: white; font-size: 24px; font-weight: bold;
  margin-right: 5vw; margin-bottom: 5vh;
  align-self: flex-end;
  border: none; outline: none;
  transition: transform 0.1s;
}
#kick {
  background: #e91e63;
}
#kick:active {
  background: #ad1457;
  transform: scale(0.95);
}
#emergency {
  background: #ff5722;
  margin-right: 5vw;
}
#emergency:active {
  background: #bf360c;
  transform: scale(0.95);
}
#rightButtons {
  display: flex; flex-direction: column;
  align-items: flex-end;
  margin-right: 5vw; margin-bottom: 5vh;
}

/* Tombol fullscreen */
#fullscreen-btn {
  position: fixed;
  top: 10px; right: 10px;
  z-index: 1000;
  background: rgba(0,0,0,0.6);
  color: white;
  border: none;
  padding: 10px 14px;
  border-radius: 8px;
  font-size: 18px;
  cursor: pointer;
  transition: 0.2s;
}
#fullscreen-btn:active {
  background: rgba(0,0,0,0.8);
  transform: scale(0.95);
}

/* Peringatan rotasi */
#rotateWarning {
  display: none;
  position: fixed; top: 0; left: 0;
  width: 100vw; height: 100vh;
  background: black; color: white;
  z-index: 9999;
  justify-content: center; align-items: center;
  flex-direction: column; text-align: center;
  font-size: 24px;
}
@media (orientation: portrait) {
  #rotateWarning { display: flex; }
  #container, #fullscreen-btn { display: none; }
}
</style>
</head>
<body>
  <div id="rotateWarning">📱<br>Putar HP ke mode <b>LANDSCAPE</b></div>

  <button id="fullscreen-btn" onclick="toggleFullscreen()">⛶ FULLSCREEN</button>

  <div id="container">
    <div id="joystick"><div id="stick"></div></div>
    <div id="rightButtons">
      <button id="kick" ontouchstart="kick()">KICK</button>
      <button id="emergency" ontouchstart="emergency()">🚨</button>
    </div>
  </div>

<script>
let joy = document.getElementById("joystick");
let stick = document.getElementById("stick");
let joyCenter = { x: 110, y: 110 };
let maxRadius = 80;
let lastSent = 0;

joy.addEventListener("touchmove", handleMove);
joy.addEventListener("touchend", resetStick);

function handleMove(e) {
  e.preventDefault();
  let touch = e.touches[0];
  let rect = joy.getBoundingClientRect();
  let x = touch.clientX - rect.left - joyCenter.x;
  let y = touch.clientY - rect.top - joyCenter.y;
  let dist = Math.sqrt(x*x + y*y);
  if (dist > maxRadius) {
    x *= maxRadius / dist;
    y *= maxRadius / dist;
  }
  stick.style.left = (joyCenter.x + x - 45) + "px";
  stick.style.top = (joyCenter.y + y - 45) + "px";

  let normX = (x / maxRadius).toFixed(2);
  let normY = (-y / maxRadius).toFixed(2);

  let now = Date.now();
  if (now - lastSent > 100) {
    fetch(`/joystick?x=${normX}&y=${normY}`);
    lastSent = now;
  }
}

function resetStick() {
  stick.style.left = "65px";
  stick.style.top = "65px";
  fetch("/joystick?x=0&y=0");
}

function kick() {
  fetch("/kick");
}

// === FULLSCREEN ===
async function toggleFullscreen() {
  const btn = document.getElementById("fullscreen-btn");
  if (!document.fullscreenElement) {
    await document.documentElement.requestFullscreen();
    if (screen.orientation && screen.orientation.lock)
      await screen.orientation.lock("landscape").catch(()=>{});
    btn.textContent = "⤢ EXIT";
  } else {
    await document.exitFullscreen();
    btn.textContent = "⛶ FULLSCREEN";
  }
}
document.addEventListener("touchstart", () => {
  if (!document.fullscreenElement) toggleFullscreen();
}, { once: true });
document.addEventListener("fullscreenchange", () => {
  const btn = document.getElementById("fullscreen-btn");
  btn.textContent = document.fullscreenElement ? "⤢ EXIT" : "⛶ FULLSCREEN";
});
</script>
</body>
</html>
)rawliteral";
}
