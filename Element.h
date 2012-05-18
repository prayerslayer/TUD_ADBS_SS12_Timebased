#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
using namespace std;

class Element {
	public:
		Element(int identity, string content);
		long int GetTimestamp();
		int GetId();
		string GetContent();
		double GetPriority();
		void SetTimestamp(long int timestamp);
		void SetContent(string content);
		void SetPriority(double priority);
	private:
		int id;
		double priority;
		string content;
		long int timestamp;
		double GeneratePriority();
		long int GetCurrentTimeInMilli();
};

#endif