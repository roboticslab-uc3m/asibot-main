// Copyright  (C)  2010  Juan G Victores <jcgvicto at ing dot uc3m dot es>

// Version: 0.3
// Author: Juan G Victores <jcgvicto at ing dot uc3m dot es>
// Maintainer: Juan G Victores <jcgvicto at ing dot uc3m dot es>
// URL: n/a

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

/**
 * \mainpage cmc_kdl
 *
 * This is the documentation of the cmc_kdl module - a Cartesian Motion Controller based on kdl methods.

 Other existing driver modules are:

 *
 */

#include "yarp/os/all.h"
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include <kdl/chain.hpp>
#include <kdl/chainfksolver.hpp>
#include <kdl/chainiksolverpos_nr_jl.hpp>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/chainiksolvervel_pinv.hpp>
#include <kdl/frames_io.hpp>
#include <kdl/frames.hpp>
#include <kdl/path_line.hpp>
#include <kdl/rotational_interpolation_sa.hpp>
#include <kdl/velocityprofile_rect.hpp>
#include <kdl/velocityprofile_trap.hpp>
#include <kdl/trajectory_segment.hpp>

using namespace yarp::os;
using namespace yarp::dev;
using namespace KDL;

#define NUM_MOTORS 5
#define THREAD_RATE 30  // In miliseconds
#define CARTPOS_PRECISION 0.005  // Meter 0.0005
#define CARTORI_PRECISION 0.5  // Degrees
#define DEFAULT_DURATION 3  // For Trajectory, 3s?
#define TIMEINCREMENT 0.03  // For Trajectory, 50ms?
#define GAIN 75 // 75 good for unstabilized sim and common real. 25 ok with stable sim.

class ControlLoop : public yarp::os::RateThread {
public:

    /** Set the Thread Rate in the class constructor **/
    ControlLoop(Property _options) : RateThread(THREAD_RATE) {
        options = _options;
    }  // In ms

