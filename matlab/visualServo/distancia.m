%%Programa para calcular la distancia euclídea entre dos puntos en 2 dimensiones

function d = distancia(P1,P2)

 d = sqrt(((P2(1)-P1(1))^2)+((P2(2)-P1(2))^2));
