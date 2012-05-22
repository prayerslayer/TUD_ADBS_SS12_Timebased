#include "ISampler.h"
#include "BPSWOR_Sampler.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <vector>
#include <string>
#include <ctime>
#include <iostream>
#include "Element.h"

using namespace std;

BPSWOR_Sampler::BPSWOR_Sampler(int ss, long int ws) {
	sample_size = ss;
	window_size = ws;
	generator.seed(static_cast<unsigned int>(time(0)));
	distribution = boost::random::uniform_int_distribution<>(0, 10000);
	candidates = vector<Element>();
	tests = vector<Element>();
}

void BPSWOR_Sampler::Add(string* content) {

}

vector<Element> BPSWOR_Sampler::GetSample() {

}

double BPSWOR_Sampler::GetRandom() {
	return (double)distribution(generator)/(double)10000;
}