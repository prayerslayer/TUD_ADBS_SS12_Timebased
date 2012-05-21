#ifndef SAMPLER_H
#define SAMPLER_H

#include "Element.h"
#include <vector>
#include <string>
#include "SingleSampler.h"

using namespace std;

class Sampler {
	public:
		Sampler(int samplesize, long int ws);
		void Add(string* e);
		vector<Element> GetSample();
	private:
		int k;
		long int window_size;
		vector<SingleSampler *> singlesamplers;
		boost::random::mt19937 generator;
		boost::random::uniform_int_distribution<> distribution;
		double GetRandom();
};

#endif