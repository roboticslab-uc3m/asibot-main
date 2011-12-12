//
// kinematics_kdl.cpp
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
#include "kinematic_chain.h"
#include <fstream>



using namespace KDL;
using namespace std;
using namespace Eigen;

// Kinematics functions
//------------------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//------------------------------------------------------------------------
/*
KDL::Frame KinematicChain :: forwardKinematics(KDL::JntArray joint_positions)
{
  KDL::Frame frame;
  // Create solver based on kinematic chain
  fksolver = new ChainFkSolverPos_recursive(chain);
  // Calculate forward position kinematics and store it in frame
  bool kinematics_status;
  kinematics_status = fksolver->JntToCart(joint_positions,frame);
  if(kinematics_status<0)
  {
    cerr << "ERROR: could not calculate forward kinematics :(" << endl;
    exit(-1);
  }
  return frame;
}

KDL::JntArray KinematicChain :: inverseKinematicsNR(KDL::Frame frame)
{
  // Calculate the inverse kinematic using newton-raphson method
  JntArray q;
  ChainIkSolverVel_pinv iksolver_vel (chain); //Inverse velocity solver
  ChainIkSolverPos_NR iksolver_pos (chain,*fksolver,iksolver_vel,100,1e-6);//Maximum 100 iterations, stop at accuracy 1e-6
  int ret = iksolver_pos.CartToJnt(joint_pos,frame,q); // joint_pos is the initial guess
  if(ret<0)
  {
    cerr << "ERROR: could not calculate inverse kinematics :(" << endl;
    exit(-1);
  }
  return q;
}

void KinematicChain :: moveCartesian(const float time, const float sampling_time, const KDL::Frame frame_destination, Eigen::MatrixXf& trajectory)
{
  KDL::JntArray q_fin;
  q_fin = inverseKinematicsNR(frame_destination);
  // Trajectory interpolation between actual pos(= joint_pos) and q_fin using 3rd order spline
  interpolation(sampling_time, time, joint_pos, q_fin, trajectory);
  printArticularTrajectory(trajectory);
  writeArticularTrajectory(trajectory);
}


bool KinematicChain :: readEndEffectorTrajectory(const string filename, vector<float>& time, vector<KDL::Frame>& cart_trajectory)
{
  int npos, lpos;
  float d;
  ifstream file;
  string line;
  vector<float> data;
  file.open(filename.c_str(),ios::in);
  if(!file.is_open()) return false;
  //FOR  NOW I ONLY MANAGE 2D TRAJECTORIES WITH TIME(trajectory obtained using the program posture_trajectories)
  while(getline(file,line))
  {
    line += ' ';
    npos=0;
    lpos=0;
    while ((npos = (int)line.find(' ',lpos)) != string::npos)
    {
      if (npos > lpos)
      {
        istringstream iss(line.substr(lpos, npos-lpos));
        if (iss >> d)
        {
          data.push_back(d);
        }
      }
    lpos = npos + 1;
    }
  }
  // Convert data
  KDL::Frame fr; // initialized as rotation = I, and vector = 0
  for (int i = 0; i < data.size(); i=i+3)
  {
    time.push_back(data.at(i));
    fr.p.data[0] = data.at(i+1);
    fr.p.data[2] = data.at(i+2); // Y axis is always 0 because we are in sagital plane
    cart_trajectory.push_back(fr);
  }
  return true;
}
*/
// Getters and Setters
//------------------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//------------------------------------------------------------------------
KDL::JntArray KinematicChain :: getJointPositions() const
{
  return joint_pos;
}

KDL::Frame KinematicChain :: getEndEffector() const
{
  return end_effector;
}


void KinematicChain :: setJointPositions(KDL::JntArray q)
{
  if (chain.getNrOfJoints() != q.rows())
  {
    cerr << "ERROR: Joint size is not the same" << endl;
    exit (-1);
  }
  joint_pos =(KDL::JntArray)q;
}




// Print functions
//------------------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//------------------------------------------------------------------------
void KinematicChain :: printFrame (KDL::Frame& frame) const
{
  printFrameOrientation(frame);
  printFramePosition(frame);
}

void KinematicChain :: printFrameOrientation (KDL::Frame& frame) const
{
  cout << "End effector's frame orientation:" << endl;
  cout << frame.M.data[0] << "\t" << frame.M.data[1]
      << "\t" << frame.M.data[2] << "\t" << endl
      << frame.M.data[3] << "\t" << frame.M.data[4]
      << "\t" << frame.M.data[5] << "\t" << endl
      << frame.M.data[6] << "\t" << frame.M.data[7]
      << "\t" << frame.M.data[8] << endl;
}

