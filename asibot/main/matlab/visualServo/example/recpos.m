%%%Programa que recibe y ordena las coordenadas que en ese momento vea la cámara

function [P] = recpos()
LoadYarp;
p = yarp.BufferedPortBottle;       %puerto por el que recibo los datos
p.close;			   %cerramos puerto
p.open('/mat2');                   %abro puerto y le llamo /mat2
if yarp.Network.connect('/colorSegmentor/state:o','/mat2')
	disp '[success] port connected from /colorSegmentor/state:o';
else
	disp '[warning] port NOT connected from /colorSegmentor/state:o, does it exist?';
end

b2 = yarp.Bottle; 		   %Clase de Yarp para recibir los datos
b2 = p.read();			   %Leemos los 4 puntos de la segmentación (8 datos en 4 estructuras)

%Recogemos los 4 puntos agrupados con su x e y correspondiente
l0 = b2.get(0).asList;             
l1 = b2.get(1).asList;
l2 = b2.get(2).asList;
l3 = b2.get(3).asList;
%Obtención de cada coordenada de cada punto por separado;
x0 = l0.get(0).asDouble;   
y0 = l0.get(1).asDouble;

x1 = l1.get(0).asDouble;
y1 = l1.get(1).asDouble;

x2 = l2.get(0).asDouble;
y2 = l2.get(1).asDouble;

x3 = l3.get(0).asDouble;
y3 = l3.get(1).asDouble;

%Matriz P

X = [x0 x1 x2 x3;y0 y1 y2 y3];
P = ordenacion(X);
p.close;
end
