# Flashy Delay Trigger Projekt

Willkommen zum Flashy Delay Trigger Projekt! Diese Anleitung führt Sie durch den Prozess, Ihren Arduino Nano mit dem bereitgestellten Code und der Hardware einzurichten.

## Voraussetzungen

1. **Arduino Nano**
2. **Steckbrett (Breadboard)**
3. **Jumper-Kabel**
4. **Komponenten, die im Schaltplan aufgeführt sind**
5. **Computer mit Internetzugang**

## Schritt 1: Installieren der Arduino IDE

1. Gehen Sie zur [Arduino Software Seite](https://www.arduino.cc/en/software).
2. Laden Sie die Arduino IDE für Ihr Betriebssystem herunter (Windows, macOS oder Linux).
3. Installieren Sie die Arduino IDE, indem Sie den Anweisungen auf dem Bildschirm folgen.

## Schritt 2: Herunterladen des Projektcodes

1. Besuchen Sie das Projekt-Repository: [FlashyDelayTrigger](https://github.com/DatBlaueHus/FlashyDelayTrigger/tree/master).
2. Klicken Sie auf den grünen "Code"-Button und wählen Sie "Download ZIP".
3. Entpacken Sie die ZIP-Datei an einem geeigneten Ort auf Ihrem Computer.

## Schritt 3: Öffnen des Projekts in der Arduino IDE

1. Starten Sie die Arduino IDE.
2. Klicken Sie auf `Datei > Öffnen`.
3. Navigieren Sie zum entpackten Projektordner und öffnen Sie die `.ino`-Datei darin.

## Schritt 4: Installieren der benötigten Libraries

1. In der Ardunio IDE, tippen Sie den Shortcut command+Shift+i (Mac) bzw. ctrl+shift+i (Windows)
2. Der Library Manager öffnet sich, und man befindet sich gleich im Suchfeld
3. Hier suchen Sie "u8g2", es wird *U8g2 by oliver <olikraus@gmail.com>* angezeigt, und ein Button zum Installieren, falls es nch nciht installiert ist
4. Diesen Button drückt man nun, und die Arduino IDE installiert die Library
5. Nun such man "RotaryEncoder", es wird *RotaryEncoder by Matthias Hertel* angeziegt, den man genauso installiert

## Schritt 5: Einrichten der Hardware

1. Beziehen Sie sich auf das Schaltbild im `board`-Unterverzeichnis des Projektordners.
2. Bauen Sie die Komponenten gemäß dem Schaltbild auf dem Steckbrett zusammen.
3. Verbinden Sie den Arduino Nano mit einem USB-Kabel mit Ihrem Computer.

## Schritt 6: Hochladen des Codes auf den Arduino Nano

1. Wählen Sie in der Arduino IDE Ihr Board und den Port aus:
    - Gehen Sie zu `Werkzeuge > Board` und wählen Sie `Arduino Nano`.
    - Gehen Sie zu `Werkzeuge > Port` und wählen Sie den richtigen Port für Ihren Arduino Nano.
2. Klicken Sie auf den `Upload`-Button (Pfeil nach rechts), um den Code auf Ihren Arduino Nano hochzuladen.
3. Warten Sie, bis der Upload abgeschlossen ist. Die Meldung "Hochladen abgeschlossen" erscheint in der Statusleiste.

## Schritt 7: Testen

1. Sobald der Upload abgeschlossen ist, beginnt der Arduino Nano, den hochgeladenen Code auszuführen.
2. Das Display sollte den Splashscreen anzeigen.
3. Mit Druck oder Drehen der großen Knopfes kommt man in die Eingabe, hier kann man die Ausgangszeit und die Korrektur eingeben
