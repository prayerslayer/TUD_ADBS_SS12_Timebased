#TUD ADBSÜ SS12

Hier entsteht eine Implementierung von Bounded-Space Priority Sampling (mit und ohne Replacement) für die Übung "Architektur von Datenbanksystemen" im Sommersemester 2012 an der Technischen Universität Dresden.

## Status

* Interfaces spezifiziert
* Generator implementiert
* Schnittstellen für Sampler implementiert
* Rudimentäre Kommandozeile ist vorhanden

## TODO

* Debuggen
* Irgendeine Form einer Thread Queue einbauen (sodass deterministisch ist, welcher Thread in seine ExpireElement() Funktion darf)
* (Kommandozeile verschönern)
* (Alles mit template classes refactoren)
* (Unit tests schreiben)