#include "Sampler.h"
#include "SingleSampler.h"
#include "Element.h"
#include <iostream>
#include <vector>

using namespace std;

Sampler::Sampler(int samplesize, long int ws) {
	window_size = ws;
	k = samplesize;
	generator.seed(static_cast<unsigned int>(time(0)));
	distribution = boost::random::uniform_int_distribution<>(0, 10000);
	singlesamplers = vector<SingleSampler *>(k);
	for (int i = 0; i < k; ++i)
	{
		auto s = new SingleSampler(i, window_size);
		singlesamplers[i] = s;
	}
	cout << "sampler created with k=" << k << endl;
}

vector<Element> Sampler::GetSample() {
	auto sample = vector<Element>();
	for (int i = 0; i < k; ++i)
	{
		Element single_sample = singlesamplers[i]->GetSample();
		if (  !single_sample.IsExpired() ) {
			if ( find(sample.begin(), sample.end(), single_sample) == sample.end() ) //TODO Geht nicht?
				sample.push_back(single_sample);	
		}
		else
			cout << "sample expired" << endl;
	}
	cout << "sample size: " << sample.size() << endl;
	return sample;
}

void Sampler::Add(string* content) {
	cout << "added element " << *content << endl;
	for (int i = 0; i < k; ++i)
	{
		Element element(content);
		element.SetPriority(GetRandom());
		singlesamplers[i]->Add(element);
	}
}

double Sampler::GetRandom() {
	return (double)distribution(generator)/(double)10000;
}