#include "SingleSampler.h"
#include "Element.h"
#include <iostream>
#include <cstdlib>

using namespace std;

SingleSampler::SingleSampler(long int ws) {
	window_size = ws;
}


void SingleSampler::Add(Element* e) {
	//arrival of item e
	cout << "new item " << e->GetContent() << " arrived" << endl;
	//if there is currently no candidate item or the prioriti of e is larger than the one of the candidate
	if (candidate==NULL || candidate->GetPriority() < e->GetPriority()) {
		//e becomes the candidate
		cout << "it's the new candidate" << endl;
		candidate = e;
	}
	//otherwise, nothing happens
}



Element* SingleSampler::GetSample() {
	return candidate;
}