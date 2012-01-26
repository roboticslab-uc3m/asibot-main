
// -----------------------------------------------------------------------------

#include "CartesianBot.h"

// ------------------- RateThread Related ------------------------------------

bool CartesianBot::threadInit() {
    printf("[success] threadInit(): Started %f ms ratethread\n",getRate());
    return true;
}

// -----------------------------------------------------------------------------

void CartesianBot::run() {
    if (cmc_status>0) {  // If it is movement
        yarp::sig::Vector x,o;
        getPose(x,o);
        bool done = false;
        checkMotionDone(&done);
        if (done) {
            printf("Target reached\n");
            startTime = 0;
            pos->setPositionMode();
            cmc_status=0;
        } else {
            //printf("Inside control loop moving.\n");
            yarp::sig::Vector xd,od,xd_dot,od_dot;
            double currentTime = Time::now();
            xd.push_back(trajX.get(startTime-currentTime));
            xd.push_back(trajY.get(startTime-currentTime));
            xd.push_back(trajZ.get(startTime-currentTime));
            od.push_back(trajPitchP.get(startTime-currentTime));
            od.push_back(trajRollP.get(startTime-currentTime));
            yarp::sig::Vector ex = (xd - x);
            yarp::sig::Vector eo = (od - o);
            xd_dot.push_back(trajX.getd(startTime-Time::now()));
            xd_dot.push_back(trajY.getd(startTime-Time::now()));
            xd_dot.push_back(trajZ.getd(startTime-Time::now()));
            od_dot.push_back(trajPitchP.getd(startTime-Time::now()));
            od_dot.push_back(trajRollP.getd(startTime-Time::now()));
            yarp::sig::Vector lawx = (ex * GAIN) + xd_dot;
            yarp::sig::Vector lawo = (ex * GAIN) + od_dot;
            
        }
    } else {  // If it is stopped or breaked, reamain unchanged
        // printf("Inside control loop stopped.\n");
    }
}

// -----------------------------------------------------------------------------

