% Make sure you have a 'yarp server' running somewhere and detectable
LoadYarp
options = yarp.Property

% For local (works in windows)
options.put('name','/ravebot')
options.put('subdevice','ravebot')
options.put('device','controlboard')
options.put('single_threaded', 1)  % Needed for Matlab not to shut down

% Instead, for remote (needed in Linux for now) use the following :
% (must have an instance open through a terminal: 'yarpdev --device ravebot name /ravebot')
% options.put('device','remote_controlboard')
% options.put('remote','/ravebot')
% options.put('local','/local')

dd = yarp.PolyDriver
dd.open(options)
pos = dd.viewIPositionControl
pos.positionMove(1,-45)
bv = yarp.BVector(pos.getAxes)
pos.checkMotionDone(bv)
bv.get(1) % 1,2,3,4; should actually loop

% If encoder values were needed:
% enc = dd.viewIEncoders

options2 = yarp.Property
options2.put('device','cartesiankontroller')
dd2 = yarp.PolyDriver
dd2.open(options2) 
cart = dd2.viewICartesianControl

v = yarp.Vector
v.push_back(0.0)
v.push_back(0.0)
v.push_back(1.0)
cart.goToPositionSync(v,4.0) % Set duration to 4 s
cart.waitMotionDone(0.1,4.0) % Check every 0.1 s with a 4 s timeout
pos.checkMotionDone(bv)
