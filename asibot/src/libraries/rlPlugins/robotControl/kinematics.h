#include <kdl/chainfksolverpos_recursive.hpp>
#include "kinematic_chain.h"

class Kinematics : public KinematicChain
{
public:
  // Kinematics functions
  KDL::Frame forwardKinematics(KDL::JntArray joint_positions);

protected:
  KDL::ChainFkSolverPos_recursive *fksolver;

};
