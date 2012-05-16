#ifndef SAMPLER_H
#define SAMPLER_H

#include "Element.h"
#include <vector>
#include "SingleSampler.h"

using namespace std;

class Sampler {
	public:
		Sampler(int samplesize, long int ws);
		void Add(Element e);
		vector<Element *> GetSample();
	private:
		int k;
		long int window_size;
		vector<SingleSampler *> singlesamplers;
};

#endif