#include "../Socket/Socket.hpp"   // unsere eigene Socket-Klasse
#include <iostream>
#include <string>
#include "MyThread.h"
#include <windows.h>    // fuer Sleep()
#include <list>

using namespace std;

// Zieladresse des Servers
constexpr int SERVER_PORT = 10000;		// Konstante für den Server-Port - zentraler Ort fuer die Port-Nummer
constexpr auto SERVER_IP = "192.168.178.191";  // Konstante für den IP-Adresse
unsigned char SOH = 0x01;// STart of Header
unsigned char STX = 0x02;// Start of Text
unsigned char ETX = 0x03;// End of Text
unsigned char EOT = 0x04;// End of Transmission
int main()
{
	while (1) {
		system("cls");
		cout << "[1] Per-Request\n"
			<< "[2] Keep-Alive\n"
			<< "[3] Keep-Dead\n"
			<< "[4] Arduiyes\n\n"
			<< ">";
		string ans;
		getline(cin, ans);
		if (ans == "1") {// Ab hier Per-Request
			while (1) {
				system("cls");
				system("color 4B");   // rot/blau Fensterfarbe
				system("mode 80,25"); // Konsole breiter machen

				cout << "=== TCP-Client ===" << endl;
				cout << "Verbinde mit Server " << SERVER_IP << ":" << SERVER_PORT << " ..." << endl;
				// 1) Socket-Objekt fuer den Client erzeugen
				Socket client(SERVER_IP, SERVER_PORT);

				// 2) Verbindung zum Server aufbauen (mehrfach versuchen)

				while (client.connect() == 0)
				{
					cout << ".";
					Sleep(400); // 400ms warten, Server evtl. noch nicht gestartet
				}

				cout << "\nVerbindung erfolgreich aufgebaut!" << endl;

				// 3) Bedienungsanleitung fuer den Nutzer
				cout << endl
					<< "Verfuegbare Befehle:" << endl
					<< " TIME               -> Server-Uhrzeit abfragen" << endl
					<< " ECHO <Text>        -> Text wird zurueckgesendet" << endl
					<< " ADD <a> <b>        -> Server addiert die Zahlen" << endl
					<< " EXIT               -> Verbindung beenden" << endl;

				// 4) Eingabe-Schleife: Benutzer schickt Anfragen an den Server
				string befehl = "";

				cout << "\n> ";
				std::getline(cin, befehl); // ganze Zeile einlesen

				// Anfrage an den Server schicken 
				//client.write(befehl + " fM8jD4cW");  fuer WarGame
				client.write(befehl);
				// Wenn EXIT: Verbindung direkt beenden
				if (befehl == "EXIT")
				{
					cout << "Verbindung wird beendet..." << endl;
					client.close();
					return 0;
				}

				// 5) Antwort vom Server lesen (bis '\n')
				string antwort = client.readLine();
				cout << "< " << antwort << endl;

				// 6) Verbindung sauber beenden
				client.close();

				cout << "Client wurde beendet." << endl;
				system("pause");
			}
		}
		else if (ans == "2") {// Ab hier Keep-Alive


			// 1) Socket-Objekt fuer den Client erzeugen
			Socket client(SERVER_IP, SERVER_PORT);

			// 2) Verbindung zum Server aufbauen (mehrfach versuchen)

			while (client.connect() == 0)
			{
				cout << ".";
				Sleep(400); // 400ms warten, Server evtl. noch nicht gestartet
			}

			cout << "\nVerbindung erfolgreich aufgebaut!" << endl;
			while (1) {
				system("cls");
				system("color 4B");   // rot/blau Fensterfarbe
				system("mode 80,25"); // Konsole breiter machen

				cout << "=== TCP-Client ===" << endl;
				cout << "Verbinde mit Server " << SERVER_IP << ":" << SERVER_PORT << " ..." << endl;
				// 3) Bedienungsanleitung fuer den Nutzer
				cout << endl
					<< "Verfuegbare Befehle:" << endl
					<< " TIME               -> Server-Uhrzeit abfragen" << endl
					<< " ECHO <Text>        -> Text wird zurueckgesendet" << endl
					<< " ADD <a> <b>        -> Server addiert die Zahlen" << endl
					<< " EXIT               -> Verbindung beenden" << endl;

				// 4) Eingabe-Schleife: Benutzer schickt Anfragen an den Server
				string befehl = "";

				cout << "\n> ";
				std::getline(cin, befehl); // ganze Zeile einlesen

				// Anfrage an den Server schicken 
				client.write(befehl);

				// Wenn EXIT: Verbindung direkt beenden
				if (befehl == "EXIT")
				{
					cout << "Verbindung wird beendet..." << endl;
					cout << "Client wurde beendet." << endl;
					system("pause");
					client.close();
					return 0;
				}
				char input[512];

				// 5) Antwort vom Server lesen (bis '\n')
				string antwort = client.readLine();
				cout << "< " << antwort << endl;
				system("pause");
			}
		}
		else if (ans == "3") { // wargame
			while (1) {
				system("cls");
				system("mode 80,25"); // Konsole breiter machen

				cout << "=== TCP-Client ===" << endl;
				cout << "Verbinde mit Server " << SERVER_IP << ":" << SERVER_PORT << " ..." << endl;
				// 1) Socket-Objekt fuer den Client erzeugen
				MyThread* threads[5] = { nullptr };
				for (int i = 0; i < 5; i++) {
					threads[i] = new MyThread(SERVER_IP, SERVER_PORT);
					threads[i]->start();
				}
				cout << "\nVerbindung erfolgreich aufgebaut!" << endl;

				// 3) Bedienungsanleitung fuer den Nutzer
				cout << endl
					<< "Verfuegbare Befehle:" << endl
					<< " TIME               -> Server-Uhrzeit abfragen" << endl
					<< " ECHO <Text>        -> Text wird zurueckgesendet" << endl
					<< " ADD <a> <b>        -> Server addiert die Zahlen" << endl
					<< " EXIT               -> Verbindung beenden" << endl;

				// 4) Eingabe-Schleife: Benutzer schickt Anfragen an den Server
				string befehl = "";

				cout << "\n> ";
				std::getline(cin, befehl); // ganze Zeile einlesen

				// Anfrage an den Server schicken 
				//client.write(befehl + " fM8jD4cW");  fuer WarGame
				// Wenn EXIT: Verbindung direkt beenden
				if (befehl == "EXIT")
				{
					cout << "Verbindung wird beendet..." << endl;
					return 0;
				}

				// 5) Antwort vom Server lesen (bis '\n')

				// 6) Verbindung sauber beenden

				cout << "Client wurde beendet." << endl;
				system("pause");
			}
		}
		else if (ans == "4") {
			string str;
			string senden;
			string empfangen;
			Socket work(SERVER_IP, SERVER_PORT);

			cout << "warte auf Verbindung\n";

			while (work.connect()) {
				Sleep(400);
			}

			cout << "Verbindung erfolgreich\n";
			while (1) {

				senden = STX; // < STX | Befehl | EOT >

				cout << "\n>";
				getline(cin, str);
				senden += str;

				senden += EOT;

				if (str.find("exit") != -1) {
					break;
				}

				work.write(senden);

				str = work.readLine();
				if ((str[0] != STX)) {
					cout << "STX fehlt\n";
				}
				str.erase(str.size() - 1, 1);
				if (str[str.size() - 1] != EOT) {
					cout << "EOT fehlt\n";
				}
				cout << str;
			}
		}
	}
	return 0;
}
//
//byte* Wetterstation::liesBinDaten() {
//
//	sendeBefehl(1);
//	Wetterdaten* Wetterdaten = liesWetterdaten();
//	if (Wetterdaten == nullptr) {
//		return nullptr;
//	}
//	string str = STX;
//	str += Wetterdaten->Wetterdaten->toString();
//	str += Wetterdaten->Wetterdaten;
//	str += pruefsumme;
//	str += ETX;
//
//	byte* erg = new byte[str.size()];
//	for (int i = 0; i < str.size(); i++) {
//		erg[i] = (byte)str[i];
//	}
//	return erg;
// }
//class Wetterdaten {
//private:
//public:
//	Wetterdaten();
//	void Wetterdaten();
//	string getdatum();
//};
//class WetterContainer {
//private:
//	list<Wetterdaten*> dieWetterdaten;
//	void laden();
//	void speichern(Wetterdaten* wetterdaten);
//public:
//	WetterContainer() { laden(); };
//	void addWetterdaten(Wetterdaten* wetterdaten) { dieWetterdaten.push_back(wetterdaten); speichern(wetterdaten); };
//	list<Wetterdaten*>& getWetterdaten(string datum) {
//		list<Wetterdaten*> erg;
//		for (Wetterdaten* c : dieWetterdaten) {
//			if (c->getdatum() == datum) {
//				erg.push_back(c);
//			}
//		}
//		return erg;
//	};
//	Wetterdaten* getWetterdatenAktuell() { return dieWetterdaten.size() == 0 ? nullptr : dieWetterdaten.back(); };
//	int size();
//	string toString();
//};