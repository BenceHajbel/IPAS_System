# IPAS — Intelligent Pollution Analytic System
## ROS 2 Szimuláció — Fejlesztői Dokumentáció

**Utolsó frissítés:** 2026-08-22
**Környezet:** ROS 2 Lyrical (Ubuntu 26.04 "Resolute")
**Projekt könyvtár:** `~/GitHub_repose/IPAS/Simulation`

---

## 1. Áttekintés

Ez a dokumentum az IPAS (Intelligent Pollution Analytic System) ROS 2 alapú
szimulációjának jelenlegi, működő állapotát írja le. A cél egy valós
ESP32 + BME680 mérőrendszer viselkedésének szimulálása ROS 2 alatt, az
adatok topicon keresztüli publikálása, fájlba mentése, majd egy webes
műszerfalon (dashboard) történő élő megjelenítése.

A rendszer **teljes egészében szimulált adatot használ** — nincs valós
hardver bekötve. A `bme680_node` véletlenszerűen ingadozó, de reális
tartományban mozgó mérési adatokat generál, alapértékek körül.

---

## 2. Architektúra

```
BME680 (szimulált, random ingadozás)
        |
        v
  ipas_bme680 node  (ROS 2, Python, ament_python)
        |
        v
  IPASMeasurement.msg  (ipas_interfaces csomag)
        |
        v
  /ipas/measurement  topic
        |
        +----------------------------+-----------------------------+
        |                            |                             |
        v                            v                             v
  ipas_logger node            rosbridge_websocket           (jovobeli fogyasztok)
  (CSV-be ment)                (WebSocket hid, 9090)
        |                            |
        v                            v
  data/measurements.csv       roslibjs (bongeszo)
                                     |
                                     v
                          ipas_webpage_live.html
                          (elo dashboard)
```

---

## 3. ROS 2 csomagok

### 3.1 `ipas_interfaces`

Interfész-csomag, amely az egyetlen egyedi üzenettípust definiálja.

**`msg/IPASMeasurement.msg`:**
```
float32 temperature
float32 humidity
float32 pressure
float32 gas_resistance
```

> Megjegyzés: a message jelenleg **nem tartalmaz `Header`-t** (időbélyeget).
> Ez egy tudatosan nyitva hagyott pont — az időbélyeget jelenleg a
> `ipas_logger` node generálja mentéskor, nem maga a message hordozza.

**Build típus:** `ament_cmake` + `rosidl_generate_interfaces`.

### 3.2 `ipas_bme680`

A szimulált BME680 szenzort megvalósító node.

- **Node név:** `bme680_node`
- **Publikál:** `/ipas/measurement` (típus: `ipas_interfaces/msg/IPASMeasurement`)
- **Frissítési frekvencia:** 1 Hz (`create_timer(1.0, ...)`)
- **Adatgenerálás:** alapértékek körüli véletlenszerű ingadozás
  (`random.uniform`), nem valós szenzoradat:
  - `temperature`: 24.5 °C ± 0.5
  - `humidity`: 48.0 % ± 2.0
  - `pressure`: 1013.2 hPa ± 1.5
  - `gas_resistance`: 12000 Ω ± 500

**Indítás:**
```bash
cd ~/GitHub_repose/IPAS/Simulation
source install/setup.bash
ros2 run ipas_bme680 bme680_node
```

### 3.3 `ipas_logger`

Feliratkozó (subscriber) node, amely minden beérkező mérést fájlba ment.

- **Node név:** `ipas_logger_node`
- **Feliratkozik:** `/ipas/measurement`
- **Kimeneti fájl:** `~/GitHub_repose/IPAS/Simulation/data/measurements.csv`
- **CSV oszlopok:** `timestamp,temperature,humidity,pressure,gas_resistance`
- Az időbélyeget a node maga generálja (`datetime.now().isoformat()`) —
  mikroszekundum pontossággal, **nem tartalmaz időzóna-jelzést** (naiv,
  helyi idő).

**Indítás:**
```bash
cd ~/GitHub_repose/IPAS/Simulation
source install/setup.bash
ros2 run ipas_logger logger_node
```

---

## 4. rosbridge_suite — WebSocket híd

A böngésző nem tud közvetlenül ROS 2 topicra feliratkozni, ezért a
`rosbridge_suite` csomagot használjuk hídként.

**Telepített csomag:** `ros-lyrical-rosbridge-suite` (4.2.1)

**Indítás — FONTOS: mindkét `source`-nak le kell futnia, ebben a sorrendben:**
```bash
cd ~/GitHub_repose/IPAS/Simulation
source /opt/ros/lyrical/setup.bash
source install/setup.bash
ros2 launch rosbridge_server rosbridge_websocket_launch.xml
```

