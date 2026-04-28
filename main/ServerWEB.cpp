#include <WiFi.h>
#include <esp_wifi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <time.h>
#include "ServerWEB.h"
#include "Model_Data.h"


// ================== CHANGE THESE ==================
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int  daylightOffset_sec = 0;


// ====================DATA==========================
String status_web = "";
String galons_web = "";
String percent_web = "";
String total_galons = "";


WebServer server(80);


// ---------- WEB ----------
const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32 Ambient</title>

<link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;600;700&display=swap" rel="stylesheet">

<style>
:root{
  --glass: rgba(255,255,255,0.045);
  --glass2: rgba(255,255,255,0.07);
  --border: rgba(255,255,255,0.08);
  --text: rgba(255,255,255,0.88);
  --muted: rgba(255,255,255,0.55);
}

*{ box-sizing:border-box; }

html,body{
  height:100%;
  margin:0;
  font-family:Inter, system-ui, sans-serif;
  color:var(--text);
}

body{
  background:
    radial-gradient(900px 600px at 20% 10%, rgba(120,120,255,0.12), transparent 60%),
    radial-gradient(900px 600px at 80% 90%, rgba(255,120,200,0.10), transparent 60%),
    linear-gradient(135deg, #0b1020, #0e1628);
}

.wrap{
  height:100%;
  display:grid;
  place-items:center;
  padding:24px;
}

.card{
  width:min(880px,100%);
  display:grid;
  grid-template-columns:1.2fr .8fr;
  gap:16px;
  padding:18px;
  border-radius:24px;
  background:linear-gradient(180deg,var(--glass2),var(--glass));
  border:1px solid var(--border);
  backdrop-filter:blur(12px);
}

@media(max-width:820px){
  .card{ grid-template-columns:1fr; }
}

/* PANEL PRINCIPAL */
.panelPercent{
  height:220px;
}

/* CHIP GRANDE */
.chip{
  position:relative;
  width:100%;
  height:100%;
  border-radius:22px;
  border:1px solid var(--border);
  background:rgba(255,255,255,0.06);
  display:flex;
  align-items:center;
  justify-content:center;
}

/* VALOR CENTRAL */
.chip-value{
  font-size:clamp(5rem,8vw,5rem);
  font-weight:1000;
  letter-spacing:.04em;
}


/* PANEL SECUNDARIO */
.panel{
  padding:18px;
  border-radius:18px;
  background:rgba(255,255,255,0.04);
  border:1px solid var(--border);
}

.title{
  font-size:.85rem;
  letter-spacing:.1em;
  text-transform:uppercase;
  color:var(--muted);
  margin-bottom:10px;
}

.kv{
  display:flex;
  justify-content:space-between;
  padding:12px 14px;
  margin-top:10px;
  border-radius:14px;
  background:rgba(255,255,255,0.04);
  border:1px solid var(--border);
}

.k{ color:var(--muted); }
.v{ font-weight:600; }

</style>
</head>

<body>
<div class="wrap">
  <div class="card">

    <!-- PANEL PRINCIPAL -->
    <div class="panelPercent">
      <div class="chip">
        <div class="chip-value" id="per_id">0.00</div>
      </div>
    </div>

    <!-- PANEL DETALLES -->
    <div class="panel">
      <div class="title">Detalles</div>

      <div class="kv"><div class="k">Galones</div><div class="v" id="ga_id">--</div></div>
      <div class="kv"><div class="k">Galones Total</div><div class="v" id="tm_id">--</div></div>
      <div class="kv"><div class="k">Estado</div><div class="v" id="st_id">--</div></div>
    </div>

  </div>
</div>

<script>
async function update(){
  try{
    ga_id.textContent = await (await fetch('/galons')).text();
    tm_id.textContent = await (await fetch('/time')).text();
    st_id.textContent = await (await fetch('/status')).text();
    per_id.textContent = await (await fetch('/percent')).text();
  }catch(e){}
}

update();
setInterval(update, 1000);
</script>

</body>
</html>
)rawliteral";



void handleRoot() {
  server.send_P(200, "text/html", MAIN_page);
}

void TaskUpdateServer(void *pvParameters)
{

  configTime(gmtOffset_sec,daylightOffset_sec,ntpServer);

  server.on("/", handleRoot);
  server.on("/time",[](){server.send(200,"text/plain",total_galons);});
  server.on("/percent",[](){server.send(200,"text/plain", percent_web);});
  server.on("/galons",[](){server.send(200,"text/plain", galons_web);});
  server.on("/status",[](){server.send(200,"text/plain", status_web);});

  server.begin();


  for (;;)
  {
    if (WL_CONNECTED){
      if (xSemaphoreTake(ActuatorsMutex, ( TickType_t ) 10 ) == pdTRUE){
        status_web = status; 
        xSemaphoreGive( ActuatorsMutex );
      }
      if (xSemaphoreTake(TankMutex, ( TickType_t ) 10 ) == pdTRUE){
        total_galons = String(CAPACITY_GALONS);
        galons_web = String(current_galons);
        percent_web = String(sensor_mm_read);
        xSemaphoreGive( TankMutex );
      }
      server.handleClient();
      vTaskDelay(pdMS_TO_TICKS(100));
    }else{
      vTaskDelay(pdMS_TO_TICKS(2000));
    }
  }
}




