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
		boost::function<void (long int, bool)> expirer = boost::bind(&BPSWOR_Sampler::ExpireElement, this, _1, _2);
		boost::thread(expirer, mew.GetTimestamp(), true);
		return;
	}

	if (size == sample_size && candidates[size-1].GetPriority() < priority) {
		//mit geringster priorität löschen
		candidates.pop_back();
	}
	//sortiert einfügen
	vector<Element>::iterator upper = upper_bound(candidates.begin(), candidates.end(), mew);
	candidates.insert(upper-1, mew);
	boost::function<void (long int, bool)> expirer = boost::bind(&BPSWOR_Sampler::ExpireElement, this, _1, _2);
	boost::thread(expirer, mew.GetTimestamp(), true);
}

vector<Element> BPSWOR_Sampler::GetSample() {
	auto union_vector = vector<Element>(candidates.begin(), candidates.end());
	merge(candidates.begin(), candidates.end(), tests.begin(), tests.end(), union_vector.begin());
	sort(union_vector.begin(), union_vector.end());
    
	if (union_vector.size()>sample_size){
        union_vector.erase(union_vector.begin()+sample_size, union_vector.end());
    }
    
	//union vektor ist jetzt top-k(Cand U Test)
	//für jedes element in union vektor schauen, ob es in candidaten vektor drin ist
	auto del_indexes = vector<int>();
	for (int i = 0; i < union_vector.size(); ++i) {
		Element element = union_vector[i];
		if ( !binary_search( candidates.begin(), candidates.end(), element) ) {
			del_indexes.push_back(i);
		}
	}
	for (int i = 0; i < del_indexes.size(); ++i)
	{
		union_vector.erase(union_vector.begin()+i);
	}
	return union_vector;
}

double BPSWOR_Sampler::GetRandom() {
	return (double)distribution(generator)/(double)10000;
}

void BPSWOR_Sampler::ExpireElement(long int timestamp, bool is_candidate) {
	//wait until window is over
	boost::this_thread::sleep(boost::posix_time::milliseconds(window_size));
	//check if item was replaced
	cout << "item " << timestamp << " expired" << endl;
	//TODO hier müsste noch ein lock hin
	Element pseudo(NULL);
	pseudo.SetTimestamp(timestamp);
	if (is_candidate) {
		//find element
		auto position = find( candidates.begin(), candidates.end(), pseudo);
		int pos = int(position - candidates.begin());
		Element mew = candidates[pos];
		//remove from candidates
		candidates.erase(position);
		//add to tests
		vector<Element>::iterator upper = upper_bound(tests.begin(), tests.end(), mew);
		tests.insert(upper-1, mew);
		//expire
		boost::function<void (long int, bool)> expirer = boost::bind(&BPSWOR_Sampler::ExpireElement, this, _1, _2);
		boost::thread(expirer, timestamp, false);
	}
	else {
		// is test
		//remove from tests
		auto position = find( tests.begin(), tests.end(), pseudo);
		int pos = int(position - tests.begin());
		tests.erase(position);
	}
}