    /** When we start the periodical function **/
    bool threadInit() {
        // 5 DOF chain for FK solver
        // Note that does not include length of first link.
        theChain.addSegment(Segment(Joint(Joint::RotZ),Frame(Vector(0.0,0.0,0.12))));
        theChain.addSegment(Segment(Joint(Joint::RotY),Frame(Vector(0.0,0.0,0.4))));
        theChain.addSegment(Segment(Joint(Joint::RotY),Frame(Vector(0.0,0.0,0.4))));
        theChain.addSegment(Segment(Joint(Joint::RotY),Frame(Vector(0.0,0.0,0.12))));
        theChain.addSegment(Segment(Joint(Joint::RotZ),Frame(Vector(0.0,0.0,0.16))));
        ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain);
        ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver.
        real_rad = JntArray(5);
        fksolver.JntToCart(real_rad,real_cartpos);
        fksolver.JntToCart(real_rad,target_cartpos);

//    port_q.open("/kdl_q");
//        Property options;
/////// UNCOMMENT NEXT 3 LINES FOR USING RAVEBOT AS A LIBRARY AND OPENING FOR REMOTE ////////
        options.put("device","controlboard");
        options.put("subdevice","ravebot");
        options.put("name","/ravebot");
//////// UNCOMMENT NEXT 1 LINE FOR USING RAVEBOT AS A LIBRARY ///////////////////////
//        options.put("device","ravebot");
//////// UNCOMMENT NEXT 3 LINES FOR CONNECTING TO REMOTE RAVEBOT /////////////
//        options.put("device","remote_controlboard");
//        options.put("local","/local");
//        options.put("remote","/ravebot");
/////        ConstString xportstr = options.find("xport").toString();
/////        options.put("local",xportstr);
        dd.open(options);
        dd.view(vel);
        dd.view(enc);
        dd.view(pos);
        vgeneral = 100;
        cmc_status = 0;
        _orient = new RotationalInterpolation_SingleAxis();
        _eqradius = 1; //0.000001;
        _aggregate = false;
        duration = DEFAULT_DURATION;
        currentTime = 0;
//    currentTrajectory = KDL::Trajectory_Segment(NULL, NULL, DURATION, _aggregate);
        //currentTrajectory = new KDL::Trajectory_Segment(NULL,NULL);
        //pos->positionMove(1,30);
        //pos->positionMove(2,-60);
        //pos->positionMove(3,-30);
        printf("Inside main loop.\n");
        return true;
    }

    /** The periodical function **/
    void run() {
        if (cmc_status>0) {  // If it is movement
            double grabValues[NUM_MOTORS];
            if(!enc->getEncoders(grabValues))
                printf("GIGANTIC encoder WARNING\n");
            for (int i=0; i<NUM_MOTORS; i++)
                real_rad(i)=toRad(grabValues[i]);
            ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain);
            ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver.
            fksolver.JntToCart(real_rad,real_cartpos);
            if (targetReached()) {
                printf("Target reached\n");
                currentTime = 0;
                pos->setPositionMode();
                double real_rot[3];
                real_cartpos.M.GetRPY(real_rot[0],real_rot[1],real_rot[2]);
                Bottle bottle_x_o;
                bottle_x_o.clear();
                bottle_x_o.addInt(0);
                bottle_x_o.addString("x");
                bottle_x_o.addDouble(real_cartpos.p.data[0]);
                bottle_x_o.addString("y");
                bottle_x_o.addDouble(real_cartpos.p.data[1]);
                bottle_x_o.addString("z");
                bottle_x_o.addDouble(real_cartpos.p.data[2]);
                bottle_x_o.addString("roll");
                bottle_x_o.addDouble(toDeg(real_rot[0]));
                bottle_x_o.addString("pitch");
                bottle_x_o.addDouble(toDeg(real_rot[1]));
                bottle_x_o.addString("yaw");
                bottle_x_o.addDouble(toDeg(real_rot[2]));
                printf("Would write: %s.\n", bottle_x_o.toString().c_str());
//                xPort->write(bottle_x_o);
                cmc_status=0;
            } else {
                //printf("Inside control loop moving.\n");
                //   Twist axis_vel;
                Twist T_desired = currentTrajectory->Vel(currentTime);
                Frame F_desired = currentTrajectory->Pos(currentTime);
                Twist T_current = diff(real_cartpos, F_desired);
                for (unsigned int i=0; i<6; i++) {
                    T_current(i) *= GAIN;
                    T_current(i) += T_desired(i);
                    //T_current(i) += T_other(i);
                }
                JntArray joint_vel(5);  // in radians
                ChainIkSolverVel_pinv iksolverv_pinv(theChain);
                //int return_code = iksolverv_pinv.CartToJnt(real_rad,axis_vel,joint_vel);
                //int return_code = iksolverv_pinv.CartToJnt(real_rad,T_current,joint_vel);
                iksolverv_pinv.CartToJnt(real_rad,T_current,joint_vel);
                double cmc_qdot[5];
                for (unsigned int i=0; i<5; i++)
                    cmc_qdot[i]=toDeg(joint_vel(i));
                // Should normalize anyway (Find out the max, etc)
                double max_qdot = 100;
                for (int motor=0; motor<5; motor++)
                    if (fabs(cmc_qdot[motor])>max_qdot)
                        max_qdot = fabs(cmc_qdot[motor]);
                if (max_qdot > 100)
                    for (int motor=0; motor<5; motor++)
                        cmc_qdot[motor]=(cmc_qdot[motor])*100.0/max_qdot;
                if(!vel->velocityMove(cmc_qdot))
                    printf("GIGANTIC velocity WARNING\n");
                currentTime = currentTime + TIMEINCREMENT;
            }
        } else {  // If it is stopped or breaked, reamain unchanged
//j//        printf("Inside control loop stopped.\n");
        }
    }

    Bottle emergencyStop() {
        printf("[cmc_kdl] Begin general stop.\n");
        pos->setPositionMode();
        cmc_status=-1;
        Bottle bottle_q_i;
        for (int i=0; i<6; i++)
            targets[i]=false;
        double grabValues[NUM_MOTORS];
        enc->getEncoders(grabValues);
        for (int i=0; i<NUM_MOTORS; i++)
            real_rad(i)=toRad(grabValues[i]);
        ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain); // Does Forward Kinematics
        ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver
        fksolver.JntToCart(real_rad,real_cartpos);
        double real_rot[3];
        real_cartpos.M.GetRPY(real_rot[0],real_rot[1],real_rot[2]);
        Bottle bottle_x_o;
        bottle_x_o.clear();
        bottle_x_o.addInt(-1);
        bottle_x_o.addString("x");
        bottle_x_o.addDouble(real_cartpos.p.data[0]);
        bottle_x_o.addString("y");
        bottle_x_o.addDouble(real_cartpos.p.data[1]);
        bottle_x_o.addString("z");
        bottle_x_o.addDouble(real_cartpos.p.data[2]);
        bottle_x_o.addString("roll");
        bottle_x_o.addDouble(toDeg(real_rot[0]));
        bottle_x_o.addString("pitch");
        bottle_x_o.addDouble(toDeg(real_rot[1]));
        bottle_x_o.addString("yaw");
        bottle_x_o.addDouble(toDeg(real_rot[2]));
        //printf("Would write: %s.\n", bottle_x_o.toString().c_str());
