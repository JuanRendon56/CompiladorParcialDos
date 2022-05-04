#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
//Creado por: Juan Manuel Rendon Garcia A01731524
using namespace std;
char mayusculas[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char minusculas[] = "abcdefghijklmnopqrstuvwxyz";
char especial[] = "+-*/()!&%$,<>=:;?@.#";
//char extension[] = "_-";
string conflicto;
vector < string > palabrasReservadas {"or", "id", "and", "not", "true", "int", "false"};
vector < string > noTerminales;
vector < string > terminales;
vector < string > lineasLeidas;
vector < string > firstCadena;
vector < string > followCadena;
vector < string > listaFollow;
vector < string > cadenasFinales;

int checarMayus (char l){ //Checa si la letra es mayuscula
    int centinela = 0;
    for (int i = 0; i < sizeof(mayusculas); i++){
        if (l == mayusculas[i])
            centinela++;
    }
    if (centinela > 0)
        return 1;
    else
        return 0;
}

int checarMinus (char l){ //Checa si la letra es minuscula
    int centinela = 0;
    for (int i = 0; i < sizeof(minusculas); i++){
        if (l == minusculas[i])
            centinela++;
    }
    if (centinela > 0)
        return 1;
    else
        return 0;
}

int checarEspecial (char l){ //Checa si la letra no es un caracter especial
    int centinela = 0;
    for (int i = 0; i < sizeof(especial); i++){
        if (l == especial[i])
            centinela++;
    }
    if (centinela > 0)
        return 1;
    else
        return 0;
}

int checarPalabra (string palabraMagica){ //Checa si la palabraTemporal no es parte de una palabra reservada
    int centinela = 0;
    for (int i = 0; i < palabrasReservadas.size(); i++){
        if (palabrasReservadas[i] == palabraMagica)
            centinela++;
    }
    if (centinela > 0)
        return 1;
    else
        return 0;
}

void quitarDuplicados (string palabraOriginal, vector < string > &vectorActual){ //Checa duplicados
    int centinela = 0;
    for (int i = 0; i < vectorActual.size(); i++){
        if (vectorActual[i] == palabraOriginal)
            centinela++;
    }
    if (centinela == 0){
        vectorActual.push_back(palabraOriginal);
    }
}

int quitarRepeticiones (string palabraPosible, vector < string > &cadena){
    int centinela = 0;
    for (int i = 0; i < cadena.size(); i++){
        //cout << "valor en CADENA i: " << cadena[i] << '\n';
        if (cadena[i] == palabraPosible){
            centinela++;
            //cout << "CASO REPETIRO: " << palabraPosible << '\n';
        }
    }
    if (centinela > 0)
        return 1;
    else{
        return 0;
    }
}

string checarLineaFirst (string lineaRecibida, string palabraFirst){    //Checa la primera palabra
    string palabraChecada;
    string vacio;
    char letras, sigLetras, epsilon;
    int centinela = 0;
    for(int j = 0; j < lineaRecibida.size(); j++){
        letras = lineaRecibida[j];
        sigLetras = lineaRecibida[j+1];
        epsilon = lineaRecibida[j+2];
        palabraChecada.push_back(letras);
        if(isspace(sigLetras) || sigLetras == '\0'){
            if (letras == '\'' && isspace(sigLetras) && lineaRecibida[j+2] == '\''){
                j = j + 2;
                palabraChecada = "' '";
                conflicto = "no";
                return palabraChecada;
            }
            if(centinela == 1){
                return palabraChecada;
            }
            else if(palabraChecada == palabraFirst){
                j = j + 4;
                palabraChecada.clear();
                centinela++;
            }
            else{
                return vacio;
            }
        }
    }
    return vacio;
}

string checarLineaFollow (string lineaRecibida, string palabraFollow){    //Checa la primera palabra
    string palabraChecada;
    string primeraPalabra;
    string vacio;
    char letras, sigLetras, epsilon;
    int centinela = 0;
    for(int j = 0; j < lineaRecibida.size(); j++){
        letras = lineaRecibida[j];
        sigLetras = lineaRecibida[j+1];
        epsilon = lineaRecibida[j+2];
        palabraChecada.push_back(letras);
        if((centinela == 2) && ((isspace(sigLetras)) || (sigLetras == '\0')) || (sigLetras == '\n')){
            //cout << "Palabra FOLLOW hallada: " << palabraChecada << '\n';
            if(palabraChecada == primeraPalabra){
                return vacio;
            }
            else{
                j++;
                palabraChecada.push_back('f');
                return palabraChecada;
            }
        }
        if((isspace(sigLetras)) && (centinela == 0)){ //Primera palabra de la linea
            primeraPalabra = palabraChecada;
            palabraChecada.clear();
            centinela++;
        }
        if((centinela == 1) && (isspace(sigLetras))){ //Si existe mas input despues
            if (letras == '\'' && isspace(sigLetras) && lineaRecibida[j+2] == '\'' && (palabraFollow == primeraPalabra)){
                primeraPalabra.push_back('F');
                conflicto = "No";
                return primeraPalabra;
            }
            if(palabraChecada != palabraFollow){ //Hasta leer el 2do match
                j++;
                palabraChecada.clear();
            }
            if(palabraChecada == palabraFollow){
                //cout << "Palabra repetida por segunda vez \n";
                j++;
                palabraChecada.clear();
                centinela++;
            }
        }
        else if((centinela == 1) && (sigLetras == '\0' || sigLetras == '\n')){  //No hay match en la linea
            //cout << "FIN DE LINEA con la palabra: " << palabraChecada << " buscando palabra: " << palabraFollow << '\n';
            if(palabraChecada == primeraPalabra){
                return vacio;
            }
            else{
                if(palabraChecada != palabraFollow){
                return vacio;
                }
                if(palabraChecada == palabraFollow){
                    primeraPalabra.push_back('F');
                    return primeraPalabra;
                }
            }
        }

    }
    return vacio;
}

void checarFirst (string palabraRecibida,vector < string > &cadena){    //Checa la primera palabra en First
    string palabraRegresada;
    char letras, sigLetras;
    int duplicados;
    duplicados = quitarRepeticiones(palabraRecibida, cadena);
    if(palabraRecibida.size() == 1 && duplicados == 0){        //Condicion letra singular terminal
        char sola = palabraRecibida[0];
        if((checarEspecial(sola) == 1) || (checarMinus(sola) == 1)){
            cadena.push_back(palabraRecibida);
        }
    }
    if(checarPalabra(palabraRecibida) == 1 && duplicados == 0){    //Condicion palabra terminal
        cadena.push_back(palabraRecibida);
    }
    for(int linea = 0; linea < lineasLeidas.size(); linea++){   //Recursion
        palabraRegresada = checarLineaFirst(lineasLeidas[linea], palabraRecibida);
        if(palabraRegresada == "' '" && duplicados == 0){
            cadena.push_back(palabraRegresada);
        }
        if((palabraRegresada != palabraRecibida) && (palabraRegresada.empty() == false)){
            checarFirst(palabraRegresada, cadena);
        }
    }
}

void checarFollow (string palabraRecibida, vector < string > &cadena){  //Checa la siguiente palabra en follow
    string palabraRegresada;
    char letras, sigLetras, ultima;
    int duplicados;
    int centinela = 0;
    if(!listaFollow.empty()){    //Checa que la lista de follows exista
        for(int i = 0; i < listaFollow.size(); i++){    //Lee los follows actuales
            if(palabraRecibida == noTerminales[i]){
                centinela++;
                duplicados = quitarRepeticiones(listaFollow[i], cadena);
                if(duplicados == 0){ //No existe un caso de Follow para la palabra en la cadena
                    //cout << "Follow de la palabra: " << palabraRecibida << " con el valor: " << listaFollow[i] << '\n';
                    cadena.push_back(listaFollow[i]);
                }
            }
        }
    }
    if(centinela > 0)
        conflicto = "No";
    if(centinela == 0){
        for(int linea = 0; linea < lineasLeidas.size(); linea++){   //Recursion
            //cout << "Linea actual: " << linea << '\n';
            //cout << "Palabra actual: " << palabraRecibida << '\n';
            palabraRegresada = checarLineaFollow(lineasLeidas[linea], palabraRecibida);
            if(!palabraRegresada.empty()){
                ultima = palabraRegresada.back();
                palabraRegresada.pop_back();
                //cout << "Palabra regresada: " << palabraRegresada << '\n';
                if((palabraRegresada != palabraRecibida) && (palabraRegresada.empty() == false)){
                    for(int i = 0; i < noTerminales.size(); i++){
                        //cout << "Ciclo no terminales" << '\n';
                        duplicados = quitarRepeticiones(palabraRegresada, cadena);
                        if(palabraRegresada.size() == 1){        //Condicion letra singular terminal
                        //cout << "Condicion palabra singular ENTRO" << '\n';
                        char sola = palabraRegresada[0];
                            if((checarEspecial(sola) == 1) || (checarMinus(sola) == 1)){
                                //cout << "Condicion palabra TERMINAL" << '\n';
                                cadena.push_back(palabraRegresada);
                                palabraRegresada.clear();
                            }
                            else if(ultima == 'f'){
                                //cout << "Condicion first palabra singular" << '\n';
                                checarFirst(palabraRegresada, cadena);
                                palabraRegresada.clear();
                            }
                            else if(ultima == 'F'){
                                //cout << "Condicion follow palabra sing" << '\n';
                                checarFollow(palabraRegresada, cadena);
                                palabraRegresada.clear();
                            }
                        }
                        else{
                            if(checarPalabra(palabraRegresada) == 1){    //Condicion palabra terminal
                                cadena.push_back(palabraRegresada);
                                palabraRegresada.clear();
                            }
                            else if(palabraRegresada == noTerminales[i]){
                                if(ultima == 'F'){
                                    checarFollow(palabraRegresada, cadena);
                                    palabraRegresada.clear();
                                }
                                else if(ultima == 'f'){
                                    //cout << "Palabra a FIRST: " << palabraRegresada << '\n';
                                    checarFirst(palabraRegresada, cadena);
                                    if(cadena.back() == "' '"){
                                        cadena.pop_back();
                                        checarFollow(palabraRegresada, cadena);
                                        conflicto = "Yes";
                                    }
                                    palabraRegresada.clear();
                                }
                            }
                        }
                    }
                }
            }
        //cout << "Termine una linea \n";
        }
    }
}

void leerInicio (vector < string > &cadena){
    string palabraChecada;
    string linea;
    int centinela;
    char letras, sigLetras;
    for(int i = 0; i < lineasLeidas.size(); i++){
        linea = lineasLeidas[i];
        centinela = 0;
        for(int j = 0; j < linea.size(); j++){
            letras = linea[j];
            sigLetras = linea[j+1];
            palabraChecada.push_back(letras);
            if(letras == '\'' && isspace(sigLetras) && linea[j+2] == '\''){
                //cout << "ME SALTE EPSILON" << '\n';
                j = j + 2;
                palabraChecada.clear();
            }
            if((isspace(sigLetras)) && (centinela == 0)){ //Primera palabra de la linea
                //cout << "Palabra a meter en no terminales: " << palabraChecada << '\n' ;
                quitarDuplicados(palabraChecada, cadena);
                palabraChecada.clear();
                centinela++;
            }
        }
        palabraChecada.clear();
    }
}

int main() {
    int entradas, tamanio, casos; //Int de inicio
    string texto; //Texto a capturar por entrada
    string palabraTemporal;   //Palabra posible a formar
    char letra, sigLetra;
    ofstream outText;
    outText.open ("Output.txt");
    getline(cin, texto);
    entradas = stoi(texto);
    texto.clear();
    for(int x = 0; x < entradas; x++){
        getline(cin, texto);
        lineasLeidas.push_back(texto);
    }
    leerInicio(noTerminales);
    for(int j = 0; j < lineasLeidas.size(); j++){
        texto = lineasLeidas[j];
        for(int k = 0; k < texto.size(); k++){
            letra = texto[k];
            sigLetra = texto[k+1];
            palabraTemporal.push_back(letra);
            if (letra == '\'' && isspace(sigLetra) && texto[k+2] == '\''){
                //cout << "CASO EPSILON HALLADO \n";
                k = k + 2;
                palabraTemporal.clear();
            }
            if(letra == '-' && sigLetra == '>'){
                palabraTemporal.clear();
                k = k+2;
            }
            if((isspace(sigLetra) || (sigLetra == '\0'))){ //Verifica si hay un salto
                //cout << "\nENTRE AL SALTO \n";
                k++;
                if(palabraTemporal.size() == 1){    //Verifica si es una letra
                    if(checarMinus(letra) == 1 || checarEspecial(letra) == 1){   //Verifica si es minuscula
                        quitarDuplicados(palabraTemporal,terminales);
                        palabraTemporal.clear();
                    }
                    else{
                        //cout << "Ingresando noTerminal TAMANIO 1: " << palabraTemporal << '\n';
                        quitarDuplicados(palabraTemporal,noTerminales);
                        palabraTemporal.clear();
                    }
                }
                else if (palabraTemporal.size() > 1){
                    if(checarPalabra(palabraTemporal) == 1){
                        quitarDuplicados(palabraTemporal,terminales);
                        palabraTemporal.clear();
                    }
                    else{
                        //cout << "Ingresando noTerminal TAMANIO GRANDE: " << palabraTemporal << '\n';
                        quitarDuplicados(palabraTemporal,noTerminales);
                        palabraTemporal.clear();
                    }
                }
            }
        }
    }
    //Inicio de FIRST/FOLLOW
    string palabraActual;   //Palabra NoTerminal a examinar
    string oracionActual;
    string oracionTemporal;
    string relleno;
    for(int i = 0; i < noTerminales.size(); i++){
        conflicto = "yes";
        palabraActual = noTerminales[i];
        oracionActual.append(palabraActual);
        checarFirst(palabraActual, firstCadena);
        if(i == 0){
            followCadena.push_back("$");
        }
        checarFollow(palabraActual, followCadena);
        relleno = " => FIRST = {";
        oracionActual.append(relleno);
        for(int x = 0; x < firstCadena.size(); x++){
            if(x == firstCadena.size()-1){
                oracionActual.append(firstCadena[x]);
            }
            else{
                relleno = ",";
                oracionActual.append(firstCadena[x]);
                oracionActual.append(relleno);
            }
        }
        relleno = "}, FOLLOW = {";
        oracionActual.append(relleno);
        for(int x = 0; x < followCadena.size(); x++){
            if(followCadena[x] == "' '"){
                //NADA
            }
            else{
                if(x == followCadena.size()-1){
                    oracionActual.append(followCadena[x]);
                    oracionTemporal.append(followCadena[x]);
                }
                else{
                    relleno = ",";
                    oracionActual.append(followCadena[x]);
                    oracionActual.append(relleno);
                    oracionTemporal.append(followCadena[x]);
                    oracionTemporal.append(relleno);
                }
            }
        }
        relleno = "}";
        oracionActual.append(relleno);
        listaFollow.push_back(oracionTemporal);
        cadenasFinales.push_back(oracionActual);
        firstCadena.clear();
        followCadena.clear();
        oracionActual.clear();
        oracionTemporal.clear();
    }
    oracionActual = "LL(1)? ";
    oracionActual.append(conflicto);
    cadenasFinales.push_back(oracionActual);
    for(int z = 0; z < cadenasFinales.size(); z++){
        //cout << cadenasFinales[z] << "\n";
        outText << cadenasFinales[z] << '\n';
    }
    outText.close();
    return 0;
}

