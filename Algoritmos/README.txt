La compilación de los programas para los archivos es la siguiente:

Estando en el directorio del codigo C, escribir los siguientes comandos
Para el caso de un codigo con sqrt y threads :
gcc "nombredelarchivo.c" -o archivo -pthread -lm
./archivo

En caso de no tener sqrt, quitar el -lm y en caso de no tener threads quitar el -pthread

La entrada para los archivos con threads va a ser:
./archivo n k
siendo n el tamaño de la matriz/número y k el numero de threads

la entrada para los archivos sin threads va a ser quitando la k, o sea:
./archivo n