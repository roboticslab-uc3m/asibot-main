%%%Programa principal de visual servoing para entorno de pruebas VS3
%e =error ultima iteracion pixel a pixel
%m =matriz con posicion de los motores a cada iteracion
%c =vector con el error euclideo a cada iteracion
%a =iteracion de convergencia del bucle (si repite 100 iteraciones mismo error), si digo que salgo del bucle por error dara la ultima itera
%paso = iteracion en la que pasa por primera vez por cierto valor la norma del error
%Las coordenadas finales son una entrada obligatoria y la profundidad, sino se introduce nada más funciona por error. Si se le mete una 
%argumento de entrada más fuciona por iteraciones siendo esa entrada el número de ellas.

function [e,m,c,a,paso] = viss (cfin,depth,varargin)

%Preparación de todos los puertos entre matlab y yarp

LoadYarp;   			      		%ATENCION PARA CUANDO SE MONTEN TODOS LOS PROGRAMAS, solo cargamos Yarp una vez
import yarp.Port;		      		%simplificamos sintaxis para enviar velocidades

p = yarp.BufferedPortBottle;          		%puerto por el que recibo los datos de la segmentación
velport = Port; 		      		%puerto de velocidades de velguai2
q = yarp.BufferedPortBottle;         		%puerto para leer posición de la cámara
velport.close;			      		%cerramos puerto de velocidades
p.close;					%cerramos puerto de segmentación
q.close;					%cerramos puerto de posición

%Apertura de puertos
                    
velport.open('/matlab/ravebot/rpc:i');
p.open('/mat2'); 
q.open('/matlab/state:o');
 
%%%conexión puerto de segmentación (recseg)

if yarp.Network.connect('/colorSegmentor/state:o','/mat2');
    disp '[success] port connected from /colorSegmentor/state:o';
else
    disp '[warning] port NOT connected from /colorSegmentor/state:o, does it exist?';
end


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

c = [];						%vector con el error guardado a cada iteración
m = [];						%matriz para posición de motores a cada iteración
m = traj(q,m);					%primera lectura de posición de motores
cact = recseg1(p);				%primeras coordenadas del cuadrado (comprobando visualmente que no falla)
cant = cact;					%las guardamaos como coordenadas anteriores por si la etiquetación falla
[e] = velocidad(cfin,cact,velport,cam,depth); 	%cálculo y envío de primera velocidad al robot
i = 0;						%variable para forzar a un número fijo de iteraciones				
done = false;	                		%variable de salida del bucle

%BUCLE DE CONTROL DE VISUAL SERVOING
if (nargin >= 3)
tope = varargin{1};				%numero de iteraciones
tic									
while ~done	
	m = traj(q,m);				%recogemos posicion de los motores                           
	cact = recseg(p,cant);			%recogemos las coordenadas que ahora tenemos (coord actuales)
	cant = cact;				%las guardamos por si en la siguiente iteración la etiquetacion falla
	[e] = velocidad(cfin,cact,velport,cam,depth);	%calculo y envío de velocidad de este paso, devolvemos error 
	c = [c; norm(e)];			%guardo norma del error de esta iteración
	if i == tope				%condición de salida con iteraciones fijas
	done = true;				%cambio de variable del bucle para salir del mismo
	end
	i = i +1;
	%norm(e)					%comprobación del error por pantalla
end
toc
disp('Completado en el número de iteraciones');

else

tic									
while ~done	
	m = traj(q,m);				%recogemos posicion de los motores                           
	cact = recseg(p,cant);			%recogemos las coordenadas que ahora tenemos (coord actuales)
	cant = cact;				%las guardamos por si en la siguiente iteración la etiquetacion falla
	[e] = velocidad(cfin,cact,velport,cam,depth);	%calculo y envío de velocidad de este paso, devolvemos error 
	c = [c; norm(e)];			%guardo norma del error de esta iteración
	if norm(e) <= 5                 	%condición de salida del lazo cerrado
	done = true;				%cambio de variable del bucle para salir del mismo
	end
	%norm(e)					%comprobación del error por pantalla
end
toc
disp('Completado en el error requerido');

end
%Una vez cumplida la condición de error, paramos al robot enviando velocidades 0 a todos sus motores


dOut = yarp.Bottle;
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


disp('Cerrando puertos...');
velport.close;
p.close;
q.close;

%Busqueda de convergencia por repetición del mismo valor de error numero de iteraciones
gerch = c(1);
xerch = [];
cont = 0;
for a=1:length(c)
	xerch = c(a);
	if cont == 100
	break;	
	else
		if (xerch == gerch)
			cont = cont + 1;
		else
			gerch = xerch;
			cont = 0;
		end
	end
end

%Búsqueda de iteracion paso por cierto valor en norma del error

for paso=1:length(c)
	if (c(paso) <=5)
	break

    	end
end
%Dibujo de gráficas
%grafica del error
plot (1:length(c),c,'r');
xlabel('Iteraciones');
ylabel('Norma del error');
title ('Error de las coordenadas de la imagen');


%graficas de los motores

figure;
subplot (2,1,1);
plot (1:length(m), m(:,1),'r',1:length(m),m(:,2),'g',1:length(m),m(:,3),'b');
hleg1 = legend('motor x','motor y','motor z');
xlabel('Iteraciones');
ylabel('Posicion (m)');
title ('Motores de traslacion');
subplot (2,1,2);
plot (1:length(m), m(:,4),'r',1:length(m),m(:,5),'g',1:length(m),m(:,6),'b');
hleg1 = legend('motor x','motor y','motor z');
xlabel('Iteraciones');
ylabel('Rotacion (grados)');
title ('Motores de rotacion');

end
