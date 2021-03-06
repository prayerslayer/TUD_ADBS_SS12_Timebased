//
//  main.cpp
//  TUD_ADBS_SS12_Timebased
//
//  Created by Marcus Kirsch on 02.05.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include "Element.h"
#include "Generator.h"
#include "ISampler.h"
#include "BPSWR_Sampler.h"
#include "BPSWOR_Sampler.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/thread/thread.hpp>

using namespace std;

int main() {

	int k = 2;
	long int ws = 10000;
	auto sampler = new BPSWOR_Sampler(k, ws);
	auto generator =  new Generator();
	generator->SetSampler(sampler);
	cout << "Generator start" << endl;
	generator->Start();
	cout << "Please enter a command [ pause | stop | start | exit ]" << endl;
	string command = "";
	while (command.compare("exit")!=0) {
		cin >> command;
		if (command.compare("stop")==0) {
			generator->Stop();
		}
		else if (command.compare("start")==0) {
			if (!generator->IsRunning())
				generator->Start();
		}
		else if (command.compare("pause")==0) {
			generator->Pause();
		}
	}
	return 0;
}
