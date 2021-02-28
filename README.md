# EasyObjectTracking
An implementation of mean shift and cam shift object tracking in C++

The purpose of this program is to demonstrate usage of mean shift and cam shift tracking in OpenCV. Its secondary purpose is to make use of object tracking simpler.

A video may be initialized using the ObjectTrack object. This calls for an attribute for either the path to a pre-recorded video or a live stream video using the camera number
as a string. For example:

// Create new object with a pre-recorded video
ObjectTrack example("C:path/to/video.mp4")

// Create new object with a live stream from the 0th camera on a system (ie, the default for most computers)
ObjectTrack example2("0")

The method objectTrack may then be called:
objectTrack(int x, int y, int width, int height, bool image, string image_path = "", string method = "cam")

int x is the x coordinate of the initial Region of Interest (ROI)
int y is the y coordinate of the initial ROI
int width is the width of the initial ROI
int height is the height of the initial ROI
bool image is whether or not the initial HSV histogram is calculated on the first frame of a video or an image. True is an image is used and false if the first frame of the 
  video is used. This is particularly helpful for live streams as estimated where the ROI is in the initial frame can be difficult. Using a image instead is much easier.
string image_path is the path to an image, if it is used. By default, this is blank.
string method is the method of image tracking. By default, "cam" is used for cam shift tracking. "mean" may be used for mean shift tracking. Using anything else
  throw an error.

example:
example.objectTrack(200, 200, 100, 100, false, "", "mean");
