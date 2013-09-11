
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
                fprintf(stderr,"[RaveBot] warning: Moving joint q%d at configured max joint limit, stopping.\n",motor+1);
            } else if ((getEncExposed(motor) < minLimit[motor]) && (velRaw[motor]<0)) {  // SW min JL
                stop(motor);  // puts jointStatus[motor]=0;
                fprintf(stderr,"[RaveBot] warning: Moving joint q%d at configured min joint limit, stopping.\n",motor+1);
            } else if((jointStatus[motor]==1)||(jointStatus[motor]==2)) {  // check if target reached in pos or rel
                if ( (velRaw[motor] > 0) &&  // moving positive...
                    (getEncExposed(motor) > (targetExposed[motor]-jointTol[motor])) ) {
                    stop(motor);  // puts jointStatus[motor]=0;
                    printf("[RaveBot] Joint q%d reached target.\n",motor+1);
                } else if ( (velRaw[motor] < 0) &&  // moving negative...
                    (getEncExposed(motor) < (targetExposed[motor]+jointTol[motor])) ) {
                    stop(motor);  // puts jointStatus[motor]=0;
                    printf("[RaveBot] Joint q%d reached target.\n",motor+1);
                }
            }
        }
    }
    lastTime = Time::now();

    // pcontroller->SetDesired(dEncRaw); // This function "resets" physics
    probot->SetJointValues(dEncRaw);  // More compatible with physics??

    drawCounter++;
    if((worldRpcResponder.robotDraw) && (drawCounter>30)) {
        drawCounter = 0;
        Transform ee = probotManip->GetEndEffector()->GetTransform();
        Transform tool;
        tool.trans = Vector(0.0,0.0,1.27);
        //tool.rot = quatFromAxisAngle(Vector(0,0,0));
        tool.rot = ee.rot;
        Transform tcp = ee * tool;
        { // lock the environment!           
        OpenRAVE::EnvironmentMutex::scoped_lock lock(penv->GetMutex());
        KinBodyPtr drawnKinBodyPtr = RaveCreateKinBody(penv,"");
        std::list<KinBody::Link::GeometryInfo> drawnInfoList;
        KinBody::Link::GeometryInfo drawnInfo;
        drawnInfo._type = KinBody::Link::GeomSphere;
        drawnInfo._t = Transform(Vector(1,0,0,0),Vector(tcp.trans.x, tcp.trans.y, tcp.trans.z));
        drawnInfo._vGeomData = Vector(worldRpcResponder.drawRadius,0,0);
        drawnInfo._bVisible = true;
        drawnInfo._fTransparency = 0;
        drawnInfo._vDiffuseColor = Vector(worldRpcResponder.drawR,worldRpcResponder.drawG,worldRpcResponder.drawB);
        drawnInfoList.push_back(drawnInfo);
        drawnKinBodyPtr->InitFromGeometries(drawnInfoList);
        ConstString drawnName("drawn_");
        std::ostringstream s;  // drawnName += std::to_string(drawnElems++);  // C++11 only
        s << drawnElems++;
        drawnName += s.str();
        drawnKinBodyPtr->SetName(drawnName.c_str());
        penv->Add(drawnKinBodyPtr,true);
        }  // the environment is not locked anymore
    }
                
    if(!!pndof) {
        std::vector<dReal> extraEncRaw;
        for(int it=0;it<extraCallbackPort.dof;it++)
            extraEncRaw.push_back(extraCallbackPort.getEncRaw(it));
        pndof->SetJointValues(extraEncRaw);  // More compatible with physics??
    }

    penv->StepSimulation(jmcMsAcc*jmcMs/1000.0);  // StepSimulation must be given in seconds

    for(unsigned int camIter = 0; camIter<pcamerasensorbase.size(); camIter++ ) {
        pcamerasensorbase[camIter]->GetSensorData(pcamerasensordata[camIter]);
        //std::vector<uint8_t> currentFrame = pcamerasensordata->vimagedata;
        //printf("Vector size: %d\n",currentFrame.size()); // i.e. 480 * 640 * 3 = 921600;
        yarp::sig::ImageOf<yarp::sig::PixelRgb>& i_imagen = p_imagen[camIter]->prepare(); 
        i_imagen.resize(cameraWidth[camIter],cameraHeight[camIter]);  // Tamaño de la pantalla
        yarp::sig::PixelRgb p;
        for (int i_x = 0; i_x < i_imagen.width(); ++i_x) {
            for (int i_y = 0; i_y < i_imagen.height(); ++i_y) {
                p.r = pcamerasensordata[camIter]->vimagedata[3*(i_x+(i_y*i_imagen.width()))];
                p.g = pcamerasensordata[camIter]->vimagedata[1+3*(i_x+(i_y*i_imagen.width()))];
                p.b = pcamerasensordata[camIter]->vimagedata[2+3*(i_x+(i_y*i_imagen.width()))];
                i_imagen.safePixel(i_x,i_y) = p;
            }
        }
        p_imagen[camIter]->write();
    }
    
    for(unsigned int laserIter = 0; laserIter<plasersensorbase.size(); laserIter++ ) {
        plasersensorbase[laserIter]->GetSensorData(plasersensordata[laserIter]);
        std::vector< RaveVector< dReal > > sensorRanges = plasersensordata[laserIter]->ranges;
        std::vector< RaveVector< dReal > > sensorPositions = plasersensordata[laserIter]->positions;
        Transform tinv = plasersensordata[laserIter]->__trans.inverse();
        // std::vector< dReal > sensorIntensity = plasersensordata[laserIter]->intensity;
        // printf("[%d] sensorPositions size: %d ",laserIter,sensorPositions.size()); // = 1; xyz of the fixed 3d sensor position.
        // printf("sensorRanges size: %d ",sensorRanges.size()); // 64 * 48 = 3072;
        // printf("sensorIntensity size: %d\n",sensorIntensity.size()); // 64 * 48 = 3072;
        //for(unsigned int i=0;i<sensorRanges.size();i++) {
        //   printf("sensorRanges[%d].x: %f  sensorRanges[%d].y: %f  sensorRanges[%d].z: %f sensorIntensity[%d]: %.2f\n",i,sensorRanges[i].x,i,sensorRanges[i].y,i,sensorRanges[i].z,i,sensorIntensity[i]);  // intensity always 1.0 or 0.0 in 3d sensor
        //}*/
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
        yarp::sig::ImageOf<yarp::sig::PixelInt>& i_depth = p_depth[laserIter]->prepare();
        if(sensorRanges.size()==3072) i_depth.resize(64,48);  // Tamaño de la pantalla (64,48)
        else if(sensorRanges.size()==12288) i_depth.resize(128,96);
        else if(sensorRanges.size()==49152) i_depth.resize(256,192);
        else if(sensorRanges.size()==307200) i_depth.resize(640,480);
        else if(sensorRanges.size()==4) i_depth.resize(2,2);
        //else printf("[warning] unrecognized laser sensor data size.\n");
        else i_depth.resize(sensorRanges.size(),1);
        for (int i_y = 0; i_y < i_depth.height(); ++i_y) {  // was y in x before
            for (int i_x = 0; i_x < i_depth.width(); ++i_x) {
                //double p = sensorRanges[i_y+(i_x*i_depth.height())].z;
                double p;
                if( sensorPositions.size() > 0 ) {
                    Vector v = tinv*(sensorRanges[i_y+(i_x*i_depth.height())] + sensorPositions[0]);
                    p = (float)v.z;
                } else {
                    Vector v = tinv*(sensorRanges[i_y+(i_x*i_depth.height())]);
                    p = (float)v.z;
                }
                i_depth(i_x,i_y) = p*1000.0;  // give mm
            }
        }
        p_depth[laserIter]->write();
    }

    for(unsigned int fIter = 0; fIter<pforce6dsensorbase.size(); fIter++ ) {
        pforce6dsensorbase[fIter]->GetSensorData(pforce6dsensordata[fIter]);
        Bottle& container = p_force6d[fIter]->prepare(); 
        container.clear();
        Bottle b_force;
        b_force.addDouble(pforce6dsensordata[fIter]->force[0]); 
        b_force.addDouble(pforce6dsensordata[fIter]->force[1]); 
        b_force.addDouble(pforce6dsensordata[fIter]->force[2]); 
        container.addList() = b_force;
        Bottle b_torque;
        b_torque.addDouble(pforce6dsensordata[fIter]->torque[0]);
        b_torque.addDouble(pforce6dsensordata[fIter]->torque[1]);
        b_torque.addDouble(pforce6dsensordata[fIter]->torque[2]);
        container.addList() = b_torque;
        p_force6d[fIter]->write();
    }

}

// -----------------------------------------------------------------------------

