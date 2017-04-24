#pragma config(Sensor, S2,     puckS,          sensorEV3_IRSensor)
#pragma config(Sensor, S3,     goalS,          sensorEV3_Color)
#pragma config(Sensor, S4,     wallS,          sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          jawsM,         tmotorEV3_Medium, PIDControl, encoder)
#pragma config(Motor,  motorB,          rightM,        tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          leftM,         tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorD,          liftM,         tmotorNXT, PIDControl, encoder)

task main()
{
	int wallRange = 0;
	int puckRange = 0;

	bFloatDuringInactiveMotorPWM  = false;



	while(1){
  	wallRange = getUSDistance(wallS);
  	puckRange = getIRDistance(puckS);

  	displayTextLine(0, "Wall: %d", wallRange);
		displayTextLine(1, "Puck: %d", puckRange);

		sleep(500);
}


}
