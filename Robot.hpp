#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <StepperMotor28BYJ48.hpp>

/**
 * Represents the robot.
 */
class Robot {
public:

	/**
	 * Initializes the robot.
	 */
	Robot();

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

private:
	/** The stepper motor that just pushes the robot forward **/
	StepperMotor28BYJ48* pushMotor;

	/** The stepper motor that steers the robot **/
	StepperMotor28BYJ48* steeringMotor;

	/**
	 * Reads a single character from stdin (without return key).
	 */
	int getch();
};

#endif
