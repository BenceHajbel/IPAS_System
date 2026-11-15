# Contributing / Közreműködés

## English

Thanks for helping. Please:

1. Open an issue before large changes.
2. Run `make -C tests test` (install dependencies once with `cd tests/js && npm install`). Add tests for new logic: portable logic lives in `include/*.h` (unit-tested natively), dashboard logic in `data/ipas-logic.js`.
3. Keep the **API version** (`kApiVersion` in `include/json_api.h` and `EXPECTED_API` in `data/app.js`) in sync and bump both when the JSON format changes.
4. Keep Hungarian and English in step: every user-visible string needs both languages (`translations` in `data/app.js`), and every document in `docs/` has an English and a Hungarian version.
5. **Hardware claims must be honest.** State in the pull request what you tested on real hardware (board, sensor, SD module) and what you only tested on the host. Never present an untested pin assignment or behaviour as verified. Update the status table in the README only with checks you actually ran.
6. After changing `data/`, remember `pio run -t uploadfs`; after changing `src/` or `include/`, `pio run -t upload`. Firmware and file system must match.

## Magyar

Köszönjük a segítséget. Kérjük:

1. Nagyobb változtatás előtt nyiss egy issue-t.
2. Futtasd a `make -C tests test` parancsot (a függőségeket egyszer telepítsd: `cd tests/js && npm install`). Új logikához írj tesztet: a hordozható logika az `include/*.h` fájlokban van (natívan egységtesztelve), a dashboard-logika a `data/ipas-logic.js` fájlban.
3. Tartsd szinkronban az **API-verziót** (`kApiVersion` az `include/json_api.h` fájlban és `EXPECTED_API` a `data/app.js` fájlban), és mindkettőt emeld, ha a JSON-formátum változik.
4. A magyar és az angol nyelv maradjon összhangban: minden, a felhasználó számára látható szövegnek mindkét nyelven kell léteznie (`translations` a `data/app.js` fájlban), és a `docs/` minden dokumentumának van angol és magyar változata.
5. **A hardveres állítások legyenek őszinték.** A pull requestben írd le, mit próbáltál ki valódi hardveren (panel, szenzor, SD-modul), és mit csak gépen. Soha ne mutass be kipróbálatlan lábkiosztást vagy viselkedést igazoltként. A README állapottábláját csak olyan ellenőrzésekkel frissítsd, amelyeket valóban lefuttattál.
6. A `data/` módosítása után ne felejtsd el a `pio run -t uploadfs` parancsot; a `src/` vagy az `include/` módosítása után a `pio run -t upload` parancsot. A firmware és a fájlrendszer egyezzen.
