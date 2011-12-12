//
// kinematics_kdl.h
//
// Author: Miguel González-Fierro <mgpalaci@ing.uc3m.es>, (C) 2010
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
#ifndef KINEMATIC_CHAIN_H
#define KINEMATIC_CHAIN_H
#include <kdl/chain.hpp>
#include <kdl/frames_io.hpp>
#include <kdl/chainfksolver.hpp>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/chainiksolvervel_pinv.hpp>
#include <kdl/chainiksolverpos_nr.hpp>
#include <Eigen/Core>
#include <Eigen/Eigen>

class KinematicChain
{
public:
  KinematicChain(){};
  ~KinematicChain(){};
public:

  // Kinematics functions
  //KDL::Frame forwardKinematics(KDL::JntArray joint_positions);
  //KDL::JntArray inverseKinematicsNR(KDL::Frame frame);
  //void moveCartesian(const float time, const float sampling_time, const KDL::Frame frame_destination, Eigen::MatrixXf& trajectory);
  //bool readEndEffectorTrajectory(const std::string filename, std::vector<float>& time, std::vector<KDL::Frame>& cart_trajectory);

  // Getters and Setters
  KDL::JntArray getJointPositions() const;
  void setJointPositions(KDL::JntArray);
  KDL::Frame getEndEffector() const;

  // Print functions
  void printJntArray(KDL::JntArray joint_position) const;
  void printFrame (KDL::Frame& frame) const;
  void printFrameOrientation (KDL::Frame& frame) const;
  void printFramePosition (KDL::Frame& frame) const;
  void printTrajectory(std::vector<KDL::Frame>& trajectory) const;
  void printTrajectoryPosition(std::vector<KDL::Frame>& trajectory) const;
  void printArticularTrajectory(Eigen::MatrixXf& trajectory) const;

  // Write functions
  void writeTrajectoryToFile(std::vector<KDL::Frame>&) const;
  void writeArticularTrajectory(Eigen::MatrixXf& trajectory) const;
  bool writeFramePosition(KDL::Frame& frame) const;
  bool writeFrameOrientation(KDL::Frame& frame) const;

protected:
  //Forward kinemtacis solver
  //KDL::ChainFkSolverPos_recursive *fksolver;
  //Frame: Position and orientation of end effector
  KDL::Frame end_effector;
  //Chain
  KDL::Chain chain;
  // Joint position
  KDL::JntArray joint_pos;
};

void interpolation (const float Ts, const float T, const KDL::JntArray q_ini, const KDL::JntArray q_fin, Eigen::MatrixXf& trajectory);

#endif /* KINEMATIC_CHAIN_H */
