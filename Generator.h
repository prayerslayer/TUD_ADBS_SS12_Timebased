#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include <string>
#include "Element.h"
#include "Sampler.h"
using namespace std;

class Generator {
	public:
		Generator();
		void Start();
		void Pause();
		void Stop();
		void SetSampler(Sampler* s);
	private:
		Element CreateElement();
		vector<string> contents;
		Sampler* sampler;
};

#endif