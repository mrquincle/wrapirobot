#ifndef _IROBOTKITWRAPPER_H
#define _IROBOTKITWRAPPER_H

#include "RobotKIT.h"
#include "IRobotBaseWrapper.h"

#include <cassert>

/**
 * Derive KaBot from RobotKIT, however, make these methods invisible to the user so they are forced to use the old
 * IRobot interface and will not mix up the new HDMR+ interface with the old one. In the latter case you are better off
 * with using RobotKIT directly namely.
 */
class KaBot: protected RobotBase, public RobotKIT {
public:
	KaBot(): RobotBase(), RobotKIT() {

	}

	//! Destructor is not virtual, because this class is not meant to be a base class anyway.
	~KaBot() {
		printf("Deallocate the Kabot robot\n");
	}

	enum Side { FRONT = 0, LEFT = 1, REAR = 2, RIGHT = 3 };

	void EnableMotors(bool enable) {
		if (enable) motorEnable();
		else printf("You can not disable the motors\n");
	}

	void MoveScrewFront(int8_t v) {
		front.motor.setVelocity(v);
	}

	void MoveScrewRear(int8_t v) {
		back.motor.setVelocity(v);
	}

	void MoveHingeToAngle(uint16_t angle) {
		//
	}

	/**
	 * Although the laser is mounted on the front side of the robot and the electronics that turns on the laser is also on the
	 * front PCB.
	 */
	void activateLaser(bool on) {
		on ? front.laser.on() : front.laser.off();
		//on ? right.laser.on() : right.laser.off();
	}

	IRValues GetIRValues(KaBot::Side side) {
		return *front.ir.getAllValues();
	}

	uint16_t GetHingeAngle() {
		printf("Seems to be forgotten. A grep on hinge only shows a mapping for the SPI bus, no command\n");
		return 0;
	}

	rgb_t GetRGB(KaBot::Side side) {
		return *front.rgbs.getAllValues();
	}

	uint16_t GetAmbientLight(KaBot::Side side) {
		return *front.ambient.getAllValues();
	}

	bool isEthernetPortConnected(KaBot::Side side) {
		// to figure out
		return true;
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
		RGBS rgbs;
		RemoteControl rc;
		LED led;
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
		fprintf(stderr, "%s(): worrisome on HDRM+ middleware, only for specific boards\n", __func__);
		switch(boardNum) {
		case FRONT: front.accel.enable(); break;
		case RIGHT: assert(false); break;
		case LEFT: assert(false); break;
		case REAR: back.accel.enable(); break;
		}
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
		switch(boardNum) {
		case FRONT: values = front.accel.getAllValues(); break;
		case RIGHT: assert(false); break;
		case LEFT: assert(false); break;
		case REAR: values = back.accel.getAllValues(); break;
		}
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

private:
	friend class IRobotFactory;

};

#endif // _IROBOTKITWRAPPER_H
