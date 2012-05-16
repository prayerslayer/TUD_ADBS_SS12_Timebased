#ifndef SINGLESAMPLER_H
#define SINGLESAMPLER_H

#include <cstdlib>
#include "Element.h"

class SingleSampler {
	public:
		SingleSampler(long int ws);
		void Add(Element* e);	
		Element* GetSample();
	private:
		long int window_size;
		Element* candidate;
		Element* test;
};

#endif