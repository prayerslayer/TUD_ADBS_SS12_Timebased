#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include "Element.h"
#include "GeneratorLoop.h"
#include "Sampler.h"
using namespace std;

class Generator {
	public:
		boost::mutex* mutex;
		Generator();
		void Start();
		void Pause();
		void Stop();
		bool IsRunning();
		void SetSampler(Sampler* s);
	private:
		boost::thread worker;
		Sampler* sampler;
};

#endif