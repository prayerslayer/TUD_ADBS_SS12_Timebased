#ifndef GENERATOR_LOOP_H
#define GENERATOR_LOOP_H

class GeneratorLoop {
	public:
		GeneratorLoop();
		void operator()();
	private:
		void Wait(int seconds);
};

#endif