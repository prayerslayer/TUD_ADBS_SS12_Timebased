#ifndef BPSWR_SAMPLER_H
#define BPSWR_SAMPLER_H

#include "Element.h"
#include <vector>
#include <string>
#include "BPSWR_SingleSampler.h"

using namespace std;

class BPSWR_Sampler {
	public:
		BPSWR_Sampler(int samplesize, long int ws);
		void Add(string* e);
		vector<Element> GetSample();
	private:
		int k;
		long int window_size;
		vector<BPSWR_SingleSampler *> singlesamplers;
		boost::random::mt19937 generator;
		boost::random::uniform_int_distribution<> distribution;
		double GetRandom();
};

#endif