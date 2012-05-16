#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include <string>
#include <boost/thread/thread.hpp>
#include "Element.h"
#include "GeneratorLoop.h"
#include "Sampler.h"
using namespace std;

class Generator {
	public:
		Generator();
		void Start();
		void Pause();
		void DoWork();
		void Stop();
		void SetSampler(Sampler* s);
	private:
		Element CreateElement();
		void Wait(int seconds);
		bool isRunning;
		vector<string> contents;
		Sampler* sampler;
};

#endif