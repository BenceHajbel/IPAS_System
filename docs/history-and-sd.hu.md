# Előzmények, óra és SD-kártya

[🇬🇧 English](history-and-sd.md) · [← README](../README.hu.md)

## Három előzményszint

| Szint | Felbontás | Kapacitás | SD-re mentve | Mire használjuk |
|---|---|---|---|---|
| perces | 1 perc | 120 (2 óra) | nem (csak RAM) | 1 órás grafikon |
| finom | 5 perc | 288 (24 óra) | igen | 12 órás és 24 órás grafikon |
| durva | 30 perc | 336 (7 nap) | a finom mintákból újraépül | 7 napos grafikon |

Minden szint a 3 másodperces mérések átlaga, körpufferben tárolva (kb. 12 kB RAM). Az érvényességi ellenőrzésen elbukó mérések (tartományon kívüli érték vagy 0 gázellenállás) soha nem kerülnek az előzményekbe. A hiányzó időszakok **szünetként** látszanak.

SD-kártya nélkül (vagy kártyahiba után) az előzmények csak a RAM-ban vannak, és **újraindításkor vagy áramszünetkor elvesznek**; a dashboard jelzi, melyik eset áll fenn.

## Az óra (nincs RTC)

Az ESP32-nek nincs valós idejű órája. A minták **idővonal-másodperceket** hordoznak: üzemidőt, amíg a böngésző be nem állította az órát, utána Unix-időt. Minden alkalommal, amikor a dashboardot megnyitják, miközben az eszköz órája nincs beállítva, a böngésző egyszer elküldi az idejét (`POST /api/time`). A firmware ekkor az aktuális indítás összes mintáját, a korábban, ugyanabban az indításban mértekét is, valós időre számolja át.

Következmények:
- Az az indítás, amelyik alatt **egyetlen böngésző sem csatlakozott**, nem helyezhető el az idővonalon, és a következő indításkori visszatöltéskor kimarad (az ilyen minták számát a `storage.discarded_unsynced` mutatja).
- Az idők csak annyira pontosak, mint a beállító telefon vagy gép órája; az eszköz kvarcjának eltérése napi néhány másodperc.
- Egy RTC-modul (például DS3231) megszüntetné ezt a korlátot.

## SD-napló

A `/ipas_log.csv` fájl (FAT32), szöveges, soronként egy rekord. Minden sor `*`-gal végződik, ezért az áramszünet miatt félbemaradt sort a betöltés figyelmen kívül hagyja.

```
B,<boot>*                                                  egy indítás kezdete
S,<boot>,<t>,<temp_c100>,<hum_c100>,<press_pa>,<gas_ohm>*  egy 5 perces minta
T,<boot>,<uptime_s>,<unix_s>*                              a böngésző beállította az órát ebben az indításban
```

Az óra beállítása előtt írt minták üzemidőt hordoznak (< 1 000 000 000); a következő `T` rekord adja az eltolást. A napló napi kb. 15 kB-ot nő. Indításkor a legutóbbi 256 kB-ot olvassa be, és ebből épül újra a 12 órás / 24 órás / 7 napos szint; a kikapcsolt időszakok szünetek maradnak. Áramszünet előtt legfeljebb az utolsó 5 perc vész el.

## A kártya felcsatolása

Egy újraindítás a kártyát parancs közepén hagyhatja, a modul stabilizátorának pedig kell egy kis idő a bekapcsolás után. A firmware ezért:

1. indításonként legfeljebb **6-szor** próbálkozik, váltakozva 4 MHz-es és 1 MHz-es SPI-vel, minden próba előtt újraszinkronizáló impulzussorozattal;
2. ha a kártya még mindig nem található, **egymás után legfeljebb 2-szer** újraindítja magát (az újraindítás gyakran megtalálja), majd SD nélkül folytatja;
3. írási hiba után 30 másodpercenként újra megpróbálja a felcsatolást.

A `GET /api/status` `storage.mount_attempts` és `storage.restarts` értéke mutatja, mi kellett. A kártyát **bekapcsolás előtt** told be; futó panelbe a kártya utólagos behelyezése nem támogatott.

Hangolás: `IPAS_SD_MOUNT_ATTEMPTS`, `IPAS_SD_AUTO_RESTARTS`, `IPAS_SD_FREQ_HZ`, `IPAS_SD_FREQ_FALLBACK_HZ` az `include/config.h` fájlban.
