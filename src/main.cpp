#include "main.h"

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
	Controller master;
	// pros::Motor left_mtr(1);
	// pros::Motor right_mtr(2);

	// std::shared_ptr<ChassisController> drive = 
	// 	ChassisControllerBuilder()
	// 		.withMotors(1,  // Top left
	// 					-2, // Top right (reversed)
	// 					-3, // Bottom right (reversed)
	// 					4   // Bottom left
	// 					)
	// 		.withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
	// 		.build();


	// std::shared_ptr<XDriveModel> drive  = std::make_shared<XDriveModel>(
    //     std::make_shared<Motor>(5),   // Top left motor
    //     std::make_shared<Motor>(-4),  // Top right motor (reversed)
    //     std::make_shared<Motor>(7),  // Bottom right motor (reversed)
    //     std::make_shared<Motor>(-16),   // Bottom left motor
	// 	std::make_shared<RotationSensor>(1, false),
	// 	std::make_shared<RotationSensor>(2, false),
    //     // 11.5_in  // Track width
	// 	600.0,
	// 	12000.0
    // );


	// std::shared_ptr<okapi::ChassisController> drive1 =
    //     okapi::ChassisControllerBuilder()
    //             .withMotors(
	// 							1,
    //                             -4,
    //                             -7,
	// 							5
    //                        )
    //             .withDimensions({okapi::AbstractMotor::gearset::blue, (1.0/1.0)}, {{2.75_in, 15_in}, okapi::imev5BlueTPR})
    //             .withOdometry()
    //             .buildOdometry();


				

// auto drive { std::dynamic_pointer_cast<okapi::XDriveModel>(drive1->getModel()) };

// drive -> getBottomLeftMotor() -> setVoltageLimit(2000);
// drive -> getBottomRightMotor() -> setVoltageLimit(2000);
// drive -> getTopLeftMotor() -> setVoltageLimit(2000);
// drive -> getTopRightMotor() -> setVoltageLimit(2000);




// std::shared_ptr<AsyncMotionProfileController> profileController =
//   AsyncMotionProfileControllerBuilder()
//     .withLimits({
//       1.0, // Maximum linear velocity of the Chassis in m/s
//       2.0, // Maximum linear acceleration of the Chassis in m/s/s
//       10.0 // Maximum linear jerk of the Chassis in m/s/s/s
//     })
//     .withOutput(drive1)
//     .buildMotionProfileController();



	// drive1->moveDistance(1_m);




	okapi::Motor motor1(1);
    okapi::Motor motor2(4);  // Reversed
    okapi::Motor motor3(7);  // Reversed
    okapi::Motor motor4(5);
	// Motor motor1(5);
	// Motor motor2(-4);
	// Motor motor3(-7);
	// auto mtr = ADIMotor('A');
	// pros::adi::DigitalOut test('A', false, false);
	// pros::ADIMotor mtr('A');
	pros::ADIDigitalOut test('A');
	test.set_value(0);

	// Motor motor4(16);

	okapi::Motor conveyer(5);
	okapi::Motor intake(1);

	while (true) {
		if(master.getDigital(ControllerDigital::L1)) test.set_value(1);
		else test.set_value(0);

		// test.set_value(1);
		// pros::delay(10000);
		// test.set_value(0);

		// int left = master.get_analog(ANALOG_LEFT_Y);
		// int right = master.get_analog(ANALOG_RIGHT_Y);


		// left_mtr = left;
		// right_mtr = right;

		if(master.getDigital(ControllerDigital::X)) {
			intake.moveVelocity(-200);
			conveyer.moveVelocity(200);
		} else {
			intake.moveVelocity(0);
			conveyer.moveVelocity(0);
		}

		// intake.moveVelocity(-200);
		// motor
		

		double strafe = master.getAnalog(ControllerAnalog::leftY);
        // double strafe = master.getAnalog(ControllerAnalog::rightY);
		double turn = master.getAnalog(ControllerAnalog::leftX);
        double forwardd = master.getAnalog(ControllerAnalog::rightX);
		std::string s = "";
		s += forwardd;
		// master.clearLine(0);
		master.setText(0,0, s);

		// drive -> getModel() -> xArcade(1.0, 2.0, 3.0, 3.0);
		// drive -> xArcade(strafe, forwardd, turn);
		// std::string test = "";
		// test += drive -> getTopLeftMotor() -> getActualVelocity();

		// master.setText(0, 0, test);



			// drive -> xArcade(forward, turn);
			// drive -> forward(2);


		// std::cout << forwardd << " " << strafe << " " << turn << std::endl;
		double m1speed = forwardd + turn + strafe;  // Top left
		double m2speed = forwardd - turn - strafe;  // Top right
		double m3speed = forwardd + turn - strafe;  // Bottom right
		double m4speed = forwardd - turn + strafe;  // Bottom left
		double maxSpeed = std::max({fabs(m1speed), fabs(m2speed), fabs(m3speed), fabs(m4speed)});
		if (maxSpeed > 0.0) {
			double scaleFactor = 600.0 / maxSpeed;
			m1speed *= scaleFactor;
			m2speed *= scaleFactor;
			m3speed *= scaleFactor;
			m4speed *= scaleFactor;
    	}
		motor1.moveVelocity(m1speed);
		motor2.moveVelocity(m2speed);
		motor3.moveVelocity(m3speed);
		motor4.moveVelocity(m4speed);



		//  if (forwardd > 0 or strafe > 0 or turn > 0) {
        //                         drive->setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
        //                 }
        //                 else {
        //                         drive->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
        //                 }

		// std::cout << drive -> getBottomLeftMotor() -> getActualVelocity() << std::endl;


		pros::delay(20);
	}
}
