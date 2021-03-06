#include "BPSWR_SingleSampler.h"
#include "Element.h"
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/function.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <cstdlib>

using namespace std;

BPSWR_SingleSampler::BPSWR_SingleSampler(int id, long int ws) {
	window_size = ws;
	candidate = NULL;
	test = NULL;
	identity = id;
	
}



void BPSWR_SingleSampler::ExpireElement(Element* e, bool is_candidate) {
	cout << "[" << identity << "] element " << *(e->GetContent()) << " expires in " << window_size << " ms" << endl;
	int timestamp_copy = e->GetTimestamp();
	//wait until window is over
	boost::this_thread::sleep(boost::posix_time::milliseconds(window_size));
	//check if item was replaced
	cout << "[" << identity << "]" << "item " << timestamp_copy << " expired" << endl;
	if (e->GetTimestamp() != timestamp_copy) {
		cout << "[" << identity << "]" <<  "id: " << e->GetTimestamp() << endl;
	}
	else {
		if (is_candidate) {
			cout << "[" << identity << "]" << *(candidate.GetContent()) << " expired" << endl;
			//candidate becomes test item
			test = candidate; 
			candidate.SetExpired(true);
			boost::function<void (Element*, bool)> expirer = boost::bind(&BPSWR_SingleSampler::ExpireElement, this, _1, _2);
			expire_test = boost::thread(expirer, &test, false);
		}		
		else {
			//test item is trashed
			cout << "[" << identity << "]" << *(test.GetContent()) << " expired" << endl;
			test.SetExpired(true);
	 	}
	}
}

void BPSWR_SingleSampler::Add(Element e) {
	//arrival of item e
	cout << "[" << identity << "]" << "new item " << *(e.GetContent()) << " (p=" << e.GetPriority() << ") arrived" << endl;
	//if there is currently no candidate item or the priority of e is larger than the one of the candidate
	if (candidate.IsExpired() || candidate.GetPriority() < e.GetPriority()) {
		//e becomes the candidate
		cout << "[" << identity << "]" << "it's the new candidate" << endl;
		candidate = e;
		boost::function<void (Element*, bool)> expirer = boost::bind(&BPSWR_SingleSampler::ExpireElement, this, _1, _2);
		expire_candidate = boost::thread(expirer, &candidate, true);
	}
	//otherwise, nothing happens
}

Element BPSWR_SingleSampler::GetSample() {
	// cout << "fetching sample..." << endl;
	//there is a candidate
	if (!candidate.IsExpired()) {
		// cout << "candidate not null..." << endl;
		//and if there is also a test
		if (!test.IsExpired()) {
			// cout << "test not null..." << endl;
			//check if priority is higher and return
			Element returnee = candidate;
			if (candidate.GetPriority() < test.GetPriority())
				returnee.SetExpired(true);
			return returnee;
		}
	}
	return candidate;
}

int BPSWR_SingleSampler::GetIdentity() {
	return identity;
}