#ifndef _IROBOTSCOUTWRAPPER_H
#define _IROBOTSCOUTWRAPPER_H

#include "RobotScout.h"
#include "IRobotBaseWrapper.h"

#include <cassert>
#include <cstdio>

/**
 * Derive ScoutBot from RobotScout, however, make these methods invisible to the user so they are forced to use the old
 * IRobot interface and will not mix up the new HDMR+ interface with the old one. In the latter case you are better off
 * with using RobotScout directly namely.
 */
class ScoutBot : protected RobotScout, public RobotBase {
public:
	ScoutBot(): RobotScout(), RobotBase() {
		printf("Creation of the ScoutBot\n");
	}

	//! Destructor is not virtual, because this class is not meant to be a base class anyway.
	~ScoutBot() {
		printf("Deallocate the ScoutBot robot\n");
	}

	enum Side { FRONT = 0, RIGHT = 3, LEFT = 1, REAR = 2 };

	void EnableMotors(bool enable) {
		if (enable) motorEnable();
		else fprintf(stderr, "%s(): you can not disable the motors\n", __func__);
	}

	/**
	 * We used to have a command for the left and right "track". Now we have velocities for the front and back PCB
	 * boards. Not so intuitive...
	 */
	void Move(int8_t vleft, int8_t vright) {
		printf("%s(): \n", __func__);
		front.motor.setVelocity(vleft);
		back.motor.setVelocity(vright);
	}

	void CloseDocking(ScoutBot::Side side) {
		switch(side) {
		case FRONT: front.dock.close(); break;
		case RIGHT: right.dock.close(); break;
		case LEFT: left.dock.close(); break;
		case REAR: back.dock.close(); break;
		}
	}

	void OpenDocking(ScoutBot::Side side) {
		switch(side) {
		case FRONT: front.dock.open(); break;
		case RIGHT: right.dock.open(); break;
		case LEFT: left.dock.open(); break;
		case REAR: back.dock.open(); break;
		}
	}

	void CalibrateDocking(ScoutBot::Side side) {
		bool result;
		switch(side) {
		case FRONT: result = front.dock.calibrate(); break;
		case RIGHT: result = right.dock.calibrate(); break;
		case LEFT: result = left.dock.calibrate(); break;
		case REAR: result = back.dock.calibrate(); break;
		}
	}

	void MoveDocking(ScoutBot::Side side, int8_t v) {
		printf("%s(): was not recommend, so not re-implemented\n", __func__);
	}

	void MoveHinge(int8_t speed) {
		if (speed == 0)
			hinge.stop();
		else {
			printf("%s(): you will need to stop the hinge too!\n", __func__);
			hinge.move(0, speed, 0);
		}
	}

	/**
	 * The move up and move down commands require uint16_t values, check if int8_t is actually sufficient.
	 */
	void MoveHingeToAngle(int8_t angle) {
		printf("%s(): might need hinge.enable() first\n", __func__);
		if (angle < 0) hinge.moveDown(angle);
		if (angle > 0) hinge.moveUp(angle);
	}

	hingeData_t GetHingeStatus() {
		HingeValues_t *values;
		values = hinge.getAllValues();
		fprintf(stderr, "%s(): conversion error, HingeValues is an int, hingeData_t expects two angle values!\n", __func__);
//		return *values;
	}

	void EnablePowerSharingCapability(bool on) {
		left.powermgt.setPower(PM_ESHARING, on);
	}

	void EnablePowerSharing(ScoutBot::Side side, bool on) {
		fprintf(stderr, "%s(): how to enable power sharing per docking unit?\n", __func__);
	}

	bool isEthernetPortConnected(ScoutBot::Side side) {
		printf("%s(): no way to check if ethernet is there\n", __func__);
		return true;
	}

    acceleration_t GetAccelerationFront() {
    	return *front.accel.getAllValues();
    }

    acceleration_t GetAccelerationRear() {
    	return *back.accel.getAllValues();
    }

    void EnableHallSensors(bool on) {
    	fprintf(stderr, "%s(): how to enable these?\n", __func__);
    	//front.hall.updatePeriodicStart(on);
    }

