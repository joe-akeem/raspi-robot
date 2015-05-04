#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <StepperMotor28BYJ48.hpp>


/**
 * Represents the robot.
 */
class Robot {
public:

	/**
	 * Initializes the robot and set a line tracking threshold.
	 *
	 * @param threshold - the threshold used to extract contours when tracking a line.
	 */
	Robot(int threshold=3);

	/**
	 * Destructs the robot.
	 */
	virtual ~Robot();

	/**
	 * Lets the robots perform a little demonstration.
	 */
	void performDemo();

	/**
	 * Starts the robot in a mode in which it can be controlled by keyboard as follows:
	 *
	 * start moving forward:  'f'-key
	 * start moving backward: 'b'-key
	 * halt:                  'h'-key
	 * turn left:             'l'-key
	 * turn right:            'r'-key
	 * quit:                  'q'-key
	 */
	void startInKeyboardControl();

	/**
	 * Lets the robot track a black line.
	 */
	void trackLine();

	/**
	 * Sets the line tracking threshold to a new value.
	 */
	void setLineTrackingThreshold(int threshold);

	/**
	 * When set to true the robot will show its camera view in extra windows.
	 */
	void setHeadless(bool hl);

	/**
	 * Steers the robot at a specified angle. An angle of 0
	 * moves the robot straight ahead. An angle of -10 steers
	 * it at an angle of ten degrees to the left. An angle of
	 * 10 steers it at an angle of then degrees to the right.
	 * The robot won't accept a steering angle greater than
	 * 20 degrees to either side. An angle greater than that
	 * will just be cut to 20 degrees (plus or minus).
	 */
	void steer(float angle);

private:
	/** The stepper motor that just pushes the robot forward */
	StepperMotor28BYJ48* pushMotor;

	/** The stepper motor that steers the robot */
	StepperMotor28BYJ48* steeringMotor;

	/** the threshold used to determine contours */
	int lineTrackingThreshold;

	/**
	 * The angle the steering wheels currently point at.
	 */
	float currentSteeringAngle;

	/**
	 * If this is set to true then the robot won't show it's camera view in extra windows.
	 */
	bool headless;

	/**
	 * Reads a single character from stdin (without return key).
	 */
	int getch();

	/**
	 * Opens the camera. Does nothing if the camera alread is opened.
	 */
	void openCamera();
};

#endif
