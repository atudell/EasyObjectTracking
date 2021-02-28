#pragma once
#include <iostream>
#include <string>
// include all the necessary files from OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

// Write a utility function which checks if a string is a number
bool isNumber(string s) {
	for (int i = 0; i < s.length(); i++)
		if (isdigit(s[i]) == false)
			// If any item in the string is not a number, return false
			return false;
	return true;
}

// Write another utility function to handle VideoCapture. VideoCapture can take either a string or int,
// so this will make it easier to deal with attributes
VideoCapture capture(string s) {
	if (isNumber(s)) {
		VideoCapture cap(stoi(s));
		return cap;
	}
	else {
		VideoCapture cap(s);
		return cap;
	}
}


// Declare base class for just a video. It's kept very basic here as further additions may be required
class Video {
protected:
	// Must be initialized with a file name. Use a number in quotes to use a video stream, ie, "0"
	string filename;

public:
	explicit Video(string file) {
		filename = file;
	}
};


// A derived class for object tracking. In this case, either meanshift or camshift will be used
class ObjectTrack: public Video {
public:
	// Use Video's constructor for object tracking
	using Video::Video;
	// declare a function to use that encapuslates both meanshift and camshift
	int objectTrack(int x, int y, int width, int height, bool image, string image_path = "", string method = "cam");

};

// Define the objectTrack method here since there's a little more space
int ObjectTrack::objectTrack(int x, int y, int width, int height, bool image, string image_path, string method) {
	
	// Initialize the matrices to use
	Mat frame, roi, hsv_roi, mask;

	VideoCapture cap = capture(filename);

	// If training an image, take the frame from the image. Otherwise, just use the first frame of the video
	if (image) {
		frame = imread(image_path);
	}
	else {
		cap >> frame;
	}

	// Build the track window
	Rect track_window(x, y, width, height);
	// set up the ROI for tracking
	roi = frame(track_window);

	// Convert to HSV colorspace
	cvtColor(roi, hsv_roi, COLOR_BGR2HSV);
	// Create mask
	inRange(hsv_roi, Scalar(0, 60, 32), Scalar(180, 255, 255), mask);

	// Calculate HSV Histogram
	float range_[] = {0, 180};
	const float* range[] = {range_};
	Mat roi_hist;
	int histSize[] = {180};
	int channels[] = {0};
	calcHist(&hsv_roi, 1, channels, mask, roi_hist, 1, histSize, range);
	normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);

	// Setup the termination criteria, either 10 iteration or move by atleast 1 pt
	TermCriteria term_crit(TermCriteria::EPS | TermCriteria::COUNT, 10, 1);

	while (true) {

		Mat hsv, dst;
		
		cap >> frame;
		
		// Check the frame is not empty
		if (frame.empty())
			break;

		// Convert the frame to HSV colorspace
		cvtColor(frame, hsv, COLOR_BGR2HSV);
		// Use backprojection
		calcBackProject(&hsv, 1, channels, roi_hist, dst, range);


		if (method == "mean") {
			// apply meanshift to get the new roi
			meanShift(dst, track_window, term_crit);
			// Draw it on image
			rectangle(frame, track_window, 255, 2);
			imshow("Meanshift Tracking", frame);
		}
		else if (method == "cam") {
			// apply camshift to get the new roi
			RotatedRect rot_rect = CamShift(dst, track_window, term_crit);

			// Draw it on image
			Point2f points[4];
			rot_rect.points(points);
			for (int i = 0; i < 4; i++)
				line(frame, points[i], points[(i + 1) % 4], 255, 2);
			imshow("Camshift Tracking", frame);
		}
		else {
			cerr << "Inproper method given. The method must be either 'mean' for meanshift or 'cam' for camshift" << endl;
			break;
		}

		int keyboard = waitKey(30);
		if (keyboard == 'q' || keyboard == 27)
			break;
	}

	return 0;
}