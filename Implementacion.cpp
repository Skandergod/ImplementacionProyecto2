#include <iostream>
#include <fstream>
#include <string>

using namespace std;

char** map;
int** mapMask;
int nCiclos = 0;
int fila = 0, columna = 0, cicloActual = 0, carrosCount = 0, motosCount = 0, semaforosCount = 0;
int ID = 0; // Variable para asignar IDs únicos a los elementos y contar cuantos elementos hay en el mapa

void disableVehicle(int i1, int j1, int i2, int j2);

enum elementType
{
    NONE,
	SEMAFORO,
	MOTO,
	AUTO,
};

class element
{
    private:
    int i, j, eType, id;

    public:
    element()
    {
        i = 0, j = 0;    
    }

    //sets y gets de i
    void seti (int a)
    {
        i = a;
    }

    int geti ()
    {
        return i;
    }

    //sets y gets de j
    void setj (int a)
    {
        j = a;
    }

    int getj ()
    {
        return j;
    }

    //sets y gets elementType
    void seteType (int a)
    {
        eType = a;
    }

    int geteType ()
    {
        return eType;
    }

    //sets y gets del id del vehiculo
    void setid (int a)
    {
        id = a;
    }

    int getid ()
    {
        return id;
    }


    //funcion para que el elemento del mapa actue
    
    virtual void accion() = 0;
};

class Semaforo : public element
{
    private:
    int offset;
    int *patron;
    int nPatron;

    public:
    //sets y gets de la direccion de Offset
    void setOffset(int a)
    {
        offset = a;
    }

    int getOffset()
    {
        return offset;
    }

    //sets y gets de la direccion de nPatron
    void setPatron(int a, int i)
    {
        patron[i] = a;
    }

    int getPatron(int i)
    {
        return patron[i];
    }

    void crearPatron(int n)
    {
        patron = new int[n];
    }

    void setPatronElement(int i, int value)
    {
        if (i >= 0 && i < nPatron)
        {
            patron[i] = value;
        }
    }
    
    int getPatronElement(int i)
    {
        if (i >= 0 && i < nPatron)
        {
            return patron[i];
        }
        return -1; // Valor por defecto si el índice es inválido
    }

    int getnPatron()
    {
        return nPatron;
    }

    void setnPatron(int n)
    {
        nPatron = n;
    }

    void accion()
    {
        cambiarColor();
        if(patron[offset] == 1)
        {
            mapMask[geti()][getj()] = 1;
        }
        else
        {
            // Lógica para el semáforo en rojo
            // Aquí se podría implementar la lógica de detención de vehículos, etc.
            mapMask[geti()][getj()] = 0; // Marca la posición del semáforo en rojo
        }
        
    }

    void cambiarColor()
    {
        // Lógica para cambiar el color del semáforo
        // dependiendo del patrón y el offset.

        if(offset == nPatron)
        {
            offset = 0; // Reiniciar el offset si se alcanza el final del patrón
        }
        else
        {
            offset++;
        }
    }
};

class vehicle : public element
{

    private:
    int diri, dirj, V, Tv, Td, Cd, A;

    //Tv = Tiempo vehiculo en movimiento
    //Td = Tiempo vehiculo detenido
    //Cd = numero de Cambios de direccion
    //A = si se choco el vehiculo deberia ser 1;
    //V = Tipo del vehiculo
    //id = id del vehiculo

    public:
    //sets y gets de la direccion de vehiculo

    vehicle()
    {
        seti(0);
        setj(0);
        seteType(NONE);
        setid(0);
        diri = 0;
        dirj = 0;
        Tv = 0;
        Td = 0;
        Cd = 0;
        A = 0; // Marca el vehículo como activo inicialmente
    }

    void setdiri (int a)
    {
        diri = a;
    }

    int getdiri ()
    {
        return diri;
    }

    void setdirj (int a)
    {
        dirj = a;
    }

    int getdirj ()
    {
        return dirj;
    }

