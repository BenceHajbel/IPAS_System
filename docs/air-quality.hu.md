# Levegőminőség-értékelés

[🇬🇧 English](air-quality.md) · [← README](../README.hu.md)

A dashboard kártyája **nagyon rossz / rossz / közepes / jó / nagyon jó** szöveget mutat (a rossz és a nagyon rossz piros, a közepes sárga, a jó és a nagyon jó zöld), mellette a gázellenállást kΩ-ban és egy pontszámot. Ugyanez a szöveg a kártya információs ablakában is olvasható.

## kΩ-ra nincs szabvány

A BME680 **gázellenállást** mér, ez nem levegőminőségi index. A Bosch definiál egy indexet (**IAQ, 0–500**: 0–50 jó, 51–100 átlagos, 101–150 kissé rossz, 151–200 rossz, 201–300 rosszabb, 301–500 nagyon rossz), de csak a zárt forráskódú **BSEC** könyvtáron keresztül. Az IAQ-t ez a projekt **nem használja**. Más projektek ugyanezt az indexet másképp nevezik (például az ESPHome).

## Mit csinál helyette a projekt

A Pimoroni `bme680-python` példaprogram (`indoor-air-quality.py`) széles körben használt, nyílt elvét követi, két eltéréssel (lent):

```
pontszám (0..100) = 75 * min(1, gáz / alapszint)  +  25 * páratartalom-tag
páratartalom-tag  = 1 40 %RH-nál, lineárisan 0-ra csökken 0 %RH és 100 %RH felé
```

- **Alapszint** (tiszta levegő viszonyítási pontja): a tárolt 30 perces gázellenállás-átlagok 95. percentilise (legfeljebb 7 nap). Legalább 6 óra adat kell hozzá; addig a kártya **KALIBRÁLÁS** feliratot mutat. (A Pimoroni-példa ehelyett rögzített, 5 perces bejáratást használ.)
- **Az osztályhatárok** a projekt saját választásai – erre a pontszámra nincs szabvány:

| Osztály | Pontszám | Közelítőleg gáz/alapszint ideális páratartalomnál |
|---|---|---|
| nagyon jó | >= 90 | >= 87 % |
| jó | >= 75 | >= 67 % |
| közepes | >= 55 | >= 40 % |
| rossz | >= 40 | >= 20 % |
| nagyon rossz | < 40 | < 20 % |

A konstansok az `include/air_quality.h` fájlban vannak; az információs ablak szövege a `data/app.js` fájlban (`aq_desc`, `AQ_LIMITS`). Egységtesztek: `tests/native/test_core.cpp`.

## Amit érdemes tudni a korlátokról

- A MOX gázszenzornak kb. **24–48 óra** működés kell a beálláshoz, és reagál a páratartalomra, a hőmérsékletre és a gázfűtésre.
- A gázokat nem tudja megkülönböztetni: az értékelés **relatív** jelzés egy szenzorra egy helyen, nem egészségügyi állítás.
- Az osztályhatárokat valós eseményekkel (szellőztetés, főzés, alkoholos törlés) **nem** validáltuk. Saját megfigyeléseid alapján állítsd át őket.
- A kártya alatti grafikon a nyers gázellenállást (kΩ) mutatja, nem a pontszámot.
