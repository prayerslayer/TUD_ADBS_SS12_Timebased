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
#include "Sampler.h"

//using namespace std;

int main() {
	auto sampler = new Sampler(5, 60*1000);
	auto generator =  new Generator();
	generator->SetSampler(sampler);
	cout << "Generator start" << endl;
	generator->Start();
	cout << "Generator fertig" << endl;
	generator->Stop();
	
	return 0;
}
