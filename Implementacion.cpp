#include <iostream>

using namespace std;

enum elementType
{
	SEMAFORO,
	MOTO,
	AUTO,
};

class element
{
    private:
    int i, j, eType;

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

    void Patron(int n)
    {
        patron = new int[n];
        nPatron = n;
    }
    

};

class vehicle : public element
{

    private:
    int dir, id, V, Tv, Td, Cd, A;

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

    //sets y gets del id del vehiculo
    void setid (int a)
    {
        id = a;
    }

    int getid ()
    {
        return id;
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
    void display()
    {

    }

};

class carro : public vehicle
{

};


int main(int argc, char* argv[])
{



}