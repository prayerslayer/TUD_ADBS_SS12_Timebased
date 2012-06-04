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
	created_elements = vector<string>();
    created_elements.reserve(100);
}

void GeneratorLoop::operator()() {
	cout << "starting work" << endl;
	while ( !(lock->try_lock()) ) {
		cout << "zzzzzzzz" << endl;
		boost::this_thread::sleep(boost::posix_time::seconds(rand()%10+1));
		string mew = contents.at(rand()%10);
		cout << "sleep is over" << endl;
        cout << "new element = " << mew << endl;
		created_elements.push_back(mew); //prevent garbage collector from eating the element
        cout << "created elements: " << created_elements.size() << endl;
        cout << "last element: " << created_elements.at(created_elements.size() - 1) << endl;
        cout << "& of last element: " << &created_elements.at(created_elements.size() - 1) << endl;
		sampler->Add(&created_elements.at(created_elements.size()-1));
		cout << "fetching sample..." << endl;
		vector<Element> sample = sampler->GetSample();
        
        for (int i = 0; i < created_elements.size(); ++i) {
            cout << "& of created element #" << i << ": " << &created_elements.at(i) << endl;
        }
        
		cout << "SAMPLE (" << sample.size() << "): " << endl;
		if (sample.size() > 0) {
			for (int i = 0; i < sample.size(); ++i)
			{
                Element element = sample.at(i);
				cout << "\t\tp =" << element.GetPriority() << endl;
				cout << "\t\tt =" << element.GetTimestamp() << endl;
                cout << "\t\t& =" << &element << endl;
				cout << "\t\t&c=" << element.GetContent() << endl;
                cout << "\t\tc =" << *(element.GetContent()) << endl; // TODO adresse des contents ändert sich wenn vektor größer wird.
                cout << endl;
			}
		}
		else
			cout << "\t(empty)" << endl;
	}
	lock->unlock();
}