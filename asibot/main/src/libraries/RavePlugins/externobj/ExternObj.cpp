/**
 * thanks Rosen Diankov

   Every plugin contains a bunch of openrave interfaces, the plugincpp plugin creates a simple OpenRAVE::ModuleBase interface named \b mymodule.

   Inside programs, load the plugin using the RaveLoadPlugin, and then create the module the plugin offers using

   \verbatim
   m=RaveCreateModule(env,"mymodule");
   \endverbatim

   To test things through the command line, do:

   \verbatim
   openrave --loadplugin libplugincpp.so --module mymodule "my args"
   \endverbatim

   This will load liboplugincpp.so and startup module "mymodule". From plugincpp, notice that mymodule
   supports some "commands". These are in-process string-based calls invoked through
   interface->SendCommand function.

   If you are using octave or matlab, then can communicate with openrave through tcp/ip, check out: http://openrave.programmingvision.com/wiki/index.php/OctaveMATLAB

   Most openrave users use python to dynamically interact with openrave. For example:

   \verbatim
   openrave.py -i  --loadplugin libplugincpp.so data/lab1.env.xml
   \endverbatim

   drops into the python promp with the plugin loaded and a scene loaded. Then it is possible to execute the following python commands to create the interface and call a command:

   \verbatim
   m=RaveCreateModule(env,'mymodule')
   env.Add(m,true,'my args')
   m.SendCommand('numbodies')
   \endverbatim

   <b>Full Example Code:</b>
 */
#include <openrave/openrave.h>
#include <openrave/plugin.h>
#include <boost/bind.hpp>

#include <yarp/os/BufferedPort.h>

using namespace std;
using namespace OpenRAVE;

class ExternObjCallbackPort : public yarp::os::BufferedPort<yarp::os::Bottle> {
  public:
    void setObjPtr(KinBodyPtr objPtr) {
        _objPtr = objPtr;
    }

  protected:
    /** KinBody object pointer. */
    KinBodyPtr _objPtr;
    
    /** Implement the actual callback. */
    void onRead(yarp::os::Bottle& b) {
        //printf("[ExternObjCallbackPort] Got %s.\n", b.toString().c_str());
        Transform T = _objPtr->GetTransform();
        //printf("[ExternObjCallbackPort] success: object \"redCan\" exists and at %f %f %f.\n",T.trans.x,T.trans.y,T.trans.z);
        T.trans.x = b.get(0).asDouble();  // [m]
        T.trans.y = b.get(1).asDouble();  // [m]
        T.trans.z = b.get(2).asDouble();  // [m]
        _objPtr->SetTransform(T);
    }
};

class ExternObj : public ModuleBase {
  public:
    ExternObj(EnvironmentBasePtr penv) : ModuleBase(penv) {
        __description = "A very simple plugin.";
        RegisterCommand("open",boost::bind(&ExternObj::Open, this,_1,_2),"opens port");
    }

    virtual ~ExternObj() {
    }

    void Destroy() {
        RAVELOG_INFO("module unloaded from environment\n");
    }

    int main(const string& cmd) {
        RAVELOG_INFO("module initialized cmd; %s\n", cmd.c_str());
        return 0;
    }

    bool Open(ostream& sout, istream& sinput) {
        _objPtr = GetEnv()->GetKinBody("redCan");
        if(!_objPtr) {
           fprintf(stderr,"[ExternObj] success: object \"redCan\" does not exist.\n");
        }
        Transform T = _objPtr->GetTransform();
        printf("[ExternObj] success: object \"redCan\" exists and at %f %f %f.\n",T.trans.x,T.trans.y,T.trans.z);

        _externObjCallbackPort.setObjPtr(_objPtr);
        bool bSuccess = _externObjCallbackPort.open("/ravebot/externobj/command:i");
        _externObjCallbackPort.useCallback();
        sout << "response";
        return bSuccess;
    }

  protected:
    ExternObjCallbackPort _externObjCallbackPort;
    KinBodyPtr _objPtr;
};

InterfaceBasePtr CreateInterfaceValidated(InterfaceType type, const std::string& interfacename, std::istream& sinput, EnvironmentBasePtr penv) {
    if( type == PT_Module && interfacename == "externobj" ) {
        return InterfaceBasePtr(new ExternObj(penv));
    }
    return InterfaceBasePtr();
}

void GetPluginAttributesValidated(PLUGININFO& info) {
    info.interfacenames[PT_Module].push_back("ExternObj");
}

OPENRAVE_PLUGIN_API void DestroyPlugin() {
    RAVELOG_INFO("destroying plugin\n");
}