    //sets y gets del Tv del vehiculo
    void setTv (int a)
    {
        Tv = a;
    }

    int getTv ()
    {
        return Tv;
    }

    //sets y gets del Td del vehiculo
    void setTd (int a)
    {
        Td = a;
    }

    int getTd ()
    {
        return Td;
    }

    //sets y gets del Cd del vehiculo
    void setCd (int a)
    {
        Cd = a;
    }

    int getCd ()
    {
        return Cd;
    }

    //sets y gets del A del vehiculo
    void setA (int a)
    {
        A = a;
    }

    int getA ()
    {
        return A;
    }

    void moverse(int x, int y)
    {
        cout << "Moviendo vehiculo: "<< getid() << " a la posicion: (" << x << ", " << y << ")," << "con direccion (" << diri << ", " << dirj << "),"  << "Esta activado: " << A << endl;
        
        if( A >= 1 ) // Si el vehículo ha chocado, no se mueve
        {
            Td = Td + 1;
            return;
        }

        if( x >= 0 && x < fila && y >= 0 && y < columna)
        {
            if(mapMask[x][y] == 1) // Verifica si la posición está libre
            {
                mapMask[geti()][getj()] = 1; // Marca la posición actual como libre
                mapMask[x][y] = geteType(); // Marca la nueva posición como ocupada
                seti(x);
                setj(y);
                Tv = Tv + 1; // Incrementa el tiempo de movimiento
            }
            else
            {
                if(map[x][y] == 'S') 
                {
                    Td = Td + 1; // Incrementa el tiempo detenido
                    return;
                }

                if(mapMask[x][y] == 3 && geteType() == AUTO) 
                {
                    dirj = dirj * -1;
                    Cd = Cd + 1; // Incrementa el contador de cambios de dirección
                    Td = Td + 1; // Incrementa el tiempo detenido
                }

                if(mapMask[x][y] == 2 && geteType() == AUTO) 
                {
                    cout << "Colisión detectada entre carro y moto en posición: (" << x << ", " << y << ")" << endl;
                    disableVehicle(geti(), getj(), x, y);
                    Td = Td + 1; // Incrementa el tiempo detenido
                }

                if(mapMask[x][y] == 3 && geteType() == MOTO) 
                {
                    cout << "Colisión detectada entre carro y moto en posición: (" << x << ", " << y << ")" << endl;
                    disableVehicle(geti(), getj(), x, y);
                    Td = Td + 1; // Incrementa el tiempo detenido
                }

                if(mapMask[x][y] == 2 && geteType() == MOTO) 
                {
                    diri = diri * -1;
                    Cd = Cd + 1; // Incrementa el contador de cambios de dirección
                    Td = Td + 1; // Incrementa el tiempo detenido
                }
            }
        }
        else
        {
            if(geteType() == AUTO) 
            {
                dirj = dirj * -1;
                Cd = Cd + 1; // Incrementa el contador de cambios de dirección
                Td = Td + 1; // Incrementa el tiempo detenido
            }

            if(geteType() == MOTO) 
            {
                diri = diri * -1;
                Cd = Cd + 1; // Incrementa el contador de cambios de dirección
                Td = Td + 1; // Incrementa el tiempo detenido
            }
        }
    }

};

class moto : public vehicle
{
    public:
    void accion()
    {
        int x = geti() + getdiri();
        int y = getj() + getdirj();
        moverse(x, y);
    }
};

class carro : public vehicle
{
    public:
    void accion()
    {
        int x = geti() + getdiri();
        int y = getj() + getdirj();
        moverse(x, y);
    }
};

carro *carros;
moto *motos;
Semaforo *semaforos;

