## Autor aplikácie
  - Meno: Adam Janda
  - Login: xjanda26
  - Dátum vytvorenia: 14.11.2022

## Popis aplikácie
Aplikácia sa pripája k požadovanému zdroju, stiahne ATOM alebo RSS v2.0 dáta, spracuje ich a vypíše užívateľovi na štandardný výstup. Pri HTTPS komunikácii, aplikácia podporuje SSL/TLS pre overenie certifikátu zdroju.
Požadovaný zdroj môže byť vložený pomocou jednej URL alebo pomocou súboru s viacerými URL pri spustení aplikácie.
Vždy sa vypíše názov zdroju a názvy jednotlivých príspevkov.
Užívateľ si môže nechať vypísať dátum publikovania, URI na konkrétny príspevok a autora príspevku.
## Rozšírenia
  - Rôzne návratové hodnoty pri chybách, definovaných v `../src/error.h`
  - Lokalizácia správ aplikácie do čestiny a angličtiny. Prepínač je v `../src/error.h`
## Obmedzenia
  - Kompilácia zdrojového kódu sa musí spraviť pomocou GNU Make.
  - Aplikácia je vyvinutá pre UNIX OS.
## Príklady spustenia
  - `./feedreader https://www.fit.vut.cz/fit/news-rss/ -u -T`
  - `./feedreader https://www.fit.vut.cz/fit/news-rss/ -u -a -c path_to_certificate`
  - `./feedreader -u -T -f path_to_file_with_urls`
## Zoznam odovzdaných súborov
    .
    ├── bin                     # Skompilované objekty zdrojových súborov
    ├── src                     # Zdrojové súbory
    │   ├── atom.c
    │   ├── atom.h
    │   ├── error.c
    │   ├── error.h
    │   ├── feed.c
    │   ├── feed.h
    │   ├── host.c
    │   ├── host.h
    │   ├── http_communication.c
    │   ├── http_communication.h
    │   ├── https_communication.c
    │   ├── https_communication.h
    │   ├── main.c
    │   ├── parameters.c
    │   ├── parameters.h
    │   ├── rss.c
    │   ├── rss.h
    │   ├── tcp_communication.c
    │   ├── tcp_communication.h 
    ├── tests                   # Automatizované testy
    |   ├── examples            # Príklady
    |   |   └── ...
    │   ├── parameter_tests.c
    │   ├── parameter_tests.h
    │   ├── tests.c
    │   ├── unit_testing.c
    │   ├── unit_testing.h
    ├── Makefile
    ├── manual.pdf
    └── README.md

