#ifndef SINGLESAMPLER_H
#define SINGLESAMPLER_H

#include <cstdlib>
#include <boost/thread/thread.hpp>
#include "Element.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

class SingleSampler {
	public:
		SingleSampler(int id, long int ws);
		void Add(Element e);	
		void ExpireElement(Element* e, bool is_candidate);
		Element GetSample();
		int GetIdentity();
	private:
		int identity;
		long int window_size;
		Element candidate;
		Element test;
		boost::thread expire_candidate;
		boost::thread expire_test;
};

#endif