#include <iostream>
#include <fstream>
#include <string>

using namespace std;

char** map;
int** mapMask;
int nCiclos = 0;
int fila = 0, columna = 0;

enum elementType
{
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

    int geti (int a)
    {
        return i;
    }

    //sets y gets de j
    void setj (int a)
    {
        j = a;
    }

    int getj (int a)
    {
        return j;
    }

    //sets y gets elementType
    void seteType (int a)
    {
        eType = a;
    }

    int geteType (int a)
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
        cout << "hello world" << endl;
    }
    

};

class vehicle : public element
{

    private:
    int dir, V, Tv, Td, Cd, A;

    //Tv = Tiempo vehiculo en movimiento
    //Td = Tiempo vehiculo detenido
    //Cd = numero de Cambios de direccion
    //A = si se choco el vehiculo deberia ser 1;
    //V = Tipo del vehiculo
    //id = id del vehiculo

    public:
    //sets y gets de la direccion de vehiculo
    void setdir (int a)
    {
        dir = a;
    }

    int getdir ()
    {
        return dir;
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



};

class moto : public vehicle
{
    public:
    void accion()
    {

    }

};

class carro : public vehicle
{
    public:
    void accion()
    {

    }
};

carro *carros;
moto *motos;
Semaforo *semaforos;

void leerArchivo(char* mapFileName, char* traficLights)
{  
    ifstream ifs;

    int carrosCount = 0, motosCount = 0, semaforosCount = 0, carrosIndex = 0, motosIndex = 0, semaforosIndex = 0;
    int ID = 0;
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
                mapMask[i][j] = 0; 

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

    carros = new carro[carrosCount];
    motos = new moto[motosCount];
    semaforos = new Semaforo[semaforosCount];

    for(int i = 0; i < fila; i++)
    {
        for(int j = 0; j < columna; j++)
        {

            if (map[i][j] == 'S')
            {
                semaforos[semaforosIndex].seti(i);
                semaforos[semaforosIndex].setj(j);
                semaforos[semaforosIndex].seteType(SEMAFORO);
                semaforos[semaforosIndex].setid(ID);
                semaforosIndex++;
                ID++;
            }
            if (map[i][j] == 'M')
            {
                motos[motosIndex].seti(i);
                motos[motosIndex].setj(j);
                motos[motosIndex].seteType(MOTO);
                motos[motosIndex].setid(ID);
                motosIndex++;
                ID++;
            }
            if (map[i][j] == 'A')
            {
                carros[carrosIndex].seti(i);
                carros[carrosIndex].setj(j);
                carros[carrosIndex].seteType(AUTO);
                carros[carrosIndex].setid(ID);
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
    
    if (ifs.is_open()) 
    {
        for(int i = 0; i < semaforosCount; i++)
        {
            ifs >> buff;
            semaforos[i].seti(stoi(buff.c_str(), 0, 10));
            ifs >> buff;
            semaforos[i].setj(stoi(buff.c_str(), 0, 10));
            ifs >> buff;
            semaforos[i].setnPatron(stoi(buff.c_str(), 0, 10));
            ifs >> buff;
            semaforos[i].setOffset(stoi(buff.c_str(), 0, 10));

            for(int j = 0; j < semaforos[i].getnPatron(); j++)
            {
                ifs >> buff;
                semaforos[i].setPatronElement(j, stoi(buff.c_str(), 0, 10));
            }
        }

        ifs.close();
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
    
}

void escribirArchivo(char* mapFileName)
{
    ofstream ofs;

    ofs.open(mapFileName, std::ofstream::out);

    if (!ofs.is_open())
    {
        cout << "Error al abrir el archivo de salida." << endl;
        return;
    }

    // Aquí se escribiría el contenido del mapa y los semáforos
    // Ejemplo:
    ofs << "Contenido del mapa y semáforos" << endl;

    ofs.close();
}

int main(int argc, char* argv[])
{

    cout << argc << endl;
    cout << argv[1] << endl;
    cout << argv[2] << endl;

    leerArchivo(argv[1], argv[2]);
    
    for(int i = 0; i < fila; i++)
    {
        for(int j = 0; j < columna; j++)
        {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }


}