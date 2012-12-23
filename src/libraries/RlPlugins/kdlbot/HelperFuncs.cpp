
// -----------------------------------------------------------------------------

#include "KdlBot.h"

// -----------------------------------------------------------------------------

double KdlBot::toDeg(const double _inRad) {
    return (_inRad * 180.0 / M_PI);
}

// -----------------------------------------------------------------------------

double KdlBot::toRad(const double _inDeg) {
    return (_inDeg * M_PI / 180.0);
}

// -----------------------------------------------------------------------------

bool KdlBot::getMatrixFromProperties(Searchable &options, ConstString &tag, yarp::sig::Matrix &H) {

    Bottle *bH=options.find(tag).asList();
    if (!bH) return false;
    
    int i=0;
    int j=0;
    H.zero();
    for (int cnt=0; (cnt<bH->size()) && (cnt<H.rows()*H.cols()); cnt++) {    
        H(i,j)=bH->get(cnt).asDouble();
        if (++j>=H.cols()) {
            i++;
            j=0;
        }
    }
    return true;
}

// -----------------------------------------------------------------------------

bool KdlBot::fwdKin(const yarp::sig::Vector &inUnits, yarp::sig::Vector &x, yarp::sig::Vector &o) {
    JntArray inRad = JntArray(cmcNumMotors);
    Frame fOutCart;
    for (int motor=0; motor<cmcNumMotors; motor++) {
        if(isPrismatic[motor]) inRad(motor)=inUnits[motor];
        else inRad(motor)=toRad(inUnits[motor]);
    }
    ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain);
    ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver.
    fksolver.JntToCart(inRad,fOutCart);

    x.clear();
    x.push_back(fOutCart.p.data[0]);  // pushed on as [0]
    x.push_back(fOutCart.p.data[1]);  // pushed on as [1]
    x.push_back(fOutCart.p.data[2]);  // pushed on as [2]

    if (angleRepr == "axisAngle") {
        o.resize(4);
        KDL::Vector rotVector = fOutCart.M.GetRot();
        o[3] = fOutCart.M.GetRotAngle(rotVector);  // Normalizes as colateral effect
        o[0] = rotVector[0];
        o[1] = rotVector[1];
        o[2] = rotVector[2];
        printf("[HelperFuncs] KDL computed current cart: %f %f %f | %f %f %f %f.\n",
            fOutCart.p.data[0],fOutCart.p.data[1],fOutCart.p.data[2],o[0],o[1],o[2],o[3]);
    } else if (angleRepr == "eulerYZ") {  // ASIBOT
        double alfa, beta, gamma;
        fOutCart.M.GetEulerZYZ(alfa, beta, gamma);
        o.clear();
        o.push_back(toDeg(beta));  // pushed on as [0]
        o.push_back(toDeg(gamma));  // pushed on as [1]
        printf("[HelperFuncs] KDL computed current cart: %f %f %f | %f %f.\n",
            fOutCart.p.data[0],fOutCart.p.data[1],fOutCart.p.data[2],o[0],o[1]);
    } else if (angleRepr == "eulerZYZ") {
        double alfa, beta, gamma;
        fOutCart.M.GetEulerZYZ(alfa, beta, gamma);
        o.clear();
        o.push_back(toDeg(alfa));  // pushed on as [0]
        o.push_back(toDeg(beta));  // pushed on as [1]
        o.push_back(toDeg(gamma));  // pushed on as [2]
        printf("[HelperFuncs] KDL computed current cart: %f %f %f | %f %f %f.\n",
            fOutCart.p.data[0],fOutCart.p.data[1],fOutCart.p.data[2],o[0],o[1],o[2]);
    } else {
        printf("[HelperFuncs] KDL computed current cart: %f %f %f\n",fOutCart.p.data[0],fOutCart.p.data[1],fOutCart.p.data[2]);
    }

    return true;
}
