#pragma once
#include <thread>
#include <atomic>

// Basisklasse fuer eigene Thread-Klassen.
// Idee:
//   - Man erbt von Thread
//   - ueberschreibt die Methode run()
//   - startet den Thread mit start()
//   - kann mit requestStop() ein sanftes Beenden anstossen
class Thread {
private:
    // Das eigentliche C++-Thread-Objekt
    std::thread p_thread;

    // Gibt an, ob der Thread aktuell laeuft.
    // atomic<bool> ist threadsicher (wird von mehreren Threads gleichzeitig gelesen/geschrieben).
    std::atomic<bool> running;

    // Flag, mit dem von aussen angefordert werden kann,
    // dass der Thread seine Arbeit beenden soll.
    std::atomic<bool> stopRequested;

public:
    // Konstruktor:
    // Beim Erzeugen laeuft noch kein Thread, daher:
    // running = false, stopRequested = false
    Thread() : running(false), stopRequested(false) {}

    // Destruktor:
    // Wird aufgerufen, wenn ein Objekt dieser Klasse (oder einer Unterklasse)
    // zerstoert wird.
    //
    // WICHTIG:
    //  - Wir fordern zuerst das Stoppen an (requestStop()).
    //  - Wenn der Thread noch laeuft (joinable()),
    //    warten wir mit join(), bis er fertig ist.
    //
    // So verhindern wir, dass ein Thread im Hintergrund weiterlaeuft,
    // waehrend das Objekt schon zerstoert ist.
    virtual ~Thread() {
        requestStop();
        if (p_thread.joinable()) {
            p_thread.join();
        }
    }

    // Abstrakte Methode run():
    // Diese Funktion MUSS in jeder Unterklasse ueberschrieben werden.
    // Hier steht der eigentliche Code, den der Thread ausfuehren soll.
    //
    // Beispiel:
    //   class MyThread : public Thread {
    //   public:
    //       void run() override {
    //           while (!stopRequestedFlag()) {
    //               // tue etwas
    //           }
    //       }
    //   };
    virtual void run() = 0;

    // Startet den Thread.
    // Rueckgabewert:
    //   - true: Thread wurde erfolgreich gestartet
    //   - false: Thread laeuft bereits, wurde nicht erneut gestartet
    virtual bool start() {
        // Verhindert, dass start() mehrfach aufgerufen wird,
        // waehrend der Thread noch laeuft.
        if (running) return false;

        running = true;
        stopRequested = false;

        // threadLoop ist eine Methode dieser Klasse.
        // &Thread::threadLoop ist ein Zeiger auf diese Methode.
        // "this" ist der Zeiger auf das aktuelle Objekt.
        //
        // Dadurch startet std::thread einen neuen Ausfuehrungsstrang,
        // der in threadLoop() beginnt.
        p_thread = std::thread(&Thread::threadLoop, this);
        return true;
    }

    // Fordert an, dass der Thread seine Arbeit beenden soll.
    // Wichtig:
    //   - Diese Methode beendet den Thread NICHT sofort.
    //   - Die run()-Methode muss stopRequestedFlag()
    //     regelmaessig abfragen und dann selbst sauber beenden.
    void requestStop() {
        stopRequested = true;
    }

    // Gibt zurueck, ob ein Stop angefordert wurde.
    // Diese Methode wird typischerweise in run() verwendet,
    // z. B. als Abbruchbedingung einer Schleife.
    bool stopRequestedFlag() const {
        return stopRequested;
    }

    // Wartet (blockiert), bis der Thread beendet ist.
    // join() darf nur aufgerufen werden, wenn der Thread auch wirklich laeuft.
    void join() {
        if (p_thread.joinable()) {
            p_thread.join();
        }
        running = false;
    }

private:
    // Interne Thread-Funktion.
    // Diese Methode wird vom neuen Thread aufgerufen.
    //
    // Ablauf:
    //   1. run() der Unterklasse wird ausgefuehrt.
    //   2. Wenn run() beendet ist, setzen wir running auf false.
    void threadLoop() {
        run();          // Ausfuehrung des eigentlichen Thread-Codes (Unterklasse)
        running = false;
    }
};