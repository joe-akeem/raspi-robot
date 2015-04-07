#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <thread>
#include <termios.h>

#include "Robot.hpp"

using namespace std;

Robot::Robot() {
	cout << "Starting robot..." << endl;
	if (wiringPiSetup() == -1) {
		cerr << "'Failed to setup wiringPi" << endl;
		exit (EXIT_FAILURE);
	}

	pushMotor = new StepperMotor28BYJ48(7, 0, 2, 3, 3, StepperMotor28BYJ48::FULL_STEP);
	steeringMotor = new StepperMotor28BYJ48(1, 4, 5, 6, 3, StepperMotor28BYJ48::FULL_STEP);
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

