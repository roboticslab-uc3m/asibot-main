%%Cálculo de velocidades en función de la imagen actual y la deseada

function [e] = velocidad2 (cfin,cact,velport,cam)

%%Preparación de puertos entre yarp y matlab%%%%%%%%%%%%%

bOut = yarp.Bottle;		%Esctructura de yarp xa enviar velocidad




%%Obtención de coordenadas y calculo de error%%%%%%%%%%%%%%%%%



e = cact - cfin;                    %cálculo del error 
e = e(:);                           %forzamos e a vector columna para cuadrar dimensiones   
depth = 1; 
J = visjac_p( cam, cact, depth );       %cálculo de la jacobiana visual, 
lambda = 1;                     %ganancia del sistema                            
%LEY DE CONTROL

vel = -lambda * pinv(J) * e;                  %
%%%Cambio de radianes a grados

vel(4) = radtodeg(vel(4));
vel(5) = radtodeg(vel(5));
vel(6) = radtodeg(vel(6));

%%Preparación del vector velocidad y envío a través del puerto de yarp


vel(4,:) = [];	%eliminamos componente wx, ya que asibot carece de ella tb
%vel(4) = 0;
%vel(5) = 0;

lambda1 = 0.09;					%ganancia de la velocidad traslación
lambda2 = 0.09;					%ganancia de la velocidad rotación
if norm (e) <= 50
	lambda1 = lambda1 / 2;			
end

vel(1) = lambda1*vel(1); 
vel(2) = lambda1*vel(2);
vel(3) = lambda1*vel(3);
vel(4) = lambda2*vel(4);
vel(5) = lambda2*vel(5);


bOut.addVocab(yarp.Vocab.encode('vmos'));
dBottle = bOut.addList;
dBottle.addDouble(vel(2)); %dBottle.addDouble(vel(1));
dBottle.addDouble(-vel(1)); %dBottle.addDouble(vel(2));
dBottle.addDouble(vel(3));
dBottle.addDouble(vel(4));
dBottle.addDouble(vel(5));
velport.write(bOut);
%%EEEE

end
