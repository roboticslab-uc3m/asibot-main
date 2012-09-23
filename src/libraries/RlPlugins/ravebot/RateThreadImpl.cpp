
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

    // pcontroller->SetDesired(dEncRaw); // This function "resets" physics
    probot->SetJointValues(dEncRaw);  // More compatible with physics??
    penv->StepSimulation(jmcMs/1000.0);  // StepSimulation must be given in seconds

    if(cameraFound) {
        pcamerasensorbase->GetSensorData(pcamerasensordata);
        //std::vector<uint8_t> currentFrame = pcamerasensordata->vimagedata;
        //printf("Vector size: %d\n",currentFrame.size()); // = 480 * 640 * 3 = 921600;
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
    if(laserFound) {
        plasersensorbase->GetSensorData(plasersensordata);
        std::vector< RaveVector< dReal > > sensorPositions = plasersensordata->positions;
        std::vector< RaveVector< dReal > > sensorRanges = plasersensordata->ranges;
        std::vector< dReal > sensorIntensity = plasersensordata->intensity;
        //printf("sensorPositions size: %d ",sensorPositions.size()); // = 1; xyz of the fixed 3d sensor position.
        //printf("sensorRanges size: %d ",sensorRanges.size()); // 64 * 48 = 3072;
        //printf("sensorIntensity size: %d\n",sensorIntensity.size()); // 64 * 48 = 3072;
        //for(unsigned int i=0;i<sensorRanges.size();i++) {
        //   printf("sensorRanges[%d].x: %f  sensorRanges[%d].y: %f  sensorRanges[%d].z: %f sensorIntensity[%d]: %.2f\n",i,sensorRanges[i].x,i,sensorRanges[i].y,i,sensorRanges[i].z,i,sensorIntensity[i]);  // intensity always 1.0 or 0.0 in 3d sensor
        //}
        /*Transform tinv = plasersensordata->__trans.inverse();
        for(size_t i = 0; i < _data->ranges.size(); ++i) {
            float* p = (float*)(&_pointcloud2msg.data.at(i*_pointcloud2msg.point_step));
            if( i < _data->positions.size() ) {
                Vector v = tinv*(_data->ranges[i] + _data->positions[i]);
                p[0] = (float)v.x;
                p[1] = (float)v.y;
                p[2] = (float)v.z;
            } else if( _data->positions.size() > 0 ) {
                Vector v = tinv*(_data->ranges[i] + _data->positions[0]);
                p[0] = (float)v.x;
                p[1] = (float)v.y;
                p[2] = (float)v.z;
            } else {
                Vector v = tinv*_data->ranges[i];
                p[0] = (float)v.x;
                p[1] = (float)v.y;
                p[2] = (float)v.z;
            }
            if( _data->intensity.size()==_data->ranges.size() ) {
                p[3] = _data->intensity[i];
            }
        }*/
        yarp::sig::ImageOf<yarp::sig::PixelFloat>& i_depth = p_depth.prepare(); 
        i_depth.resize(48,64);  // Tamaño de la pantalla (64,48)
        for (int i_x = 0; i_x < i_depth.width(); ++i_x) {
            for (int i_y = 0; i_y < i_depth.height(); ++i_y) {
                double p = sensorRanges[i_x+(i_y*i_depth.width())].z;
                i_depth.safePixel(i_x,i_y) = p*50;  // Should adjust this value depending on max/min dist
            }
        }
        p_depth.write();
    }

}

// -----------------------------------------------------------------------------