void leerArchivo(char* mapFileName, char* traficLights)
{  
    ifstream ifs;

    int carrosIndex = 0, motosIndex = 0, semaforosIndex = 0;
    string buff;

    ifs.open(mapFileName, std::ifstream::in);

    if (!ifs.is_open())
    {
        cout << "Error al abrir el archivo de salida." << endl;
        return;
    }

    // Aquí se lee el contenido del mapa y los semáforos
    
    if (ifs.is_open()) 
    {
        cout << "Se abrio el archivo: " << mapFileName << endl;
        ifs >> buff;
        fila = stoi (buff.c_str(), 0, 10);
        ifs >> buff;
        columna = stoi (buff.c_str(), 0, 10);
        ifs >> buff;
        nCiclos = stoi (buff.c_str(), 0, 10);

        map = new char*[fila];
        mapMask = new int*[fila];

        for(int i = 0; i < fila; i++)
        {
            map[i] = new char[columna];
            mapMask[i] = new int[columna];
        }

        for(int i = 0; i < fila; i++)
        {
            for(int j = 0; j < columna; j++)
            {
                ifs >> buff;
                map[i][j] = buff[0];
                mapMask[i][j] = 1; 

                if (buff == "S")
                {
                    semaforosCount++;
                }
                else if (buff == "M")
                {
                    motosCount++;
                }
                else if (buff == "A")
                {
                    carrosCount++;
                }
            }
        }

        ifs.close();
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }

    cout << "Mapa Cargado" << endl;

    carros = new carro[carrosCount];
    motos = new moto[motosCount];
    semaforos = new Semaforo[semaforosCount];

    for(int i = 0; i < fila; i++)
    {
        for(int j = 0; j < columna; j++)
        {

            if (map[i][j] == 'S')
            {
                cout << "cargando semaforo en: " << i << ", " << j << endl;
                semaforos[semaforosIndex].seti(i);
                semaforos[semaforosIndex].setj(j);
                semaforos[semaforosIndex].seteType(SEMAFORO);
                semaforos[semaforosIndex].setid(ID);
                semaforosIndex++;
                ID++;
            }
            if (map[i][j] == 'M')
            {
                cout << "cargando moto en: " << i << ", " << j << endl;
                motos[motosIndex].seti(i);
                motos[motosIndex].setj(j);
                motos[motosIndex].seteType(MOTO);
                motos[motosIndex].setid(ID);
                motos[motosIndex].setdiri(-1); 
                motos[motosIndex].setdirj(0);
                motos[motosIndex].setCd(0); 
                motos[motosIndex].setTv(0); // Tiempo de movimiento inicial
                motos[motosIndex].setTd(0); // Tiempo detenido inicial
                motos[motosIndex].setCd(0);
                motos[motosIndex].setA(0); // Marca la moto como activa inicialmente
                mapMask[i][j] = 2; // Marca la posición de la moto en el mapa
                motosIndex++;
                ID++;
            }
            if (map[i][j] == 'A')
            {
                cout << "cargando carro en: " << i << ", " << j << endl;
                carros[carrosIndex].seti(i);
                carros[carrosIndex].setj(j);
                carros[carrosIndex].seteType(AUTO);
                carros[carrosIndex].setid(ID);
                carros[carrosIndex].setdiri(0);
                carros[carrosIndex].setdirj(-1);
                carros[carrosIndex].setTv(0); // Tiempo de movimiento inicial
                carros[carrosIndex].setTd(0); // Tiempo detenido inicial
                carros[carrosIndex].setCd(0); // Cambios de dirección inicial
                carros[carrosIndex].setA(0); // Marca la moto como activa inicialmente
                mapMask[i][j] = 3; // Marca la posición del carro en el mapa
                carrosIndex++;
                ID++;
            }
        }
    }

    ifs.open(traficLights, std::ifstream::in);

    if (!ifs.is_open())
    {
        cout << "Error al abrir el archivo de salida." << endl;
        return;
    }

    // Aquí se lee el contenido del mapa y los semáforos
    buff = "";
    if (ifs.is_open()) 
    {
        cout << "Se abrio el archivo: " << traficLights << endl;
        if( semaforosCount == 0 )
        {
            cout << "No se encontraron semáforos en el archivo." << endl;
        }
        else
        {
            for(int i = 0; i < semaforosCount; i++)
            {
                ifs >> buff;
                cout << buff << endl;
                semaforos[i].seti(stoi(buff.c_str(), 0, 10));
                ifs >> buff;
                semaforos[i].setj(stoi(buff.c_str(), 0, 10));
                ifs >> buff;
                semaforos[i].setnPatron(stoi(buff.c_str(), 0, 10));
                ifs >> buff;
                semaforos[i].setOffset(stoi(buff.c_str(), 0, 10));
                semaforos[i].crearPatron(semaforos[i].getnPatron());

                for(int j = 0; j < semaforos[i].getnPatron(); j++)
                {
                    ifs >> buff;
                    semaforos[i].setPatronElement(j, stoi(buff.c_str(), 0, 10));
                }
                
            }
        }
        ifs.close();

        for(int j = 0; j < semaforosCount; j++)
        {
            mapMask[semaforos[j].geti()][semaforos[j].getj()] = semaforos[j].getPatronElement(semaforos[j].getOffset()); // Marca la posición del semáforo en el mapa
        }

    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }

    cout << "Archivos Cargados" << endl;
    
}

