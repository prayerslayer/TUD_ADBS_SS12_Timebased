#ifndef SINGLESAMPLER_H
#define SINGLESAMPLER_H

#include <cstdlib>
#include "Element.h"

class SingleSampler {
	public:
		SingleSampler(long int ws);
		void AddElement(Element e);	
		
	private:
		long int window_size;
};

#endif