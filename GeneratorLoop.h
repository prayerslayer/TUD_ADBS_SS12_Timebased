#ifndef GENERATOR_LOOP_H
#define GENERATOR_LOOP_H

#include "Sampler.h"
#include "Element.h"
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>
#include <string>

class GeneratorLoop {
	public:
		GeneratorLoop(Sampler * s, boost::mutex* mutex);
		void operator()();
	private:
		Sampler* sampler;
		boost::mutex* lock;
		vector<string> contents;
		vector<Element> created_elements;
		int current_id;
};

#endif