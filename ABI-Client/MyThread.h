#pragma once
#include <iostream>
#include <string>
#include "Thread.h"
#include "../Socket/Socket.hpp" 

using namespace std;
class MyThread : public Thread
{
private:
	static int count;
	int ThreadNr;
	Socket* work;
	int SERVER_PORT;		// Konstante für den Server-Port - zentraler Ort fuer die Port-Nummer
	string SERVER_IP;  // Konstante für den IP-Adresse
public:
	MyThread(string SERVER_IP, int SERVER_PORT) {
		work = nullptr; ThreadNr = count; count++; this->SERVER_IP = SERVER_IP; this->SERVER_PORT = SERVER_PORT;};
	void setSocket(Socket* work) { this->work = work; };
	Socket* getSocket() { return work; };
	void run() override {
		string ans;
		for (char B = 'A'; B < 'S' + 1;B++) {
			work = new Socket(SERVER_IP, SERVER_PORT);
			while (work == nullptr) {
				work = new Socket(SERVER_IP, SERVER_PORT);
				this_thread::sleep_for(chrono::milliseconds(200));
			}
			ans = "x";// passwort einfügen
			ans += " ";
			ans += (char)(ThreadNr+'0');
			ans += " ";
			ans += B;
			work->write(ans);

			cout << work->readLine() << "\n";

			work->close();
			work = nullptr;
		}
	};
};
class ArduinoThread : public Thread {
private:
	Socket* work;
	bool cmd;
public:
	ArduinoThread(Socket* iniwork) { work = iniwork; cmd = false; };
	void setCmd(bool c) { cmd = c; };
	bool getCmd() { return cmd; };
	void run() override {
		while (!stopRequestedFlag()) {
			if (work->dataAvailable()) {
				/*while (cmd) {

				}*/
				cmd = true;
				cout << "Arduino : " << work->readLine();
				cmd = false;
			}
		}
	};

};

