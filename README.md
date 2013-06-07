# wrapirobot

The IRobot middleware on the Replicator/Symbrion robots is a middleware that has been created at Stuttgart university. It provides the basic functionality on an individual robot to address the MSPs on the robot. The code to access the cameras is not integrated with this middleware, but provided in a separate libcam folder. There are two versions of this folder, one for V4L1 and one for V4L2. This middleware has been thorougly tested on the ActiveWheel robot from Stuttgart and the Scout robot from Pisa. 

The new HDMR+ middleware has been created at Karlsruhe Institute of Technology. It has been tested mainly with the robots created by Karlsruhe, but the code for the other robots is available. This new middleware is better in the sense that it allows for multiple controllers to run in parallel. The middleware negotiates which controller will actually have access to the motors.

## What does it do?

The wrapirobot code is glue code so partners in the project can still use the old-fashioned iRobot interface, while they are now actually using the HDMR+ middleware underlying it.

## Is there more?

There is the so-called jockey framework that provides functional code on the levels of controllers: https://github.com/mrquincle/equids. This is build up in a modular fashion allowing other partners to use it. The jockey framework has been set up by Czech Technical University in Prague, and amended by Almende B.V.

## What is still missing?

There use to be an action selection mechanism that would select a controller depending on the context. With the new HDMR+ middleware that functionality is not there anymore. The controllers can now run safely in parallel, but there is no entity which selects which one will need to run next, neither are the controllers yielding control out of themselves.

## How to use?

Place the wrapirobot code in the "HDRM-Plus" folder and compile the parent folder. It is not a stand-alone thing. Copy then the files from the "hdmrplus_install" folder to the robot, especially the ones:

    lib/libirobot_app.so
    lib/libirobot_common.so

Naturally, all the other files that belong to the HDRM+ middleware has to reside on the robot too.

## On the robot

The wrapirobot requires one thing, an environmental variable that tells the wrapper which robot this is:

    export ROBOT=SCOUT

This information used to be acquired from the MSPs, but in the HDRM+ software, there is no equivalent function call. Of course, set this environmental variable in /etc/rc.local if you wish. However, in that case be aware that if you login through telnet, you end up in a shell without this environment set. Drop to a root shell by:

    sh
    set

And you will be able to run everything from here:

    cd /flash
    ./CManager.bin &
    ./SpiModule.bin &
    your program

## License

The wrapirobot code is open-source and under LGPL.
