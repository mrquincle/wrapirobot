#ifndef _IROBOTWRAPPER_H
#define _IROBOTWRAPPER_H

#include "IRobotBaseWrapper.h"
#include "IRobotKITWrapper.h"
#include "IRobotScoutWrapper.h"
#include "IRobotAWheelWrapper.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Very clever the construction with the singleton pattern with RobotBase that returns the right
 * robot instance. There is no need for that here. It makes more sense to separate the basis functionality
 * from the constructor/destructor functionality.
 * SO: http://stackoverflow.com/questions/137975/what-is-so-bad-about-singletons
 */
class IRobotFactory {
public:
	//! Default robot is a KIT robot
	IRobotFactory(): robot(NULL), firmware(new RobotBase::BoardFirmware()) {
	}

	~IRobotFactory() {
		if (robot != NULL) delete robot;
		delete firmware;
	}

	RobotBase * GetRobot() {
		if (robot != NULL) return robot;
		GetType();
		return robot;
	}

	RobotBase::RobotType GetType() {
		char *robot_type = getenv("ROBOT");
		if (robot_type != NULL) {
			if (!strcmp(robot_type, "activewheel"))
				firmware->type = RobotBase::ACTIVEWHEEL;
			else if (!strcmp(robot_type, "scout"))
				firmware->type = RobotBase::SCOUTBOT;
			else if (!strcmp(robot_type, "backbone"))
				firmware->type = RobotBase::KABOT;
			else {
				fprintf(stderr, "ERROR! There is no environmental variable \"ROBOT\" defined!\n");
				fprintf(stderr, "run export ROBOT=activewheel, export ROBOT=scout, or export ROBOT=backbone\n");
			}
		}
		if (robot == NULL) {
			SetRobot(firmware->type);
		} else {
			printf("Robot is already set before\n");
		}
		return firmware->type;
	}

	void SetRobot(RobotBase::RobotType type) {
		printf("%s\n",__func__);
		int ret = 0;
		switch(type) {
		case RobotBase::KABOT:
			robot = new KaBot();
			if ((ret = ((KaBot*)robot)->init(RobotFunc::USES_SPI)) != 0) {
				printf("Error (%d) on init(). Exiting\n", ret);
			} else {
				printf("Initialized SPI daemon\n");
			}
			break;
		case RobotBase::SCOUTBOT:
			robot = new ScoutBot();
			if ((ret = ((ScoutBot*)robot)->init(RobotFunc::USES_SPI)) != 0) {
				printf("Error (%d) on init(). Exiting\n", ret);
			} else {
				printf("Initialized SPI daemon\n");
			}
			break;
		case RobotBase::ACTIVEWHEEL:
			robot = new ActiveWheel();
			if ((ret = ((ActiveWheel*)robot)->init(RobotFunc::USES_SPI)) != 0) {
				printf("Error (%d) on init(). Exiting\n", ret);
			} else {
				printf("Initialized SPI daemon\n");
			}
			break;
		case RobotBase::UNKNOWN: default:
			fprintf(stderr, "Unknown robot, a KIT robot will be created\n");
			robot = new KaBot();
			break;
		}
		RobotBase::Instance(robot);
	}
private:
	RobotBase *robot;
	RobotBase::BoardFirmware *firmware;
};

#endif // _IROBOTWRAPPER_H
