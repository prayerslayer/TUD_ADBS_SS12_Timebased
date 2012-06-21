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
		cout << "\tContent " << i << ": " << contents.at(i) << endl;
	}
}

void GeneratorLoop::operator()() {
	cout << "starting work" << endl;
	while ( !(lock->try_lock()) ) {
		//cout << "zzzzzzzz" << endl;
		boost::this_thread::sleep(boost::posix_time::seconds(rand()%10+1));
		//cout << "sleep is over" << endl;
		sampler->Add(&contents.at(rand()%10));
		//cout << "fetching sample..." << endl;
		vector<Element> sample = sampler->GetSample();
        
		cout << "SAMPLE (" << sample.size() << "): " << endl;
		if (sample.size() > 0) {
			for (int i = 0; i < sample.size(); i++)
			{
                if ( i > 0 )
                    cout << "\t\t-----------------" << endl;
                
                Element element = sample.at(i);
                
				cout << "\t\tp =" << element.GetPriority() << endl;
				cout << "\t\tt =" << element.GetTimestamp() << endl;
                cout << "\t\t& =" << &sample.at(i) << endl;
				cout << "\t\t&c=" << element.GetContent() << endl;
                cout << "\t\tc =" << *(element.GetContent()) << endl;               			
            }
		}
		else
			cout << "\t(empty)" << endl;
	}
	lock->unlock();
}