void disableVehicle(int i1, int j1, int i2, int j2)
{
    // Lógica para deshabilitar un vehículo
    // Aquí se podría implementar la lógica de choque, por ejemplo, marcando el vehículo como inactivo o eliminándolo del mapa

    for(int j = 0; j < motosCount; j++)
    {
        if (motos[j].geti() == i1 && motos[j].getj() == j1)
        {
            cout << "Moto en posición (" << i1 << ", " << j1 << ") deshabilitada." << endl;
            motos[j].setA(cicloActual+1); // Marca la moto como inactiva
            motos[j].setdiri(0); // Reinicia el tiempo de movimiento
            motos[j].setdirj(0); // Reinicia la dirección
        }

        if (motos[j].geti() == i2 && motos[j].getj() == j2)
        {
            cout << "Moto en posición (" << i2 << ", " << j2 << ") deshabilitada." << endl;
            motos[j].setA(cicloActual+1); // Marca la moto como inactiva
            motos[j].setdiri(0); // Reinicia el tiempo de movimiento
            motos[j].setdirj(0); // Reinicia la dirección
        }
    }

    for(int j = 0; j < carrosCount; j++)
    {
        if (carros[j].geti() == i1 && carros[j].getj() == j1)
        {
            cout << "Carro en posición (" << i1 << ", " << j1 << ") deshabilitado." << endl;
            carros[j].setA(cicloActual+1); // Marca el carro como inactivo
            carros[j].setdiri(0); // Reinicia el tiempo de movimiento
            carros[j].setdirj(0); // Reinicia la dirección
        }

        if (carros[j].geti() == i2 && carros[j].getj() == j2)
        {
            cout << "Carro en posición (" << i2 << ", " << j2 << ") deshabilitado." << endl;
            carros[j].setA(cicloActual+1); // Marca el carro como inactivo
            carros[j].setdiri(0); // Reinicia el tiempo de movimiento
            carros[j].setdirj(0); // Reinicia la dirección
        }
    }
    
}

