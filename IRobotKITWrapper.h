#ifndef _IROBOTKITWRAPPER_H
#define _IROBOTKITWRAPPER_H

#include "RobotKIT.h"
#include "IRobotBaseWrapper.h"

/**
 * Derive KaBot from RobotKIT, however, make these methods invisible to the user so they are forced to use the old
 * IRobot interface and will not mix up the new HDMR+ interface with the old one. In the latter case you are better off
 * with using RobotKIT directly namely.
 */
class KaBot: protected RobotBase, public RobotKIT {
public:
    KaBot(): RobotBase(), RobotKIT() {

    }

    virtual ~KaBot() { }

    enum Side { FRONT = 0, LEFT = 1, REAR = 2, RIGHT = 3 };

    void EnableMotors(bool enable) {
        if (enable) motorEnable();
        else printf("You can not disable the motors");
    }

    void enableIR(SPIDeviceNum boardNum, bool on) {
    	on ? front.ir.enable() : front.ir.disable();
    }

    bool SetLEDAll(SPIDeviceNum boardNum, uint8_t color) {
    	return front.led.all(color);
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
private:
    friend class IRobotFactory;

};

#endif // _IROBOTKITWRAPPER_H
