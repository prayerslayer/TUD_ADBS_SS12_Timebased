#include "Generator.h"
#include "Sampler.h"
#include "GeneratorLoop.h"
#include "Element.h"
#include <ctime>
#include <string>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

using namespace std;

//constructor

Generator::Generator() {
	mutex = new boost::mutex();
}

// setter

void Generator::SetSampler(Sampler* s) {
	sampler = s;
}

void Generator::Pause() {
	//TODO should be something else
	Stop();
}

void Generator::Stop() {
	mutex->unlock();
	worker.join();
}

void Generator::Start() {
	if (mutex->try_lock()) {
		cout << "gonna start worker thread" << endl;
		auto loop = GeneratorLoop(sampler, mutex);
		worker = boost::thread(loop);
	}
	else
		cout << "could not start worker thread (mutex is locked)" << endl;
}

bool Generator::IsRunning() {
	bool runs = mutex->try_lock();
	if (runs)
		mutex->unlock();
	return runs;
}