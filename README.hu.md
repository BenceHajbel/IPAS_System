# IPAS - Intelligent Pollution Analyser System (Intelligens Szennyezésmérő Rendszer)

[🇬🇧 English version](README.md)

Önálló környezetfigyelő rendszer **ESP32-C3 SuperMini** panelre és **BME680** szenzorra építve. Az ESP32 kiolvassa a szenzort, létrehozza a **saját Wi-Fi hálózatát**, kiszolgál egy kétnyelvű (magyar / angol) **dashboardot**, és az előzményeket **microSD-kártyán** is tárolhatja. Nem kell hozzá router, internet, szerver vagy másik számítógép.

Iskolai mérnöki kutatási projektként készül a Szent Ferenc Katolikus Általános Iskola és Gimnáziumban (Vecsés).

## Funkciók

- Élő hőmérséklet, relatív páratartalom, légnyomás és gázellenállás a BME680-ból
- **1 órás, 12 órás, 24 órás és 7 napos** grafikonok, a hiányzó időszakok szünetként látszanak
- Őszinte állapotok az oldalon: élő, elavult, nincs adat, szenzorhiba, nincs kapcsolat; demóadat csak a `?demo=1` címmel és mindig megjelölve – semmi nem szimulálódik csendben
- **Levegőminőség-értékelés** (nagyon rossz ... nagyon jó) a gázellenállásból és a páratartalomból, a módszer az információs ablakban is le van írva ([részletek](docs/air-quality.hu.md))
- **Nyomástrend** 3 órára (trendjelző, **nem időjárás-előrejelzés**)
- **microSD-naplózás**: a grafikonok túlélik az áramszünetet, és a kimaradt időszak szünetként látszik ([részletek](docs/history-and-sd.hu.md))
- Saját hozzáférési pont `IPAS-Sensor` néven, a dashboard a `http://192.168.4.1/` címen, captive-portal átirányítással, hogy a telefonok csatlakozás után felajánlhassák az oldal megnyitását
- Magyar és angol felület

## Hogyan működik

```
BME680 --I2C--> ESP32-C3 SuperMini --Wi-Fi AP / HTTP--> telefon vagy laptop böngészője
microSD --SPI-> (szenzorciklus, előzmények, SD-napló, webszerver, dashboard-fájlok a LittleFS-en)
```

## A projekt állapota

Tisztán, mi lett ellenőrizve valódi hardveren és mi nem (egy panel, egy szerző):

| Hardveren igazolva | Még nem igazolt |
|---|---|
| A firmware lefordul PlatformIO-val (espressif32 6.13.0, Arduino core 2.0.17) | Az előzmények visszatöltése valódi áramszünet után a végleges firmware-rel |
| A BME680 a 0x77 címen megvan, stabil értékek | A telefonokon az oldal automatikus megnyílása (captive portal) |
| A Wi-Fi hozzáférési pont elérhető gépről és telefonról | Az SD automatikus újrapróbálkozása / újraindítása sok indításon át |
| Az SD-kártya felcsatolódik, az 5 perces minták íródnak a naplóba | A szenzor pontossága referenciaműszerhez képest |
| Egy korábbi verzió (SD nélkül) 24 óránál tovább hiba nélkül futott | A levegőminőségi osztályhatárok valós eseményekkel; többnapos tartóspróba SD-vel |

A gépi tesztek (C++ egységtesztek, dashboard-tesztek utánzott eszközzel, firmware-szintaxisellenőrzés) lefutnak; ezek **nem** bizonyítják a hardveres működést.

## Gyors indulás

**Hardver:** ESP32-C3 SuperMini, BME680 modul (I2C), opcionális microSD modul (SPI). Bekötés: [docs/wiring.hu.md](docs/wiring.hu.md).

