#ifndef SAMPLER_H
#define SAMPLER_H

#include "Element.h"

class Sampler {
	public:
		Sampler();
		Sampler(int samplesize, long int ws);
		void Add(Element e);
	private:
		int k;
		long int window_size;
};

#endif