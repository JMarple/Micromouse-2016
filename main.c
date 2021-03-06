#include "processor.h"
#include "mouse.h"
#include "motion.h"
#include "callistoMenu.h"
#include "callistoSerial.h"
#include "cellToCell.h"
#include "callistoMovement.h"
#include "maze.h"
#include "robotState.h"

void waitForHand();
void setupBasicMouseSettings(MouseInfo* info);

int main(void)
{
    Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();

    mouse->initiate();
    BuzzerBufferAddTone(&mouse->buzzer, 800, 200);
    proc->serialSetRXCallback(TestCallBackFunction);

    executeMainMenu();

    waitForHand();
    mouse->calibrateGyro();

    MouseInfo mouseInfo;
    MotionInfo motionInfo;

    mouseInfo.straightVelocity = 85;
    mouseInfo.straightAccel = 0.5;
    mouseInfo.turningVelocity = 75;
    mouseInfo.turningAcceleration = 0.01;
    mouseInfo.turningRadius = 50;
	mouseInfo.turnInLength = 1650;
	mouseInfo.turnOutLength = 1200;

	mouseInfo.fastStraightSpeed = 150;
	mouseInfo.fastStraightAccel = 0.5;
	mouseInfo.fastDiagonalSpeed = 150;
	mouseInfo.fastDiagonalAccel = 0.5;

	mouseInfo.fast90.radius = 90;
	mouseInfo.fast90.accel = 0.01;
	mouseInfo.fast90.speed = 75;
	mouseInfo.fast90.deg = 90;
	mouseInfo.fast90.inLength = 5500;

	mouseInfo.fastTight90.radius = 50;
	mouseInfo.fastTight90.accel = 0.01;
	mouseInfo.fastTight90.speed = 75;
	mouseInfo.fastTight90.deg = 90;

	mouseInfo.fast45.radius = 90;
	mouseInfo.fast45.accel = 0.01;
	mouseInfo.fast45.speed = 75;
	mouseInfo.fast45.deg = 45;
	mouseInfo.fast45.inLength = 700;
	mouseInfo.fast45.inLength45 = 4000;

	mouseInfo.fast135.deg = 135;
	mouseInfo.fast135.radius = 70;
	mouseInfo.fast135.speed = 75;
	mouseInfo.fast135.accel = 0.01;

    motionInfo.useWalls = 0;
    motionInfo.currentVelocity = 0;
    motionInfo.is45 = 0;

    setupBasicMouseSettings(&mouseInfo);

    Maze maze;
    RobotState state;

    /*FastStraight(&mouseInfo, &motionInfo, 1, 0, '>');
    FastTurn(&mouseInfo, &motionInfo, '>');
    FastDiagonal(&mouseInfo, &motionInfo, 1, '>', '^');
    FastTurn(&mouseInfo, &motionInfo, '^');
    FastDiagonal(&mouseInfo, &motionInfo, 1, '^', 'v');
    FastTurn(&mouseInfo, &motionInfo, 'v');
    FastDiagonal(&mouseInfo, &motionInfo, 1, 'v', '^');
    FastTurn(&mouseInfo, &motionInfo, '^');
    FastDiagonal(&mouseInfo, &motionInfo, 1, '^', '\\');
    FastTurn(&mouseInfo, &motionInfo, '\\');
    FastStraight(&mouseInfo, &motionInfo, 1, '^', '>');
    FastTurn(&mouseInfo, &motionInfo, '>');
    FastDiagonal(&mouseInfo, &motionInfo, 3, '>', '>');
    FastTurn(&mouseInfo, &motionInfo, '>');
    FastStraight(&mouseInfo, &motionInfo, 1, '>', 0);*/

    FastStraight(&mouseInfo, &motionInfo, 1, 0, '>', 1);
    FastTurn(&mouseInfo, &motionInfo, '>');
    FastDiagonal(&mouseInfo, &motionInfo, 3, '>', '<', 1);
    FastTurn(&mouseInfo, &motionInfo, '<');
    FastStraight(&mouseInfo, &motionInfo, 1, 0, '<', 0);
    /*FastDiagonal(&mouseInfo, &motionInfo, 2, '<', '<');
    FastTurn(&mouseInfo, &motionInfo, '<');
    FastStraight(&mouseInfo, &motionInfo, 1, '<', 0);*/

    //CallistoSearch(&maze, &state, &mouseInfo, &motionInfo);

/*    char * path = "flrf";

    SearchingFirstCell(&mouseInfo, &motionInfo);

    while (*path != 0)
    {
    	CallistoExecuteMovement(*path, &mouseInfo, &motionInfo);
    	path++;
    }*/

    mouse->motorValueLeft = 0;
    mouse->motorValueRight = 0;
    proc->setMotor(LEFT_MOTOR, mouse->motorValueLeft);
	proc->setMotor(RIGHT_MOTOR, mouse->motorValueRight);

	proc->setScreenWithString("DONE");
    // Stop n stuff
    while(1==1);

    return 0;
}

void waitForHand()
{
    Processor* proc = SingletonProcessor();

	while (proc->getSensor(SENSOR_LEFT_1) < 1200*32)
	{
		// Do nothing, just wait.
	}
}

void setupBasicMouseSettings(MouseInfo* info)
{
	info->searchingWallFrontValue = 17000;
	info->searchingWallExists = 12000;
	info->searchingLeftWallValue = 32000;
	info->searchingRightWallValue = 32000;
	info->firstCellDistance = 14200;
	info->normalCellDistance = 20000;
	info->diagonalCellDistance = 14000;

	// Setup Straight Trap Profile PID's
	PIDSetup(&info->lT1, 0.3, 0.0, 0.4);
	PIDSetup(&info->lT2, 0.6, 0.0, 0.1);
	PIDSetup(&info->lT3, 0.3, 0.0, 0.0);

	PIDSetup(&info->rT1, 0.3, 0.0, 0.4);
	PIDSetup(&info->rT2, 0.6, 0.0, 0.1);
	PIDSetup(&info->rT3, 0.3, 0.0, 0.0);

    info->linearProfile.rT1 = &info->rT1;
    info->linearProfile.rT2 = &info->rT2;
    info->linearProfile.rT3 = &info->rT3;
    info->linearProfile.lT1 = &info->lT1;
    info->linearProfile.lT2 = &info->lT2;
    info->linearProfile.lT3 = &info->lT3;

    // Setup Angular Velocity PID's
    // TODO: Are these ever used?
    PIDSetup(&info->glT1, 1.00, 0.0, 0.0);
    PIDSetup(&info->glT2, 1.00, 0.0, 0.0);
    PIDSetup(&info->glT3, 2.00, 0.0, 0.0);
    PIDSetup(&info->grT1, 1.00, 0.0, 0.0);
    PIDSetup(&info->grT2, 1.00, 0.0, 0.0);
    PIDSetup(&info->grT3, 2.00, 0.0, 0.0);

    info->angularProfile.lT1 = &info->glT1;
    info->angularProfile.lT2 = &info->glT2;
    info->angularProfile.lT3 = &info->glT3;
    info->angularProfile.rT1 = &info->grT1;
    info->angularProfile.rT2 = &info->grT2;
    info->angularProfile.rT3 = &info->grT3;

    // Setup Wall/Gyroscope PID's for straight corrections.
    PIDSetup(&info->gyroPositionPID, 1.00, 0.00, 0.00);
    PIDSetup(&info->wallPositionPID, 0.0003, 0.00, 0.00);
}

