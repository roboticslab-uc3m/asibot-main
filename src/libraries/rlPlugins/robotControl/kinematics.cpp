#include "kinematics.h"

using namespace KDL;
using namespace std;

KDL::Frame Kinematics :: forwardKinematics(KDL::JntArray joint_positions)
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