void escribirArchivo(char* mapFileName, char* statsFileName)
{
    ofstream ofs;

    char** finalMap;
    finalMap = new char*[fila];
    for(int i = 0; i < fila; i++)
    {
        finalMap[i] = new char[columna];
    }


    for(int i = 0; i < fila; i++)
    {
        for(int j = 0; j < columna; j++)
        {
            finalMap[i][j] = 'C';
        }
    }

    for(int i = 0; i < fila; i++)
    {
        for(int j = 0; j < columna; j++)
        {
            for(int k = 0; k < motosCount; k++)
            {
                if (motos[k].geti() == i && motos[k].getj() == j)
                {
                    finalMap[i][j] = 'M';
                }
            }

            for(int k = 0; k < carrosCount; k++)
            {
                if (carros[k].geti() == i && carros[k].getj() == j)
                {
                    finalMap[i][j] = 'A';
                }
            }
        }
    }

    for(int i = 0; i < fila; i++)
    {
        for(int j = 0; j < columna; j++)
        {
            for(int k = 0; k < semaforosCount; k++)
            {
                if (semaforos[k].geti() == i && semaforos[k].getj() == j)
                {
                    finalMap[i][j] = 'S';
                }
            }
        }
    }

    for(int i = 0; i < fila; i++)
    {
        for(int j = 0; j < columna; j++)
        {
            cout << finalMap[i][j] << " ";
        }

        cout << endl;
    }

    ofs.open(mapFileName, std::ofstream::out);

    if (ofs.is_open())
    {
        for(int i = 0; i < fila; i++)
        {
            for(int j = 0; j < columna; j++)
            {
                ofs << finalMap[i][j] << " ";
            }
            ofs << endl;
        }
    }

    // Aquí se escribiría el contenido del mapa y los semáforos
    // Ejemplo:

    ofs.close();




    ofs.open(statsFileName, std::ofstream::out);

    if (ofs.is_open())
    {
        for(int k = 0; k < ID; k++)
        {
            for(int i = 0; i < motosCount; i++)
            {
                if (motos[i].getid() == k)
                {
                    ofs     << motos[i].getid() + 1 << " "
                            << "Moto" << " " 
                            << motos[i].getTv() << " "
                            << motos[i].getTd() << " "
                            << motos[i].getCd() << " "
                            << motos[i].getA()  << " " << endl;
                }
            }

            for(int i = 0; i < carrosCount; i++)
            {
                if (carros[i].getid() == k)
                {
                    ofs     << carros[i].getid() + 1 << " "
                            << "Auto" << " " 
                            << carros[i].getTv() << " "
                            << carros[i].getTd() << " "
                            << carros[i].getCd() << " "
                            << carros[i].getA()  << " " << endl;
                }
            }
        }
    }

    ofs.close();

}

void ejecutarAccion(int elemento)
{
    for(int i = 0; i < semaforosCount; i++)
    {
        if (semaforos[i].getid() == elemento)
        {
            cout << "Ejecutando accion para el elemento con ID: " << elemento << endl;
            semaforos[i].accion();
        }
    }

    for(int i = 0; i < motosCount; i++)
    {
        if (motos[i].getid() == elemento)
        {
            cout << "Ejecutando accion para el elemento con ID: " << elemento << endl;
            motos[i].accion();
        }
    }

    for(int i = 0; i < carrosCount; i++)
    {
        if (carros[i].getid() == elemento)
        {
            cout << "Ejecutando accion para el elemento con ID: " << elemento << endl;
            carros[i].accion();
        }
    }
}

void imprimirEstadisticas(int elemento)
{
    for(int i = 0; i < motosCount; i++)
    {
        if (motos[i].getid() == elemento)
        {
            cout    << motos[i].getid() + 1 << " "
                    << "Moto" << " " 
                    << motos[i].getTv() << " "
                    << motos[i].getTd() << " "
                    << motos[i].getCd() << " "
                    << motos[i].getA()  << " " << endl;
        }
    }

    for(int i = 0; i < carrosCount; i++)
    {
        if (carros[i].getid() == elemento)
        {
            cout    << carros[i].getid() + 1 << " "
                    << "Auto" << " " 
                    << carros[i].getTv() << " "
                    << carros[i].getTd() << " "
                    << carros[i].getCd() << " "
                    << carros[i].getA()  << " " << endl;
        }
    }
}



int main(int argc, char* argv[])
{

    leerArchivo(argv[1], argv[2]);
    
    for(int i = 0; i < fila; i++)
    {
        for(int j = 0; j < columna; j++)
        {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }

    for(cicloActual = 0; cicloActual < nCiclos; cicloActual++)
    {
        cout << "Ciclo: " << cicloActual + 1 << endl;
        for(int j = 0; j < ID; j++)
        {
            ejecutarAccion(j);
        }
    }

    escribirArchivo((char*)"log_simulacion.out", (char*)"estadisticas.out");

}