    hallSensorScout_t GetHallSensorValues2D(void) {
    	HALLValues_t* values;
    	fprintf(stderr, "%s(): where are these?\n", __func__);
    	values = front.hall.getAllValues(); // this returns a motorData object!
    	fprintf(stderr, "%s(): conversion error!\n", __func__);
//    	return *values;
    }

    int GetMicrophoneBuffer(ScoutBot::Side side, uint16_t *buffer, int buffer_size) {
    	fprintf(stderr, "%s(): microphone doesn't work anyway\n", __func__);
    	return -1;
    }

    /**
     * Laser is on the right PCB at the Scout robot, even if it is pointed towards to the front.
     */
	void activateLaser(bool on) {
		on ? right.laser.on() : right.laser.off();
	}

	uint8_t GetDScrewRevolutions(ScoutBot::Side side) {
		fprintf(stderr, "%s(): getAllValues returns here 64-bit, I have only 8-bit to put it in\n", __func__);
		ICMH_t* values = NULL;
		switch(side) {
		case FRONT: values = front.motor.getAllValues(); break;
		case RIGHT: assert(false); break;
		case LEFT: assert(false); break;
		case REAR: values = back.motor.getAllValues(); break;
		}
		return *values;
	}

    uint16_t GetDScrewISense(ScoutBot::Side side) {
		fprintf(stderr, "%s(): where does this 16-bit come from?\n", __func__);
    }

    /*******************************************************************************************************************
     * Implementations from the base class have now be implemented in the separated classes, because the base class has
     * no access to front, right, left, and back objects, and not even the general "sides". The functions below this
     * line should however be reachable through sides[i], and the annoying switch statements that prevent the user from
     * for example assuming that there is a motor on a board, where it is not, are not necessary anymore.
     * The classes that belong to a common Side object:
  		Identity identity;
		Reset rst;
		Dock dock;
		IR ir;
		Ambient ambient;
		Accelerometer accel;
		RGBS rgbs;
		RemoteControl rc;
		Compass compass;
		HALL hall;
		LED led;
		Laser laser;
     *
     ******************************************************************************************************************/

    bool IsBoardRunning(SPIDeviceNum boardNum) {
    	fprintf(stderr, "%s(): how to see this?\n", __func__);
    	return true;
    }

	void SetLED(SPIDeviceNum boardNum, uint8_t rgbTopLeft, uint8_t rgbTopRight, uint8_t rgbBotLeft, uint8_t rgbBotRight) {
		sides[boardNum]->led.single(POSTOPLEFT, rgbTopLeft);
		sides[boardNum]->led.single(POSTOPRIGHT, rgbTopRight);
		sides[boardNum]->led.single(POSBOTLEFT, rgbBotLeft);
		sides[boardNum]->led.single(POSBOTRIGHT, rgbBotRight);
	}

	/**
	 * The color is defined in the IRobot file InterfaceTypes.h. This defines masks such as LED_WHITE, LED_RED, LED_CYAN
	 * and to be able to turn the light off, LED_OFF.
	 */
	void SetLEDAll(SPIDeviceNum boardNum, uint8_t color) {
		sides[boardNum]->led.all(color);
	}

	/**
	 * Positions are numbered 0 to 3 in the form of a LEDPosition enum in InterfaceTypes.h:
	 * 	POSTOPLEFT, POSTOPRIGHT, POSBOTLEFT, POSBOTRIGHT
	 */
	void SetLEDOne(SPIDeviceNum boardNum, uint8_t LEDPos, uint8_t color) {
		LEDPosition pos = (LEDPosition)LEDPos;
		sides[boardNum]->led.single(pos, color);
	}

    /**
     * What is this actually?
     */
	void enableAccelerometer(SPIDeviceNum boardNum, bool on) {
		if (on) sides[boardNum]->accel.enable();
		else sides[boardNum]->accel.disable();
	}

	void enableAmbientLight(SPIDeviceNum boardNum, bool on) {
		if (on) sides[boardNum]->ambient.enable();
		else sides[boardNum]->ambient.disable();
	}

	void enableDockingSense(SPIDeviceNum boardNum, bool on) {
		if (on) sides[boardNum]->dock.enableSensor();
		else sides[boardNum]->dock.disableSensor();
	}

