#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include "Element.h"
#include "GeneratorLoop.h"
#include "BPSWR_Sampler.h"
using namespace std;

class Generator {
	public:
		boost::mutex* mutex;
		Generator();
		void Start();
		void Pause();
		void Stop();
		bool IsRunning();
		void SetSampler(BPSWR_Sampler* s);
	private:
		boost::thread worker;
		BPSWR_Sampler* sampler;
};

#endif