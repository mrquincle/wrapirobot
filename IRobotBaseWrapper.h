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

	virtual ~RobotBase() {}

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

	/**
	 * The default robot that will be returned is a KIT robot, so it does not use anymore the MSP
	 * communication to detect what the type of robot is that it is running. This should be solved.
	 */
	//    static RobotType Initialize(std::string controllerName,
	//            RobotBase::RobotType t = KABOT, bool activate = true) {
	//    	firmware.type = t;
	//        return t;
	//    }

	static void MSPReset() {
		printf("%s is not implemented", __func__);
	}

	void EnableMotors(bool enable) {
		printf("%s is not implemented in base class\n", __func__);
	}

	void enableIR(SPIDeviceNum boardNum, bool on) {
		printf("%s is not implemented in base class\n", __func__);
	}

	bool IsBoardRunning(SPIDeviceNum boardNum) {
		printf("No check anymore if a board is running...\n");
		return true;
	}

	void enableAmbientLight(SPIDeviceNum boardNum, bool on) {
		printf("%s is not implemented in base class\n", __func__);
	}

	// Only active wheel and scout have hall sensors, e.g. front.hall.getAllValues();
	motorData_t GetMotorData(SPIDeviceNum boardNum);

	IRValues GetIRValues(SPIDeviceNum boardNum);

	uint16_t GetAmbientLight(SPIDeviceNum boardNum);

	rgb_t GetRGB(SPIDeviceNum boardNum);

	acceleration_t GetAcceleration(SPIDeviceNum boardNum);

	//! Set all LEDS in a certain color
	bool SetLEDAll(SPIDeviceNum boardNum, uint8_t color);

	void SetIRLED(SPIDeviceNum boardNum, uint8_t LEDMask) {
		printf("%s is not implemented in base class\n", __func__);
	}

	void SetIRPulse(SPIDeviceNum boardNum, uint8_t PulseMask) {
		printf("%s is not implemented in base class\n", __func__);
	}

	void SetIRMode(SPIDeviceNum boardNum, IRLEDMode mode) {
		printf("%s is not implemented in base class\n", __func__);
	}
};


#endif // _IROBOTBASEWRAPPER_H
