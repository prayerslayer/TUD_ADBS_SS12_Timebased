#TUD ADBSÜ SS12

Hier entsteht eine Implementierung von Bounded-Space Priority Sampling Without Replacements (BPSWOR) für die Übung "Architektur von Datenbanksystemen" im Sommersemester 2012 an der Technischen Universität Dresden.

## Status

* Interfaces spezifiziert

## Fragen

* Der Sampler für *k*>1 mehrere SingleSampler parallel laufen lassen muss. Was passiert genau, wenn ein Element in den Sampler hinzugefügt wird? Wird es auf die SingleSampler verteilt? Werden die abwechselnd oder zufällig gefüttert?...

## TODO

* Generator implementieren
* SingleSampler implementieren
* Sampler implementieren