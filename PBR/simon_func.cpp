#include <Simon/simon_func.h>
using namespace std;

void initRand() { //only call this once
	srand(time(NULL));
}

//produces a float number that a <= result < b.
//requirements: a<b
float randFloat(float a, float b) {
	float randf = ((float)rand()) / (float)RAND_MAX;
	while (randf == 1) {
		randf = ((float)rand()) / (float)RAND_MAX;
	}
	return a + (b - a) * randf;
}

string fileToStr(const char* fileName) {
	ifstream v(fileName);
	if (!v.good()) cout << "Error! cannot find shader file: " << fileName << endl;
	stringstream buffer;
	buffer << v.rdbuf();
	string bufferstr = buffer.str();
	return bufferstr;
}