#include "easyObjTrack.h"
using namespace std;

int main()
{
	// Example video
	ObjectTrack example("0");
	// Call the function
	example.objectTrack(200, 200, 100, 100, false, "", "mean");

	return 0;
}