# Microshell-Project\Specyfikacja zadania
Zadanie polega na wykorzystaniu języka C do napisania prostego programu powłoki działającego pod Linuxem – Microshell. Projekt jest obowiązkowy dla wszystkich studentów i należy go zaliczyć. Program ten powinien przyjmować na wejściu polecenia, a następnie wykonywać działania zgodne z ich treścią. Powłoka powinna:

Punkty	Obowiązkowe	Opis
2 pkt.	✓	wyświetlać znak zachęty w postaci [{path}] $, gdzie {path} jest ścieżką do bieżącego katalogu roboczego;
2 pkt.	✓	obsługiwać polecenie cd, działające analogicznie jak cd znane nam z powłoki bash;
2 pkt.	✓	obsługiwać polecenie exit, kończące działanie programu powłoki;
2 pkt.	✓	obsługiwać polecenie help, wyświetlające na ekranie informacje o autorze programu i oferowanych przez niego funkcjonalnościach;
6 pkt.		obsługiwać dwa inne, dowolnie wybrane polecenia powłoki (chodzi tutaj np. o własną, samodzielną, średnio zaawansowaną implementację dwóch poleceń, np. cp, których kody nie były podane podczas laboratoriów, nie są rozwiązaniami zadań domowych, ani nie pochodzą ze strony Bartłomieja Przybylskiego); bardzo proste implementacje, takie jak np. echo przy pomocy printf() nie są akceptowane;
6 pkt.	✓	przyjmować polecenia odwołujące się przez nazwę do programów znajdujących się w katalogach opisanych wartością zmiennej środowiskowej PATH oraz umożliwiać wywołanie tych skryptów i programów z argumentami (czyt. fork() + exec*());
4 pkt	✓	wypisywać komunikat błędu, gdy niemożliwe jest poprawne zinterpretowanie polecenia;
6 pkt.		posiadać tzw. dodatkowe bajery, np. wyświetlanie loginu aktualnie zalogowanego użytkownika, obsługę kolorów, obsługę argumentów w cudzysłowach, sensowną obsługę sygnałów (np. Ctrl+Z), obsługę historii poleceń poprzez strzałki, uzupełnianie składni, itp.; punkty są przyznawane w zależności od stopnia skomplikowania problemu.
Zadanie uznaje się za zaliczone po uzyskaniu co najmniej 18 punktów, tj. wszystkich obowiązkowych elementów projektu.

Kod programu należy umieścić w jednym pliku microshell.c. Jako pomoc przy kompilacji można użyć plik Makefile.

Testowe działanie programu (nie na punkty) można pokazać podczas dyżuru. Dzięki temu prowadzący może wskazać elementy wymagające poprawy.

Poniżej znajduje się kilka ogólnych wskazówek programistycznych, które warto uwzględnić podczas szlifowania swojego warsztatu programistycznego i wzbogacenia kultury programistycznej:

Podczas pracy najlepiej będzie przygotować sobie repozytorium Git na wydziałowym serwerze Gitea, na którym będą przechowywane postępy pracy.
Dla wygodniejszego programowania warto użyć jakieś zintegrowane środowisko programistyczne (IDE), np. CLion, Code::Blocks, Visual Studio Code, etc. Nie korzystaj z kompilatorów online.
Jeśli korzystasz z IDE, to podczas prezentacji pokaż z poziomu terminala w jaki sposób możesz skompilować ręcznie swój projekt (np. poleceniem make) oraz w jaki sposób możesz uruchomić skompilowany plik projektu.
Jeśli chodzi o standard języka C, to można wybrać ANSI C, C99, C17 lub nowszy (aczkolwiek ANSI C lub C99 wystarczy na potrzeby tego projektu).
Szlifując swoje umiejętności programistyczne warto również sprawdzić czy w programie występują wycieki pamięci, np. przy pomocy narzędzia valgrind.
Warto również zadbać o to by podczas kompilacji nie wyświetlały się żadne ostrzeżenia (flaga -Wall).
Na żadnym etapie pracy nad projektem proszę nie umieszczać w swoich programach wulgaryzmów np. jako nazw zmiennych – potem takie kwiatki wychodzą podczas prezentacji projektu 😉
Najczęstsze błędy w implementacji
Używanie funkcji system() do uruchamiania poleceń wprowadzanych przez użytkownika i do czyszczenia ekranu (system("clear")). Jest to niezgodne ze specyfikacją zadania, gdyż system() tworzy de facto dwa procesy (/bin/sh, który potem uruchamia kolejny proces), a dodatkowo uczy złych nawyków.
Brak funkcji fork().
Po próbie uruchomienia nieistniejącego polecenia nie można zakończyć normalnie pracy microshella (w takiej sytuacji exec*() kończy się niepowodzeniem i należy wyświetlić błąd i zakończyć proces potomny).
Konieczność poprzedzenia polecenia do uruchomienia słowem run/execute/etc. (np. run ls -al /usr; powinno wystarczyć podanie samego ls -al /usr; jeżeli nie obsługujemy natywnie danego polecenia wprowadzonego przez użytkownika, to próbujemy je uruchomić przez jedną z funkcji exec*()).
Konieczność interaktywnego wprowadzenia w kolejnych liniach polecenia razem z argumentami (np. najpierw ls, potem w nowej linii -l, potem nazwa katalogu, etc; odczytanie polecenia powinno być wygodne i przebiegać tak jak np. w bashu, czyli jest podawane po prostu w jednej linii.
Konieczność podawania pełnej ścieżki do programów, które są w PATH i/lub mnóstwo rozgałęzień if-else przy wykorzystaniu execl*(). Wystarczy użyć execvp().
Uruchamianie implementacji własnych poleceń w procesie potomnym (po prostu nie ma takiej potrzeby).
Użycie goto.
Praca pod Windowsem: dołączanie nagłówków windows.h, conio.h, praca pod Cygwinem, używanie CreateProcess() do tworzenia procesów. Całe zajęcia poświęcamy Linuxowi i w tym środowisku powinien zostać przygotowany projekt.
