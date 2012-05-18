#include "Sampler.h"
#include "SingleSampler.h"
#include "Element.h"
#include <iostream>
#include <vector>

using namespace std;

Sampler::Sampler(int samplesize, long int ws) {
	window_size = ws;
	k = samplesize;
	singlesamplers = vector<SingleSampler *>(k);
	for (int i = 0; i < k; ++i)
	{
		auto s = new SingleSampler(window_size);
		singlesamplers[i] = s;
	}
	cout << "sampler created with k=" << k << endl;
}

vector<Element *> Sampler::GetSample() {
	auto sample = vector<Element *>();
	for (int i = 0; i < k; ++i)
	{
		if ( singlesamplers[i]->GetSample() != NULL ) {
			sample.push_back(singlesamplers[i]->GetSample());	
		}
	}
	cout << "sample size" << sample.size() << endl;
	return sample;
}

void Sampler::Add(Element element) {
	cout << "added element " << element.GetContent() << " (p=" << element.GetPriority() << ")" << endl;
	for (int i = 0; i < k; ++i)
	{
		singlesamplers[i]->Add(&element);
	}
}