	void enableIR(SPIDeviceNum boardNum, bool on) {
		if (on) sides[boardNum]->ir.enable();
		else sides[boardNum]->ir.disable();
	}

	void enableMicrophone(SPIDeviceNum boardNum, bool on, int sampleRate) {
    	fprintf(stderr, "%s(): microphone doesn't work anyway\n", __func__);
	}

	void enableRGBSensor(SPIDeviceNum boardNum, bool on) {
		if (on) sides[boardNum]->rgbs.enable();
		else sides[boardNum]->rgbs.disable();
	}

	IRValues GetIRValues(SPIDeviceNum boardNum) {
		IRValues *values = NULL;
		values = sides[boardNum]->ir.getAllValues();
		return *values;
	}

	void SetIRLED(SPIDeviceNum boardNum, uint8_t LEDMask) {
		sides[boardNum]->ir.selectLED(LEDMask);
	}

	void SetIRPulse(SPIDeviceNum boardNum, uint8_t PulseMask) {
		sides[boardNum]->ir.selectPulse(PulseMask);
	}

	void SetIRMode(SPIDeviceNum boardNum, IRLEDMode mode) {
		sides[boardNum]->ir.setMode(mode);
	}

	void CalibrateIR(SPIDeviceNum boardNum) {
		sides[boardNum]->ir.calibrate();
	}

	void SendIRMessage(SPIDeviceNum boardNum, uint8_t* message, uint8_t size) {
		ir_comm_t msg;
		assert(size < 256);
		memcpy(msg.data, message, size);
		msg.len = size;
		sides[boardNum]->ir.sendMessageString(&msg);
	}

	void SetIRRX(SPIDeviceNum boardNum, bool topRX) {
		sides[boardNum]->ir.rxmux(topRX);
	}

	void GetIRMessage(SPIDeviceNum boardNum, uint8_t *buffer, uint8_t buffer_size) {
		ir_comm_t *msg;
		msg = sides[boardNum]->ir.getCommValues();
		if (buffer_size != msg->len) {
			fprintf(stderr, "%s(): buffer_size is not by reference, can they actually be different? \n", __func__);
			assert(buffer_size == msg->len);
		}
		memcpy(buffer, msg->data, msg->len);
	}

    uint16_t GetAmbientLight(SPIDeviceNum boardNum) {
    	AmbientValues_t* values = NULL;
		values = sides[boardNum]->ambient.getAllValues();
		return *values;
    }

    rgb_t GetRGB(SPIDeviceNum boardNum) {
    	rgb_t* values = NULL;
		values = sides[boardNum]->rgbs.getAllValues();
		return *values;
    }

    acceleration_t GetAcceleration(SPIDeviceNum boardNum) {
    	acceleration_t* values = NULL;
		values = sides[boardNum]->accel.getAllValues();
		return *values;
    }

    motorData_t GetMotorData(SPIDeviceNum boardNum) {
    	fprintf(stderr, "%s(): worrisome HDRM+ middleware, only specific motor data available\n", __func__);
		ICMH_t* values = NULL;
		switch(boardNum) {
		case FRONT: values = front.motor.getAllValues(); break;
		case RIGHT: assert(false); break;
		case LEFT: assert(false); break;
		case REAR: values = back.motor.getAllValues(); break;
		}
		fprintf(stderr, "%s(): what is the difference between ICMH_t and motorData_t?\n", __func__);
//		return *values;
    }

/*
    UbisenseClient::TagInfo getUbisensePosition() {
    	fprintf(stderr, "%s(): ubisense code is not compiled into new libirobot_app.so yet\n", __func__);
    	UbisenseClient::TagInfo tag;
    	tag = UbisenseClient::getPos();
    	//std::cout << tag.tagID << " " << tag.p.x << " " << tag.p.y << " " << tag.p.z << std::endl; ;
    	return tag;
    }

    void setUbisenseTagID(uint32_t id = 0) {
    	  UbisenseClient::setIDToListen(id);
    }
*/


private:
	friend class IRobotFactory;
};

#endif // _IROBOTSCOUTWRAPPER_H
