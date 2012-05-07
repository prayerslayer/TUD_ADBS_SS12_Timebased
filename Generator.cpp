#include "Generator.h"
#include "Sampler.h"
#include "GeneratorLoop.h"
#include <ctime>
#include <string>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <boost/thread/thread.hpp>

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
	GeneratorLoop loop();
}

// setter

void Generator::SetSampler(Sampler* s) {
	sampler = s;
}

void Generator::Stop() {
	if (isRunning) {
		isRunning = false;
	}
	loopthread->join();
}

void Generator::Start() {
	if (isRunning)
		return;
	isRunning = true;
	loopthread = new boost::thread(loop);
}

