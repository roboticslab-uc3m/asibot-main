%%%Programa para llevar camarabot a posicion determinada (vel x tiempo)


function x = posicionar ()

%Preparación de todos los puertos entre matlab y yarp

LoadYarp;   			      		%ATENCION PARA CUANDO SE MONTEN TODOS LOS PROGRAMAS, solo cargamos Yarp una vez
import yarp.Port;		      		%simplificamos sintaxis para enviar velocidades


velport = Port; 		      		%puerto de velocidades de velguai2
q = yarp.BufferedPortBottle;         		%puerto para leer posición de la cámara
velport.close;			      		%cerramos puerto de velocidades
q.close;					%cerramos puerto de posición

%Apertura de puertos
                    
velport.open('/matlab/ravebot/rpc:i');
q.open('/matlab/state:o');


%%%conexión puerto de velocidades (velguai)

if yarp.Network.connect('/matlab/ravebot/rpc:i','/ravebot/rpc:i')  
    disp '[success] port connected from /ravebot/rpc:i';
else
    disp '[warning] port NOT connected from /ravebot/rpc:i, does it exist?';
end

%%%conexión puerto de posiciones (traj)

if yarp.Network.connect('/ravebot/state:o','/matlab/state:o')  
    disp '[success] port connected from /ravebot/state:o';
else
    disp '[warning] port NOT connected from /ravebot/state:o, does it exist?';
end

%Modelado de la cámara que guía al robot con sus parámetros intrínsecos

cam = CentralCamera('resolution',[1280 1024],'pixel',[3.6e-6 3.6e-6],'focal',0.0036,'centre',[640 512]); 

bOut = yarp.Bottle;				%bottle para enviar vel


bOut.addVocab(yarp.Vocab.encode('set'));
bOut.addVocab(yarp.Vocab.encode('vmos'));
dBottle = bOut.addList;
dBottle.addDouble(0.0);
dBottle.addDouble(0.0);
dBottle.addDouble(0.0);
dBottle.addDouble(1);
dBottle.addDouble(1);
dBottle.addDouble(1);
velport.write(bOut);
pause (10);

disp('Parando motores');
dOut = yarp.Bottle;				%bottle para enviar vel cero
dOut.addVocab(yarp.Vocab.encode('set'));
dOut.addVocab(yarp.Vocab.encode('vmos'));
dBottle = dOut.addList;
dBottle.addDouble(0.0);
dBottle.addDouble(0.0);
dBottle.addDouble(0.0);
dBottle.addDouble(0.0);
dBottle.addDouble(0.0);
dBottle.addDouble(0.0);
velport.write(dOut);

t = yarp.Bottle; 		   		%Clase de Yarp para recibir los datos
t = q.read();
a0 = t.get(0).asDouble;
a1 = t.get(1).asDouble;
a2 = t.get(2).asDouble;
a3 = t.get(3).asDouble;
a4 = t.get(4).asDouble;
a5 = t.get(5).asDouble;
m = [a0 a1 a2 a3 a4 a5];
x = m;						%valor que devuelve el programa (error en pixeles respecto a los deseados)

disp('Cerrando puertos...');
velport.close;
q.close;


end
