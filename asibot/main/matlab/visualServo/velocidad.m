%%Cálculo de velocidades en función de la imagen actual y la deseada

function [e] = velocidad (cfin,cact,velport,cam,depth)

%%Preparación de puertos entre yarp y matlab%%%%%%%%%%%%%

bOut = yarp.Bottle;			%Esctructura de yarp xa enviar velocidad




%%Obtención de coordenadas y cálculo de error%%%%%%%%%%%%%%%%%



e = cact - cfin;                    	%cálculo del error 
e = e(:);                           	%forzamos e a vector columna para cuadrar dimensiones   
%depth = 1; 
J = visjac_p( cam, cact, depth );       %cálculo de la jacobiana visual, 

lambda = 1;                     	%ganancia del sistema, (antigua) ahora no modifica nada     
                      
%LEY DE CONTROL. (vel =(Vx,Vy,Vz,Wx,Wy,Wz))

vel = -lambda * pinv(J) * e;                  

%%%Cambio de radianes a grados en velocidad angular para introducirlos en concordancia con ASIBOT

vel(4) = radtodeg(vel(4));
vel(5) = radtodeg(vel(5));
vel(6) = radtodeg(vel(6));

%Aplicación de ganacias por separado, cuando este mas cerca del objetivo se reducen

lambda1 = 4;
lambda2 = 0.05*lambda1;

if norm (e) <= 25
	lambda1 = lambda1 / 2;			%ganancia de la velocidad lineal
	lambda2 = lambda1 * 0.05;		%ganacia de la velocidad angular
end



vel(1) = lambda1*vel(1); 
vel(2) = lambda1*vel(2);
vel(3) = lambda1*vel(3);
vel(4) = lambda2*vel(4);
vel(5) = lambda2*vel(5);
vel(6) = lambda2*vel(6);

%%Preparación del vector velocidad y envío a través del puerto de yarp, será un bottle dentro de un bottle
%Hay que enviar 2 vocab para enviar en modo velocidad, "set" y "vmos"
%Luego enviar las velocidades en sí como doubles de una lista


bOut.addVocab(yarp.Vocab.encode('set'));
bOut.addVocab(yarp.Vocab.encode('vmos'));
dBottle = bOut.addList;
dBottle.addDouble(vel(1));		%Vx
dBottle.addDouble(vel(2));		%Vy
dBottle.addDouble(vel(3));		%Vz
dBottle.addDouble(vel(4));		%Wx
dBottle.addDouble(vel(5));		%Wy
dBottle.addDouble(vel(6));		%Wz

velport.write(bOut);


end
