#ifndef _IROBOTSCOUTWRAPPER_H
#define _IROBOTSCOUTWRAPPER_H

#include "RobotScout.h"
#include "IRobotBaseWrapper.h"

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

    ~ScoutBot() { }

    enum Side { FRONT = 0, RIGHT = 3, LEFT = 1, REAR = 2 };

    bool isEthernetPortConnected(ScoutBot::Side side) {
        // to figure out
        return true;
    }

    void EnableMotors(bool enable) {
        if (enable) motorEnable();
        else fprintf(stderr, "You can not disable the motors\n");
    }

    void enableIR(SPIDeviceNum boardNum, bool on) {
    	on ? front.ir.enable() : front.ir.disable();
    }

    bool SetLEDAll(SPIDeviceNum boardNum, uint8_t color) {
    	return front.led.all(color);
    }

    void activateLaser(bool on) {
        printf(on ? "Turn laser on\n": "Turn laser off\n");
        on ? right.laser.on() : right.laser.off();
    }

    void MoveHingeToAngle(uint16_t angle) {
    	//
    }

    /**
     * We used to have a command for the left and right "track". Now we have velocities for the
     * front and back PCB boards. Not so intuitive...
     */
    void Move(int8_t vleft, int8_t vright) {
    	front.motor.setVelocity(vleft);
    	back.motor.setVelocity(vright);
    }
private:
    friend class IRobotFactory;
};

#endif // _IROBOTSCOUTWRAPPER_H
