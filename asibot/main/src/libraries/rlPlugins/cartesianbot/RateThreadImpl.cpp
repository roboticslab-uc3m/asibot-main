
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
            yarp::sig::Vector xP,xdP,xdP_dot,eP,lawxP;
            double ozRad = atan2(x[1],x[0]);
            xP.push_back(sqrt(x[0]*x[0]+x[1]*x[1]));
            xP.push_back(x[2]-A0);
            xP.push_back(o[0]);
            //printf("Problem statement:\n");
            //printf("oz: %f\nxP: %f\nzP: %f\n",toDeg(ozRad),xP[0],xP[1]);
            double currentTime = Time::now();
            if((currentTime-startTime)>trajXP.getT()) printf ("[warning] out of time\n");
            xdP.push_back(trajXP.get(startTime-currentTime));
            xdP.push_back(trajZP.get(startTime-currentTime));
            xdP.push_back(trajPitchP.get(startTime-currentTime));
            eP.resize(3);
            eP = xdP - xP;
            xdP_dot.push_back(trajXP.getd(startTime-currentTime));
            xdP_dot.push_back(trajZP.getd(startTime-currentTime));
            xdP_dot.push_back(trajPitchP.getd(startTime-currentTime));
            lawxP.resize(3);
            lawxP = (eP * GAIN) + xdP_dot;
            yarp::sig::Matrix Ja(3,3);
            double grabValues[NUM_MOTORS];
            if(!enc->getEncoders(grabValues)) printf("[warning] CartesianBot::getPose() failed to getEncoders()\n");
            for (int i=0; i<NUM_MOTORS; i++)
                realRad(i)=toRad(grabValues[i]);
            Ja(0,0) = A2*cos(realRad(1)+realRad(2)) + A1*cos(realRad(1)) + A3*cos(realRad(1)+realRad(2)+realRad(3));
            Ja(0,1) = A2*cos(realRad(1)+realRad(2)) + A3*cos(realRad(1) + realRad(2) + realRad(3));
            Ja(0,2) = A3*cos(realRad(1)+realRad(2) + realRad(3));
            Ja(1,0) = -A2*sin(realRad(1)+realRad(2)) - A1*sin(realRad(1))-A3*sin(realRad(1)+realRad(2)+realRad(3));
            Ja(1,1) = -A2*sin(realRad(1)+realRad(2)) - A3*sin(realRad(1)+realRad(2)+realRad(3));
            Ja(1,2) = -A3*sin(realRad(1)+realRad(2)+realRad(3));
            Ja(2,0) = 1;
            Ja(2,1) = 1;
            Ja(2,2) = 1;
            yarp::sig::Matrix Ja_pinv(pinv(Ja));
            yarp::sig::Vector t;
            t.resize(3);
            t = Ja_pinv * lawxP;
            double q_dot[NUM_MOTORS];
            double eoz = trajoz.get(startTime-currentTime) - grabValues[0];
            q_dot[0] = trajoz.getd(startTime-currentTime) + GAIN*eoz;  // lawoz
            q_dot[1] = t[0];
            q_dot[2] = t[1];
            q_dot[3] = t[2];
            double eRollP = trajRollP.get(startTime-currentTime) - grabValues[4];
            q_dot[4] = trajRollP.getd(startTime-currentTime) + GAIN*eRollP;  // lawRollP
            if(!vel->velocityMove(q_dot))
                printf("GIGANTIC velocity WARNING\n");
        }
    } else {  // If it is stopped or breaked, reamain unchanged
        // printf("Inside control loop stopped.\n");
    }
}

// -----------------------------------------------------------------------------

