%%%Programa principal de visual servoing


function x = viss2 (cfin)

%Preparación de todos los puertos entre matlab y yarp

LoadYarp;   			      %ATENCION PARA CUANDO SE MONTE 
import yarp.Port;		      %para enviar velocidades

p = yarp.BufferedPortBottle;          %puerto por el que recibo los datos de la segmentación
velport = Port; 		      %puerto de velocidades de velguai2
velport.close;
p.close;
%Apertura de puertos
                    
velport.open('/matlab/ravebot/cartesianServer/command:i');
p.open('/mat2'); 
 
%%%conexión puerto de segmentación (recseg)
if yarp.Network.connect('/colorSegmentor/state:o','/mat2');
    disp '[success] port connected from /colorSegmentor/state:o';
else
    disp '[warning] port NOT connected from /colorSegmentor/state:o, does it exist?';
end


%%%conexión puerto de velocidades (velguai)
if yarp.Network.connect('/matlab/ravebot/cartesianServer/command:i','/ravebot/cartesianServer/command:i')  
    disp '[success] port connected from /ravebot/cartesianServer/command:i';
else
    disp '[warning] port NOT connected from /ravebot/cartesianServer/command:i, does it exist?';
end


cam = CentralCamera('resolution',[1280 1024],'pixel',[3.6e-6 3.6e-6],'focal',0.0036,'centre',[640 512]); %modelado de la camara de asibot

cact = recseg1(p);				%coordenadas actuales del cuadrado
cant = cact;					%las guardamaos como coordenadas anteriores por si la etiquetación falla
[e] = velocidad2(cfin,cact,velport,cam);

done = false;										
while ~done	                           
	cact = recseg(p,cant);
	cant = cact;
	[e] = velocidad2(cfin,cact,velport,cam);
	
	if norm(e) <= 20
	done = true;
	end
	
	norm(e)
end

disp('Has conseguido el error requerido');
dOut = yarp.Bottle;
dOut.addVocab(yarp.Vocab.encode('vmos'));
dBottle = dOut.addList;
dBottle.addDouble(0.0);
dBottle.addDouble(0.0);
dBottle.addDouble(0.0);
dBottle.addDouble(0.0);
dBottle.addDouble(0.0);
velport.write(dOut);
x = e;

disp('Cerrando puertos...');
velport.close;
p.close;

end
