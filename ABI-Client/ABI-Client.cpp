#include "../Socket/Socket.hpp"   // unsere eigene Socket-Klasse
#include <iostream>
#include <string>
#include "MyThread.h"
#include <windows.h>    // fuer Sleep()
#include <list>

using namespace std;

// Zieladresse des Servers
constexpr int SERVER_PORT = 5000;		// Konstante für den Server-Port - zentraler Ort fuer die Port-Nummer
constexpr auto SERVER_IP = "10.35.189.32";  // Konstante für den IP-Adresse
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
				string befehl = "";
				Socket client(SERVER_IP, SERVER_PORT);
				cout << "=== TCP-Client ===" << endl;
				cout << "Verbinde mit Server " << SERVER_IP << ":" << SERVER_PORT << " ..." << endl;
				while (client.connect() == 0)
				{
					cout << ".";
					Sleep(400); // 400ms warten, Server evtl. noch nicht gestartet
				}

				cout << "\nVerbindung erfolgreich aufgebaut!" << endl;

				cout << "\n> ";
				std::getline(cin, befehl);

				//client.write(befehl + " fM8jD4cW");  fuer WarGame
				client.write(befehl);
				if (befehl == "EXIT" && befehl == "exit")
				{
					cout << "Verbindung wird beendet" << endl;
					client.close();
					return 0;
				}

				string antwort = client.readLine();
				cout << "< " << antwort << endl;

				client.close();

				cout << "Client wurde beendet." << endl;
				system("pause");
			}
		}
		else if (ans == "2") {
			Socket client(SERVER_IP, SERVER_PORT);

			while (client.connect() == 0)
			{
				cout << ".";
				Sleep(400); // 400ms warten, Server evtl. noch nicht gestartet
			}

			cout << "\nVerbindung erfolgreich aufgebaut!" << endl;
			while (1) {
				cout << "=== TCP-Client ===" << endl;
				cout << "Verbinde mit Server " << SERVER_IP << ":" << SERVER_PORT << " ..." << endl;
				string befehl = "";

				cout << "\n> ";
				std::getline(cin, befehl); 
				client.write(befehl);

				if (befehl == "EXIT" && befehl == "exit")
				{
					cout << "Verbindung wird beendet..." << endl;
					cout << "Client wurde beendet." << endl;
					system("pause");
					client.close();
					return 0;
				}
				string antwort = client.readLine();
				cout << "< " << antwort << endl;
				system("pause");
			}
		}
		else if (ans == "3") { // wargame
			while (1) {

				cout << "=== TCP-Client ===" << endl;
				cout << "Verbinde mit Server " << SERVER_IP << ":" << SERVER_PORT << " ..." << endl;
				MyThread* threads[5] = { nullptr };
				for (int i = 0; i < 5; i++) {
					threads[i] = new MyThread(SERVER_IP, SERVER_PORT);
					threads[i]->start();
				}
				cout << "\nVerbindung erfolgreich aufgebaut!" << endl;
				string befehl = "";

				cout << "\n> ";
				std::getline(cin, befehl); 
				//client.write(befehl + " fM8jD4cW");  
				if (befehl == "EXIT")
				{
					cout << "Verbindung wird beendet..." << endl;
					return 0;
				}
				cout << "Client wurde beendet." << endl;
				system("pause");
			}
		}
		else if (ans == "4") {
			string str;
			string senden;
			Socket work(SERVER_IP, SERVER_PORT);
			
			cout << "\nverbinde mit " << SERVER_IP << "\n\n";
			cout << "warte auf Verbindung\n";

			if (work.connect()) cout << "Verbindung erfolgreich\n";

			while (1) {
				str = work.readLine();
				cout << str;
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