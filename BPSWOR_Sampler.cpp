#include "ISampler.h"
#include "BPSWOR_Sampler.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <iostream>
#include "Element.h"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

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
	//element erzeugen, priorität zuweisen
	Element mew = Element(content);
	double priority = GetRandom();
	mew.SetPriority(priority);
	//in kandidaten einfügen (oder nicht)
	int size = candidates.size();

	if (size == 0) {
		candidates.push_back(mew);
		boost::function<void (Element*, bool)> expirer = boost::bind(&BPSWOR_Sampler::ExpireElement, this, _1, _2);
		boost::thread(expirer, &candidates[0], true);
		return;
	}

	if (size == sample_size && candidates[size-1].GetPriority() < priority) {
		//mit geringster priorität löschen
		candidates.pop_back();
	}
	//sortiert einfügen
	vector<Element>::iterator upper(candidates.begin(), candidates.end(), mew);
	candidates.insert(upper-1, mew);
	boost::function<void (Element*, bool)> expirer = boost::bind(&BPSWOR_Sampler::ExpireElement, this, _1, _2);
	boost::thread(expirer, &candidates[int(upper-candidates.begin())-1], true);
}

vector<Element> BPSWOR_Sampler::GetSample() {

}

double BPSWOR_Sampler::GetRandom() {
	return (double)distribution(generator)/(double)10000;
}

void BPSWOR_Sampler::ExpireElement(Element* e, bool is_candidate) {
	int timestamp_copy = e->GetTimestamp();
	//wait until window is over
	boost::this_thread::sleep(boost::posix_time::milliseconds(window_size));
	//check if item was replaced
	cout << "item " << timestamp_copy << " expired" << endl;
}