//        xPort->write(bottle_x_o);
        printf("[cmc_kdl] End general stop.\n");
        return bottle_x_o;
    }

    Bottle controlStat() {
        printf("[cmc_kdl] Begin controlStat.\n");
        double grabValues[NUM_MOTORS];
        enc->getEncoders(grabValues);
        for (int i=0; i<NUM_MOTORS; i++)
            real_rad(i)=toRad(grabValues[i]);
        ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain); // Does Forward Kinematics
        ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver
        fksolver.JntToCart(real_rad,real_cartpos);
        double real_rot[3];
        real_cartpos.M.GetRPY(real_rot[0],real_rot[1],real_rot[2]);
        Bottle bottle_x_o;
        bottle_x_o.clear();
//        bottle_x_o.addInt(0);
        bottle_x_o.addInt(cmc_status);
        bottle_x_o.addString("x");
        bottle_x_o.addDouble(real_cartpos.p.data[0]);
        bottle_x_o.addString("y");
        bottle_x_o.addDouble(real_cartpos.p.data[1]);
        bottle_x_o.addString("z");
        bottle_x_o.addDouble(real_cartpos.p.data[2]);
        bottle_x_o.addString("roll");
        bottle_x_o.addDouble(toDeg(real_rot[0]));
        bottle_x_o.addString("pitch");
        bottle_x_o.addDouble(toDeg(real_rot[1]));
        bottle_x_o.addString("yaw");
        bottle_x_o.addDouble(toDeg(real_rot[2]));
        //printf("Would write: %s.\n", bottle_x_o.toString().c_str());