> Ha csak a rendszer ROS 2-t forrásoljuk (`/opt/ros/lyrical/setup.bash`),
> de a saját workspace-t (`install/setup.bash`) nem, a rosbridge nem
> találja az `ipas_interfaces` csomagot, és ezzel a hibával fut el:
> `Unable to import ipas_interfaces.msg from package ipas_interfaces`.

**Port:** `9090` (WebSocket), mindkét interfészen (IPv4 és IPv6) figyel.

---

## 5. Webes dashboard integráció

A dashboard forrásfájlja (`~/GitHub_repose/IPAS/software/webpage/ipas_webpage.html`)
**változatlan marad** — ez az eredeti design/prototípus. A ROS-integráció
egy **másolaton** történik:

```
~/GitHub_repose/IPAS/Simulation/web/
├── ipas_webpage_live.html   (élő, ROS-hoz kötött verzió)
└── roslib.min.js            (helyi roslibjs könyvtár, v1.3.0)
```

> **Miért helyi fájl, nem CDN?** A `cdnjs.cloudflare.com`-on elérhető
> `roslib.min.js` URL nem szolgáltatott érvényes tartalmat (hibás MIME
> típus / HTML hibaoldal érkezett JS helyett). Emiatt a hivatalos npm
> csomagból (`roslib@1.3.0`) letöltött, ellenőrzött `roslib.min.js`
> fájlt helyben tároljuk — ez egyben stabilabb is, nincs hálózati
> függőség a szimuláció futtatásához.

### 5.1 Élő komponensek (a `/ipas/measurement` topicból táplálva)

| Dashboard elem | Állapot | Forrás |
|---|---|---|
| **01 Aktuális Állapot** kártyák (Hőmérséklet, Páratartalom, Légnyomás, Levegőminőség) | ✅ Élő | `/ipas/measurement` (rosbridge WebSocket) |
| **ONLINE/OFFLINE** státusz | ✅ Élő | WebSocket kapcsolat + 10 mp-es időtúllépés |
| **02 Mérési Előzmények** grafikon | ✅ Élő | `data/measurements.csv` (HTTP fetch, 5 mp-enként frissül) |
| **03 Légnyomás — aktuális érték** (nagy szám) | ✅ Élő | `/ipas/measurement` |
| **03 Légnyomás — trend/delta/grafikon** | ⚠️ Szimulált | előre generált, statikus adatsor |
| **04 Hivatalos adatok összehasonlítása** (táblázat + radar grafikon) | ✅ Élő (IPAS oldal) | `measurementData` objektum |
| **04 Hivatalos adatok** (referencia oszlop) | Szimulált (fix) | `officialData` objektum — ez a hivatalos mérőállomás adata, jelenleg kézzel megadott konstans |

### 5.2 ONLINE/OFFLINE logika

- Minden beérkező `/ipas/measurement` üzenet `markOnline()`-t hív, ami
  resetel egy 10 másodperces időzítőt.
- Ha 10 másodpercig nem érkezik új üzenet, a felület automatikusan
  **OFFLINE**-ra vált (`markOffline()`).
- Ha maga a WebSocket kapcsolat megszakad (`ros.on('close')`) vagy hiba
  történik (`ros.on('error')`), a felület azonnal OFFLINE-t jelez.

### 5.3 Levegőminőség (VOC) kategorizálás

A BME680 csak `gas_resistance`-t (Ω) szolgáltat, nincs beépített VOC index.
A kártyán megjelenő kategória a `gas_resistance` kΩ-ban kifejezett értéke
alapján kerül besorolásra:

| Kategória | Tartomány (kΩ) |
|---|---|
| Kiváló | > 150 |
| Jó | 50 – 150 |
| Közepes | 15 – 50 |
| Rossz | 5 – 15 |
| Nagyon rossz | < 5 |

A **04 Hivatalos összehasonlítás** grafikonhoz emellett egy numerikus,
0–100 skálájú index is számolódik (lineáris leképezés: 200 kΩ → 100,
0 kΩ → 0), mert a radar grafikon és a táblázat numerikus értéket vár.
Ez egy egyszerűsített, prototípus-szintű közelítés, később finomítható.

---

## 6. A teljes rendszer futtatása — lépésről lépésre

A tesztekhez **öt párhuzamos terminál** szükséges (mindegyikben forrásolva
a megfelelő ROS 2 környezet):

**1. terminál — rosbridge (WebSocket híd):**
```bash
cd ~/GitHub_repose/IPAS/Simulation
source /opt/ros/lyrical/setup.bash
source install/setup.bash
ros2 launch rosbridge_server rosbridge_websocket_launch.xml
```

