#include "Generator.h"
#include "Sampler.h"
#include <ctime>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

//constructor

Generator::Generator() {
	srand(time(NULL));
	cout << "initializing generator" << endl;
	isRunning = false;
	contents = vector<string>(10);
	contents[0] = "HTTP";
	contents[1] = "FTP";
	contents[2] = "SMTP";
	contents[3] = "TELNET";
	contents[4] = "IMAP";
	contents[5] = "POP3";
	contents[6] = "TCP";
	contents[7] = "UDP";
	contents[8] = "RIP";
	contents[9] = "ICMP";
	for (int i = 0; i < contents.size(); ++i)
	{
		cout << "Content " << i << ": " << contents.at(i) << endl;
	}
}

// setter

void Generator::SetSampler(Sampler* s) {
	sampler = s;
}

void Generator::Stop() {
	if (isRunning) {
		isRunning = false;
	}
}

void Generator::Start() {
	if (isRunning)
		return;
	isRunning = true;
	auto loop_handle = async(DoWork);
	DoWork();
}

void Generator::DoWork() {
	while (isRunning) {
		Wait(1);
		cout << "waiting..." << endl;
	}
}

void Generator::Wait(int seconds) {
	clock_t endwait;
	endwait = clock () + seconds * CLOCKS_PER_SEC ;
	while (clock() < endwait) {
		//do nothing
	}
}