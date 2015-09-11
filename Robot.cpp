#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <thread>
#include <termios.h>
#include <raspicam/raspicam_cv.h>

#include "Robot.hpp"

using namespace std;

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"

using namespace cv;
using namespace raspicam;

/** The name of the window that displays the robot's view. */
const string MAIN_WINDOW_NAME = "Robot View";

/** The name of the window that displays the region of interest for line tracking.*/
const string ROI_WINDOW_NAME = "Region of Interest";

/** The name of the window that displays the threshold image.*/
const string THRESHOLD_WINDOW_NAME = "Threshold View";

/** TODO */
const int MAX_BINARY_VALUE = 255;

/** TODO */
const int LOOK_AHEAD = 240;

Robot::Robot(int threshold) {
	cout << "Starting robot..." << endl;
	if (wiringPiSetup() == -1) {
		cerr << "'Failed to setup wiringPi" << endl;
		exit (EXIT_FAILURE);
	}

	pushMotor = new StepperMotor28BYJ48(7, 0, 2, 3, 3, StepperMotor28BYJ48::FULL_STEP);
	steeringMotor = new StepperMotor28BYJ48(1, 4, 5, 6, 3, StepperMotor28BYJ48::FULL_STEP);
	lineTrackingThreshold = threshold;
	currentSteeringAngle = 0;
	headless = false;
}

Robot::~Robot() {
	delete(pushMotor);
	delete(steeringMotor);
}

void Robot::performDemo() {
	thread t1(&StepperMotor28BYJ48::performDemo, pushMotor);
	thread t2(&StepperMotor28BYJ48::performDemo, steeringMotor);
	t1.join();
	t2.join();
}

void Robot::startInKeyboardControl() {
	bool started = true;
	cout << "Control the robot as follows:" << endl;
	cout << "start moving forward:  'f'-key" << endl;
	cout << "start moving backward: 'b'-key" << endl;
	cout << "halt:                  'h'-key" << endl;
	cout << "turn left:             'l'-key" << endl;
	cout << "turn right:            'r'-key" << endl;
	cout << "quit:                  'q'-key" << endl;
	while(started) {
		char c = getch();
		switch(c) {
			case 'f':
				pushMotor->startClockwise();
				break;
			case 'b':
				pushMotor->startCounterClockwise();
				break;
			case 'h':
				pushMotor->stop();
				break;
			case 'l':
				steeringMotor->angleRotation(30);
				break;
			case 'r':
				steeringMotor->angleRotation(-30);
				break;
			case 'q':
				started = false;
				pushMotor->stop();
				break;
			default:
				break;
		}
	}
}

void Robot::steer(float angle) {
	float a = angle;
	if (a < -30) {
		a = -30;
	}
	if (a > 30) {
		a = 30;
	}
	steeringMotor->angleRotation(a - currentSteeringAngle);
	currentSteeringAngle = a;
}

void Robot::trackLine() {
	RaspiCam_Cv camera;

	if (camera.open()) {
		Mat captured;
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		while (1) {
			camera.grab();
			camera.retrieve(captured);

			Point capturedImageCenter(captured.cols / 2, captured.rows / 2);
			circle(captured, capturedImageCenter, 5, Scalar(0, 255, 0), -1, 8,
					0);

			// region of interest. currently around the center of the
			// captured image. might need to be moved depending on the
			// camera angle, speed of the robot etc.
			Rect roi(0, capturedImageCenter.y - 45, captured.cols, 100);
			Mat roiImg = captured(roi).clone();

			// extract the line to follow from the background
			cvtColor(roiImg, roiImg, COLOR_BGR2GRAY);
			threshold(roiImg, roiImg, lineTrackingThreshold, MAX_BINARY_VALUE, 1);

			// displaying this is useful to see if the threshold is OK
			if (!headless) {
				imshow(THRESHOLD_WINDOW_NAME, roiImg);
				waitKey(30);
			}

			Mat erodeElmt = getStructuringElement(MORPH_RECT, Size(3, 3));
			Mat dilateElmt = getStructuringElement(MORPH_RECT, Size(5, 5));
			erode(roiImg, roiImg, erodeElmt);
			dilate(roiImg, roiImg, dilateElmt);

			// determine the angle the robot needs to move
			findContours(roiImg, contours, hierarchy, CV_RETR_TREE,
					CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

			if (contours.size() == 0) {
				cout << "no tracking line detected." << endl;
				pushMotor->stop();
//				return;
			}

			for (size_t i = 0; i < contours.size(); i++) {
				float area = contourArea(contours[i]);
				if (area > 2000) {
					Moments mu;
					mu = moments(contours[i], false);
					Point2f roiCenter(mu.m10 / mu.m00,
							capturedImageCenter.y - LOOK_AHEAD); // point in center (x only)
					circle(captured, roiCenter, 5, Scalar(0, 255, 0), -1, 8, 0);
					line(captured, capturedImageCenter, roiCenter,
							Scalar(0, 255, 0));

					float ankathete = LOOK_AHEAD;
					float gegenkathete = capturedImageCenter.x - roiCenter.x;
					float bogenmass = atan(gegenkathete / ankathete);
					float gradmass = bogenmass * 180 / 3.1415926535;

					string caption = format("%f Grad", gradmass);
					putText(captured, caption,
							Point(capturedImageCenter.x + 20,
									capturedImageCenter.y + 20),
							FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0));

					if (!pushMotor->isStarted()) {
						pushMotor->startCounterClockwise();
					}
					steer(gradmass);
				}
			}

			if (!headless) {
				imshow(MAIN_WINDOW_NAME, captured);
				imshow(ROI_WINDOW_NAME, roiImg);
				waitKey(30);
			}
		}
	} else {
		cout << "failed to open camera." << endl;
		exit(EXIT_FAILURE);
	}
}

void Robot::setLineTrackingThreshold(int threshold) {
	lineTrackingThreshold = threshold;
}

void Robot::setHeadless(bool hl) {
	headless = hl;
}

int Robot::getch() {
	static int ch = -1, fd = 0;
	struct termios neu, alt;
	fd = fileno(stdin);
	tcgetattr(fd, &alt);
	neu = alt;
	neu.c_lflag &= ~(ICANON|ECHO);
	tcsetattr(fd, TCSANOW, &neu);
	ch = getchar();
	tcsetattr(fd, TCSANOW, &alt);
	return ch;
}

