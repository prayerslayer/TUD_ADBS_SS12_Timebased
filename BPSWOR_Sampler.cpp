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
#include <climits>

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
    
    //lock!
    while ( !threadlock.try_lock() )
        /*warten*/ ;
    
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
            cout << "size of candidates: " << candidates.size() << endl;
            cout << "position to insert: " << upper_pos << endl;
            if (upper_pos > 0)
                candidates.insert(upper, mew);
            else 
                candidates.insert(candidates.begin(), mew);
            cout << "size of cands after insert: " << candidates.size() << endl;
            
        }
        //cout << "new candidate size: " << candidates.size() << endl;
        boost::function<void (bool)> expirer = boost::bind(&BPSWOR_Sampler::ExpireElement, this, _1);
		boost::thread(expirer, true);
	}
    threadlock.unlock();
}

vector<Element> BPSWOR_Sampler::GetSample() {
    //lock lock lock
    while ( !threadlock.try_lock() )
        /* wait! */;
    
    cout << "candidates:" << endl;
    for (int i = 0; i < candidates.size(); i++) {
        if ( i > 0 )
            cout << "--------" << endl;
        cout << "\t p = " << candidates.at(i).GetPriority() << endl;
        cout << "\t t = " << candidates.at(i).GetTimestamp() << endl;
        cout << "\t&c = " << candidates.at(i).GetContent() << endl;
    }
    
	//make copy of candidates and tesƒts for thread safety
	vector<Element> cand_copy( candidates.size() );
    copy( candidates.begin(), candidates.end(), cand_copy.begin() );
	vector<Element> test_copy( tests.size() );
    copy( tests.begin(), tests.end(), test_copy.begin() );
    
    threadlock.unlock();
    
    cout << "candidates (" << cand_copy.size() << "), tests (" << test_copy.size() << ")" << endl;
    // union (cand, test)
    vector<Element> union_vector( cand_copy.size() + test_copy.size() );
    merge(cand_copy.begin(), cand_copy.end(), test_copy.begin(), test_copy.end(), union_vector.begin());
    // top-k(union(cand, test))
    sort( union_vector.begin(), union_vector.end() );
    reverse (union_vector.begin(), union_vector.end() );
    if ( union_vector.size() > sample_size )
        union_vector.erase( union_vector.begin() + sample_size, union_vector.end() );
    // top-k(union(cand, test)) intersect candidates
    vector<int> deletions;
    for (int i = 0; i<union_vector.size(); i++) {
        Element temp = union_vector.at(i);
        if ( binary_search(test_copy.begin(), test_copy.end(), temp) )
            deletions.push_back(i);
    }
    for (int j = 0; j < deletions.size(); j++) {
        union_vector.erase(union_vector.begin() + deletions.at(j) );
    }
    
    return union_vector;
}

double BPSWOR_Sampler::GetRandom() {
	return (double)distribution(generator)/(double)10000;
}

void BPSWOR_Sampler::ExpireElement( bool is_candidate ) {
	cout << "starting timer" << endl;
	//wait until window is over
	boost::this_thread::sleep(boost::posix_time::milliseconds( window_size ));
	//check if item was replaced
	cout << "item expired" << endl;
	while ( !threadlock.try_lock() ) 
        /*busy waiting*/;
    
	if ( is_candidate ) {
		//find candidate with lowest timestamp
        if ( candidates.size() > 0 ) {
            Element mew = candidates.at(0);
            int mew_index = 0;
            if ( candidates.size() > 1 ) {
                for (int i = 1; i < candidates.size(); i++) {
                    if ( candidates.at( i ).GetTimestamp() < mew.GetTimestamp() ) {
                        mew = candidates.at( i );
                        mew_index = i;
                    }
                }
            }
            // mew = candidate with lowest timestamp
            
            cout << "expire candidate with p = " << mew.GetPriority() << endl; 
            //remove from candidates
            candidates.erase( candidates.begin() + mew_index );
            //add to tests
            if (tests.size() > 0) {
                //sortiert einfügen, wenn nicht leer
                vector<Element>::iterator upper = upper_bound( tests.begin(), tests.end(), mew );
                tests.insert( upper, mew );
            }
            else
                tests.push_back( mew );
            
            //expire
            boost::function<void (bool)> expirer = boost::bind(&BPSWOR_Sampler::ExpireElement, this, _1);
            boost::thread( expirer, false );
        }    
        
    }
	else {
		// is test
        if ( tests.size() > 0 ) {
            Element mew = tests.at(0);
            int mew_index = 0;
            if ( tests.size() > 1 ) {
                for (int i = 1; i < tests.size(); i++) {
                    if ( tests.at( i ).GetTimestamp() < mew.GetTimestamp() ) {
                        mew = tests.at( i );
                        mew_index = i;
                    }
                }
            }
            //mew = test with lowest timestamp
            cout << "expire test with p = " << mew.GetPriority() << endl;
            //delete from tests
            tests.erase( tests.begin() + mew_index );
        }
    }
    threadlock.unlock();
}