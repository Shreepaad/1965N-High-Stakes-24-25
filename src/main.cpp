#include "main.h"

Controller master;

//Right Side Motors are Reversed
okapi::Motor FL(1);
okapi::Motor FR(4);
okapi::Motor RL(5);
okapi::Motor RR(7);
	
pros::ADIDigitalOut tower('A');
okapi::Motor conveyer(5);
okapi::Motor intake(1);



/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

	FR.setReversed(true);
    	RR.setReversed(true);
	tower.set_value(0);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	while (true) {
		//Handle pneumatics
		if(master.getDigital(ControllerDigital::L1)) tower.set_value(1);
		else tower.set_value(0);

		//Handle intake
		if(master.getDigital(ControllerDigital::X)) {
			intake.moveVelocity(-200);
			conveyer.moveVelocity(200);
		} else {
			intake.moveVelocity(0);
			conveyer.moveVelocity(0);
		}
		

		double strafe = master.getAnalog(ControllerAnalog::leftX);
		double turn = master.getAnalog(ControllerAnalog::rightX);
        double forwardd = master.getAnalog(ControllerAnalog::leftY);
	
		//FL(1)
		//FR(4)
		//RL(5)
		//RR(7)
		
		double FLspeed = forwardd + turn + strafe;
		double FRspeed = forwardd - turn - strafe;
		double RLspeed = forwardd + turn - strafe;
		double RRspeed = forwardd - turn + strafe;
		double maxSpeed = std::max({fabs(FLspeed), fabs(FRspeed), fabs(RLspeed), fabs(RRspeed)});
		if (maxSpeed > 0.0) {
			double scaleFactor = 600.0 / maxSpeed;
			FLspeed *= scaleFactor;
			FRspeed *= scaleFactor;
			RLspeed *= scaleFactor;
			RRspeed *= scaleFactor;
    		}
		FL.moveVelocity(FLspeed);
		FR.moveVelocity(FRspeed);
		RL.moveVelocity(RLspeed);
		RR.moveVelocity(RRspeed);

		
		// Active Breaking
		 if (fabs(forwardd) > 0 or fabs(strafe) > 0 or fabs(turn) > 0) {
			 FL.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
			 FR.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
			 RL.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
			 RR.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
		 } else {
			 FL.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
			 FR.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
			 RL.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
			 RR.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
		 }


		pros::delay(20);
	}
}
