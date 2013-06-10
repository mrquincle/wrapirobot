#ifndef _IROBOTAWHEELWRAPPER_H
#define _IROBOTAWHEELWRAPPER_H

#include "RobotAWheel.h"
#include "IRobotBaseWrapper.h"

/**
 * Derive ActiveWheel from RobotAWheel, however, make these methods invisible to the user so they are forced to use the old
 * IRobot interface and will not mix up the new HDMR+ interface with the old one. In the latter case you are better off
 * with using RobotAWheel directly namely.
 */
class ActiveWheel: protected RobotAWheel, public RobotBase {
public:
    ActiveWheel(): RobotBase(), RobotAWheel() {
        printf("There is no middleware implementation of the ActiveWheel\n");
    }

    //! Destructor is not virtual, because this class is not meant to be a base class anyway.
    ~ActiveWheel() {
    	printf("Deallocate the ActiveWheel robot\n");
    }

    enum Side { RIGHT = 2, FRONT = 3, LEFT = 0, REAR = 1 };

    bool isEthernetPortConnected(ActiveWheel::Side side) {
        // to figure out
        return true;
    }

    float GetHingeAngle() {
        printf("There is no middleware implementation of the ActiveWheel\n");
        return 0;
    }

    void EnableMotors(bool enable) {
        if (enable) motorEnable();
        else printf("You can not disable the motors");
    }

    motorData_t GetMotorDataFront() {
        printf("There is no middleware implementation of the ActiveWheel\n");
        return *front.hall.getAllValues();
    }

    motorData_t GetMotorDataRear() {
        printf("There is no middleware implementation of the ActiveWheel\n");
        return *front.hall.getAllValues();
    }

    acceleration_t GetAcceleration(ActiveWheel::Side side) {
        return *front.accel.getAllValues();
    }

    acceleration_t GetAcceleration() {
        return *front.accel.getAllValues();
    }

    bool MoveWheelsFront(int8_t vleft, int8_t vright) {
        front.motor.setVelocity(vleft);
        front.motor.setVelocity(vright);
        return true;
    }

	bool MoveWheelsRear(int8_t vleft, int8_t vright) {
        back.motor.setVelocity(vleft);
        back.motor.setVelocity(vright);
        return true;
    }

    bool MoveHinge(int8_t speed) {
    	return true;
    }

    bool MoveHingeToAngle(float angle, uint8_t speed) {
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
private:
    friend class IRobotFactory;

};

#endif // _IROBOTAWHEELWRAPPER_H
