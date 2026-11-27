# Сборка и запуск из командной строки

Из корня репозитория.

## C++ (ЛР 2–4): g++ с поддержкой C++17

```
g++ -std=c++17 -g *.cpp -o main
./main
```

В Windows: `.\main.exe` (PowerShell) или `main.exe` (cmd).

## Java (ЛР 3–4): JDK 21

```
javac -d out src/combat/*.java
java -cp out combat.Main
```
