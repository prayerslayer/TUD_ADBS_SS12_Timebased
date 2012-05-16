#include "SingleSampler.h"
#include "Element.h"
#include <iostream>
#include <cstdlib>

using namespace std;

SingleSampler::SingleSampler(long int ws) {
	window_size = ws;
}

void SingleSampler::Add(Element* e) {
	//algorithm awesomeness goes here
	cout << "single sampler received element " << e->GetContent() << endl;
	candidate = e;
}

Element* SingleSampler::GetSample() {
	return candidate;
}