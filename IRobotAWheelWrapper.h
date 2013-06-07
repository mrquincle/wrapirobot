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

    ~ActiveWheel() { }

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

    void enableIR(SPIDeviceNum boardNum, bool on) {
    	on ? front.ir.enable() : front.ir.disable();
    }

    void enableAmbientLight(SPIDeviceNum boardNum, bool on) {
    	on ? front.ambient.enable() : front.ambient.disable();
    }

    IRValues GetIRValues(SPIDeviceNum boardNum) {
    	return *front.ir.getAllValues();
    }

    uint16_t GetAmbientLight(SPIDeviceNum boardNum) {
    	return *front.ambient.getAllValues();
    }

    rgb_t GetRGB(SPIDeviceNum boardNum) {
    	return *front.rgbs.getAllValues();
    }

    motorData_t GetMotorDataFront() {
        printf("There is no middleware implementation of the ActiveWheel\n");
        return *front.hall.getAllValues();
    }

    motorData_t GetMotorDataRear() {
        printf("There is no middleware implementation of the ActiveWheel\n");
        return *front.hall.getAllValues();
    }

    bool SetLEDAll(SPIDeviceNum boardNum, uint8_t color) {
    	return front.led.all(color);
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
private:
    friend class IRobotFactory;

};

#endif // _IROBOTAWHEELWRAPPER_H
