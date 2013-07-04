%%%Programa que recibe datos a través de puertos de Yarp 
%%%Los datos son las coordenadas de 4 esferas situadas en el simulador de asibot
%%%Se extraen y se ordenan en una matriz según la máquina de visión de Peter Corke

function [cact] = recseg(p,cant)



b2 = yarp.Bottle; 		   %Clase de Yarp para recibir los datos
b2 = p.read();			   %Leemos los 4 puntos de la segmentación (8 datos en 4 estructuras)

if (b2.size() ~= 4)
        disp 'WARNING: Point extraction failure'
end

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

%Cálculo de distancias euclídeas, si alguna es cero , la etiquetación esta repitiendo un blob
d = [];
for j = 1:4
	for i= 1:4
	if i ~= j
   	d =[d distancia(X(:,j),X(:,i))];		%guardo las distancias en un vector
	end
	end
end
%d'                        %%%EEEEEEEEEEEEEEEEEEE
a = false;
for i = 1:length(d)
      if (d(i) == 0)
      a = true;						%si alguna distancia es cero, ha fallado el etiquetado
      break
      end
end

%Si el etiquetado ha fallado devuelvo las coordenadas anteriores, sino las actuales
if (a == true)

	cact = cant;
else

	cact = ordenacion(X);
end
end

