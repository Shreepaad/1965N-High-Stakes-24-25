#include "main.h"

Controller master;

//Right Side Motors are Reversed
okapi::Motor FL(1);
okapi::Motor FR(4);
okapi::Motor RL(5);
okapi::Motor RR(7);
	
pros::ADIDigitalOut tower('A');
okapi::Motor conveyer(9);
okapi::Motor intake(10);

pros::IMU imu_sensor(16);

double FLspeed;
double FRspeed;
double RLspeed;
double RRspeed;

double wheelDiameter = 2.75;
double ticksPerRev = 900.0;
double wheelCicumference = M_PI * wheelDiameter;
double ticksPerInch = ticksPerRev/wheelCicumference;

double robotRadius = 8.5;

//speed between 0 and 600
void moveForward(double speed, double distanceInInches) {
	// double targetHeading = imu_sensor.get_heading();
	double targetTicks = ((fabs(distanceInInches) * ticksPerInch) / sqrt(2))/64;

	// FR.tarePosition();
	// FL.tarePosition();
	// RL.tarePosition();
	// RR.tarePosition();

	// double direction = (distanceInInches > 0.0) ? 1.0 : -1.0;

	// while(fabs(FL.getPosition()) < targetTicks) {
	// 	double currentHeading = imu_sensor.get_heading();

	// 	double error = targetHeading - currentHeading;

	// 	if(error > 180) error -= 360;
	// 	if(error < -180) error += 360;

	// 	double correction = error * 2;

	// 	FL.moveVelocity(direction * speed + correction);
	// 	FR.moveVelocity(direction * speed - correction);
	// 	RL.moveVelocity(direction * speed + correction);
	// 	RR.moveVelocity(direction * speed - correction);

	// 	pros::delay(20);
	// }
	
	// FL.moveVelocity(0);
	// FR.moveVelocity(0);
	// RL.moveVelocity(0);
	// RR.moveVelocity(0);

	FL.moveRelative(targetTicks, speed);
    FR.moveRelative(targetTicks, -speed);
    RL.moveRelative(targetTicks, speed);
    RR.moveRelative(targetTicks, -speed);
	while (!FL.isStopped() || !FR.isStopped() || !RL.isStopped() || !RR.isStopped()) {
        pros::delay(20);
    }
}

void rotateRelative(double degrees, double speed) {
	double arcLength = 2 * M_PI * robotRadius * (degrees / 360.0);

	double targetTicks = arcLength * ticksPerInch / sqrt(2);

	FL.moveRelative(targetTicks, speed);
    FR.moveRelative(targetTicks, speed);
    RL.moveRelative(targetTicks, speed);
    RR.moveRelative(targetTicks, speed);
	while (!FL.isStopped() || !FR.isStopped() || !RL.isStopped() || !RR.isStopped()) {
        pros::delay(20);
    }
}

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
// void on_center_button() {
// 	static bool pressed = false;
// 	pressed = !pressed;
// 	if (pressed) {
// 		pros::lcd::set_text(2, "I was pressed!");
// 	} else {
// 		pros::lcd::clear_line(2);
// 	}
// }

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	imu_sensor.reset();
	pros::delay(2000);

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
void autonomous() {
	// moveForward(150.0, 12.0);
	// rotateRelative(360.0, 150.0);
	FL.moveRelative(-900, 100);
    FR.moveRelative(-900, 100);
    RL.moveRelative(-900, 100);
    RR.moveRelative(-900, 100);
	while (!FL.isStopped() || !FR.isStopped() || !RL.isStopped() || !RR.isStopped()) {
        pros::delay(20);
    }
	pros::delay(1000);
	tower.set_value(1);
}

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
	int ct = 0;
	bool intakeOn = false;

	// int intakeTime = 2000;
	// int intakeStartTime = 0;
	// bool intakePressed = false;
	bool intakeReverse = false;

	while (true) {
		//Handle pneumatics
		if(master.getDigital(ControllerDigital::L1)) tower.set_value(1);
		else tower.set_value(0);

		//Handle intake
		if(intakeOn) {
			if(!intakeReverse) {
				intake.moveVelocity(-200);
				conveyer.moveVelocity(200);
			} else {
				intake.moveVelocity(200);
				conveyer.moveVelocity(-200);
			}
		} else {
			intake.moveVelocity(0);
			conveyer.moveVelocity(0);
		}

		if(master.getDigital(ControllerDigital::X)) {
			intakeOn = true;
			intakeReverse = false;
			// if(!intakePressed) {
			// 	intakeStartTime = pros::millis();
			// 	intakePressed = true;
			// } else {
			// 	if(pros::millis() - intakeStartTime >= intakeTime) {
			// 		if(!intakeOn) intakeOn = true;
			// 		else intakeOn = false;

			// 		intakePressed = false;
			// 	}
			// }
		}

		if(master.getDigital(ControllerDigital::Y)) {
			intakeOn = true;
			intakeReverse = true;
			// if(!intakePressed) {
			// 	intakeStartTime = pros::millis();
			// 	intakePressed = true;
			// } else {
			// 	if(pros::millis() - intakeStartTime >= intakeTime) {
			// 		if(!intakeOn) intakeOn = true;
			// 		else intakeOn = false;

			// 		intakePressed = false;
			// 	}
			// }
		}

		if(master.getDigital(ControllerDigital::B)) {
			intakeOn = false;


			// if(!intakePressed) {
			// 	intakeStartTime = pros::millis();
			// 	intakePressed = true;
			// } else {
			// 	if(pros::millis() - intakeStartTime >= intakeTime) {
			// 		if(!intakeOn) intakeOn = true;
			// 		else intakeOn = false;

			// 		intakePressed = false;
			// 	}
			// }
		}

		ct++;	
		// master.setText(0,0, std::to_string(ct));
		if(ct == 100) {
			ct = 0;
			FLspeed = FL.getActualVelocity();
			FRspeed = FR.getActualVelocity();
			RLspeed = RL.getActualVelocity();
			master.clear();
			RRspeed = RR.getActualVelocity();

			// master.setText(0, 0, "FL: ");
			// master.setText(0, 8, "FR: ");
			// master.setText(1, 0, "RL: ");
			// master.setText(1, 8, "RR: ");
		}
		
			master.setText(1, 0, "FL: " + std::to_string((FLspeed)).substr(0,4) + " FR: " + std::to_string(FRspeed).substr(0,4));
			master.setText(2, 0, "RL: " + std::to_string((RLspeed)).substr(0,4) + " RR: " + std::to_string(RRspeed).substr(0,4));
			// master.setText(0, 10, "FR: " + std::to_string(static_cast<int>(FRspeed)));
			// master.setText(1, 0, "RL: " + std::to_string(static_cast<int>(RLspeed)));
			// master.setText(1, 8, "RR: " + std::to_string(static_cast<int>(RRspeed)));

		double strafe = master.getAnalog(ControllerAnalog::leftX);
		double turn = master.getAnalog(ControllerAnalog::rightX);
        double forwardd = master.getAnalog(ControllerAnalog::leftY);

		if(fabs(turn) < 0.5) turn = 0.0;
	
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
			double scaleFactor = 200.0 / maxSpeed;
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
		//  if (fabs(forwardd) > 0 or fabs(strafe) > 0 or fabs(turn) > 0) {
		// 	 FL.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
		// 	 FR.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
		// 	 RL.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
		// 	 RR.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
		//  } else {
		// 	 FL.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
		// 	 FR.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
		// 	 RL.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
		// 	 RR.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
		//  }


		pros::delay(20);
	}
}
