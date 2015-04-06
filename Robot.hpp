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

	void performDemo();

private:
	/** The stepper motor that just pushes the robot forward **/
	StepperMotor28BYJ48* pushMotor;

	/** The stepper motor that steers the robot **/
	StepperMotor28BYJ48* steeringMotor;
};

#endif
