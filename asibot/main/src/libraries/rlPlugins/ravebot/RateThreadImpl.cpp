
// -----------------------------------------------------------------------------

#include "RaveBot.h"

// ------------------- RateThread Related ------------------------------------

bool RaveBot::threadInit() {
    printf("[success] ravebot threadInit()\n");
    lastTime = Time::now();
    return true;
}

// -----------------------------------------------------------------------------

void RaveBot::run() {
    std::vector<dReal> dEncRaw(mismotores.size());
    for(unsigned int motor=0;motor<numMotors;motor++){
        setEncRaw(motor, getEncRaw(motor)+(velRaw[motor])*(Time::now()-lastTime));
        dEncRaw[motor] = getEncRaw(motor);
        if((jointStatus[motor]==1)||(jointStatus[motor]==2)||(jointStatus[motor]==3)) {  // if set to move...
            if ((getEncExposed(motor) > maxLimit[motor])  && (velRaw[motor]>0)) {  // SW max JL
                stop(motor);  // puts jointStatus[motor]=0;
                printf("[warning] Moving joint q%d at configured max joint limit, stopping.\n",motor+1);
            } else if ((getEncExposed(motor) < minLimit[motor]) && (velRaw[motor]<0)) {  // SW min JL
                stop(motor);  // puts jointStatus[motor]=0;
                printf("[warning] Moving joint q%d at configured min joint limit, stopping.\n",motor+1);
            } else if((jointStatus[motor]==1)||(jointStatus[motor]==2)) {  // check if target reached in pos or rel
                if ( (velRaw[motor] > 0) &&  // moving positive...
                    (getEncExposed(motor) > (targetExposed[motor]-jointTol[motor])) ) {
                    stop(motor);  // puts jointStatus[motor]=0;
                    printf("Joint q%d reached target.\n",motor+1);
                } else if ( (velRaw[motor] < 0) &&  // moving negative...
                    (getEncExposed(motor) < (targetExposed[motor]+jointTol[motor])) ) {
                    stop(motor);  // puts jointStatus[motor]=0;
                    printf("Joint q%d reached target.\n",motor+1);
                }
            }
        }
    }
    lastTime = Time::now();

    if (toolFound) {
        if(theToolPort.status==1) {
//            if (fabs(theToolPort.target-real_tool)>MOTOR_PRECISION) {
            if (fabs(theToolPort.target-real_tool)>0.25) {
                if(theToolPort.target-real_tool>0)
                    real_tool+=jmcMs*TOOL_SPEED_ADJ;
                else
                    real_tool-=jmcMs*TOOL_SPEED_ADJ;
                printf("Tool at %f\n",real_tool);
            } else {
                theToolPort.status = 0;
            }
        }
        dEncRaw[5] = toRad(real_tool);
        dEncRaw[6] = -toRad(real_tool);
    }

    // pcontroller->SetDesired(dEncRaw); // This function "resets" physics
    probot->SetJointValues(dEncRaw);  // More compatible with physics??
    penv->StepSimulation(jmcMs/1000.0);  // StepSimulation must be given in seconds

    if(cameraFound) {
        psensorbase->GetSensorData(pcamerasensordata);
        //std::vector<uint8_t> currentFrame = pcamerasensordata->vimagedata;
        // printf("Vector size: %d",currentFrame.size()); // = 480 * 640 * 3 = 921600;
        yarp::sig::ImageOf<yarp::sig::PixelRgb>& i_imagen = p_imagen.prepare(); 
        i_imagen.resize(640,480);  // Tamaño de la pantalla (640,480)
        yarp::sig::PixelRgb p;
        for (int i_x = 0; i_x < i_imagen.width(); ++i_x) {
            for (int i_y = 0; i_y < i_imagen.height(); ++i_y) {
                p.r = pcamerasensordata->vimagedata[3*(i_x+(i_y*i_imagen.width()))];
                p.g = pcamerasensordata->vimagedata[1+3*(i_x+(i_y*i_imagen.width()))];
                p.b = pcamerasensordata->vimagedata[2+3*(i_x+(i_y*i_imagen.width()))];
                i_imagen.safePixel(i_x,i_y) = p;
            }
        }
        p_imagen.write();
    }

}

// -----------------------------------------------------------------------------

