#ifndef ISAMPLER_H
#define ISAMPLER_H

#include "Element.h"
#include <string>
#include <vector>

class ISampler {
	public: 
		virtual vector<Element> GetSample() = 0;
		virtual ~ISampler() {};
		virtual void Add(string* content) = 0;
	private:
		long int window_size;
		int sample_size;
};

#endif