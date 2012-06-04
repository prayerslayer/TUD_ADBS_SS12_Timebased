#ifndef BPSWOR_SAMPLER_H
#define BPSWOR_SAMPLER_H

#include "ISampler.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "Element.h"
#include <boost/thread/thread.hpp>
#include <vector>

class BPSWOR_Sampler : public ISampler {
	public:
		BPSWOR_Sampler(int samplesize, long int ws);
		void Add(string* e);
		vector<Element> GetSample();
	private:
		long int window_size;
		int sample_size;
		boost::random::mt19937 generator;
		boost::random::uniform_int_distribution<> distribution;
		vector<Element> candidates;
		vector<Element> tests;
		double GetRandom();
		void ExpireElement(long int timestamp, bool is_candidate);
        boost::mutex threadlock;
};

#endif