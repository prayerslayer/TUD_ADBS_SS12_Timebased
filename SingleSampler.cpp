#include "SingleSampler.h"
#include "Element.h"
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <cstdlib>

using namespace std;

SingleSampler::SingleSampler(int id, long int ws) {
	window_size = ws;
	candidate = NULL;
	test = NULL;
	identity = id;
}

void SingleSampler::ExpireElement(Element** e, bool is_candidate) {
	cout << "[" << identity << "] element " << (*e)->GetContent() << " expires in " << window_size << " ms" << endl;
	int local_copy_of_id = (*e)->GetId();
	//wait until window is over
	boost::this_thread::sleep(boost::posix_time::milliseconds(window_size));
	//check if item was replaced
	cout << "[" << identity << "]" << "item " << local_copy_of_id << " expired" << endl;
	if ((*e) == NULL || (*e)->GetId() != local_copy_of_id) {
		if ((*e)!=NULL)
			cout << "[" << identity << "]" <<  "id: " << (*e)->GetId() << endl;
		else
			cout << "[" << identity << "]" << "element is null" << endl;
	}
	else {
		if (is_candidate) {
			cout << "[" << identity << "]" << candidate->GetContent() << " expired" << endl;
			//candidate becomes test item
			test = candidate; 
			candidate = NULL;
			boost::function<void (Element**, bool)> expirer = boost::bind(&SingleSampler::ExpireElement, this, _1, _2);
			expire_test = boost::thread(expirer, &test, false);
		}		
		else {
			//test item is trashed
			cout << "[" << identity << "]" << test->GetContent() << " expired" << endl;
			test = NULL;
	 	}
	}
}
//TOOD nach inklusive 2. aufruf ist e=candidate. VOR dem if-statement
void SingleSampler::Add(Element* e) {
	//arrival of item e
	cout << "[" << identity << "]" << "new item " << e->GetContent() << " (p=" << e->GetPriority() << ") arrived" << endl;
	//if there is currently no candidate item or the priority of e is larger than the one of the candidate
	if (candidate==NULL || candidate->GetPriority() < e->GetPriority()) {
		//e becomes the candidate
		cout << "[" << identity << "]" << "it's the new candidate" << endl;
		candidate = e;
		boost::function<void (Element**, bool)> expirer = boost::bind(&SingleSampler::ExpireElement, this, _1, _2);
		expire_candidate = boost::thread(expirer, &candidate, true);
	}
	//otherwise, nothing happens
}

Element* SingleSampler::GetSample() {
	// cout << "fetching sample..." << endl;
	//there is a candidate
	if (candidate != NULL) {
		// cout << "candidate not null..." << endl;
		//and if there is also a test
		if (test != NULL) {
			// cout << "test not null..." << endl;
			//check if priority is higher and return
			return candidate->GetPriority() > test->GetPriority() ? candidate : NULL;
		}
		// cout << "test null" << endl;
		//if there is no test item, return candidate
		return candidate;
	}
	// cout << "no candidate" << endl;
	//if there is no candidate... well... 
	return NULL;
}

int SingleSampler::GetIdentity() {
	return identity;
}