1. Telepítsd a [PlatformIO](https://platformio.org/)-t (VS Code-bővítmény vagy `pip install platformio`).
2. A projektet **szóköz és zárójel nélküli** útvonalra tedd.
3. Opcionális: másold az `include/config_local.example.h` fájlt `include/config_local.h` néven, és állítsd be a saját értékeidet (Wi-Fi-jelszó, TX-teljesítmény, lábak). Ezt a fájlt a git figyelmen kívül hagyja.
4. Csatlakoztasd a panelt USB-n, és futtasd **mindkét** parancsot:
   ```
   pio run -t upload      # firmware
   pio run -t uploadfs    # a data/ mappa dashboard-fájljai -> LittleFS (a data/ módosítása után ismételd)
   pio device monitor     # soros napló, 115200 baud
   ```
   Ha a feltöltés nem sikerül, USB-be dugás közben tartsd lenyomva a BOOT gombot.
5. Csatlakozz a **`IPAS-Sensor`** Wi-Fi-hez (alapértelmezett jelszó: **`ipas1234`**, változtasd meg), és nyisd meg a **`http://192.168.4.1/`** címet (a teljes címet írd be `http://`-vel). Maradj csatlakozva, ha a telefon "nincs internet" üzenetet ad.

Az SD-kártyát (FAT32) a panel bekapcsolása **előtt** told be, és minden bekapcsolás után nyisd meg egyszer a dashboardot, hogy az óra beálljon.

## Beállítás

Az alapértékek az `include/config.h` fájlban vannak; a módosításaidat az `include/config_local.h` fájlba írd:

```c
#define IPAS_AP_SSID "IPAS-Sensor"
#define IPAS_AP_PASSWORD "valtoztasd-meg"           // 8..63 karakter, "" = nyitott hálózat
#define IPAS_WIFI_TX_POWER WIFI_POWER_8_5dBm        // csökkentsd, ha a telefonok nem tudnak csatlakozni
#define IPAS_TEMP_OFFSET_C -1.5f                    // kalibrációs eltolások
#define IPAS_I2C_SDA 8
#define IPAS_I2C_SCL 9
#define IPAS_SD_SCK 4
#define IPAS_SD_MISO 5
#define IPAS_SD_MOSI 6
#define IPAS_SD_CS 7
```

## Dokumentáció

| Téma | English | Magyar |
|---|---|---|
| Bekötés | [wiring.md](docs/wiring.md) | [wiring.hu.md](docs/wiring.hu.md) |
| Levegőminőség-értékelés | [air-quality.md](docs/air-quality.md) | [air-quality.hu.md](docs/air-quality.hu.md) |
| Előzmények, óra, SD-kártya | [history-and-sd.md](docs/history-and-sd.md) | [history-and-sd.hu.md](docs/history-and-sd.hu.md) |
| HTTP API | [api.md](docs/api.md) | [api.hu.md](docs/api.hu.md) |
| Hibaelhárítás | [troubleshooting.md](docs/troubleshooting.md) | [troubleshooting.hu.md](docs/troubleshooting.hu.md) |

## Tesztek

```
cd tests/js && npm install && cd ..   # egyszer (jsdom)
make test                              # C++ egységtesztek + dashboard-tesztek + firmware-szintaxisellenőrzés
```

Lefedik az előzményszinteket, az óraátszámítást, az SD-napló formátumát és visszatöltését (csonka sorok, óra nélküli indítások, kimaradási szünetek), a trendet, a levegőminőség pontozását és osztályait, a JSON-kimenetet, az állapotok besorolását, valamint a dashboardot az eszköz utánzatával szemben (valódi `<script src>` betöltés, minden időtartomány, óraszinkron, nyelvváltás, demó-kapuzás). A GitHub Actions minden push-nál lefuttatja őket, és lefordítja a firmware-t.

## A repó felépítése

```
src/main.cpp          Arduino-ragasztó: hozzáférési pont, HTTP-szerver, BME680-ciklus, SD-indítás
include/              hordozható logika (előzmények, trend, levegőminőség, naplóformátum, JSON) és Arduino-részek (sd_store.h, config.h)
data/                 a LittleFS-ről kiszolgált dashboard: index.html, app.js, ipas-logic.js, logo.png
tests/                gépi tesztek, Arduino-API stubok
docs/                 dokumentáció (angol + magyar)
```

## Ismert korlátok

- Nincs valós idejű óra: a böngésző indításonként egyszer állítja be az időt (a böngészőlátogatás nélküli indítás nem helyezhető el az idővonalon); egy RTC-modul ezt megszüntetné.
- Az előzményszintek átlagok; az 1 órás grafikon csak a RAM-ból készül; áramszünet előtt legfeljebb az utolsó 5 perc vész el.
- A gázszenzor relatív jelzést ad, nem kalibrált levegőminőségi indexet; az osztályhatárok a projekt saját választásai.
- A hőmérséklet és a páratartalom az önmelegedés miatt kissé eltér; referenciával való összehasonlítás után használd az eltolás-beállításokat.
- A Wi-Fi hozzáférési pontnak szándékosan nincs internete (a telefonok figyelmeztethetnek erre).

## Licenc és arculat

Kód és dokumentáció: [MIT licenc](LICENSE). Az iskola neve és a `data/logo.png` fájlban lévő logó az iskola tulajdona, és **nem** tartozik az MIT licenc alá; ha elágaztatod a projektet, cseréld le őket a sajátodra, vagy gondoskodj a használati engedélyről.

## Közreműködés

Lásd a [CONTRIBUTING.md](CONTRIBUTING.md) fájlt (angolul és magyarul).