//        xPort->write(bottle_x_o);
        printf("[cmc_kdl] End controlStat.\n");
        return bottle_x_o;
    }

    bool controlMovAbs(bool bool_x_i[6],double double_x_i[6], double velocity) {
        printf("[cmc_kdl] Begin absolute base movement.\n");
        double grabValues[NUM_MOTORS];
        enc->getEncoders(grabValues);
        for (int i=0; i<NUM_MOTORS; i++)
            real_rad(i)=toRad(grabValues[i]);
        vel->setVelocityMode();
        ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain); // Does Forward Kinematics
        ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver
        fksolver.JntToCart(real_rad,real_cartpos);
        for (int axis=0; axis<6; axis++)
            targets[axis]=bool_x_i[axis];
        // Recalculate the target
        target_cartpos = real_cartpos;  // Where we are (should be pos + ori)
        if (bool_x_i[0]) target_cartpos.p.x(double_x_i[0]);
        else target_cartpos.p.x(real_cartpos.p.data[0]);
        if (bool_x_i[1]) target_cartpos.p.y(double_x_i[1]);
        else target_cartpos.p.y(real_cartpos.p.data[1]);
        if (bool_x_i[2]) target_cartpos.p.z(double_x_i[2]);
        else target_cartpos.p.z(real_cartpos.p.data[2]);
        if ((bool_x_i[3])|(bool_x_i[4])|(bool_x_i[5])) {
            target_cartpos.M = Rotation::RPY(toRad(double_x_i[3]),toRad(double_x_i[4]),toRad(double_x_i[5]));
        } else printf("No orientation taken into account.\n");
        KDL::Path_Line testPathLine(real_cartpos, target_cartpos, _orient, _eqradius, _aggregate);
        //KDL::VelocityProfile_Rectangular::VelocityProfile_Rectangular(double _maxvel = 0)
        double _maxVel = 0.2; //0.1; //TODO
        double  maxAcc = 0.2; //0.1; //TODO
        //KDL::VelocityProfile_Rectangular testVelocityProfile(_maxVel);
        KDL::VelocityProfile_Trap testVelocityProfile(_maxVel, maxAcc);
        //Trajectory_Segment (Path *_geom, VelocityProfile *_motprof, double duration, bool _aggregate=true)
        KDL::Trajectory_Segment testTrajectory(testPathLine.Clone(), testVelocityProfile.Clone(), duration, _aggregate);
        //currentTrajectory = KDL::Trajectory_Segment(testPathLine.Clone(), testVelocityProfile.Clone(), DURATION, _aggregate);
        //delete currentTrajectory;
        currentTrajectory = testTrajectory.Clone();
        // Set the status
        currentTime = 0;

        // Set the status
        vgeneral=velocity;
        cmc_status=1;
        printf("[cmc_kdl] End absolute base movement.\n");
        return true;
    }

    bool controlMovRelBase(bool bool_x_i[6],double double_x_i[6], double velocity) {
        printf("[cmc_kdl] Begin relative base movement.\n");
        double grabValues[NUM_MOTORS];
        enc->getEncoders(grabValues);
        for (int i=0; i<NUM_MOTORS; i++)
            real_rad(i)=toRad(grabValues[i]);
        vel->setVelocityMode();
        ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain); // Does Forward Kinematics
        ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver
        fksolver.JntToCart(real_rad,real_cartpos);
        for (int axis=0; axis<6; axis++)
            targets[axis]=bool_x_i[axis];
        // Recalculate the target

        // Recalculate the target
        Frame newtarget_cartpos = Frame::Identity();
        if (bool_x_i[0]) {
            newtarget_cartpos.p.x(double_x_i[0]);
            targets[0]=true;
        }
        if (bool_x_i[1]) {
            newtarget_cartpos.p.y(double_x_i[1]);
            targets[1]=true;
        }
        if (bool_x_i[2]) {
            newtarget_cartpos.p.z(double_x_i[2]);
            targets[2]=true;
        }
        if ((bool_x_i[3])|(bool_x_i[4])|(bool_x_i[5])) {
            newtarget_cartpos.M = Rotation::RPY(toRad(double_x_i[3]),toRad(double_x_i[4]),toRad(double_x_i[5]));
            for (int axis=3; axis<6; axis++)
                targets[axis]=true;
        } else printf("No orientation taken into account.\n");
        target_cartpos=newtarget_cartpos*real_cartpos;  // Where we are (should be pos + ori)
        if (targets[0])
            printf("Must move %f on Base X.\n",((real_cartpos.p.data[0])-(target_cartpos.p.data[0])));
        else printf("No move of Base X.\n");
        if (targets[1])
            printf("Must move %f on Base Y.\n",((real_cartpos.p.data[1])-(target_cartpos.p.data[1])));
        else printf("No move of Base Y.\n");
        if (targets[2])
            printf("Must move %f on Base Z.\n",((real_cartpos.p.data[2])-(target_cartpos.p.data[2])));
        else printf("No move of BaseZ.\n");
        KDL::Path_Line testPathLine(real_cartpos, target_cartpos, _orient, _eqradius, _aggregate);
        //KDL::VelocityProfile_Rectangular::VelocityProfile_Rectangular(double _maxvel = 0)
        double _maxVel = 0.2; //0.1; //TODO
        double  maxAcc = 0.2; //0.1; //TODO
        //KDL::VelocityProfile_Rectangular testVelocityProfile(_maxVel);
        KDL::VelocityProfile_Trap testVelocityProfile(_maxVel, maxAcc);
        //Trajectory_Segment (Path *_geom, VelocityProfile *_motprof, double duration, bool _aggregate=true)
        KDL::Trajectory_Segment testTrajectory(testPathLine.Clone(), testVelocityProfile.Clone(), duration, _aggregate);
        //currentTrajectory = KDL::Trajectory_Segment(testPathLine.Clone(), testVelocityProfile.Clone(), DURATION, _aggregate);
        //delete currentTrajectory;
        currentTrajectory = testTrajectory.Clone();
        // Set the status
        currentTime = 0;
        // Set the status
        vgeneral=velocity;
        cmc_status=2;
        printf("[cmc_kdl] End relative base movement.\n");
        return true;
    }

    bool controlMovRelEE(bool bool_x_i[6],double double_x_i[6], double velocity) {
        printf("[cmc_kdl] Begin relative EE movement.\n");
        double grabValues[NUM_MOTORS];
        enc->getEncoders(grabValues);
        for (int i=0; i<NUM_MOTORS; i++)
            real_rad(i)=toRad(grabValues[i]);
        vel->setVelocityMode();
        ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain); // Does Forward Kinematics
        ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver
        fksolver.JntToCart(real_rad,real_cartpos);
        // Recalculate the target
        Frame newtarget_cartpos = Frame::Identity();
        if (bool_x_i[0]) newtarget_cartpos.p.x(double_x_i[0]);
        if (bool_x_i[1]) newtarget_cartpos.p.y(double_x_i[1]);
        if (bool_x_i[2]) newtarget_cartpos.p.z(double_x_i[2]);
        if ((bool_x_i[0])||(bool_x_i[1])||(bool_x_i[2]))
            for (int axis=0; axis<3; axis++)
                targets[axis]=true;
        if ((bool_x_i[3])|(bool_x_i[4])|(bool_x_i[5])) {
            newtarget_cartpos.M = Rotation::RPY(toRad(double_x_i[3]),toRad(double_x_i[4]),toRad(double_x_i[5]));
            for (int axis=3; axis<6; axis++)
                targets[axis]=true;
        } else printf("No orientation taken into account.\n");
        target_cartpos=real_cartpos*newtarget_cartpos;  // Where we are (should be pos + ori)
        if (targets[0]) printf("Must move %f on Base X.\n",((real_cartpos.p.data[0])-(target_cartpos.p.data[0])));
        else printf("No move of Base X.\n");
        if (targets[1]) printf("Must move %f on Base Y.\n",((real_cartpos.p.data[1])-(target_cartpos.p.data[1])));
        else printf("No move of Base Y.\n");
        if (targets[2]) printf("Must move %f on Base Z.\n",((real_cartpos.p.data[2])-(target_cartpos.p.data[2])));
        else printf("No move of BaseZ.\n");
        KDL::Path_Line testPathLine(real_cartpos, target_cartpos, _orient, _eqradius, _aggregate);
        //KDL::VelocityProfile_Rectangular::VelocityProfile_Rectangular(double _maxvel = 0)
        double _maxVel = 0.2; //0.1; //TODO
        double  maxAcc = 0.2; //0.1; //TODO
        //KDL::VelocityProfile_Rectangular testVelocityProfile(_maxVel);
        KDL::VelocityProfile_Trap testVelocityProfile(_maxVel, maxAcc);
        //Trajectory_Segment (Path *_geom, VelocityProfile *_motprof, double duration, bool _aggregate=true)
        KDL::Trajectory_Segment testTrajectory(testPathLine.Clone(), testVelocityProfile.Clone(), duration, _aggregate);
        //currentTrajectory = KDL::Trajectory_Segment(testPathLine.Clone(), testVelocityProfile.Clone(), DURATION, _aggregate);
        //delete currentTrajectory;
        currentTrajectory = testTrajectory.Clone();
        // Set the status
        currentTime = 0;
        vgeneral=velocity;
        cmc_status=3;
        printf("[cmc_kdl] End relative EE movement.\n");
        return true;
    }

    bool setTrajTime(const double t=0) {
        if(t==0) duration=DEFAULT_DURATION;
        else duration=t;
        printf("Duration set to %f.\n",duration);
        return true;
    }

    /* Returns true if all have reached */
    bool targetReached() {
        if ((targets[0])&&(fabs((real_cartpos.p.data[0])-(target_cartpos.p.data[0])))>CARTPOS_PRECISION) return false;
        if ((targets[1])&&(fabs((real_cartpos.p.data[1])-(target_cartpos.p.data[1])))>CARTPOS_PRECISION) return false;
        if ((targets[2])&&(fabs((real_cartpos.p.data[2])-(target_cartpos.p.data[2])))>CARTPOS_PRECISION) return false;
        double target_rpy[3] = {0,0,0};
        double real_rpy[3] = {0,0,0};
        real_cartpos.M.GetRPY(real_rpy[0],real_rpy[1],real_rpy[2]);
        target_cartpos.M.GetRPY(target_rpy[0],target_rpy[1],target_rpy[2]);
        if ((targets[3])&&(fabs(toDeg(target_rpy[0])-toDeg(real_rpy[0])))>CARTORI_PRECISION) return false;
        if ((targets[4])&&(fabs(toDeg(target_rpy[1])-toDeg(real_rpy[1])))>CARTORI_PRECISION) return false;
        if ((targets[5])&&(fabs(toDeg(target_rpy[2])-toDeg(real_rpy[2])))>CARTORI_PRECISION) return false;
        return true;
    }

    double toDeg(const double inRad) {
        return (inRad * 180.0 / 3.14159265);
    }

    double toRad(const double inDeg) {
        return (inDeg * 3.14159265 / 180.0);
    }

private:
    Property options;
    PolyDriver dd;
    IEncoders *enc;
    IPositionControl *pos;
    IVelocityControl *vel;
//    Port *xPort;
    int cmc_status;
    bool targets[6];
    Chain theChain;
    Frame real_cartpos;
    Frame target_cartpos;
    JntArray real_rad;  // in radians
    double vgeneral;
    RotationalInterpolation_SingleAxis* _orient;
    double _eqradius;
    bool _aggregate;
//  Trajectory_Segment currentTrajectory;
    Trajectory* currentTrajectory;
    double currentTime;
    double duration;
};