void KinematicChain :: printFramePosition (KDL::Frame& frame) const
{
  cout << "End effector's frame position:" << endl;
  cout << "( " << frame.p.data[0] << ", " << frame.p.data[1] <<  ", " << frame.p.data[2] << ")"<< endl;
}

void KinematicChain :: printTrajectory(vector<KDL::Frame>& trajectory) const
{
  //Print the trajectory
  for (int i=0;i<trajectory.size();i++)
  {
    cout << "Iteration number " << i << endl;
    printFrame(trajectory.at(i));
  }
}


void KinematicChain :: printTrajectoryPosition(std::vector<KDL::Frame>& trajectory) const
{
  for (int i=0;i<trajectory.size();i++)
  {
    cout << "Iteration number " << i << endl;
    printFramePosition(trajectory.at(i));
  }
}

void KinematicChain :: printJntArray(KDL::JntArray array) const
{
  cout << "Joint Positions: ";
  for (int i = 0; i < array.rows(); i++)
  {
    cout << array.data[i]  << "\t";
  }
  cout << endl;
}




void KinematicChain :: printArticularTrajectory(Eigen::MatrixXf& trajectory) const
{
  int columns = trajectory.cols();
  int rows = trajectory.rows();
  cout << "Articular trajectory: " << endl;
  for(int i = 0; i < columns; i++)
  {
    for(int j = 0; j < rows; j++)
    {
      cout << "q(" << j << ") = " << trajectory(j,i) << "\t";
    }
    cout << "\n";
  }
}

// Write functions
//------------------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//------------------------------------------------------------------------
void KinematicChain :: writeTrajectoryToFile(vector<KDL::Frame>& trajectory) const
{
  //If there is a file, delete it
  system("rm -f Position.txt");
  system("rm -f Orientation.txt");
  for (int i=0;i<trajectory.size();i++) //number of rows of trajectory
  {
    //Writing the end effector's position
    if(!writeFramePosition(trajectory.at(i)))
      cerr << "ERROR: Can not write frame position" << endl;
    //Writing the end effector's orientation
    if(!writeFrameOrientation(trajectory.at(i)))
      cerr << "ERROR: Can not write frame orientation" << endl;
  }
}

void KinematicChain :: writeArticularTrajectory(Eigen::MatrixXf& trajectory) const
{
  //If there is a file, delete it
  system("rm -f Joint_trajectory.txt");
  int columns = trajectory.cols();
  int rows = trajectory.rows();
  ofstream file_pos;
  file_pos.open("Joint_trajectory.txt",ios::app);
  if(!file_pos.is_open())
  {
    cerr << "ERROR: Could not open file" << endl;
    exit(-1);
  }
  for(int i = 0; i < columns; i++)
  {
    for(int j = 0; j < rows; j++)
    {
      file_pos  << trajectory(j,i) << "\t";
    }
    file_pos << "\n";
  }
  file_pos.close();
}


bool KinematicChain :: writeFramePosition(KDL::Frame& frame) const
{
  ofstream file_pos;
  file_pos.open("Position.txt",ios::app);
  if(!file_pos.is_open()) return false;
  file_pos << frame.p.data[0] << "\t" << frame.p.data[1] << "\t" << frame.p.data[2] << endl;
  file_pos.close();
  return true;
}

bool KinematicChain :: writeFrameOrientation(KDL::Frame& frame) const
{
  ofstream file_or;
  file_or.open("Orientation.txt",ios::app);
  if(!file_or.is_open()) return false;
  file_or << frame.M.data[0] << "\t" << frame.M.data[1] << "\t" << frame.M.data[2] << "\t"
      << frame.M.data[3] << "\t" << frame.M.data[4] << "\t" << frame.M.data[5] << "\t"
      << frame.M.data[6] << "\t" << frame.M.data[7] << "\t" << frame.M.data[8] << endl;
  file_or.close();
  return true;
}

// OTHER FUNCTIONS
//------------------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//------------------------------------------------------------------------
void interpolation (const float Ts, const float T, const KDL::JntArray q_ini, const KDL::JntArray q_fin, Eigen::MatrixXf& trajectory)
{
  int iterations;
  float t,temp;
  float a,b,c, d;
  iterations = T/Ts+1;
  // Definition of a matrix using Eigen.
  int n_rows = q_ini.rows();
  trajectory.resize(n_rows,iterations);

  for (int i = 0; i < n_rows; i++)
  {
    a = q_ini.data[i];
    c = (3*(q_fin.data[i]-q_ini.data[i]))/(T*T);
    d = (-2*(q_fin.data[i]-q_ini.data[i]))/(T*T*T);
    t = 0;
    for (int j = 0; j < iterations; j++)
    {
      trajectory(i,j) = a + c*t*t + d*t*t*t;
      t = t+Ts;
    }
  }
}

