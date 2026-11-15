# Hibaelhárítás

[🇬🇧 English](troubleshooting.md) · [← README](../README.hu.md)

Először nyisd meg a soros monitort (`pio device monitor`, 115200 baud); a legtöbb hiba egyértelmű sort ír ki.

## Az oldalon látszik a fejléc és a logó, de nincsenek mérőkártyák, és a gombok nem csinálnak semmit
A dashboard programja (`app.js`) nem indult el. Szinte mindig az van, hogy a **firmware és a fájlrendszer nem egyezik** (például csak az `uploadfs` futott le, vagy a firmware feltöltése hibára futott). Futtasd **mindkettőt**: `pio run -t upload` és `pio run -t uploadfs`, majd frissíts Ctrl+F5-tel. Ilyenkor az oldal piros sávot vagy verzióeltérés-jelzést mutat. Nézd meg a `http://192.168.4.1/app.js` címet: JavaScriptet kell mutatnia, nem a dashboardot.

## A `pio run -t upload` hibára fut: `sh: 1: Syntax error: "(" unexpected`
A projektmappa útvonalában zárójel vagy szóköz van (például a böngészőből letöltött `ipas-esp32(4)`). Költöztesd a projektet egyszerű útvonalra, például `~/ipas`, és ott futtasd a parancsokat.

## A Wi-Fi hálózat látszik, de a telefon nem tud csatlakozni
- Egyes ESP32-C3 SuperMini panelek rosszul viselik a teljes adóteljesítményt. Az `include/config_local.h` fájlban állítsd be: `#define IPAS_WIFI_TX_POWER WIFI_POWER_8_5dBm` (vagy alacsonyabbra), és töltsd fel újra a firmware-t.
- A jelszó kizárásához átmenetileg állítsd üresre: `#define IPAS_AP_PASSWORD ""` (nyitott hálózat). Utána állíts be újra jelszót.
- Próbálj másik USB-kábelt vagy adaptert, felejtsd el a hálózatot a telefonon, és próbáld egy második eszközzel. A soros napló kiírja a `client associated / got an IP address / disconnected` sorokat.

## Csatlakozás után az oldal nem nyílik meg automatikusan
Nyisd meg magad a `http://192.168.4.1/` címet (a teljes címet írd be `http://`-vel). Az automatikus megnyitás a telefontól függ: iOS-en általában kis ablak nyílik, Androidon gyakran "bejelentkezés a hálózatba" értesítés jelenik meg. A soros napló minden, a telefon által küldött kapcsolatellenőrzésre kiírja: `portal probe: <host><uri>`; ha csatlakozás után egyik sem jelenik meg, a telefon nem kérdezett. Hosszabb használathoz kapcsold ki a mobiladatot, és használj normál böngészőt.

## `BME680 NOT found` / `read failed: beginReading`
- Ellenőrizd a bekötést és a tápot (3V3, GND, SDA, SCL); a soros napló kiírja az I2C-szkennelést. Sok modul működik bekötetlen CS és SDO lábbal, néhány nem (lásd [bekötés](wiring.hu.md)).
- Húzd ki teljesen az USB-t néhány másodpercre, majd dugd vissza (egy sima reset nem biztos, hogy kioldja a beragadt buszt). 10 sikertelen olvasás után a firmware újrainicializálja a szenzort, és maga is kioldja az I2C-buszt.
- Az `invalid values (out of range or gas resistance 0)` azt jelenti, hogy a szenzor olyan értéket adott, amely nem valódi mérés; ezt eldobja.

## `SD: no card / mount failed`
- **FAT32** kártyát told be **bekapcsolás előtt**; ellenőrizd a VCC-t (a MICROSD-M-nél 5V, lásd [bekötés](wiring.hu.md)), hogy a MISO/MOSI nincs-e felcserélve, és hogy a vezetékek rövidek-e.
- A firmware már maga újrapróbálkozik, és legfeljebb kétszer újraindítja magát; az `/api/status` `storage.mount_attempts` / `storage.restarts` értéke mutatja, mi történt.
- Az előzmények csak azokra az indításokra állnak vissza, amelyek alatt egy böngésző megnyitotta a dashboardot (az órának be kell állnia). Lásd [előzmények és SD-kártya](history-and-sd.hu.md).

## Újraindítás után üresek a grafikonok
- SD nélkül az előzmények a RAM-ban vannak, és elvesznek. SD-vel csak a 12 órás / 24 órás / 7 napos grafikon áll vissza; az 1 órás csak a RAM-ból készül, és üresen indul.
- Minden bekapcsolás után nyisd meg egyszer a dashboardot, hogy az óra beálljon.

## A grafikon időpontjai rosszak
Az órát az a böngésző állítja be, amelyik az indítás után elsőként megnyitotta a dashboardot. Javítsd a telefon vagy a gép óráját, és indítsd újra a panelt.

## A levegőminőség KALIBRÁLÁS feliratot mutat
Legalább 6 óra gázadat kell hozzá; a szenzornak 24–48 óra kell a beálláshoz. Lásd [levegőminőség](air-quality.hu.md).
