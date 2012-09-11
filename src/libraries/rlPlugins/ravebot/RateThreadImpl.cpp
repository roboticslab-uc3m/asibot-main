
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
    std::vector<dReal> encExposed(mismotores.size());
    for(unsigned int motor=0;motor<numMotors;motor++){
        if((jointStatus[motor]==1)||(jointStatus[motor]==2)||(jointStatus[motor]==3)||(jointStatus[motor]==5)) {
            if (fabs(targetExposed[motor]-encRaw[motor])<jointTol[motor]){
                printf("Joint q%d reached target.\n",motor+1);
                jointStatus[motor]=0;
                velRaw[motor]=0;
            } else {  // TODO: check for joint limits
                // encRaw[motor]+=(velRaw[motor])*(JMC_MS/1000.0);
                encRaw[motor]+=(velRaw[motor])*(Time::now()-lastTime);
            }
        }
        if(mismotores[motor]->IsPrismatic(0)) encExposed[motor] = encRaw[motor];
        else encExposed[motor]=toRad(encRaw[motor]);
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
        encExposed[5] = toRad(real_tool);
        encExposed[6] = -toRad(real_tool);
    }

    // pcontroller->SetDesired(encExposed); // This function "resets" physics
    probot->SetJointValues(encExposed);  // More compatible with physics??
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

