# Air-quality rating

[🇭🇺 Magyar](air-quality.hu.md) · [← README](../README.md)

The dashboard card shows **very bad / bad / medium / good / very good** (bad and very bad in red, medium in yellow, good and very good in green), the gas resistance in kΩ and a score. The same text is in the card's info popup.

## There is no standard for kΩ

The BME680 measures a **gas resistance**; it is not an air-quality index. Bosch defines an index (**IAQ, 0-500**: 0-50 good, 51-100 average, 101-150 little bad, 151-200 bad, 201-300 worse, 301-500 very bad) but only through its closed-source **BSEC** library. IAQ is **not used** in this project. Other projects label the same index differently (for example ESPHome).

## What this project does instead

It follows the widely used open approach of the Pimoroni `bme680-python` example (`indoor-air-quality.py`), with two differences described below:

```
score (0..100) = 75 * min(1, gas / baseline)  +  25 * humidity_term
humidity_term  = 1 at 40 %RH, falling linearly to 0 at 0 %RH and at 100 %RH
```

- **Baseline** (clean-air reference): the 95th percentile of the stored 30-minute gas-resistance averages (up to 7 days). It needs at least 6 hours of data; until then the card shows **CALIBRATING**. (The Pimoroni example uses a fixed 5-minute burn-in instead.)
- **Classes** are this project's own choice - no standard defines them for this score:

| Class | Score | About gas/baseline at ideal humidity |
|---|---|---|
| very good | >= 90 | >= 87 % |
| good | >= 75 | >= 67 % |
| medium | >= 55 | >= 40 % |
| bad | >= 40 | >= 20 % |
| very bad | < 40 | < 20 % |

Constants live in `include/air_quality.h`; the popup text in `data/app.js` (`aq_desc`, `AQ_LIMITS`). Unit tests: `tests/native/test_core.cpp`.

## Limits you should know

- The MOX gas sensor needs roughly **24-48 hours** of operation to settle and reacts to humidity, temperature and the gas heater.
- It cannot tell gases apart: the rating is a **relative** indicator for one sensor in one place, not a health statement.
- The class limits have **not** been validated against real events (ventilation, cooking, alcohol wipe test). Adjust them after your own observations.
- The graph below the card shows the raw gas resistance (kΩ), not the score.
