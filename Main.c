#pragma config(Sensor, S2,     puckS,          sensorEV3_IRSensor)
#pragma config(Sensor, S3,     goalS,          sensorEV3_Color)
#pragma config(Sensor, S4,     wallS,          sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          jawsM,         tmotorEV3_Medium, PIDControl, encoder)
#pragma config(Motor,  motorB,          rightM,        tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          leftM,         tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorD,          liftM,         tmotorNXT, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//return the number of degrees to turn the wheel distance cm
float getDegrees(float distance){

	return (distance / (2 * PI * 3.3)) * 360;
}

//move the robot forward a given numver of cm
void moveForward( float distance, int speed){
	float target_degrees = getDegrees(distance);
	//displayCenteredTextLine(2, "Target degrees: %2.2f" , target_degrees);

	//ensure both motors going forwards
	setMotorReversed(leftM, false);
	setMotorReversed(rightM, false);

	setMotorSync(leftM, rightM, 1, 1);


	moveMotorTarget(leftM, target_degrees, speed);
	moveMotorTarget(rightM, target_degrees, speed);
}

//lift the jaws to the up position
void jawsHigh(void){
	setMotorReversed(liftM, true);
	moveMotorTarget(liftM, 110, 10);
	delay(500);
}

//lower the jaws to the down position
void jawsLow(void){
	//lower the motor till it hits the ground
	setMotorBrakeMode(liftM, 0);
	setMotorReversed(liftM, false);
	moveMotorTarget(liftM, 110, 20);
	delay(500);

	//raise the jaws up just a little bit so they don't scrape
	//setMotorBrakeMode(liftM, 1);
	//setMotorReversed(liftM, true);
	//moveMotorTarget(liftM, 20, 10);

	//added the non scrapping method after releasing the puck
}

//open the jaws
void jawsOpen(void){
	setMotorReversed(jawsM, false);
	moveMotorTarget(jawsM, 115, 60);
}

//close the jaws
void jawsClose(void){
		setMotorReversed(jawsM, true);
		moveMotorTarget(jawsM, 115, 60);
}

//the number of degrees to turn the robot
//direction === true is left, false is right
void turn(bool direction, float degrees, int speed){
	float wheel_width = 16;
	float full_circumfrence= PI*wheel_width/2;

	float turn_distance = degrees/360 * full_circumfrence;
	float turn_degrees = getDegrees(turn_distance);

	if (!direction){
		setMotorReversed(leftM, false);
		setMotorReversed(rightM, true);

	}
	else {
		setMotorReversed(leftM, true);
		setMotorReversed(rightM, false);
	}

	moveMotorTarget(leftM, turn_degrees, speed);
	moveMotorTarget(rightM, turn_degrees, speed);

}

void exitMaze(void){
	displayCenteredTextLine(2, "IRval: %d", getIRDistance(puckS));
	while( getIRDistance(puckS) > 5){
				moveForward(2,30);
				delay(200);
				displayCenteredTextLine(2, "IRval: %d", getIRDistance(puckS));
	}

	turn(true, 90, 20);
	delay(1000);
	displayCenteredTextLine(2, "IRval: %d", getIRDistance(puckS));

	while ( getIRDistance(puckS) > 30){
		moveForward(2, 30);
		delay(200);
		displayCenteredTextLine(2, "IRval: %d", getIRDistance(puckS));
	}

	turn(false, 80, 20);
	delay(500);
	displayCenteredTextLine(2, "IRval: %d", getIRDistance(puckS));

	moveForward(40,30);

	delay(200);
	displayCenteredTextLine(2, "IRval: %d", getIRDistance(puckS));
}

void findLight(){
	//for testing, stop scraping
	//setMotorBrakeMode(liftM, 1);
	//setMotorReversed(liftM, true);
	//moveMotorTarget(liftM, 40, 20);
	//sleep(500);//needs time to finish


	int bestLight = 0;
	int location = 0;
	for (int i=0;i<36;i++){
		int temp = getColorAmbient(goalS);
		displayTextLine(0, "Angle: %d", i*10);
		displayTextLine(1, "Light: %d", temp);
		displayTextLine(2, "Best Angle: %d", location*10);
		displayTextLine(3, "Best Light: %d", bestLight);
   	if(temp > bestLight){
   		bestLight = temp;
   		location = i;
   	}
   	turn(true, 10, 20);
   	sleep(500);
	}
	turn(true, location*10, 20);//turn towards the brightest direction
	sleep(1000);
}

void pickupPuck(void){
	jawsLow();
	sleep(2000);
	jawsClose();
	sleep(2000);
	jawsHigh();
	sleep(2000);
}

void disposePuck(){
	int light = getColorAmbient(goalS);
	int samepos = 10;
	while(getColorAmbient(goalS) >= light){
		int temp = getColorAmbient(goalS);

		if(temp == light){
			if(samepos == 0){
				break;
			}
			samepos--;
		}else{
			samepos = 10;
		}

		light = temp;

		displayCenteredTextLine(0, "light: %d", light);
		displayCenteredTextLine(0, "same position: %d", samepos);

		moveForward(10,10);
		sleep(500);
	}

	//drop puck
	jawsOpen();
	sleep(2000);
	//turn away
	turn(false, 90, 40);
	//lower jaw
	jawsLow();
	//stop scrapping
	setMotorBrakeMode(liftM, 1);
	setMotorReversed(liftM, true);
	moveMotorTarget(liftM, 40, 20);
	sleep(500);
}

void searchforPucks(void){
	//position the jaw at lowest angle otherwise this wont work

	//stop scrapping
	setMotorBrakeMode(liftM, 1);
	setMotorReversed(liftM, true);
	moveMotorTarget(liftM, 40, 20);
	sleep(500);//needs time to finish

	//move straight until you find a puck
	while ( getIRDistance(puckS) > 80){
		moveForward(2, 15);
		delay(1000);
		displayCenteredTextLine(0, "IRval: %d", getIRDistance(puckS));
	}

	//lift and open the jaws
	jawsHigh();
	jawsOpen();

	//adjust so the jaws are over the puck
	turn(false, 10, 10);
	delay(1000);
	moveForward(15, 10);
	delay(3000);

	//pickupPuck(); //done

	//somehow test if we picked up the puck or dropped it

	//findLight(); //done

	//disposePuck(); //robot stops too early
}

task main()
{
	waitForButtonPress();
	//TEST FUNCTIONS
	//exitMaze();
	//findLight()
	disposePuck();

	//MAIN FUNCTIONS
	//searchforPucks();

	while (1){
		waitForButtonPress();
		//moveForward(10);
		//turn(true, 90, 25);
		//jawsOpen();
		//sleep(1000);
		//jawsClose();

		//setMotorBrakeMode(liftM, 0);
		int enc = getMotorEncoder(liftM);
		//displayCenteredTextLine(2, "enc: %d", enc);

		delay(100);
	}
}