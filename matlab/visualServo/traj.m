
function m = traj (q,m)


t = yarp.Bottle; 		   %Clase de Yarp para recibir los datos
t = q.read();
a0 = ((t.get(0).asDouble)*pi)/180;
a1 = ((t.get(1).asDouble)*pi)/180;
a2 = ((t.get(2).asDouble)*pi)/180;
a3 = (t.get(3).asDouble);
a4 = (t.get(4).asDouble);
a5 = (t.get(5).asDouble);
m = [m;a0 a1 a2 a3 a4 a5];
end
