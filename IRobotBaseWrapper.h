#ifndef _IROBOTBASEWRAPPER_H
#define _IROBOTBASEWRAPPER_H

#include <stdio.h>
#include <string>

typedef int SPIDeviceNum;

//! Forward declaration
class IRobotFactory;

/**
 * RobotBase is not derived from RobotCommon, because that file does not contain functionality shared across
 * the robots as the RobotBase class in irobot does. So, all functions here are just stubs that now have to
 * be implemented in the derived classes.
 */
class RobotBase {
public:
	RobotBase() {}

	virtual ~RobotBase() {
		printf("Destruct RobotBase\n");
	}

	enum RobotType
	{
		UNKNOWN,
		KABOT,
		ACTIVEWHEEL,
		SCOUTBOT
	};

	struct BoardFirmware {
		//! Default constructor creates KIT robot
		BoardFirmware() { type = RobotBase::KABOT; }
		BoardFirmware(char* boardID) {}
		~BoardFirmware() {}

		RobotType type;
		std::string board;
		int version_major, version_minor, version_revision;
	};

	static RobotBase* Instance(RobotBase *base = NULL) {
		static RobotBase * robotbase_instance;
		if (base != NULL) {
			printf("Set global instance to RobotBase\n");
			robotbase_instance = base;
		}
		return robotbase_instance;
	}

	static void MSPReset() {
		printf("%s(): is not implemented, not needed\n", __func__);
	}

	virtual void EnableMotors(bool enable) = 0;

	virtual void enableIR(SPIDeviceNum boardNum, bool on) = 0;

	virtual bool IsBoardRunning(SPIDeviceNum boardNum) = 0;

	virtual void enableAmbientLight(SPIDeviceNum boardNum, bool on) = 0;

	// Only active wheel and scout have hall sensors, e.g. front.hall.getAllValues();
	virtual motorData_t GetMotorData(SPIDeviceNum boardNum) = 0;

	virtual IRValues GetIRValues(SPIDeviceNum boardNum) = 0;

	virtual uint16_t GetAmbientLight(SPIDeviceNum boardNum) = 0;

	virtual rgb_t GetRGB(SPIDeviceNum boardNum) = 0;

	virtual acceleration_t GetAcceleration(SPIDeviceNum boardNum) = 0;

	/**
	 * Set all LEDS in a certain color.
	 *
	 * It is inconvenient, but it is defined as a pure virtual, because implementation in the base class itself will be
	 * quite hard for the different robots having different numbering schemes for the sides in IRobot as well as in the
	 * HDMR+ middleware.
	 */
	virtual void SetLEDAll(SPIDeviceNum boardNum, uint8_t color) = 0;

	virtual void SetIRLED(SPIDeviceNum boardNum, uint8_t LEDMask) = 0;

	virtual void SetIRPulse(SPIDeviceNum boardNum, uint8_t PulseMask) = 0;

	virtual void SetIRMode(SPIDeviceNum boardNum, IRLEDMode mode) = 0;
};


#endif // _IROBOTBASEWRAPPER_H