**2. terminál — szimulált BME680 szenzor node:**
```bash
cd ~/GitHub_repose/IPAS/Simulation
source install/setup.bash
ros2 run ipas_bme680 bme680_node
```

**3. terminál — adatmentő (logger) node:**
```bash
cd ~/GitHub_repose/IPAS/Simulation
source install/setup.bash
ros2 run ipas_logger logger_node
```

**4. terminál — helyi HTTP szerver (a dashboard CSV-olvasásához szükséges):**
```bash
cd ~/GitHub_repose/IPAS/Simulation
python3 -m http.server 8080
```

**5. Böngésző:**
```
http://localhost:8080/web/ipas_webpage_live.html
```

> A dashboardot **nem** szabad `file://`-ként megnyitni, mert a böngésző
> biztonsági szabályai miatt a `fetch()` (amivel a CSV-t olvassuk be)
> nem működik helyi fájlrendszerről — csak HTTP-n keresztül.

### Ellenőrzési pontok

```bash
ros2 node list
# Elvart: /bme680_node, /ipas_logger_node, /rosbridge_websocket, /rosapi

ros2 topic info /ipas/measurement
# Elvart: Type: ipas_interfaces/msg/IPASMeasurement, Publisher count: 1

cat ~/GitHub_repose/IPAS/Simulation/data/measurements.csv
# Elvart: fejlec sor + folyamatosan novekvo mert sorok
```

Böngészőben `F12` → Console fül: `[IPAS] Connected to rosbridge at ws://localhost:9090`
üzenetnek kell megjelennie, piros hiba nélkül.

---

## 7. Fájlstruktúra (jelenlegi állapot)

```
~/GitHub_repose/IPAS/
├── Simulation/
│   ├── src/
│   │   ├── ipas_interfaces/
│   │   │   ├── CMakeLists.txt
│   │   │   ├── package.xml
│   │   │   └── msg/IPASMeasurement.msg
│   │   ├── ipas_bme680/
│   │   │   ├── package.xml
│   │   │   ├── setup.py
│   │   │   └── ipas_bme680/bme680_node.py
│   │   └── ipas_logger/
│   │       ├── package.xml
│   │       ├── setup.py
│   │       └── ipas_logger/logger_node.py
│   ├── build/, install/, log/     (colcon altal generalt)
│   ├── data/
│   │   └── measurements.csv       (elo mert adatok)
│   └── web/
│       ├── ipas_webpage_live.html (ROS-hoz kotott, elo dashboard)
│       └── roslib.min.js          (helyi roslibjs konyvtar)
└── software/
    └── webpage/
        └── ipas_webpage.html       (eredeti, erintetlen design/prototipus)
```

---

## 8. Ismert korlátok és jövőbeli lépések

1. **Nincs valós hardver** — minden adat szimulált a `bme680_node`-ban.
2. **`IPASMeasurement.msg`-ben nincs `Header`** — az időbélyeg jelenleg
   csak a logger oldalán keletkezik, nem a message része.
3. **A légnyomás-trend (03. szekció) grafikonja és a `+/− hPa / 6h`
   delta érték továbbra is szimulált** — csak a nagy, aktuális szám
   (`pressure-current`) él ezidáig.
4. **A "Hivatalos adatok" (`officialData`) referenciaértékei fixek,
   kézzel beírtak** — nincs valós hivatalos API-integráció.
5. **A CSV-történet böngészőoldali olvasása korlátos méretű**
   (`MAX_HISTORY_POINTS = 60` sor) — hosszabb távú, nagy mennyiségű
   adat kezelésére ez egyelőre nem optimalizált megoldás.
6. **Az időbélyegek időzóna-kezelése nem egzakt** — a CSV-ben naiv,
   helyi idő szerepel, a böngésző JS `Date` parsere ezt előfordulhat,
   hogy más időzóna-feltevéssel értelmezi (kisebb, kozmetikai eltérés
   lehetséges a grafikon időfeliratain).
7. **Nincs hitelesítés / hozzáférés-védelem** a rosbridge WebSocket
   végponton — ez egy fejlesztői/labor környezetre szánt megoldás,
   nem produkciós biztonsági szintű.

---

## 9. Gyors hivatkozási táblázat — parancsok

| Cél | Parancs |
|---|---|
| Teljes workspace build | `colcon build` (a `Simulation/` gyökérből) |
| Egy csomag build | `colcon build --packages-select <csomagnev>` |
| Environment forrásolás | `source install/setup.bash` |
| Interface ellenőrzés | `ros2 interface show ipas_interfaces/msg/IPASMeasurement` |
| Node lista | `ros2 node list` |
| Topic lista | `ros2 topic list` |
| Topic info | `ros2 topic info /ipas/measurement` |
| Élő adat megtekintése terminálban | `ros2 topic echo /ipas/measurement` |
