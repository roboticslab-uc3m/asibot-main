%%Programa para ordenar coordenadas de 4 puntos escritos como una matriz
function [P1] = ordenacion (P) 
suma = zeros(1,4);   				     %Vector del tamaño de P que contendra la suma de las coordenadas de cada punto
min=P(1,1)+P(2,1);			     %asignamos el mínimo a la suma del primer punto de P
max=0;indmax=1;indmin=1;
%%%%CREO que sacando la x min de este bucle me vale para el de abajo (submin)
%Bucle encontar max y min suma de coordenadas (1º y 3º punto de la matriz final)
				     
for i=1:4					     
	suma(i) = P(1,i)+P(2,i);		     	     %Vamos metiendo cada suma de puntos 
	if (suma(i) >= max )				     %comparación con el máximo
	max = suma(i);					     %actualización de la suma máxima
	indmax = i;					     %guardamos la posición del máximo
	end
	if (suma(i) <= min )				     %comparacion con el mínimo
	min = suma(i);					     %actualizamos la suma mínima
	indmin = i;					     %guardamos posición de la suma mínima
	end
end

%Bucle para encontrar el indice x max (2º ó 4º punto de la matriz final ), el restante se deduce de la suma

indsubmin=0;indsubmax=0;submax=0;
for i=1:4
	if ((i~= indmin) && (i~= indmax))		     %Descartamos los puntos encontrados antes
        	if (P(1,i) >= submax )			     %Comparamos con el submáximo
        	submax = P(1,i);			     %actualizamos submáximo
        	indsubmax = i;				     %guardamos indice de submáximo
        	end				     
	end
end

indsubmin = 10 - (indmin + indmax + indsubmax);
%indsubmax
P1(:,1)=[floor(P(1,indmin)) floor(P(2,indmin)) ]; 
P1(:,2)=[floor(P(1,indsubmin)) floor(P(2,indsubmin)) ]; 
P1(:,3)=[floor(P(1,indmax)) floor(P(2,indmax)) ]; 
P1(:,4)=[floor(P(1,indsubmax)) floor(P(2,indsubmax)) ]; 
