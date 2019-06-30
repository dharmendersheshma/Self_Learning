#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

// structure to be used in mouseHandler function
struct userdata {
	Mat im;
	vector<Point2f> points;
};


void mouseHandler(int event, int x, int y, int flags, void* data_ptr)
{
	if (event == EVENT_LBUTTONDOWN) {
		userdata* data = ((userdata*)data_ptr);
		circle(data->im, Point(x, y), 3, Scalar(0, 0, 255), 5, LINE_AA);
		imshow("Image", data->im);
		if (data->points.size() < 1) {
			data->points.push_back(Point2f(x, y));
		}
	}
}

int main(int argc, char** argv) {

	// Take video frame from camera to select color of material
	VideoCapture capt(0);
	Mat frames;
	capt >> frames;
	Mat hsvimg;

	// Converting image from BGR to HSV
	cvtColor(frames, hsvimg, COLOR_BGR2HSV);

    // Set data for mouse event
	Mat im_temp = frames.clone();
	userdata data;
	data.im = im_temp;

	cout << "Select the point on image for the color you want to create cloak and than press 'Enter'" << endl;

	// Show image and wait for a click.
	imshow("Image", im_temp);

	// Set the callback function for any mouse event
	setMouseCallback("Image", mouseHandler, &data);
	waitKey(0);
	
	//defining the HSV values of the point selected
    Vec3b HSV_Color = hsvimg.at<Vec3b>(data.points[0]);
	int hue = HSV_Color.val[0];
	int saturation = HSV_Color.val[1];
	int value = HSV_Color.val[2];
	
	// Create a VideoCapture object and open the input file for demonstrating the cloak working
    // If the input is the web camera, pass 0 instead of the video file name
	// In first frame only background should be there, i.e., no person present
	VideoCapture cap(0);

	// Check if camera opened successfully
	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		return -1;
	}

	Mat background;
	for (int i = 0; i < 30; i++)
	{
		cap >> background;
	}

	//Laterally invert the image / flip the image.
	flip(background, background, 1);

	

	while (1)
	{

		Mat frame;
		// Capture frame-by-frame
		cap >> frame;

		// Laterally invert the image / flip the image
		flip(frame, frame, 1);

		//Converting image from BGR to HSV color space.
		Mat hsv;
		cvtColor(frame, hsv, COLOR_BGR2HSV);

		Mat mask1, mask2;
		// Creating masks to detect the upper and lower red color.
		// Otherwise mask1 and mask2 are same for other colors
		// Making different conditions according to hue values 
		// Take help from this: https://stackoverflow.com/questions/10948589/choosing-the-correct-upper-and-lower-hsv-boundaries-for-color-detection-withcv
		if (saturation > 100) {
			if (hue <= 10 || hue>165) {
				inRange(hsv, Scalar(0, 120, 20), Scalar(10, 255, 255), mask1);
				inRange(hsv, Scalar(170, 120, 20), Scalar(180, 255, 255), mask2);
			}
			else if (10<hue<=25) {
				inRange(hsv, Scalar(10, 120, 20), Scalar(25, 255, 255), mask1);
				inRange(hsv, Scalar(10, 120, 20), Scalar(25, 255, 255), mask2);
			}
			else if (25 < hue <= 38) {
				inRange(hsv, Scalar(25, 120, 20), Scalar(35, 255, 255), mask1);
				inRange(hsv, Scalar(25, 120, 20), Scalar(35, 255, 255), mask2);
			}
			else if (38 < hue <= 71) {
				inRange(hsv, Scalar(38, 100, 20), Scalar(71, 255, 255), mask1);
				inRange(hsv, Scalar(38, 100, 20), Scalar(71, 255, 255), mask2);
			}
			else if (71 < hue <= 100) {
				inRange(hsv, Scalar(71, 120, 20), Scalar(95, 255, 255), mask1);
				inRange(hsv, Scalar(71, 120, 20), Scalar(95, 255, 255), mask2);
			}
			else if (100 < hue <= 140) {
				inRange(hsv, Scalar(100, 150, 20), Scalar(130, 255, 255), mask1);
				inRange(hsv, Scalar(100, 150, 20), Scalar(130, 255, 255), mask2);
			}
			else if (140 < hue <= 165) {
				inRange(hsv, Scalar(140, 120, 20), Scalar(170, 255, 255), mask1);
				inRange(hsv, Scalar(140, 120, 20), Scalar(170, 255, 255), mask2);
			}
		}
		else { 
			cout << "Use colored material." << endl;
			break;
		}
		
		// Generating the final mask
		mask1 = mask1 + mask2;
		
		Mat kernel = Mat::ones(3, 3, CV_32F);
		morphologyEx(mask1, mask1, cv::MORPH_OPEN, kernel);
		morphologyEx(mask1, mask1, cv::MORPH_DILATE, kernel);

		// creating an inverted mask to segment out the cloth from the frame
		bitwise_not(mask1, mask2);
		
		Mat res1, res2, final_output;
        // Segmenting the cloth out of the frame using bitwise and with the inverted mask
		bitwise_and(frame, frame, res1, mask2);

		// creating image showing static background frame pixels only for the masked region
		bitwise_and(background, background, res2, mask1);

		// Generating the final augmented output.
		addWeighted(res1, 1, res2, 1, 0, final_output);
		imshow("magic", final_output);
		waitKey(1);

		// Press  ESC on keyboard to exit
		char c = (char)waitKey(25);
		if (c == 27)
			break;
		// Also relese all the mat created in the code to avoid memory leakage.
		frame.release(), hsv.release(), mask1.release(), mask2.release(), res1.release(), res2.release(), final_output.release();

	}

	// When everything done, release the video capture object
	cap.release();

	// Closes all the frames
	cv::destroyAllWindows();

	return 0;
}