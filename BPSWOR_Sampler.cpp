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
    cout << "content: " << *content << endl;
	Element mew = Element(content);
	double priority = GetRandom();
	mew.SetPriority(priority);
	cout << "* * * * * * * * * * * * * * * " << endl;
	cout << "* new element has p=" << priority << "  *" << endl;
	cout << "* * * * * * * * * * * * * * * " << endl;
	//in kandidaten einfügen (oder nicht)
	int size = candidates.size();
	if (size == sample_size && candidates.at(size-1).GetPriority() < priority) {
		//mit geringster priorität löschen
		candidates.pop_back();
		//cout << "deleted candidate with smallest p" << endl;
	}
	if (size >= 0 && size < sample_size) {
		//sortiert einfügen
        if (size == 0) {
            candidates.push_back(mew);       }
        else {
            vector<Element>::iterator upper = upper_bound(candidates.begin(), candidates.end(), mew);
            int upper_pos = int(upper-candidates.begin());
            //cout << "size of candidates: " << candidates.size() << endl;
            //cout << "position to insert: " << upper_pos << endl;
            if (upper_pos > 0)
                candidates.insert(upper-1, mew);
            else 
                candidates.insert(candidates.begin(), mew);
            
        }
        //cout << "new candidate size: " << candidates.size() << endl;
        boost::function<void (long int, bool)> expirer = boost::bind(&BPSWOR_Sampler::ExpireElement, this, _1, _2);
		boost::thread(expirer, mew.GetTimestamp(), true);
	}
}

vector<Element> BPSWOR_Sampler::GetSample() {
	auto union_vector = vector<Element>(tests.size() + candidates.size());
	merge(candidates.begin(), candidates.end(), tests.begin(), tests.end(), union_vector.begin());
	sort(union_vector.begin(), union_vector.end());
    
	if (union_vector.size()>sample_size){
        union_vector.erase(union_vector.begin()+sample_size, union_vector.end());
    }
    
	//union vektor ist jetzt top-k(Cand U Test)
	//für jedes element in union vektor schauen, ob es in candidaten vektor drin ist
	auto del_indexes = vector<int>();
	for (int i = 0; i < union_vector.size(); ++i) {
		Element element = union_vector.at(i);
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
	cout << "starting timer for item with t=" << timestamp << endl;
	//wait until window is over
	boost::this_thread::sleep(boost::posix_time::milliseconds(window_size));
	//check if item was replaced
	cout << "item with t=" << timestamp << " expired" << endl;
	while (!threadlock.try_lock()) 
        /*busy waiting*/;
	Element pseudo(NULL);
	pseudo.SetTimestamp(timestamp);
	if (is_candidate) {
		//find element
		//cout << "item was candidate!" << endl;
		auto position = find( candidates.begin(), candidates.end(), pseudo);
		int pos = int(position - candidates.begin());
		Element mew = candidates.at(pos);
		//cout << "item is at " << pos << endl;
		//remove from candidates
		candidates.erase(position);
		//cout << "item deleted from candidates" << endl;
		//add to tests
		if (tests.size() > 0) {
			//sortiert einfügen, wenn nicht leer
			vector<Element>::iterator upper = upper_bound(tests.begin(), tests.end(), mew);
            int testpos = int(upper-tests.begin());
            //cout << "tests size: " << tests.size() << endl;
            //cout << "insert to : " << testpos << endl;
			tests.insert(upper-1, mew);
		}
		else
			tests.push_back(mew);

		//cout << "item added to tests" << endl;
		//expire
		boost::function<void (long int, bool)> expirer = boost::bind(&BPSWOR_Sampler::ExpireElement, this, _1, _2);
		boost::thread(expirer, timestamp, false);
	}
	else {
		// is test
		//cout << "item was test item!!" << endl;
		//remove from tests
		auto position = find( tests.begin(), tests.end(), pseudo);
		int pos = int(position - tests.begin());
        //cout << "item is at " << pos << endl;
		tests.erase(position);
	}
    threadlock.unlock();
}