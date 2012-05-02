#include "Sampler.h"
#include "Element.h"
#include <iostream>

using namespace std;

Sampler::Sampler() {
	window_size = 1;
	k = 0;
}

Sampler::Sampler(int samplesize, long int ws) {
	window_size = ws;
	k = samplesize;
}

void Sampler::Add(Element element) {
	cout << "added element" << endl;
}