#include "GeneratorLoop.h"
#include "Element.h"
#include "ISampler.h"
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

GeneratorLoop::GeneratorLoop(ISampler* s, boost::mutex* mutex) {
	srand(time(NULL));
	sampler = s;
	current_id = 0;
	lock = mutex;
	cout << "initializing generator" << endl;
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
	created_elements = vector<string>();
}

void GeneratorLoop::operator()() {
	cout << "starting work" << endl;
	while ( !(lock->try_lock()) ) {
		cout << "zzzzzzzz" << endl;
		boost::this_thread::sleep(boost::posix_time::seconds(rand()%10+1));
		string mew = contents[rand()%10];
		created_elements.push_back(mew); //prevent garbage collector from eating the element
		sampler->Add(&created_elements[created_elements.size()-1]);
		vector<Element> sample = sampler->GetSample();
		if (sample.size() > 0) {
			for (int i = 0; i < sample.size(); ++i)
			{
				cout << " === " << *(sample[i].GetContent()) << " === " << endl;
			}
		}
		else
			cout << " === empty sample === " << endl;
	}
	lock->unlock();
}