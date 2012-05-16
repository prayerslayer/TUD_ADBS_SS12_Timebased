#ifndef GENERATOR_LOOP_H
#define GENERATOR_LOOP_H

class GeneratorLoop {
	public:
		GeneratorLoop(int time);
		void operator()();
	private:
		int secs_to_wait;
		void Wait(int seconds);
};

#endif