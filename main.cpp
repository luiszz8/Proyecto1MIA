#include <iostream>
#include "fstream"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include <time.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


using namespace std;



typedef struct{
    int i_uid=-1;
    int I_gid=-1;
    int i_s;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
    int i_block[15]= {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    char i_type;
    int i_perm;
}Inodo;

typedef struct{
    char tipo_operacion[10] = "";
    char tipo = '1';
    char path[40] = "";
    char contenido[100] = ""; // Validar cuando espacio necesitan.
    char log_fecha[20] = "";
    int size = 0;
}Journaling; //typedef

typedef struct{
    int b_pointers[16]= {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
}BloquePuntero; //typedef

typedef struct{
    char b_name[12];
    int b_inodo=-1;
}content;

typedef struct{
    content b_content[4];
}CarpetaBloque;

typedef struct{
    char b_content[64];
}ArchivoBloque;

typedef struct{
    int s_filesystem_type;
    int  s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_s;
    int s_block_s;
    int s_firts_ino=0;
    int s_first_blo=0;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
}SuperBloque;

typedef struct{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_s;
    char part_name[16];
}Partition;

typedef struct{
    Partition particion;
    string id;
    string ruta;
    bool montada=false;
}PartitionMontada;

typedef struct{
    string idPart;
    string user;
    string pass;
    string grupo;
    bool active=false;
    bool admin=false;
    PartitionMontada actual;
}usuarioLogueado;

typedef struct{
    int IdNum ;
    int comienzo;
    int fin;
    int anterior;
    int siguiente;
}Traslado;



typedef struct{
    char part_status;
    char part_fit;
    int part_start;
    int part_s;
    int part_next;
    char part_name[16];
}EBR;

typedef struct{
    int mbr_tamano;
    time_t mbr_fecha_creacion ;
    int mbr_dsk_signature;
    char dsk_fit;
    Partition mbr_partition_1;
    Partition mbr_partition_2;
    Partition mbr_partition_3;
    Partition mbr_partition_4;
}MBR;

typedef struct{
    //s
    int tamanio;
    //f
    char ajuste;
    //u
    string unidades;
    //path
    string ruta;
}MkDiskEstruc;

typedef struct{
    //s
    int tamanio;
    //f
    string ajuste;
    //u
    string unidades;
    //path
    string ruta;
    //tipo
    string tipo;
    //delete
    bool borrar;
    //name
    string name;
    //add
    int add;
}Particion;

void mkdisk(string);
void rmdisk(string);
void fdisk(string);
void mount(string);
void unmount(string);
void mkfs(string);
MBR LeerDisco(string ruta);
string split(string s, string del);
bool Exist(string path);
void crearDirectorios(string);
void ejecutar(string);
int LeerParticionLogica(string ruta);
void ReporteDisco(vector<string>);
MBR AgregarParticionN(MBR mbr, Partition p, vector<Traslado> t, vector<Partition> ps, int u,int inicio);
class ListaGenericaOrdenada;
ListaGenericaOrdenada *lista;
int InicioGeneral=0;
void CrearParticionLogica(Partition partition, Partition ep, string p);
vector<PartitionMontada> particionesMontadas;
vector<EBR> ObtenerLogicas(Partition partition, string p);
void dks(string p, string id);
bool login(string);
usuarioLogueado userActive;
void tree(string p,string id);
void mkdir(string);
void mkgrp(string);
vector<string> splitArreglo(string str, char divisor);
void mkurs(string linea);
string Minuscula(string cadena);
void firstFit(int blockSize[], int m,
              int processSize[], int n);
MBR AgregarParticionNueva(MBR mbr, Partition p, vector<Traslado> t, vector<Partition> ps, int u);
void inicio();
MBR bestFitPartitton(MBR disco,vector<Traslado> t,vector<Partition> p,Partition nueva);
MBR worstFitPartitton(MBR disco,vector<Traslado> t,vector<Partition> p,Partition nueva);
int extendidaInicio=0;
void Rmbr(string p, string id);
void pausa();
void reporte(string linea);
void logout();

void reporte(string linea){
    size_t pos = linea.find(" ");
    string limpio = linea.substr (pos+1);
    //ruta
    string ruta=split(limpio,"-path");
    if(strcmp(limpio.c_str(),ruta.c_str())==0){
        cout <<  "El parametro path es obligatorio" << endl;
    }else{
        int end = ruta.find(" ");
        string comillas=ruta.substr(2, end-2);
        string comilla="\"";
        //cout <<  ruta.substr(2, 1) << endl;
        //cout <<  comilla << endl;
        if (strcmp(ruta.substr(2, 1).c_str(),comilla.c_str())==0){
            cout <<  "Tiene Comillas" << endl;
            int tamanio=ruta.length();
            int lugarFin=ruta.substr(3, tamanio).find(comilla);
            string rutaNueva=ruta.substr(3, lugarFin);
            //cout <<  rutaNueva << endl;
            //cout <<  lugarFin << endl;
            ruta=rutaNueva;
        }else{
            cout <<  "No tiene Comillas" << endl;
            //cout <<  ruta.substr(2, end-2) << endl;
            ruta=ruta.substr(2, end-2);
        }

        cout <<  "La ruta es" << endl;
        cout <<  ruta << endl;
    }

    //-name
    string name=split(limpio,"-name");
    if(strcmp(limpio.c_str(),name.c_str())==0){
        cout <<  "No esta el parametro name" << endl;
        //part.name='F';
    }else{
        int end = name.find(" ");
        string comillas=name.substr(2, end-2);
        string comilla="\"";
        //cout <<  ruta.substr(2, 1) << endl;
        //cout <<  comilla << endl;
        if (strcmp(name.substr(2, 1).c_str(),comilla.c_str())==0){
            cout <<  "Tiene Comillas" << endl;
            int tamanio=name.length();
            int lugarFin=name.substr(3, tamanio).find(comilla);
            string rutaNueva=name.substr(3, lugarFin);
            //cout <<  rutaNueva << endl;
            //cout <<  lugarFin << endl;
            name=rutaNueva;
        }else{
            cout <<  "No tiene Comillas" << endl;
            //cout <<  ruta.substr(2, end-2) << endl;
            name=name.substr(2, end-2);
        }

        cout <<  "La name es" << endl;
        cout <<  name << endl;
    }

    //-id
    string f=split(limpio,"-id");
    string id;
    if(strcmp(limpio.c_str(),f.c_str())==0){
        cout <<  "No esta el parametro id" << endl;
        return;
    }else{
        int end = f.find(" ");

        id=f.substr(2, end-2);

        cout <<  "El ID es" << endl;
        cout <<  id << endl;
    }

    if(name=="mbr"){
        Rmbr(ruta,id);
    }else if(name=="disk"){
        dks(ruta,id);
    }else if(name=="tree"){
        tree(ruta,id);
    }
}

void mkdisk(string linea){
    MkDiskEstruc disk;
    MBR disco=MBR ();
    size_t pos = linea.find(" ");
    string limpio = linea.substr (pos+1);
    bool megas= false;
    //-s
    string s=split(limpio,"-s");
    if(strcmp(limpio.c_str(),s.c_str())==0){
        cout <<  "El parametro -s es requerido" << endl;
        return;
    }else{
        int end = s.find(" ");
        disk.tamanio=stoi(s.substr(2, end-2));
        cout <<  "Tamaño disco" << endl;
        cout <<  disk.tamanio << endl;
        if(disk.tamanio<1){
            cout <<  "Tamaño invalido" << endl;
            return;
        }
        disco.mbr_tamano=disk.tamanio;
    }
    //Tiempo
    time_t result = time(nullptr);
    disco.mbr_fecha_creacion=result;
    //-f
    string f=split(limpio,"-f");
    if(strcmp(limpio.c_str(),f.c_str())==0){
        cout <<  "No esta el parametro f" << endl;
        disk.ajuste='f';
        disco.dsk_fit='f';
    }else{
        int end = f.find(" ");
        //disk.ajuste=f.substr(2, end-2);
        string bf="bf";
        string ff="ff";
        string wf="wf";
        if(strcmp(f.substr(2, end-2).c_str(),bf.c_str())==0){
            //cout <<  "El ajuste BF" << endl;
            disk.ajuste='b';
            disco.dsk_fit='b';
        }else if(strcmp(f.substr(2, end-2).c_str(),ff.c_str())==0){
            //cout <<  "El ajuste FF" << endl;
            disk.ajuste='f';
            disco.dsk_fit='f';
        }else if(strcmp(f.substr(2, end-2).c_str(),wf.c_str())==0){
            //cout <<  "El ajuste WF" << endl;
            disk.ajuste='w';
            disco.dsk_fit='w';
        }else{
            cout <<  "El ajuste no es correcto" << endl;
            return;
        }
        cout <<  "El ajuste es" << endl;
        cout <<  disk.ajuste << endl;
    }

    //-u
    string u=split(limpio,"-u");
    if(strcmp(limpio.c_str(),u.c_str())==0){
        cout <<  "No esta el parametro u" << endl;
        disk.unidades="m";
        megas=true;
    }else{
        int end = u.find(" ");
        cout <<  "La unidad es" << endl;
        disk.unidades=u.substr(2, end-2);
        string m="m";
        string k="k";
        if(strcmp(disk.unidades.c_str(),m.c_str())==0){
            //cout <<  "El ajuste BF" << endl;exec -path->/home/luis/Escritorio/Archvio Entrada/Archivos de Entrada/Parte 2/parte2.mia
            megas=true;
        }else if(strcmp(disk.unidades.c_str(),k.c_str())==0){
            //cout <<  "El ajuste FF" << endl;exec -path->/home/luis/Escritorio/Archvio Entrada/Archivos de Entrada/Parte 1/parte1.mia
        }else{
            cout <<  "La unidad es incorrecta" << endl;
            return;
        }
    }
    if(megas){
        disco.mbr_tamano=disco.mbr_tamano*1024*1024;
    }else{
        disco.mbr_tamano=disco.mbr_tamano*1024;
    }
    cout <<  disk.unidades << endl;
    //ruta
    string ruta=split(limpio,"-path");
    if(strcmp(limpio.c_str(),ruta.c_str())==0){
        cout <<  "El parametro path es obligatorio" << endl;
    }else{
        int end = ruta.find(" ");
        string comillas=ruta.substr(2, end-2);
        string comilla="\"";
        //cout <<  ruta.substr(2, 1) << endl;
        //cout <<  comilla << endl;
        if (strcmp(ruta.substr(2, 1).c_str(),comilla.c_str())==0){
            cout <<  "Tiene Comillas" << endl;
            int tamanio=ruta.length();
            int lugarFin=ruta.substr(3, tamanio).find(comilla);
            string rutaNueva=ruta.substr(3, lugarFin);
            //cout <<  rutaNueva << endl;
            //cout <<  lugarFin << endl;
            ruta=rutaNueva;
        }else{
            cout <<  "No tiene Comillas" << endl;
            //cout <<  ruta.substr(2, end-2) << endl;
            ruta=ruta.substr(2, end-2);
        }
        disk.ruta=ruta;
        cout <<  "La ruta es" << endl;
        cout <<  disk.ruta << endl;
        disco.mbr_dsk_signature=rand();
        if(!Exist(disk.ruta)){
            FILE *file;
            file = fopen(disk.ruta.c_str(), "wb");
            if(file==0){
                cout << "archivo no encontrado"<<endl;
                int tamRuta=disk.ruta.length();
                int nombreTemporal=split(disk.ruta,"/").length();
                string directorios=disk.ruta.substr(0,tamRuta-nombreTemporal-1);
                crearDirectorios(directorios);
                file = fopen(disk.ruta.c_str(), "wb");
            }else{
                cout << "archivo creado"<<endl;
            }
            if(megas){
                for(int i = 0; i < disk.tamanio*1024*1024; i++){
                    char c = '\0';
                    fwrite(&c, 1,1,file);
                }
            }else{
                for(int i = 0; i < disk.tamanio*1024; i++){
                    char c = '\0';
                    fwrite(&c, 1,1,file);
                }
            }

            fclose(file);
            cout << "archivo creado"<<endl;
        }

        FILE *file;
        file = fopen(disk.ruta.c_str(), "rb+");
        int posicion=0;
        int existe=1;
        bool primero=false;
        MBR aux;
        fseek(file, 0, SEEK_SET);
        int contador=0;
        //while(existe==1){
        //    fread(&aux, sizeof(MBR), 1, file);
        //    if (aux.mbr_dsk_signature==0){
        //        existe=0;
        //    }
        //    contador++;
        //}
        //fseek(file, ftell(file)-sizeof(MBR), SEEK_SET);
        fwrite(&disco, sizeof(MBR), 1, file);

        fclose(file);
        LeerDisco(disk.ruta);
    }

}

string split(string s, string del)
{
    int start = 0;
    int end = s.find(del);
    while (end != -1) {
        //cout << s.substr(start, end - start) << endl;
        start = end + del.size();
        end = s.find(del, start);
    }
    return s.substr(start, end - start);
}

bool Exist(string path){
    FILE *archivo;

    if((archivo = fopen(path.c_str(), "r"))){
        fclose(archivo);
        cout <<  "Existe" << endl;
        return true;
    }
    cout <<  "No Existe" << endl;
    return false;
}

void rmdisk(string linea){
    cout <<  "Desea Eliminar S/N " << endl;
    string seguro;
    getline(cin,seguro);
    if(seguro=="N"){
        return;
    }
    cout <<  "Salio pausa en pausa " << endl;
    size_t pos = linea.find(" ");
    string limpio = linea.substr (pos+1);

    //ruta
    string ruta=split(limpio,"-path");
    if(strcmp(limpio.c_str(),ruta.c_str())==0){
        cout <<  "El parametro path es obligatorio" << endl;
    }else{
        int end = ruta.find(" ");
        string comillas=ruta.substr(2, end-2);
        string comilla="\"";
        //cout <<  ruta.substr(2, 1) << endl;
        //cout <<  comilla << endl;
        if (strcmp(ruta.substr(2, 1).c_str(),comilla.c_str())==0){
            cout <<  "Tiene Comillas" << endl;
            int tamanio=ruta.length();
            int lugarFin=ruta.substr(3, tamanio).find(comilla);
            string rutaNueva=ruta.substr(3, lugarFin);
            //cout <<  rutaNueva << endl;
            //cout <<  lugarFin << endl;
            ruta=rutaNueva;
        }else{
            cout <<  "No tiene Comillas" << endl;
            //cout <<  ruta.substr(2, end-2) << endl;
            ruta=ruta.substr(2, end-2);
        }

        cout <<  "La ruta a borrar es" << endl;
        cout <<  ruta << endl;
        if(Exist(ruta)){
            remove(ruta.c_str());
            cout <<  "Disco eliminado" << endl;
        }
    }
};

void fdisk(string linea){
    try {
        Particion part;
        size_t pos = linea.find(" ");
        string limpio = linea.substr (pos+1);
        bool agregar= false;
        bool eliminarB= false;
        bool crear= false;

        string temporal=limpio;
        bool primerEspacio=true;
        int existeAdd=limpio.find("-add");
        int n1=0;
        int n2=0;
        int n3=0;
        if(existeAdd>-1){
            n1=existeAdd;
        }else{
            n1=100000;
        }
        int existeDelete=limpio.find("-delete");
        if(existeDelete>-1){
            n2=existeDelete;
        }else{
            n2=300000;
        }
        int existeS=limpio.find("-s");
        if(existeS>-1){
            n3=existeS;
        }else{
            n3=400000;
        }

        if(existeAdd==-1 && existeDelete==-1 && existeS==-1){
            cout <<  "No están los parametros necesarios" << endl;
            return;
        }
        int mayor=0;
        int menor=0;
        if(n1>n2 && n2>n3){mayor = n1; menor = n3;}

        if(n1>n3 && n3>n2){mayor = n1; menor = n2;}

        if(n2>n3 && n3>n1){mayor = n2; menor = n1;}

        if(n2>n1 && n1>n3){mayor = n2; menor = n3;}

        if(n3>n2 && n2>n1){mayor = n3; menor = n1;}

        if(n3>n1 && n1>n2){mayor = n3; menor = n2;}
        if(menor==existeS){
            crear=true;
        }else if(menor==existeAdd){
            agregar=true;
        }else if(menor==existeDelete){
            eliminarB=true;
        }




        //-s
        string s=split(limpio,"-s");
        if(strcmp(limpio.c_str(),s.c_str())==0){
            cout <<  "El parametro -s no esta" << endl;
        }else{
            int end = s.find(" ");
            part.tamanio=stoi(s.substr(2, end-2));
            cout <<  "Tamaño disco" << endl;
            cout <<  part.tamanio << endl;
            if(part.tamanio<1){
                cout <<  "Tamaño invalido" << endl;
                return;
            }
        }

        //-u
        bool megas=false;
        bool kil=false;
        string u=split(limpio,"-u");
        if(strcmp(limpio.c_str(),u.c_str())==0){
            cout <<  "No esta el parametro u" << endl;
            part.unidades="k";
            part.tamanio=part.tamanio*1024;
        }else{
            int end = u.find(" ");
            cout <<  "La unidad es" << endl;
            part.unidades=u.substr(2, end-2);
            string m="m";
            string k="k";
            string b="b";
            if(strcmp(part.unidades.c_str(),m.c_str())==0){
                //cout <<  "El ajuste BF" << endl;
                //megas=true;
                part.tamanio=part.tamanio*1024*1024;
                megas=true;
            }else if(strcmp(part.unidades.c_str(),k.c_str())==0){
                //cout <<  "El ajuste FF" << endl;
                part.tamanio=part.tamanio*1024;
                kil=true;
            }else if(strcmp(part.unidades.c_str(),b.c_str())==0){
                //cout <<  "El ajuste FF" << endl;
            }else{
                cout <<  "La unidad es incorrecta" << endl;
                return;
            }
            cout <<  part.unidades << endl;
        }

        //-add
        string add=split(limpio,"-add");
        if(strcmp(limpio.c_str(),add.c_str())==0){
            cout <<  "El parametro -add no esta" << endl;
        }else{
            int end = add.find(" ");
            part.add=stoi(add.substr(2, end-2));
            cout <<  "ADD" << endl;
            if(megas){
                part.add=part.add*1024;
            }else if(kil){
                part.add=part.add;
            }

            cout << part.add << endl;
        }
        //ruta
        string ruta=split(limpio,"-path");
        if(strcmp(limpio.c_str(),ruta.c_str())==0){
            cout <<  "El parametro path es obligatorio" << endl;
        }else{
            int end = ruta.find(" ");
            string comillas=ruta.substr(2, end-2);
            string comilla="\"";
            //cout <<  ruta.substr(2, 1) << endl;
            //cout <<  comilla << endl;
            if (strcmp(ruta.substr(2, 1).c_str(),comilla.c_str())==0){
                cout <<  "Tiene Comillas" << endl;
                int tamanio=ruta.length();
                int lugarFin=ruta.substr(3, tamanio).find(comilla);
                string rutaNueva=ruta.substr(3, lugarFin);
                //cout <<  rutaNueva << endl;
                //cout <<  lugarFin << endl;
                ruta=rutaNueva;
            }else{
                cout <<  "No tiene Comillas" << endl;
                //cout <<  ruta.substr(2, end-2) << endl;
                ruta=ruta.substr(2, end-2);
            }
            part.ruta=ruta;
            cout <<  "La ruta es" << endl;
            cout <<  part.ruta << endl;

        }
        //-t
        string t=split(limpio,"-t");
        if(strcmp(limpio.c_str(),t.c_str())==0){
            cout <<  "No esta el parametro t" << endl;
            part.tipo='p';
        }else{
            int end = t.find(" ");
            //disk.ajuste=f.substr(2, end-2);
            string primaria="p";
            string extendida="e";
            string logica="l";
            if(strcmp(t.substr(2, end-2).c_str(),primaria.c_str())==0){
                //cout <<  "El ajuste BF" << endl;
                part.tipo='p';
            }else if(strcmp(t.substr(2, end-2).c_str(),extendida.c_str())==0){
                //cout <<  "El ajuste FF" << endl;
                part.tipo='e';
            }else if(strcmp(t.substr(2, end-2).c_str(),logica.c_str())==0){
                //cout <<  "El ajuste WF" << endl;
                part.tipo='l';
            }else{
                cout <<  "El tipo no es correcto" << endl;
                return;
            }
            cout <<  "El tipo es" << endl;
            cout <<  part.tipo << endl;
        }

        //-f
        string f=split(limpio,"-f");
        if(strcmp(limpio.c_str(),f.c_str())==0){
            cout <<  "No esta el parametro f" << endl;
            part.ajuste='f';
        }else{
            int end = f.find(" ");
            part.ajuste=f.substr(2, end-2);
            string bf="bestfit";
            string ff="firstfit";
            string wf="worstfit";
            if(strcmp(f.substr(2, end-2).c_str(),bf.c_str())==0){
                //cout <<  "El ajuste BF" << endl;
                part.ajuste="b";
            }else if(strcmp(f.substr(2, end-2).c_str(),ff.c_str())==0){
                //cout <<  "El ajuste FF" << endl;
                part.ajuste="f";
            }else if(strcmp(f.substr(2, end-2).c_str(),wf.c_str())==0){
                //cout <<  "El ajuste WF" << endl;
                part.ajuste="w";
            }else{
                cout <<  "El ajuste no es correcto" << endl;
                return;
            }
            cout <<  "El ajuste es" << endl;
            cout <<  part.ajuste << endl;
        }

        //-delete
        string eliminar=split(limpio,"-delete");
        if(strcmp(limpio.c_str(),eliminar.c_str())==0){
            cout <<  "No esta el parametro delete" << endl;
        }else{
            int end = eliminar.find(" ");
            //disk.ajuste=f.substr(2, end-2);
            string completo="full";
            if(strcmp(eliminar.substr(2, end-2).c_str(),completo.c_str())==0){
                //cout <<  "El ajuste BF" << endl;
                part.borrar=true;
            }else{
                cout <<  "El delete no es correcto" << endl;
                return;
            }
            cout <<  "El delete es" << endl;
            cout <<  part.borrar << endl;
        }

        //-name
        string name=split(limpio,"-name");
        if(strcmp(limpio.c_str(),name.c_str())==0){
            cout <<  "No esta el parametro name" << endl;
            //part.name='F';
        }else{
            int end = name.find(" ");
            string comillas=name.substr(2, end-2);
            string comilla="\"";
            //cout <<  ruta.substr(2, 1) << endl;
            //cout <<  comilla << endl;
            if (strcmp(name.substr(2, 1).c_str(),comilla.c_str())==0){
                cout <<  "Tiene Comillas" << endl;
                int tamanio=name.length();
                int lugarFin=name.substr(3, tamanio).find(comilla);
                string rutaNueva=name.substr(3, lugarFin);
                //cout <<  rutaNueva << endl;
                //cout <<  lugarFin << endl;
                name=rutaNueva;
            }else{
                cout <<  "No tiene Comillas" << endl;
                //cout <<  ruta.substr(2, end-2) << endl;
                name=name.substr(2, end-2);
            }
            part.name=name;
            cout <<  "La name es" << endl;
            cout <<  part.name << endl;
        }

        //Logica particiones
        if(Exist(part.ruta)){
            if(crear){
                MBR discoAux=LeerDisco(part.ruta);
                int tamParticiones=discoAux.mbr_partition_1.part_s+discoAux.mbr_partition_2.part_s+discoAux.mbr_partition_3.part_s+discoAux.mbr_partition_4.part_s;
                if(tamParticiones+part.tamanio>discoAux.mbr_tamano){
                    cout <<  "Tamaño no valida se desborda" << endl;
                    return;
                }
                if (part.tipo=="e"){
                    if (discoAux.mbr_partition_1.part_type=='e'||discoAux.mbr_partition_2.part_type=='e'||discoAux.mbr_partition_3.part_type=='e'||discoAux.mbr_partition_4.part_type=='e'){
                        cout <<  "Ya existe una partición extendida" << endl;
                        return;
                    }
                }
                if(part.tipo=="l"){
                    if (discoAux.mbr_partition_1.part_type=='e'||discoAux.mbr_partition_2.part_type=='e'||discoAux.mbr_partition_3.part_type=='e'||discoAux.mbr_partition_4.part_type=='e'){
                        int posicion=LeerParticionLogica(part.ruta);
                        FILE *file;
                        EBR logica;
                        logica.part_s=part.tamanio;
                        logica.part_status='n';
                        logica.part_next=-1;
                        logica.part_start=posicion;
                        strcpy(logica.part_name, part.name.substr(0,16).c_str());
                        logica.part_fit=part.ajuste.c_str()[0];
                        file = fopen(part.ruta.c_str(), "rb+");
                        fseek(file, posicion, SEEK_SET);
                        fwrite(&logica, sizeof(EBR), 1, file);

                        fclose(file);
                        cout <<  "Se creo particion logica" << endl;
                        return;
                    }else{
                        cout <<  "No existe particón extendida" << endl;
                        return;
                    }
                }
                vector<Traslado> auxParticiones;
                int cantidad=0;

                int libre=sizeof(discoAux);
                Partition extendida;
                if(discoAux.mbr_partition_1.part_s!=0){
                    Traslado aux;
                    aux.IdNum=cantidad+1;
                    aux.comienzo=discoAux.mbr_partition_1.part_start;
                    aux.fin=discoAux.mbr_partition_1.part_start+discoAux.mbr_partition_1.part_s;
                    aux.anterior=discoAux.mbr_partition_1.part_start;
                    if(discoAux.mbr_partition_2.part_s!=0)
                    {
                        //auxParticiones.at(cantidad-1).siguiente = aux.comienzo - (auxParticiones.at(cantidad-1).fin);
                        aux.siguiente=discoAux.mbr_partition_2.part_start;
                    }else{
                        aux.siguiente=discoAux.mbr_tamano+discoAux.mbr_partition_1.part_start;
                    }
                    auxParticiones.push_back(aux);
                    cantidad++;
                    if(discoAux.mbr_partition_1.part_type=='e'){
                        extendida=discoAux.mbr_partition_1;
                    }
                }
                if(discoAux.mbr_partition_2.part_s!=0){
                    Traslado aux;
                    aux.IdNum=cantidad+1;
                    aux.comienzo=discoAux.mbr_partition_2.part_start;
                    aux.fin=discoAux.mbr_partition_2.part_start+discoAux.mbr_partition_2.part_s;
                    aux.anterior=auxParticiones.at(0).fin;//discoAux.mbr_partition_2.part_start-libre;
                    if(discoAux.mbr_partition_3.part_s!=0)
                    {
                        //auxParticiones.at(cantidad-1).siguiente = aux.comienzo - (auxParticiones.at(cantidad-1).fin);
                        aux.siguiente=discoAux.mbr_partition_3.part_start;
                    }else{
                        aux.siguiente=discoAux.mbr_tamano+discoAux.mbr_partition_1.part_start;
                    }
                    auxParticiones.push_back(aux);
                    cantidad++;
                    if(discoAux.mbr_partition_2.part_type=='e'){
                        extendida=discoAux.mbr_partition_2;
                    }
                }
                if(discoAux.mbr_partition_3.part_s!=0){
                    Traslado aux;
                    aux.IdNum=cantidad+1;
                    aux.comienzo=discoAux.mbr_partition_3.part_start;
                    aux.fin=discoAux.mbr_partition_3.part_start+discoAux.mbr_partition_3.part_s;
                    aux.anterior=auxParticiones.at(1).fin;//discoAux.mbr_partition_3.part_start-libre;
                    if(discoAux.mbr_partition_4.part_s!=0)
                    {
                        //auxParticiones.at(cantidad-1).siguiente = aux.comienzo - (auxParticiones.at(cantidad-1).fin);
                        aux.siguiente=discoAux.mbr_partition_4.part_start;
                    }else{
                        aux.siguiente=discoAux.mbr_tamano+discoAux.mbr_partition_1.part_start;
                    }
                    auxParticiones.push_back(aux);
                    cantidad++;
                    if(discoAux.mbr_partition_3.part_type=='e'){
                        extendida=discoAux.mbr_partition_3;
                    }
                }
                if(discoAux.mbr_partition_4.part_s!=0){
                    Traslado aux;
                    aux.IdNum=cantidad+1;
                    aux.comienzo=discoAux.mbr_partition_4.part_start;
                    aux.fin=discoAux.mbr_partition_4.part_start+discoAux.mbr_partition_4.part_s;
                    aux.anterior=auxParticiones.at(2).fin;//discoAux.mbr_partition_4.part_start-libre;
                    //if(cantidad !=0)
                    //{
                      //  auxParticiones.at(cantidad-1).siguiente = aux.comienzo - (auxParticiones.at(cantidad-1).fin);
                    //}
                    aux.siguiente=discoAux.mbr_tamano+discoAux.mbr_partition_1.part_start;
                    auxParticiones.push_back(aux);
                    cantidad++;
                    if(discoAux.mbr_partition_4.part_type=='e'){
                        extendida=discoAux.mbr_partition_4;
                    }
                }

                //if (cantidad != 0) {
                //    auxParticiones.at(auxParticiones.size() - 1).siguiente = discoAux.mbr_tamano + auxParticiones.at(auxParticiones.size() - 1).fin;
                //}
                vector<Partition> prueba;
                prueba.push_back(discoAux.mbr_partition_1);
                prueba.push_back(discoAux.mbr_partition_2);
                prueba.push_back(discoAux.mbr_partition_3);
                prueba.push_back(discoAux.mbr_partition_4);

                Partition nuevaParticion;

                nuevaParticion.part_status='n';
                nuevaParticion.part_type=part.tipo.c_str()[0];
                nuevaParticion.part_fit=part.ajuste.c_str()[0];


                nuevaParticion.part_s=part.tamanio;
                strcpy(nuevaParticion.part_name, part.name.substr(0,15).c_str());
                if (nuevaParticion.part_type=='l') {
                    CrearParticionLogica(nuevaParticion, extendida, part.ruta.c_str());
                    return;
                }
                MBR nuevaparticion=MBR();
                nuevaparticion=AgregarParticionNueva(discoAux,nuevaParticion,auxParticiones,prueba,cantidad);

                FILE *file;
                file = fopen(part.ruta.c_str(), "rb+");
                int posicion=0;
                int existe=1;
                bool primero=false;
                //MBR aux;
                fseek(file, 0, SEEK_SET);
                fwrite(&nuevaparticion, sizeof(MBR), 1, file);

                if (nuevaParticion.part_type=='e') {
                    EBR ebr=EBR ();
                    ebr.part_start = extendidaInicio;
                    ebr.part_next=-1;
                    fseek(file, extendidaInicio, SEEK_SET);
                    fwrite(&ebr, sizeof(EBR), 1, file);
                }

                fclose(file);
                cout <<  "Partición Creada" << endl;
            }else if(agregar){
                MBR discoAux=LeerDisco(part.ruta);
                if(discoAux.mbr_partition_1.part_name==part.name){
                    if(part.add<0){
                        if(discoAux.mbr_partition_1.part_s>(part.add*-1)){
                            discoAux.mbr_partition_1.part_s=discoAux.mbr_partition_1.part_s+part.add;
                        }else{
                            cout <<  "Supera tamaño para quitar" << endl;
                            return;
                        }
                    }
                    if(part.add>0&&discoAux.mbr_partition_2.part_s==0&&discoAux.mbr_partition_3.part_s==0&&discoAux.mbr_partition_4.part_s==0){
                        if(discoAux.mbr_tamano>discoAux.mbr_partition_1.part_s+part.add){
                            discoAux.mbr_partition_1.part_s=discoAux.mbr_partition_1.part_s+part.add;
                            //cout <<  "Se agrego espacio" << endl;
                        }else{
                            cout <<  "Supera tamaño de disco" << endl;
                            return;
                        }
                    }else if(part.add<0&&discoAux.mbr_partition_2.part_s==0&&discoAux.mbr_partition_3.part_s==0&&discoAux.mbr_partition_4.part_s==0){
                        if(discoAux.mbr_partition_1.part_s>(part.add*-1)){
                            discoAux.mbr_partition_1.part_s=discoAux.mbr_partition_1.part_s+part.add;
                        }else{
                            cout <<  "Supera tamaño para quitar" << endl;
                            return;
                        }
                    }
                    bool discoSiguiente=false;
                    int inicioDisco=0;
                    if(discoAux.mbr_partition_2.part_s!=0){
                        discoSiguiente=true;
                        inicioDisco=discoAux.mbr_partition_2.part_start;
                    } else if(discoAux.mbr_partition_3.part_s!=0){
                        discoSiguiente=true;
                        inicioDisco=discoAux.mbr_partition_3.part_start;
                    }else if(discoAux.mbr_partition_4.part_s!=0){
                        discoSiguiente=true;
                        inicioDisco=discoAux.mbr_partition_4.part_start;
                    }

                    if(discoAux.mbr_partition_1.part_s+part.add<inicioDisco &&0<discoAux.mbr_partition_1.part_s+part.add){
                        discoAux.mbr_partition_1.part_s=discoAux.mbr_partition_1.part_s+part.add;
                    }else{
                        cout <<  "Supera tamaño" << endl;
                        return;
                    }
                }

                if(discoAux.mbr_partition_2.part_name==part.name){
                    if(part.add<0){
                        if(discoAux.mbr_partition_2.part_s>(part.add*-1)){
                            discoAux.mbr_partition_2.part_s=discoAux.mbr_partition_2.part_s+part.add;
                        }else{
                            cout <<  "Supera tamaño para quitar" << endl;
                            return;
                        }
                    }
                    if(part.add>0&&discoAux.mbr_partition_3.part_s==0&&discoAux.mbr_partition_4.part_s==0){
                        if(discoAux.mbr_tamano>discoAux.mbr_partition_2.part_s+part.add){
                            discoAux.mbr_partition_2.part_s=discoAux.mbr_partition_2.part_s+part.add;
                            return;
                        }else{
                            cout <<  "Supera tamaño de disco" << endl;
                            return;
                        }
                    }else if(part.add<0&&discoAux.mbr_partition_3.part_s==0&&discoAux.mbr_partition_4.part_s==0){
                        if(discoAux.mbr_partition_2.part_s>(part.add*-1)){
                            discoAux.mbr_partition_2.part_s=discoAux.mbr_partition_2.part_s+part.add;
                        }else{
                            cout <<  "Supera tamaño para quitar" << endl;
                            return;
                        }
                    }
                    bool discoSiguiente=false;
                    int inicioDisco=0;
                    if(discoAux.mbr_partition_3.part_s!=0){
                        discoSiguiente=true;
                        inicioDisco=discoAux.mbr_partition_3.part_start;
                    }else if(discoAux.mbr_partition_4.part_s!=0){
                        discoSiguiente=true;
                        inicioDisco=discoAux.mbr_partition_4.part_start;
                    }

                    if(discoAux.mbr_partition_2.part_s+part.add<inicioDisco){
                        discoAux.mbr_partition_2.part_s=discoAux.mbr_partition_2.part_s+part.add;
                    }else{
                        cout <<  "Supera tamaño" << endl;
                        return;
                    }
                }

                if(discoAux.mbr_partition_3.part_name==part.name){
                    if(part.add<0){
                        if(discoAux.mbr_partition_3.part_s>(part.add*-1)){
                            discoAux.mbr_partition_3.part_s=discoAux.mbr_partition_3.part_s+part.add;
                        }else{
                            cout <<  "Supera tamaño para quitar" << endl;
                            return;
                        }
                    }
                    if(part.add>0&&discoAux.mbr_partition_4.part_s==0){
                        if(discoAux.mbr_tamano>discoAux.mbr_partition_3.part_s+part.add){
                            discoAux.mbr_partition_3.part_s=discoAux.mbr_partition_3.part_s+part.add;
                            //cout <<  "Se agrego espacio" << endl;
                        }else{
                            cout <<  "Supera tamaño de disco" << endl;
                            return;
                        }
                    }else if(part.add<0&&discoAux.mbr_partition_4.part_s==0){
                        if(discoAux.mbr_partition_3.part_s>(part.add*-1)){
                            discoAux.mbr_partition_3.part_s=discoAux.mbr_partition_3.part_s+part.add;
                        }else{
                            cout <<  "Supera tamaño para quitar" << endl;
                            return;
                        }
                    }
                    bool discoSiguiente=false;
                    int inicioDisco=0;
                    if(discoAux.mbr_partition_4.part_s!=0){
                        discoSiguiente=true;
                        inicioDisco=discoAux.mbr_partition_4.part_start;
                    }

                    if(discoAux.mbr_partition_3.part_s+part.add<inicioDisco){
                        discoAux.mbr_partition_3.part_s=discoAux.mbr_partition_3.part_s+part.add;
                    }else{
                        cout <<  "Supera tamaño" << endl;
                        return;
                    }
                }

                if(discoAux.mbr_partition_4.part_name==part.name){
                    if(part.add<0){
                        if(discoAux.mbr_partition_4.part_s>(part.add*-1)){
                            discoAux.mbr_partition_4.part_s=discoAux.mbr_partition_4.part_s+part.add;
                        }else{
                            cout <<  "Supera tamaño para quitar" << endl;
                            return;
                        }
                    }
                    if(part.add>0){
                        if(discoAux.mbr_tamano>discoAux.mbr_partition_4.part_s+part.add){
                            discoAux.mbr_partition_4.part_s=discoAux.mbr_partition_4.part_s+part.add;
                            cout <<  "Se agrego espacio" << endl;
                        }else{
                            cout <<  "Supera tamaño de disco" << endl;
                            return;
                        }
                    }else if(part.add<0){
                        if(discoAux.mbr_partition_4.part_s>(part.add*-1)){
                            discoAux.mbr_partition_4.part_s=discoAux.mbr_partition_4.part_s+part.add;
                        }else{
                            cout <<  "Supera tamaño para quitar" << endl;
                            return;
                        }
                    }
                    bool discoSiguiente=false;
                    int inicioDisco=0;


                    if(discoAux.mbr_partition_4.part_s+part.add<inicioDisco){
                        discoAux.mbr_partition_4.part_s=discoAux.mbr_partition_4.part_s+part.add;
                    }
                }
                FILE *file;
                file = fopen(part.ruta.c_str(), "rb+");
                int posicion=0;
                int existe=1;
                bool primero=false;
                //MBR aux;
                fseek(file, 0, SEEK_SET);
                fwrite(&discoAux, sizeof(MBR), 1, file);
                fclose(file);
                cout <<  "Espacio modificado" << endl;
            }else if(eliminarB){
                cout <<  "Desea Eliminar S/N " << endl;
                string seguro;
                getline(cin,seguro);
                if(seguro=="N"){
                    return;
                }
                MBR discoAux=LeerDisco(part.ruta);

                if(discoAux.mbr_partition_1.part_name==name){
                    discoAux.mbr_partition_1=discoAux.mbr_partition_2;
                    discoAux.mbr_partition_2=discoAux.mbr_partition_3;
                    discoAux.mbr_partition_3=discoAux.mbr_partition_4;
                    discoAux.mbr_partition_4=Partition ();
                }
                if(discoAux.mbr_partition_2.part_name==name){
                    discoAux.mbr_partition_2=discoAux.mbr_partition_3;
                    discoAux.mbr_partition_3=discoAux.mbr_partition_4;
                    discoAux.mbr_partition_4=Partition ();
                }
                if(discoAux.mbr_partition_3.part_name==name){
                    discoAux.mbr_partition_3=discoAux.mbr_partition_4;
                    discoAux.mbr_partition_4=Partition ();
                }
                if(discoAux.mbr_partition_4.part_name==name){
                    discoAux.mbr_partition_4=Partition ();
                }

                FILE *file;
                file = fopen(part.ruta.c_str(), "rb+");
                int posicion=0;
                int existe=1;
                bool primero=false;
                //MBR aux;
                fseek(file, 0, SEEK_SET);
                fwrite(&discoAux, sizeof(MBR), 1, file);
                fclose(file);
                cout <<  "Partición Eliminada" << endl;
            }
        }else{
            cout <<  "El disco no existe" << endl;
        }
    }catch(exception &e){
        cout <<  &e << endl;
    };
}

void mount(string linea){
    Particion part;
    size_t pos = linea.find(" ");
    string limpio = linea.substr (pos+1);

    //ruta
    string ruta=split(limpio,"-path");
    if(strcmp(limpio.c_str(),ruta.c_str())==0){
        cout <<  "El parametro path es obligatorio" << endl;
    }else{
        int end = ruta.find(" ");
        string comillas=ruta.substr(2, end-2);
        string comilla="\"";
        //cout <<  ruta.substr(2, 1) << endl;
        //cout <<  comilla << endl;
        if (strcmp(ruta.substr(2, 1).c_str(),comilla.c_str())==0){
            cout <<  "Tiene Comillas" << endl;
            int tamanio=ruta.length();
            int lugarFin=ruta.substr(3, tamanio).find(comilla);
            string rutaNueva=ruta.substr(3, lugarFin);
            //cout <<  rutaNueva << endl;
            //cout <<  lugarFin << endl;
            ruta=rutaNueva;
        }else{
            cout <<  "No tiene Comillas" << endl;
            //cout <<  ruta.substr(2, end-2) << endl;
            ruta=ruta.substr(2, end-2);
        }
        part.ruta=ruta;
        cout <<  "La ruta es" << endl;
        cout <<  part.ruta << endl;

    }
    //-name
    string name=split(limpio,"-name");
    if(strcmp(limpio.c_str(),name.c_str())==0){
        cout <<  "No esta el parametro name" << endl;
        return;
        //part.name='F';
    }else{
        int end = name.find(" ");
        string comillas=name.substr(2, end-2);
        string comilla="\"";
        //cout <<  ruta.substr(2, 1) << endl;
        //cout <<  comilla << endl;
        if (strcmp(name.substr(2, 1).c_str(),comilla.c_str())==0){
            cout <<  "Tiene Comillas" << endl;
            int tamanio=name.length();
            int lugarFin=name.substr(3, tamanio).find(comilla);
            string rutaNueva=name.substr(3, lugarFin);
            //cout <<  rutaNueva << endl;
            //cout <<  lugarFin << endl;
            name=rutaNueva;
        }else{
            cout <<  "No tiene Comillas" << endl;
            //cout <<  ruta.substr(2, end-2) << endl;
            name=name.substr(2, end-2);
        }
        part.name=name;
        cout <<  "La name es" << endl;
        cout <<  part.name << endl;
    }
    string carnet="39";
    string numPart="0";
    if(!Exist(part.ruta.c_str())){
        cout <<  "No esta el parametro name" << endl;
        return;
    }
    MBR auxDisco=LeerDisco(part.ruta);
    string  nombreDico= split(part.ruta,"/");
    int punto=nombreDico.find(".");
    nombreDico=nombreDico.substr(0,punto);
    PartitionMontada actual=PartitionMontada ();
    if(auxDisco.mbr_partition_1.part_name==part.name){
        auxDisco.mbr_partition_1.part_status='s';
        numPart="1";
        actual.particion=auxDisco.mbr_partition_1;
    }else if(auxDisco.mbr_partition_2.part_name==part.name){
        auxDisco.mbr_partition_2.part_status='s';
        numPart="2";
        actual.particion=auxDisco.mbr_partition_2;
    }else if(auxDisco.mbr_partition_3.part_name==part.name){
        auxDisco.mbr_partition_3.part_status='s';
        numPart="3";
        actual.particion=auxDisco.mbr_partition_3;
    }else if(auxDisco.mbr_partition_4.part_name==part.name){
        auxDisco.mbr_partition_4.part_status='s';
        numPart="4";
        actual.particion=auxDisco.mbr_partition_4;
    }

    FILE *file;
    file = fopen(part.ruta.c_str(), "rb+");
    int posicion=0;
    int existe=1;
    bool primero=false;
    //MBR aux;
    fseek(file, 0, SEEK_SET);
    fwrite(&auxDisco, sizeof(MBR), 1, file);
    fclose(file);
    cout <<  carnet+numPart+nombreDico << endl;
    actual.id=carnet+numPart+nombreDico;
    actual.ruta=part.ruta;
    actual.montada=true;
    particionesMontadas.push_back(actual);
    cout <<  "--------------" << endl;
    cout <<  "Montadas" << endl;
    for(PartitionMontada particionM:particionesMontadas){
        cout <<  particionM.particion.part_name << endl;
    }
    cout <<  "--------------" << endl;
}

void unmount(string linea){
    Particion part;
    size_t pos = linea.find(" ");
    string limpio = linea.substr (pos+1);
    //-f
    string f=split(limpio,"-id");
    string id="";
    if(strcmp(limpio.c_str(),f.c_str())==0){
        cout <<  "No esta el parametro id" << endl;
        return;
    }else{
        int end = f.find(" ");
        id=f.substr(2, end-2);
        part.ajuste=f.substr(2, end-2);

        cout <<  "El ID es" << endl;
        cout <<  part.ajuste << endl;
    }

    PartitionMontada auxMontada;
    for (PartitionMontada actual :particionesMontadas) {
        if (actual.id==id){
            auxMontada=actual;
            break;
        }
    }

    MBR auxDisco=LeerDisco(auxMontada.ruta);


    if(auxDisco.mbr_partition_1.part_start==auxMontada.particion.part_start){
        auxDisco.mbr_partition_1.part_status='n';


    }else if(auxDisco.mbr_partition_2.part_start==auxMontada.particion.part_start){
        auxDisco.mbr_partition_2.part_status='n';


    }else if(auxDisco.mbr_partition_3.part_start==auxMontada.particion.part_start){
        auxDisco.mbr_partition_3.part_status='n';


    }else if(auxDisco.mbr_partition_4.part_start==auxMontada.particion.part_start){
        auxDisco.mbr_partition_4.part_status='n';


    }

    FILE *file;
    file = fopen(auxMontada.ruta.c_str(), "rb+");
    int posicion=0;
    int existe=1;
    bool primero=false;
    //MBR aux;
    fseek(file, 0, SEEK_SET);
    fwrite(&auxDisco, sizeof(MBR), 1, file);
    fclose(file);
    cout <<  "Particion desmontada" << endl;
}



MBR LeerDisco(string ruta){

    FILE *file;
    file = fopen(ruta.c_str(), "rb");

    fseek(file, 0, SEEK_SET);


    MBR aux=MBR ();
    int bandera=1;
    fread(&aux, sizeof(MBR), 1, file);
    cout <<  "------------------------" << endl;
    cout <<  "201700339" << endl;
    cout <<  "Luis Sańchez" << endl;
    cout <<  "Tamaño disco" << endl;
    cout <<  aux.mbr_tamano << endl;
    cout <<  "Ajuste disco" << endl;
    cout <<  aux.dsk_fit << endl;
    cout <<  "ID disco" << endl;
    cout <<  aux.mbr_dsk_signature << endl;
    cout <<  "Fecha" << endl;
    cout <<  asctime(std::localtime(&aux.mbr_fecha_creacion)) << endl;
    cout <<  "---------------------------" << endl;
    fclose(file);
    return aux;
}

void crearDirectorios(string ruta){

    int largo=ruta.length();
    int largoTemporal;
    int status;
    status = mkdir(ruta.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    string temporal;
    string rutaTemporal=ruta;

    if(status==-1){
        while(status!=0){
            temporal = split(rutaTemporal,"/");
            largoTemporal = temporal.length();
            temporal=rutaTemporal.substr(0,rutaTemporal.length()-largoTemporal-1);
            status = mkdir(temporal.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            rutaTemporal=temporal;
            if(status==0){
                status = mkdir(ruta.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                rutaTemporal=ruta;
            }
        }
    }


}

void ejecutar(string ruta){

    int flecha=ruta.find("->");
    ruta=ruta.substr(flecha+2,ruta.length());
    string nombreArchivo = ruta;
    ifstream archivo(nombreArchivo.c_str());
    string linea;
    // Obtener línea de archivo, y almacenar contenido en "linea"//
    while (getline(archivo, linea)) {
        int comentario=linea.find("#");
        linea=linea.substr(0,comentario);
        linea= Minuscula(linea);
        int espacio=linea.find(" ");
        string temporal=linea.substr(0,espacio);
        if(temporal=="mkdisk"){
            string tem=linea.substr(espacio+1);
            mkdisk(linea);
        }else if(temporal=="fdisk"){
            fdisk(linea);
        }else if(temporal=="pause"){
            pausa();
        }else if(temporal=="rep"){
            reporte(linea);
        }else if(temporal=="mount"){
            mount(linea);
        }else if(temporal=="mkfs"){
            mkfs(linea);
        }else if(temporal=="login"){
            login(linea);
        }else if(temporal=="logout"){
            logout();
        }else if(temporal=="mkgrp"){
            mkgrp(linea);
        }else if(temporal=="mkusr"){
            mkurs(linea);
        }else if(temporal=="rmdisk"){
            rmdisk(linea);
        }else if(temporal=="unmount"){
            unmount(linea);
        }
    }
}

int LeerParticionLogica(string ruta){
    int inicio=sizeof(MBR);
    FILE *file;
    file = fopen(ruta.c_str(), "rb");

    fseek(file, inicio, SEEK_SET);
    int existe=1;
    int posicion=0;
    EBR aux=EBR();
    while(existe==1){
        fread(&aux, sizeof(EBR), 1, file);
        if (aux.part_s==0){
            existe=0;
            posicion=ftell(file)-sizeof(EBR);
            fclose(file);
            return posicion;
        }
        cout <<  "------------------------" << endl;
        cout <<  "Tamaño particion" << endl;
        cout <<  aux.part_s << endl;
        cout <<  "Ajuste particion" << endl;
        cout <<  aux.part_fit << endl;
        cout <<  "Nombre particion" << endl;
        cout <<  aux.part_name << endl;
        cout <<  "Inicio" << endl;
        cout <<  aux.part_start << endl;
        cout <<  "Status" << endl;
        cout <<  aux.part_status<< endl;
        cout <<  "Next" << endl;
        cout <<  aux.part_next<< endl;
        cout <<  "---------------------------" << endl;
    }
    fclose(file);
    return posicion;
}

void funcionAdd(Particion part){
    MBR discoAux=LeerDisco(part.ruta);
    //Disco 1
    if(discoAux.mbr_partition_1.part_name==part.name){
        if(part.add<0){
            if(discoAux.mbr_partition_1.part_s>(part.add*-1)){
                discoAux.mbr_partition_1.part_s=discoAux.mbr_partition_1.part_s+part.add;
            }else{
                cout <<  "Supera tamaño para quitar" << endl;
                return;
            }
        }
        if(part.add>0&&discoAux.mbr_partition_2.part_s==0&&discoAux.mbr_partition_3.part_s==0&&discoAux.mbr_partition_4.part_s==0){
            if(discoAux.mbr_tamano>discoAux.mbr_partition_1.part_s+part.add){
                discoAux.mbr_partition_1.part_s=discoAux.mbr_partition_1.part_s+part.add;
                cout <<  "Se agrego espacio" << endl;
                return;
            }else{
                cout <<  "Supera tamaño de disco" << endl;
                return;
            }
        }else if(part.add<0&&discoAux.mbr_partition_2.part_s==0&&discoAux.mbr_partition_3.part_s==0&&discoAux.mbr_partition_4.part_s==0){
            if(discoAux.mbr_partition_1.part_s>(part.add*-1)){
                discoAux.mbr_partition_1.part_s=discoAux.mbr_partition_1.part_s+part.add;
            }else{
                cout <<  "Supera tamaño para quitar" << endl;
                return;
            }
        }
        bool discoSiguiente=false;
        int inicioDisco=0;
        if(discoAux.mbr_partition_2.part_s!=0){
            discoSiguiente=true;
            inicioDisco=discoAux.mbr_partition_2.part_start;
        } else if(discoAux.mbr_partition_3.part_s!=0){
            discoSiguiente=true;
            inicioDisco=discoAux.mbr_partition_3.part_start;
        }else if(discoAux.mbr_partition_4.part_s!=0){
            discoSiguiente=true;
            inicioDisco=discoAux.mbr_partition_4.part_start;
        }

        if(part.add>0 && discoAux.mbr_partition_1.part_s+part.add<inicioDisco){
            discoAux.mbr_partition_1.part_s=discoAux.mbr_partition_1.part_s+part.add;
        }
    }

    //Disco 2
    if(discoAux.mbr_partition_2.part_name==part.name){
        if(part.add<0){
            if(discoAux.mbr_partition_2.part_s>(part.add*-1)){
                discoAux.mbr_partition_2.part_s=discoAux.mbr_partition_2.part_s+part.add;
            }else{
                cout <<  "Supera tamaño para quitar" << endl;
                return;
            }
        }
        if(part.add>0&&discoAux.mbr_partition_3.part_s==0&&discoAux.mbr_partition_4.part_s){
            if(discoAux.mbr_tamano>discoAux.mbr_partition_2.part_s+part.add){
                discoAux.mbr_partition_2.part_s=discoAux.mbr_partition_2.part_s+part.add;
                cout <<  "Se agrego espacio" << endl;
                return;
            }else{
                cout <<  "Supera tamaño de disco" << endl;
                return;
            }
        }else if(part.add<0&&discoAux.mbr_partition_3.part_s==0&&discoAux.mbr_partition_4.part_s){
            if(discoAux.mbr_partition_2.part_s>(part.add*-1)){
                discoAux.mbr_partition_2.part_s=discoAux.mbr_partition_2.part_s+part.add;
            }else{
                cout <<  "Supera tamaño para quitar" << endl;
                return;
            }
        }
        bool discoSiguiente=false;
        int inicioDisco=0;
        if(discoAux.mbr_partition_3.part_s!=0){
            discoSiguiente=true;
            inicioDisco=discoAux.mbr_partition_3.part_start;
        }else if(discoAux.mbr_partition_4.part_s!=0){
            discoSiguiente=true;
            inicioDisco=discoAux.mbr_partition_4.part_start;
        }

        if(part.add>0 && discoAux.mbr_partition_2.part_s+part.add<inicioDisco){
            discoAux.mbr_partition_2.part_s=discoAux.mbr_partition_2.part_s+part.add;
        }
    }

    //Disco 3
    if(discoAux.mbr_partition_3.part_name==part.name){
        if(part.add<0){
            if(discoAux.mbr_partition_3.part_s>(part.add*-1)){
                discoAux.mbr_partition_3.part_s=discoAux.mbr_partition_3.part_s+part.add;
            }else{
                cout <<  "Supera tamaño para quitar" << endl;
                return;
            }
        }
        if(part.add>0&&discoAux.mbr_partition_4.part_s){
            if(discoAux.mbr_tamano>discoAux.mbr_partition_3.part_s+part.add){
                discoAux.mbr_partition_3.part_s=discoAux.mbr_partition_3.part_s+part.add;
                cout <<  "Se agrego espacio" << endl;
                return;
            }else{
                cout <<  "Supera tamaño de disco" << endl;
                return;
            }
        }else if(part.add<0&&discoAux.mbr_partition_4.part_s){
            if(discoAux.mbr_partition_3.part_s>(part.add*-1)){
                discoAux.mbr_partition_3.part_s=discoAux.mbr_partition_3.part_s+part.add;
            }else{
                cout <<  "Supera tamaño para quitar" << endl;
                return;
            }
        }
        bool discoSiguiente=false;
        int inicioDisco=0;
        if(discoAux.mbr_partition_4.part_s!=0){
            discoSiguiente=true;
            inicioDisco=discoAux.mbr_partition_4.part_start;
        }

        if(part.add>0 && discoAux.mbr_partition_3.part_s+part.add<inicioDisco){
            discoAux.mbr_partition_3.part_s=discoAux.mbr_partition_3.part_s+part.add;
        }
    }

    //Disco 4
    if(discoAux.mbr_partition_4.part_name==part.name){
        if(part.add<0){
            if(discoAux.mbr_partition_4.part_s>(part.add*-1)){
                discoAux.mbr_partition_4.part_s=discoAux.mbr_partition_4.part_s+part.add;
            }else{
                cout <<  "Supera tamaño para quitar" << endl;
                return;
            }
        }
        if(part.add>0){
            if(discoAux.mbr_tamano>discoAux.mbr_partition_4.part_s+part.add){
                discoAux.mbr_partition_4.part_s=discoAux.mbr_partition_4.part_s+part.add;
                cout <<  "Se agrego espacio" << endl;
                return;
            }else{
                cout <<  "Supera tamaño de disco" << endl;
                return;
            }
        }else if(part.add<0){
            if(discoAux.mbr_partition_4.part_s>(part.add*-1)){
                discoAux.mbr_partition_4.part_s=discoAux.mbr_partition_4.part_s+part.add;
            }else{
                cout <<  "Supera tamaño para quitar" << endl;
                return;
            }
        }
    }
}

int main() {

    //mkdisk("mkdisk -s->10 -f->BF -u->m -path->/home/luis/Descargas/Gatos/p/g/Disco4.dsk");
    //fdisk("fdisk -s->400 -name->\"Particion\" -path->/home/luis/Descargas/Gatos/p/g/Disco4.dsk -delete->Full");
    //mount("mount -path->/home/luis/Descargas/Gatos/p/g/Disco4.dsk -name->Particion");
    //mkfs("mkfs -type->full -id->#391Disco4 -fs->2fs"); //
    //login("login -usr->root -pass->123 -id->#391Disco4");
    //mkgrp("mkgrp -name->usuarios3");
    //mkurs("mkusr -usr->user1 -pass->usuario -grp->usuarios3");
    //mkurs("mkusr -usr->user1 -pass->usuario -grp->usuarios3");
    //mkurs("mkusr -usr->user2 -pass->usuario -grp->usuarios3");
    //mkurs("mkusr -usr->user3 -pass->usuario -grp->usuarios4");
    //tree("#391Disco4");


    //mkdisk("mkdisk -s->5 -f->FF -path->\"/home/mis discos/Disco3.dsk\" -u->m");

    //mkdisk("mkdisk -s->30 -f->BF -u->k -path->/home/luis/Descargas/Disco3.dsk");
    //rmdisk("rmdisk -path->/home/luis/Descargas/Disco1.dsk");
    //fdisk("fdisk -add->300 -t->p -name->\"Particion6\" -path->/home/luis/Descargas/Gatos/p/g/Disco14.dsk -delete->Full");

    //fdisk("fdisk -s->500 -name->\"Particion3\" -path->/home/luis/Descargas/Gatos/p/g/Disco10.dsk -delete->Full");
    //fdisk("fdisk -s->600 -name->\"Particion4\" -path->/home/luis/Descargas/Gatos/p/g/Disco10.dsk -delete->Full");
    //fdisk("fdisk -path->/home/Disco1.dsk -s->10 -add->-500 -name->\"Particion1\" -delete->full");


    //mkdir("mkdir -path->/home");


    //unmount("unmount -id->061Disco1");

    //LeerDisco("/home/luis/Descargas/Gatos/p/g/Disco2.dsk");


    //system("dot -Tpng /home/luis/Descargas/Reportes/prueba.dot -o /home/luis/Descargas/Reportes/prueba.png");
    //vector<string> nombres;
    //nombres.push_back("particion 1");
    //nombres.push_back("particion 2");
    //nombres.push_back("particion 3");
    //ReporteDisco(nombres);
    inicio();



    return 0;
}
void inicio(){
    cout <<  "Proyecto 1" << endl;
    cout <<  "Luis Sánchez" << endl;
    cout <<  "201700339" << endl; //#391Disco16
    cout <<  "Ingrese Comando" << endl;
    string ruta;
    getline(cin,ruta);
    int espacio=ruta.find(" ");
    int largoRuta=ruta.length();
    string orden=ruta.substr(0,espacio);
    if(orden=="exec"){
        string temporal=ruta.substr(espacio+1,largoRuta);
        ejecutar(ruta.substr(espacio+1,largoRuta));
    }
}
void ReporteDisco(vector<string> nombre){
    ofstream file;
    file.open("/home/luis/Descargas/Reportes/prueba2.dot");
    file << "digraph R {\n"
            "  rankdir=LR\n"
            "  node3 [shape=record, label=\"{ ";


    for (int i = 0; i < nombre.size(); ++i) {
        file << nombre[i];
        if(i!=nombre.size()-1){
            file << "|";
        }

    }
    file << "}\"]";
    file << "}";
    file.close();
    system("dot -Tpng /home/luis/Descargas/Reportes/prueba2.dot -o /home/luis/Descargas/Reportes/prueba2.png");
}

MBR AgregarParticionN(MBR mbr, Partition p, vector<Traslado> t, vector<Partition> ps, int u,int inicio){//Disco, nueva particion, espacios, particones, cantidad
    if (u == 0) {
        p.part_start = sizeof(mbr);
        inicio = p.part_start;
        mbr.mbr_partition_1 = p;
        InicioGeneral=inicio;
        return mbr;
    } else {
        Traslado actual;
        int c = 0;
        for (Traslado espacio : t) {
            if (c == 0) {
                actual = espacio;
                c++;
                continue;
            }

            if (toupper(mbr.dsk_fit) == 'F') {
                if (actual.anterior >= p.part_s || actual.siguiente >= p.part_s) {
                    break;
                }
                actual = espacio;
            } else if (toupper(mbr.dsk_fit) == 'B') {
                if (actual.anterior < p.part_s || actual.siguiente < p.part_s) {
                    actual = espacio;
                } else {
                    if (espacio.anterior >= p.part_s || espacio.siguiente >= p.part_s) {
                        int b1 = actual.anterior - p.part_s;
                        int a1 = actual.siguiente - p.part_s;
                        int b2 = espacio.anterior - p.part_s;
                        int a2 = espacio.siguiente - p.part_s;

                        if ((b1 < b2 && b1 < a2) || (a1 < b2 && a1 < a2)) {
                            c++;
                            continue;
                        }
                        actual = espacio;
                    }
                }
            } else if (toupper(mbr.dsk_fit) == 'W') {
                if (!(actual.anterior >= p.part_s) || !(actual.siguiente >= p.part_s)) {
                    actual = espacio;
                } else {
                    if (espacio.anterior >= p.part_s || espacio.siguiente >= p.part_s) {
                        int b1 = actual.anterior - p.part_s;
                        int a1 = actual.siguiente - p.part_s;
                        int b2 = espacio.anterior - p.part_s;
                        int a2 = espacio.siguiente - p.part_s;

                        if ((b1 > b2 && b1 > a2) || (a1 > b2 && a1 > a2)) {
                            c++;
                            continue;
                        }
                        actual = espacio;
                    }
                }
            }
            c++;
        }
        if (actual.anterior >= p.part_s || actual.siguiente >= p.part_s) {
            if (toupper(mbr.dsk_fit) == 'F') {
                if (actual.anterior >= p.part_s) {
                    p.part_start = (actual.comienzo - actual.anterior);
                    inicio = p.part_start;
                } else {
                    p.part_start = actual.fin;
                    inicio = p.part_start;
                }
            } else if (toupper(mbr.dsk_fit) == 'B') {
                int b1 = actual.anterior - p.part_s;
                int a1 = actual.siguiente - p.part_s;

                if ((actual.anterior >= p.part_s && b1 < a1) || !(actual.siguiente >= p.part_start)) {
                    p.part_start = (actual.comienzo - actual.anterior);
                    inicio = p.part_start;
                } else {
                    p.part_start = actual.fin;
                    inicio = p.part_start;
                }
            } else if (toupper(mbr.dsk_fit) == 'W') {
                int b1 = actual.anterior - p.part_s;
                int a1 = actual.siguiente - p.part_s;

                if ((actual.anterior >= p.part_s && b1 > a1) || !(actual.siguiente >= p.part_start)) {
                    p.part_start = (actual.comienzo - actual.anterior);
                    inicio = p.part_start;
                } else {
                    p.part_start = actual.fin;
                    inicio = p.part_start;
                }
            }
            Partition partitions[4];
            for (int i = 0; i < ps.size(); i++) {
                partitions[i] = ps.at(i);
            }
            for (auto &partition : partitions) {
                if (partition.part_s == 0) {
                    partition = p;
                    break;
                }
            }

            Partition aux;
            for (int i = 3; i >= 0; i--) {
                for (int j = 0; j < i; j++) {
                    if ((partitions[j].part_start > partitions[j + 1].part_start)) {
                        aux = partitions[j + 1];
                        partitions[j + 1] = partitions[j];
                        partitions[j] = aux;
                    }
                }
            }

            for (int i = 3; i >= 0; i--) {
                for (int j = 0; j < i; j++) {
                    if (partitions[j].part_s == 0) {
                        aux = partitions[j];
                        partitions[j] = partitions[j + 1];
                        partitions[j + 1] = aux;
                    }
                }
            }
            mbr.mbr_partition_1 = partitions[0];
            mbr.mbr_partition_2 = partitions[1];
            mbr.mbr_partition_3 = partitions[2];
            mbr.mbr_partition_4 = partitions[3];
            InicioGeneral=inicio;
            return mbr;
        } else {
            throw runtime_error("no hay espacio suficiente");
        }
    }
}

void CrearParticionLogica(Partition partition, Partition ep, string p) {
    EBR nlogic;
    nlogic.part_status = 'n';
    nlogic.part_fit = partition.part_fit;
    nlogic.part_s = partition.part_s;
    nlogic.part_next = -1;
    strcpy(nlogic.part_name, partition.part_name);

    FILE *file = fopen(p.c_str(), "rb+");
    rewind(file);
    EBR tmp;
    fseek(file, ep.part_start, SEEK_SET);
    fread(&tmp, sizeof(EBR), 1, file);
    int size;
    do {
        size += sizeof(EBR) + tmp.part_s;
        if (tmp.part_s == 0 && tmp.part_next == -1) {
            nlogic.part_start = tmp.part_start;
            nlogic.part_next = nlogic.part_start + nlogic.part_s + sizeof(EBR);
            if ((ep.part_s - size) <= nlogic.part_s) {
                throw runtime_error("no hay espacio para más particiones lógicas");
            }
            fseek(file, nlogic.part_start, SEEK_SET);
            fwrite(&nlogic, sizeof(EBR), 1, file);
            fseek(file, nlogic.part_next, SEEK_SET);
            EBR addLogic;
            addLogic.part_status = '0';
            addLogic.part_next = -1;
            addLogic.part_start = nlogic.part_next;
            fseek(file, addLogic.part_start, SEEK_SET);
            fwrite(&addLogic, sizeof(EBR), 1, file);
            //shared.response("FDISK", "partición creada correctamente");
            fclose(file);
            return;
        }
        fseek(file, tmp.part_next, SEEK_SET);
        fread(&tmp, sizeof(EBR), 1, file);
    } while (true);
}

Partition ObtenerParticion(MBR mbr, string name, string path) {

    bool extExiste = false;
    Partition extendida;
    if (mbr.mbr_partition_1.part_s=!0){
        if (mbr.mbr_partition_1.part_name==name) {
            return mbr.mbr_partition_1;
        } else if (mbr.mbr_partition_1.part_type == 'e') {
            extExiste = true;
            extendida = mbr.mbr_partition_1;
        }
    }

    if (mbr.mbr_partition_2.part_s=!0){
        if (mbr.mbr_partition_2.part_name==name) {
            return mbr.mbr_partition_2;
        } else if (mbr.mbr_partition_2.part_type == 'e') {
            extExiste = true;
            extendida = mbr.mbr_partition_2;
        }
    }

    if (mbr.mbr_partition_3.part_s=!0){
        if (mbr.mbr_partition_3.part_name==name) {
            return mbr.mbr_partition_3;
        } else if (mbr.mbr_partition_3.part_type == 'e') {
            extExiste = true;
            extendida = mbr.mbr_partition_3;
        }
    }

    if (mbr.mbr_partition_4.part_s=!0){
        if (mbr.mbr_partition_4.part_name==name) {
            return mbr.mbr_partition_4;
        } else if (mbr.mbr_partition_4.part_type == 'e') {
            extExiste = true;
            extendida = mbr.mbr_partition_4;
        }
    }

    if (extExiste) {
        vector<EBR> ebrs = ObtenerLogicas(extendida, path);
        for (EBR ebr : ebrs) {
            if (ebr.part_s != 0) {
                if (ebr.part_name==name) {
                    Partition tmp;
                    return tmp;
                }
            }
        }
    }
    throw runtime_error("la partición no existe");
}

vector<EBR> ObtenerLogicas(Partition partition, string p) {
    vector<EBR> listaLogicas;

    FILE *file = fopen(p.c_str(), "rb+");
    rewind(file);
    EBR tmp=EBR ();
    fseek(file, partition.part_start, SEEK_SET);
    fread(&tmp, sizeof(EBR), 1, file);
    do {
        if (!(tmp.part_s == 0)) {
            if (tmp.part_s != 0) {
                listaLogicas.push_back(tmp);
            }
            fseek(file, tmp.part_next, SEEK_SET);
            fread(&tmp, sizeof(EBR), 1, file);
        } else {
            fclose(file);
            break;
        }
    } while (true);
    return listaLogicas;
}


void mkfs(string linea){
    Particion part;
    size_t pos = linea.find(" ");
    string limpio = linea.substr (pos+1);
    //-f
    string f=split(limpio,"-id");
    string id;
    if(strcmp(limpio.c_str(),f.c_str())==0){
        cout <<  "No esta el parametro id" << endl;
        return;
    }else{
        int end = f.find(" ");
        part.ajuste=f.substr(2, end-2);
        id=f.substr(2, end-2);

        cout <<  "El ID es" << endl;
        cout <<  part.ajuste << endl;
    }
    //-type
    string tipo=split(limpio,"-type");
    if(strcmp(limpio.c_str(),tipo.c_str())==0){
        cout <<  "No esta el parametro type" << endl;
    }else{
        int end = tipo.find(" ");
        //disk.ajuste=f.substr(2, end-2);
        string completo="full";
        if(strcmp(tipo.substr(2, end-2).c_str(),completo.c_str())==0){
            //cout <<  "El ajuste BF" << endl;
            cout <<  "El type  es " << endl;
            cout <<  tipo.substr(2, end-2) << endl;
        }else{
            cout <<  "El type es full" << endl;

        }
    }
    //-fs
    string fs=split(limpio,"-fs");
    bool sistema2=true;
    if(strcmp(limpio.c_str(),fs.c_str())==0){
        cout <<  "No esta el parametro fs" << endl;
    }else{
        int end = fs.find(" ");
        //disk.ajuste=f.substr(2, end-2);
        string ext2="2fs";
        string ext3="3fs";
        if(strcmp(fs.substr(2, end-2).c_str(),ext2.c_str())==0){
            //cout <<  "El ajuste BF" << endl;
            cout <<  "El fs  es " << endl;
            cout <<  fs.substr(2, end-2) << endl;
        }else if(strcmp(fs.substr(2, end-2).c_str(),ext3.c_str())==0){
            cout <<  "El fs  es " << endl;
            cout <<  fs.substr(2, end-2) << endl;
            sistema2=false;
        }else{
            cout <<  "El fs  no es disponible" << endl;
        }
    }
    PartitionMontada auxMontada;
    for (PartitionMontada actual :particionesMontadas) {
        if (actual.id==id){
            auxMontada=actual;
            break;
        }
    }

    int numEstructuras;
    SuperBloque superBlock=SuperBloque();
    if (sistema2){
        numEstructuras=(auxMontada.particion.part_s-sizeof(SuperBloque))/(4+sizeof(Inodo)+(3*sizeof(CarpetaBloque)));
        numEstructuras= floor(numEstructuras);
        superBlock.s_filesystem_type=2;
    }else{
        numEstructuras=(auxMontada.particion.part_s-sizeof(SuperBloque))/(4+sizeof(Inodo)+(3*sizeof(CarpetaBloque))+sizeof(Journaling));
        numEstructuras= floor(numEstructuras);
        superBlock.s_filesystem_type=3;
    }
    superBlock.s_inodes_count=numEstructuras;
    superBlock.s_blocks_count=3*numEstructuras;
    superBlock.s_free_blocks_count=3*numEstructuras;
    superBlock.s_free_inodes_count=numEstructuras;
    superBlock.s_mtime=time(nullptr);
    superBlock.s_umtime=time(nullptr);
    superBlock.s_mnt_count = 1;
    superBlock.s_magic=61267;
    superBlock.s_block_s=sizeof(CarpetaBloque);
    superBlock.s_inode_s=sizeof(Inodo);
    char ceros = '0';
    string grupo = "1,G,root\n1,U,root,root,123\n";
    if(sistema2){
        superBlock.s_bm_inode_start = auxMontada.particion.part_start + sizeof(SuperBloque);
        superBlock.s_bm_block_start = superBlock.s_bm_inode_start + numEstructuras;
        superBlock.s_inode_start = superBlock.s_bm_block_start + (3 * numEstructuras);
        superBlock.s_block_start = superBlock.s_bm_inode_start + (numEstructuras * sizeof(Inodo));

        FILE *file = fopen(auxMontada.ruta.c_str(), "rb+");
        fseek(file, auxMontada.particion.part_start, SEEK_SET);
        fwrite(&superBlock, sizeof(SuperBloque), 1, file);
        fseek(file, superBlock.s_bm_inode_start, SEEK_SET);
        for (int i = 0; i < numEstructuras; i++) {
            fwrite(&ceros, sizeof(ceros), 1, file);
        }
        fseek(file, superBlock.s_bm_block_start, SEEK_SET);
        for (int i = 0; i < (3 * numEstructuras); i++) {
            fwrite(&ceros, sizeof(ceros), 1, file);
        }
        Inodo inodo=Inodo();
        fseek(file, superBlock.s_inode_start, SEEK_SET);
        for (int i = 0; i < numEstructuras; i++) {
            fwrite(&inodo, sizeof(Inodo), 1, file);
        }
        CarpetaBloque carpeta=CarpetaBloque();
        fseek(file, superBlock.s_block_start, SEEK_SET);
        for (int i = 0; i < (3 * numEstructuras); i++) {
            fwrite(&carpeta, sizeof(CarpetaBloque), 1, file);
        }
        fclose(file);

        SuperBloque recuperado;
        FILE *archivo = fopen(auxMontada.ruta.c_str(), "rb");
        fseek(archivo, auxMontada.particion.part_start, SEEK_SET);
        fread(&recuperado, sizeof(SuperBloque), 1, archivo);
        fclose(archivo);
        inodo.i_uid = 1;
        inodo.I_gid= 1;
        inodo.i_atime = superBlock.s_umtime;
        inodo.i_ctime = superBlock.s_umtime;
        inodo.i_mtime = superBlock.s_umtime;
        inodo.i_block[0] = 0;
        inodo.i_type = 0;
        inodo.i_perm = 664;


        CarpetaBloque raiz=CarpetaBloque ();
        strcpy(raiz.b_content[0].b_name, ".");
        raiz.b_content[0].b_inodo = 0;
        strcpy(raiz.b_content[1].b_name, "..");
        raiz.b_content[1].b_inodo = 0;
        strcpy(raiz.b_content[2].b_name, "user.txt");
        raiz.b_content[2].b_inodo = 1;


        Inodo unionInodo=Inodo();
        unionInodo.i_uid = 1;
        unionInodo.I_gid = 1;
        unionInodo.i_s = sizeof(grupo.c_str()) + sizeof(CarpetaBloque);
        unionInodo.i_atime = superBlock.s_umtime;
        unionInodo.i_ctime = superBlock.s_umtime;
        unionInodo.i_mtime = superBlock.s_umtime;
        unionInodo.i_type = 1;
        unionInodo.i_perm = 664;
        unionInodo.i_block[0] = 1;

        inodo.i_s = unionInodo.i_s + sizeof(CarpetaBloque) + sizeof(Inodo);

        ArchivoBloque archivoBloque=ArchivoBloque();
        strcpy(archivoBloque.b_content, grupo.c_str());

        FILE *bfiles = fopen(auxMontada.ruta.c_str(), "rb+");
        fseek(bfiles, superBlock.s_bm_inode_start, SEEK_SET);
        char caracter = '1';
        fwrite(&caracter, sizeof(caracter), 1, bfiles);
        fwrite(&caracter, sizeof(caracter), 1, bfiles);

        fseek(bfiles, superBlock.s_bm_block_start, SEEK_SET);
        fwrite(&caracter, sizeof(caracter), 1, bfiles);
        fwrite(&caracter, sizeof(caracter), 1, bfiles);

        fseek(bfiles, superBlock.s_inode_start, SEEK_SET);
        fwrite(&inodo, sizeof(Inodo), 1, bfiles);
        fwrite(&unionInodo, sizeof(Inodo), 1, bfiles);

        fseek(bfiles, superBlock.s_block_start, SEEK_SET);
        fwrite(&raiz, sizeof(CarpetaBloque), 1, bfiles);
        fwrite(&archivoBloque, sizeof(ArchivoBloque), 1, bfiles);
        fclose(bfiles);
    }else{
        superBlock.s_bm_inode_start = auxMontada.particion.part_start + sizeof(SuperBloque) + (numEstructuras * sizeof(Journaling));
        superBlock.s_bm_block_start = superBlock.s_bm_inode_start + numEstructuras;
        superBlock.s_inode_start = superBlock.s_bm_block_start + (3 * numEstructuras);
        superBlock.s_block_start = superBlock.s_bm_inode_start + (numEstructuras * sizeof(Inodo));

        FILE *file = fopen(auxMontada.ruta.c_str(), "rb+");
        fseek(file, auxMontada.particion.part_start, SEEK_SET);
        fwrite(&superBlock, sizeof(SuperBloque), 1, file);
        fseek(file, superBlock.s_bm_inode_start, SEEK_SET);
        for (int i = 0; i < numEstructuras; i++) {
            fwrite(&ceros, sizeof(ceros), 1, file);
        }
        fseek(file, superBlock.s_bm_block_start, SEEK_SET);
        for (int i = 0; i < (3 * numEstructuras); i++) {
            fwrite(&ceros, sizeof(ceros), 1, file);
        }
        Inodo inodo=Inodo();
        fseek(file, superBlock.s_inode_start, SEEK_SET);
        for (int i = 0; i < numEstructuras; i++) {
            fwrite(&inodo, sizeof(Inodo), 1, file);
        }
        CarpetaBloque carpeta=CarpetaBloque();
        fseek(file, superBlock.s_block_start, SEEK_SET);
        for (int i = 0; i < (3 * numEstructuras); i++) {
            fwrite(&carpeta, sizeof(CarpetaBloque), 1, file);
        }
        fclose(file);

        SuperBloque recuperado;
        FILE *archivo = fopen(auxMontada.ruta.c_str(), "rb");
        fseek(archivo, auxMontada.particion.part_start, SEEK_SET);
        fread(&recuperado, sizeof(SuperBloque), 1, archivo);
        fclose(archivo);
        inodo.i_uid = 1;
        inodo.I_gid= 1;
        inodo.i_atime = superBlock.s_umtime;
        inodo.i_ctime = superBlock.s_umtime;
        inodo.i_mtime = superBlock.s_umtime;
        inodo.i_block[0] = 0;
        inodo.i_type = 0;
        inodo.i_perm = 664;


        CarpetaBloque raiz=CarpetaBloque ();
        strcpy(raiz.b_content[0].b_name, ".");
        raiz.b_content[0].b_inodo = 0;
        strcpy(raiz.b_content[1].b_name, "..");
        raiz.b_content[1].b_inodo = 0;
        strcpy(raiz.b_content[2].b_name, "user.txt");
        raiz.b_content[2].b_inodo = 1;


        Inodo unionInodo=Inodo();
        unionInodo.i_uid = 1;
        unionInodo.I_gid = 1;
        unionInodo.i_s = sizeof(grupo.c_str()) + sizeof(CarpetaBloque);
        unionInodo.i_atime = superBlock.s_umtime;
        unionInodo.i_ctime = superBlock.s_umtime;
        unionInodo.i_mtime = superBlock.s_umtime;
        unionInodo.i_type = 1;
        unionInodo.i_perm = 664;
        unionInodo.i_block[0] = 1;

        inodo.i_s = unionInodo.i_s + sizeof(CarpetaBloque) + sizeof(Inodo);

        ArchivoBloque archivoBloque=ArchivoBloque();
        strcpy(archivoBloque.b_content, grupo.c_str());

        FILE *bfiles = fopen(auxMontada.ruta.c_str(), "rb+");
        fseek(bfiles, superBlock.s_bm_inode_start, SEEK_SET);
        char caracter = '1';
        fwrite(&caracter, sizeof(caracter), 1, bfiles);
        fwrite(&caracter, sizeof(caracter), 1, bfiles);

        fseek(bfiles, superBlock.s_bm_block_start, SEEK_SET);
        fwrite(&caracter, sizeof(caracter), 1, bfiles);
        fwrite(&caracter, sizeof(caracter), 1, bfiles);

        fseek(bfiles, superBlock.s_inode_start, SEEK_SET);
        fwrite(&inodo, sizeof(Inodo), 1, bfiles);
        fwrite(&unionInodo, sizeof(Inodo), 1, bfiles);

        fseek(bfiles, superBlock.s_block_start, SEEK_SET);
        fwrite(&raiz, sizeof(CarpetaBloque), 1, bfiles);
        fwrite(&archivoBloque, sizeof(ArchivoBloque), 1, bfiles);
        fclose(bfiles);
    }
}

vector<string> getElements(string txt, char c) {
    vector<string> v;
    string line;
    if (c == ',') {
        txt.push_back(',');
    }
    for (char &x: txt) {
        if (x == c) {
            v.push_back(line);
            line = "";
            continue;
        }
        line += x;
    }

    if (v.empty()) {
        throw runtime_error("no hay archivo txt");
    }
    return v;
}

bool login(string linea) {
    Particion part;
    size_t pos = linea.find(" ");
    string limpio = linea.substr(pos + 1);

    //-usr
    string usr = split(limpio, "-usr");
    string user;
    if (strcmp(limpio.c_str(), usr.c_str()) == 0) {
        cout << "No esta el parametro user" << endl;
        return false;
    } else {
        int end = usr.find(" ");

        user=usr.substr(2, end - 2);

        cout << "El user es" << endl;
        cout << user << endl;
    }

    //-pass
    string pass = split(limpio, "-pass");
    string password;
    if (strcmp(limpio.c_str(), pass.c_str()) == 0) {
        cout << "No esta el parametro pass" << endl;
        return false;
    } else {
        int end = pass.find(" ");

        password=pass.substr(2, end - 2);

        cout << "El pass es" << endl;
        cout << password << endl;
    }

    //-id
    string f=split(limpio,"-id");
    string id;
    if(strcmp(limpio.c_str(),f.c_str())==0){
        cout <<  "No esta el parametro id" << endl;
        return false;
    }else{
        int end = f.find(" ");
        part.ajuste=f.substr(2, end-2);
        id=f.substr(2, end-2);

        cout <<  "El ID es" << endl;
        cout <<  id << endl;
    }

    PartitionMontada auxMontada;
    for (PartitionMontada actual :particionesMontadas) {
        if (actual.id==id){
            auxMontada=actual;
            break;
        }
    }


    SuperBloque super;
    FILE *rfile = fopen(auxMontada.ruta.c_str(), "rb");
    fseek(rfile, auxMontada.particion.part_start, SEEK_SET);
    fread(&super, sizeof(SuperBloque), 1, rfile);


    ArchivoBloque fb;
    fseek(rfile, super.s_block_start + sizeof(CarpetaBloque), SEEK_SET);
    fread(&fb, sizeof(ArchivoBloque), 1, rfile);
    fclose(rfile);

    string txt;
    txt += fb.b_content;

    vector<string> vctr = getElements(txt, '\n');
    for (string line:vctr) {
        if (line[2] == 'U' || line[2] == 'u') {
            vector<string> in = getElements(line, ',');
            if (in[3]==user&&in[4]==password) {
                userActive.idPart = id;
                userActive.user = user;
                userActive.pass = password;
                userActive.grupo = stoi(in[0]);
                userActive.active = true;
                userActive.actual=auxMontada;
                if(user=="root"){
                    userActive.admin = true;
                }
                cout <<  "Bienvenido" << endl;
                return true;
            }
        }
    }
    cout <<  "No se pudo iniciar sesión" << endl;
    return false;
}

void logout(){
    if(!userActive.active){
        cout <<  "Sin usuario logueado" << endl;
        return;
    }
    userActive.idPart = "";
    userActive.user = "";
    userActive.pass = "";
    userActive.grupo = "";
    userActive.active = false;
    userActive.admin = false;
    cout <<  "Adios" << endl;
}

vector<string> getpath(string s) {
    vector<string> result;
    if (s.empty()) {
        return result;
    }

    s.push_back('/');
    string tmp;
    int status = -1;
    for (char &c : s) {
        if (status != -1) {
            if (status == 2 && c == '\"') {
                status = 3;
                continue;
            } else if (status == 1) {
                if (c == '\"') {
                    status = 2;
                    continue;
                } else if (c == '/') {
                    continue;
                }
                status = 3;
            }

            if ((status == 3) && c == '/') {
                status = 1;
                result.push_back(tmp);
                tmp = "";
                continue;
            }
            tmp += c;
        } else if (c == '/') {
            status = 1;
        }
    }
    return result;
}

int getfree(SuperBloque spr, string pth, string t) {
    char ch = 'x';
    FILE *file = fopen(pth.c_str(), "rb");
    if (t == "BI") {
        fseek(file, spr.s_bm_inode_start, SEEK_SET);
        for (int i = 0; i < spr.s_inodes_count; i++) {
            fread(&ch, sizeof(ch), 1, file);
            if (ch == '0') {
                fclose(file);
                return i;
            }
        }
    } else {
        fseek(file, spr.s_bm_block_start, SEEK_SET);
        for (int i = 0; i < spr.s_blocks_count; i++) {
            fread(&ch, sizeof(ch), 1, file);
            if (ch == '0') {
                fclose(file);
                return i;
            }
        }
    }
    fclose(file);
    return -1;
}

void updatebm(SuperBloque spr, string pth, string t) {
    char ch = 'x';
    char one = '1';
    int num;
    FILE *file = fopen(pth.c_str(), "rb+");
    if (t == "BI") {
        fseek(file, spr.s_bm_inode_start, SEEK_SET);
        for (int i = 0; i < spr.s_inodes_count; i++) {
            fread(&ch, sizeof(ch), 1, file);
            if (ch == '0') {
                num = i;
                break;
            }
        }
        fseek(file, spr.s_bm_inode_start + num, SEEK_SET);
        fwrite(&one, sizeof(one), 1, file);
    } else {
        fseek(file, spr.s_bm_block_start, SEEK_SET);
        for (int i = 0; i < spr.s_blocks_count; i++) {
            fread(&ch, sizeof(ch), 1, file);
            if (ch == '0') {
                num = i;
                break;
            }
        }
        fseek(file, spr.s_bm_block_start + num, SEEK_SET);
        fwrite(&one, sizeof(one), 1, file);
    }
    fclose(file);
}

void tree(string p,string id) {
    try {

        string path;
        SuperBloque spr;
        Inodo inode=Inodo();
        Inodo inodeArchivo=Inodo();


        Partition partition=Partition ();
        PartitionMontada auxMontada=PartitionMontada ();
        for (PartitionMontada actual :particionesMontadas) {
            if (actual.id==id){
                auxMontada=actual;
                partition=auxMontada.particion;
                path=auxMontada.ruta;
            }
        }


        FILE *file = fopen(path.c_str(), "rb+");
        if (file == NULL) {
            throw runtime_error("disco no existente");
        }

        fseek(file, partition.part_start, SEEK_SET);
        fread(&spr, sizeof(SuperBloque), 1, file);

        fseek(file, spr.s_inode_start, SEEK_SET);
        fread(&inode, sizeof(Inodo), 1, file);
        fseek(file, spr.s_inode_start +sizeof(Inodo),SEEK_SET);
        fread(&inodeArchivo, sizeof(Inodo), 1, file);

        int freeI = getfree(spr, path, "BI");

        string pd = p.substr(0, p.find('.'));
        string tipo=p.substr(p.find('.')+1);
        bool pdf=false;
        bool png=false;
        bool jpg=false;
        if(tipo=="pdf"){
            pdf=true;
        }else if(tipo=="png"){
            png=true;
        }else if(tipo=="jpg"){
            jpg=true;
        }
        pd += ".dot";
        FILE *doc = fopen(pd.c_str(), "r");
        if (doc == NULL) {
            string cmm = "mkdir -p \"" + pd + "\"";
            string cmm2 = "rmdir \"" + pd + "\"";
            system(cmm.c_str());
            system(cmm2.c_str());
        } else {
            fclose(doc);
        }

        string content;
        content = "digraph G{\n"
                  "rankdir=LR;\n"
                  "graph [ dpi = \"600\" ]; \n"
                  "forcelabels= true;\n"
                  "node [shape = plaintext];\n";

        for (int i = 0; i < freeI; ++i) {
            content += "inode" + to_string(i) + "  [label = <<table BGCOLOR=\"#87CEEB\">\n"
                                                "<tr><td COLSPAN = '2'><font color=\"black\">INODO " +
                       to_string(i) + "</font></td></tr>\n"
                                      "<tr>\n"
                                      "<td>i_uid</td>\n"
                                      "<td>" +
                       to_string(inode.i_uid) + "</td>\n"
                                                "</tr>\n"
                                                "<tr>\n"
                                                "<td>i_gid</td>\n"
                                                "<td>" +
                       to_string(inode.I_gid) + "</td>\n"
                                                "</tr>\n"
                                                "<tr>\n"
                                                "<td>i_s</td>\n"
                                                "<td>" +
                       to_string(inode.i_s) + "</td>\n"
                                                 "</tr>\n"
                                                 "<tr>\n"
                                                 "<td>i_atime</td>\n"
                                                 "<td>" +
                       asctime(localtime(&inode.i_atime)) + "</td>\n"
                                                  "</tr>\n"
                                                  "<tr>\n"
                                                  "<td>i_ctime</td>\n"
                                                  "<td>" +
                    asctime(localtime(&inode.i_ctime)) + "</td>\n"
                                                  "</tr>\n"
                                                  "<tr>\n"
                                                  "<td>i_mtime</td>\n"
                                                  "<td>" +
                    asctime(localtime(&inode.i_mtime)) + "</td>\n"
                                                  "</tr>\n";
            for (int j = 0; j < 15; ++j) {
                content += "<tr>\n"
                           "<td>i_block_" + to_string(j + 1) + "</td>\n"
                                                               "<td port=\"" + to_string(j) + "\">" +
                           to_string(inode.i_block[j]) + "</td>\n"
                                                         "</tr>\n";
            }
            content += "<tr>\n"
                       "<td>i_type</td>\n"
                       "<td>" + to_string(inode.i_type) + "</td>\n"
                                                          "</tr>\n"
                                                          "<tr>\n"
                                                          "<td>i_perm</td>\n"
                                                          "<td>" + to_string(inode.i_perm) + "</td>\n"
                                                                                             "</tr>\n</table>>];\n";
            if (inode.i_type == 0) {
                for (int j = 0; j < 15; j++) {
                    if (inode.i_block[j] != -1) {
                        content +=
                                "inode" + to_string(i) + ":" + to_string(j) + "-> BLOCK" + to_string(inode.i_block[j]) +
                                "\n";

                        CarpetaBloque foldertmp;
                        fseek(file, spr.s_block_start + (sizeof(CarpetaBloque) * inode.i_block[j]),
                              SEEK_SET);
                        fread(&foldertmp, sizeof(CarpetaBloque), 1, file);

                        content += "BLOCK" + to_string(inode.i_block[j]) + "  [label = <<table BGCOLOR=\"#f4a020\">\n"
                                                                           "<tr><td COLSPAN = '2'><font color=\"black\">BLOCK " +
                                   to_string(inode.i_block[j]) + "</font></td></tr>\n";
                        for (int k = 0; k < 4; ++k) {
                            string ctmp;
                            ctmp += foldertmp.b_content[k].b_name;
                            content += "<tr>\n"
                                       "<td>" + ctmp + "</td>\n"
                                                       "<td port=\"" + to_string(k) + "\">" +
                                       to_string(foldertmp.b_content[k].b_inodo) + "</td>\n"
                                                                                   "</tr>\n";
                        }
                        content += "</table>>];\n";

                        for (int b = 0; b < 4; b++) { //VER SI ELIMINO
                            if (foldertmp.b_content[b].b_inodo != -1) {
                                string nm(foldertmp.b_content[b].b_name);
                                if (!((nm == ".") || (nm == ".."))) {
                                    content +=
                                            "BLOCK" + to_string(inode.i_block[j]) + ":" + to_string(b) + " -> inode" +
                                            to_string(foldertmp.b_content[b].b_inodo) + ";\n";
                                }
                            }
                        }

                        if (j > 11) {
                            //Metodo para graficar bloques indirectos
                        }
                    }
                }
            } else {
                for (int j = 0; j < 15; j++) {
                    if (inodeArchivo.i_block[j] != -1) {
                        if (j < 12) {

                            ArchivoBloque filetmp;
                            fseek(file, spr.s_block_start + (sizeof(ArchivoBloque) * (j+1)),
                                  SEEK_SET);
                            fread(&filetmp, sizeof(ArchivoBloque), 1, file);

                            content +=
                                    "inode" + to_string(i) + ":" + to_string(j) + "-> BLOCK" +
                                    to_string(inodeArchivo.i_block[j]) +
                                    "\n";
                            content += "BLOCK" + to_string(inodeArchivo.i_block[j]) + " [label = <<table BGCOLOR=\"#008000\">\n"
                                                                               "<tr><td COLSPAN = '2'>BLOCK " +
                                       to_string(inodeArchivo.i_block[j]) +
                                       "</td></tr>\n <tr><td COLSPAN = '2'>" + filetmp.b_content +
                                       "</td></tr>\n</table>>];\n";
                        }
                    }
                }
            }
            fseek(file, spr.s_inode_start + (sizeof(Inodo) * (i + 1)), SEEK_SET);
            fread(&inode, sizeof(Inodo), 1, file);
        }
        fclose(file);
        content += "\n\n}\n";

        ofstream outfile(pd);
        outfile << content.c_str() << endl;
        outfile.close();string function="";
        if(png){
            function = "dot -Tpng " + pd + " -o " + p;
        }else if(pdf){
            function = "dot -Tpdf " + pd + " -o " + p;
        }else if(jpg){
            function = "dot -Tjpg " + pd + " -o " + p;
        }
        system(function.c_str());
        //function = "rm \"" + pd + "\"";
        //system(function.c_str());
        //shared.response("REPORT", "generado correctamente");
    } catch (exception &e) {
        //shared.handler("REPORT", e.what());
    }
}

void mkgrp(string linea) {
    try {
        if(userActive.admin==false){
            cout <<  "No eres el admin" << endl;
            return;
        }
        size_t pos = linea.find(" ");
        string limpio = linea.substr (pos+1);
        //-name
        string name=split(limpio,"-name");
        if(strcmp(limpio.c_str(),name.c_str())==0){
            cout <<  "No esta el parametro name" << endl;
            return;
            //part.name='F';
        }else{
            int end = name.find(" ");
            string comillas=name.substr(2, end-2);
            string comilla="\"";
            //cout <<  ruta.substr(2, 1) << endl;
            //cout <<  comilla << endl;
            if (strcmp(name.substr(2, 1).c_str(),comilla.c_str())==0){
                cout <<  "Tiene Comillas" << endl;
                int tamanio=name.length();
                int lugarFin=name.substr(3, tamanio).find(comilla);
                string rutaNueva=name.substr(3, lugarFin);
                //cout <<  rutaNueva << endl;
                //cout <<  lugarFin << endl;
                name=rutaNueva;
            }else{
                cout <<  "No tiene Comillas" << endl;
                //cout <<  ruta.substr(2, end-2) << endl;
                name=name.substr(2, end-2);
            }
        }
        name;
        //if (!(shared.compare(logged.user, "root"))) {
        //    throw runtime_error("no se puede realizar la acción sin el usuario root");
        //}
        string path;
        PartitionMontada auxMontada=userActive.actual;

        SuperBloque super=SuperBloque ();
        FILE *rfile = fopen(auxMontada.ruta.c_str(), "rb+");
        fseek(rfile, auxMontada.particion.part_start, SEEK_SET);
        fread(&super, sizeof(SuperBloque), 1, rfile);

        Inodo inodoArchvio=Inodo();
        fseek(rfile, super.s_inode_start + sizeof(Inodo), SEEK_SET);
        fread(&inodoArchvio, sizeof(Inodo), 1, rfile);

        ArchivoBloque fb=ArchivoBloque ();
        fseek(rfile, super.s_block_start + sizeof(CarpetaBloque), SEEK_SET);
        string txt;
        for (int i = 0; i < 13; ++i) {
            fread(&fb, sizeof(ArchivoBloque), 1, rfile);
            if(fb.b_content[0]!='0'){
                txt += fb.b_content;
            }
        }




        vector<string> listaT = splitArreglo(txt, '\n');
        listaT.pop_back();
        int c = 0;
        for (string line:listaT) {
            vector<string> listaG = splitArreglo(line, ',');
            if ((listaG[1] == "G" || listaG[1] == "g")) {
                c++;
                if (listaG[2] == name) {
                    if (listaG[0] != "0") {
                        cout <<  "El grupo ya existe" << endl;
                        return;
                    }
                }
            }
        }
        txt += to_string(c + 1) + ",G," + name + "\n";
        if (txt.length()>64){
            int longitudTexto=txt.length();
            int longitudConst=txt.length();
            int contador=0;
            while(longitudTexto>64){
                longitudTexto=longitudTexto-64;
                contador++;
            }
            for (int i = 0; i <= contador; ++i) {
                if(i==0){
                    string prueba=txt.substr(0,63).c_str();
                    strcpy(fb.b_content, txt.substr(0,63).c_str());
                    fseek(rfile, super.s_block_start + sizeof(CarpetaBloque), SEEK_SET);
                    fwrite(&fb, sizeof(ArchivoBloque), 1, rfile);

                }else{

                    fseek(rfile, super.s_inode_start + sizeof(Inodo), SEEK_SET);
                    inodoArchvio.i_block[i]=i+1;
                    fwrite(&inodoArchvio, sizeof(Inodo), 1, rfile);
                    fseek(rfile, super.s_inode_start + sizeof(Inodo), SEEK_SET);
                    fread(&inodoArchvio, sizeof(Inodo), 1, rfile);
                    int maximimo=64*(i+1);
                    if(longitudConst>maximimo){

                    }else{
                        maximimo=longitudTexto;
                    }
                    string prueba=txt.substr(64*(i)-1,maximimo).c_str();
                    strcpy(fb.b_content, txt.substr(64*(i)-1,maximimo+2).c_str());
                    fseek(rfile, super.s_block_start + sizeof(CarpetaBloque)+sizeof(ArchivoBloque)*contador, SEEK_SET);
                    fwrite(&fb, sizeof(ArchivoBloque), 1, rfile);

                }
            }
            fclose(rfile);

        }else{
            strcpy(fb.b_content, txt.c_str());
            fseek(rfile, super.s_block_start + sizeof(CarpetaBloque), SEEK_SET);
            fwrite(&fb, sizeof(ArchivoBloque), 1, rfile);
            fclose(rfile);
        }
        cout << "Grupo creado" << endl;
        //shared.response("MKGRP", "grupo creado correctamente");
    }
    catch (exception &e) {
        cout << "Grupo no creado" << endl;
    }
}

vector<string> splitArreglo(string str, char divisor) {

    int posInit = 0;
    int posFound = 0;
    string splitted;
    vector<string> results;

    while(posFound >= 0){
        posFound = str.find(divisor, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        results.push_back(splitted);
    }

    return results;
}

void mkurs(string linea) {
    try {
        if(userActive.admin==false){
            cout <<  "No eres el admin" << endl;
            return;
        }
        size_t pos = linea.find(" ");
        string limpio = linea.substr (pos+1);
        //-usr
        string usr = split(limpio, "-usr");
        string user;
        if (strcmp(limpio.c_str(), usr.c_str()) == 0) {
            cout << "No esta el parametro user" << endl;
            return;
        } else {
            int end = usr.find(" ");

            user=usr.substr(2, end - 2);

            cout << "El user es" << endl;
            cout << user << endl;
        }

        //-pass
        string pass = split(limpio, "-pass");
        string password;
        if (strcmp(limpio.c_str(), pass.c_str()) == 0) {
            cout << "No esta el parametro pass" << endl;
            return;
        } else {
            int end = pass.find(" ");

            password=pass.substr(2, end - 2);

            cout << "El pass es" << endl;
            cout << password << endl;
        }
        //-name
        string name=split(limpio,"-grp");
        if(strcmp(limpio.c_str(),name.c_str())==0){
            cout <<  "No esta el parametro grp" << endl;
            return;
            //part.name='F';
        }else{
            int end = name.find(" ");
            string comillas=name.substr(2, end-2);
            string comilla="\"";
            //cout <<  ruta.substr(2, 1) << endl;
            //cout <<  comilla << endl;
            if (strcmp(name.substr(2, 1).c_str(),comilla.c_str())==0){
                cout <<  "Tiene Comillas" << endl;
                int tamanio=name.length();
                int lugarFin=name.substr(3, tamanio).find(comilla);
                string rutaNueva=name.substr(3, lugarFin);
                //cout <<  rutaNueva << endl;
                //cout <<  lugarFin << endl;
                name=rutaNueva;
            }else{
                cout <<  "No tiene Comillas" << endl;
                //cout <<  ruta.substr(2, end-2) << endl;
                name=name.substr(2, end-2);
            }
        }
        name;
        //if (!(shared.compare(logged.user, "root"))) {
        //    throw runtime_error("no se puede realizar la acción sin el usuario root");
        //}
        string path;
        PartitionMontada auxMontada=userActive.actual;

        SuperBloque super=SuperBloque ();
        FILE *rfile = fopen(auxMontada.ruta.c_str(), "rb+");
        fseek(rfile, auxMontada.particion.part_start, SEEK_SET);
        fread(&super, sizeof(SuperBloque), 1, rfile);

        Inodo inodoArchvio=Inodo();
        fseek(rfile, super.s_inode_start + sizeof(Inodo), SEEK_SET);
        fread(&inodoArchvio, sizeof(Inodo), 1, rfile);

        ArchivoBloque fb=ArchivoBloque ();
        fseek(rfile, super.s_block_start + sizeof(CarpetaBloque), SEEK_SET);
        string txt;
        for (int i = 0; i < 13; ++i) {
            fread(&fb, sizeof(ArchivoBloque), 1, rfile);
            if(fb.b_content[0]!='0'){
                txt += fb.b_content;
            }
        }

        vector<string> listaTG = splitArreglo(txt, '\n');
        listaTG.pop_back();
        bool existeG=false;
        for (string line:listaTG) {
            vector<string> listaG = splitArreglo(line, ',');
            if ((listaG[1] == "G" || listaG[1] == "g")) {

                if (listaG[2] == name) {
                    existeG=true;
                }
            }
        }
        if(!existeG){
            cout <<  "El grupo no existe" << endl;
            return;
        }

        vector<string> listaT = splitArreglo(txt, '\n');
        listaT.pop_back();
        int c = 0;
        for (string line:listaT) {
            vector<string> listaU = splitArreglo(line, ',');
            if ((listaU[1] == "U" || listaU[1] == "u")) {
                c++;
                if (listaU[3] == user) {
                    cout <<  "El usuario ya existe" << endl;
                    return;
                }
            }
        }
        txt += to_string(c + 1) + ",U," + name +","+user+","+password+ "\n";
        if (txt.length()>64){
            int longitudTexto=txt.length();
            int longitudConst=txt.length();
            int contador=0;
            while(longitudTexto>64){
                longitudTexto=longitudTexto-64;
                contador++;
            }
            for (int i = 0; i <= contador; ++i) {
                if(i==0){
                    string prueba=txt.substr(0,63).c_str();
                    strcpy(fb.b_content, txt.substr(0,63).c_str());
                    fseek(rfile, super.s_block_start + sizeof(CarpetaBloque), SEEK_SET);
                    fwrite(&fb, sizeof(ArchivoBloque), 1, rfile);

                }else{

                    fseek(rfile, super.s_inode_start + sizeof(Inodo), SEEK_SET);
                    inodoArchvio.i_block[i]=i+1;
                    fwrite(&inodoArchvio, sizeof(Inodo), 1, rfile);
                    fseek(rfile, super.s_inode_start + sizeof(Inodo), SEEK_SET);
                    fread(&inodoArchvio, sizeof(Inodo), 1, rfile);
                    int maximimo=64*(i+1);
                    if(longitudConst>maximimo){

                    }else{
                        maximimo=longitudTexto;
                    }
                    string prueba=txt.substr(64*(i)-1,maximimo).c_str();
                    strcpy(fb.b_content, txt.substr(64*(i)-1,maximimo+2).c_str());
                    fseek(rfile, super.s_block_start + sizeof(CarpetaBloque)+sizeof(ArchivoBloque)*contador, SEEK_SET);
                    fwrite(&fb, sizeof(ArchivoBloque), 1, rfile);

                }
            }
            fclose(rfile);

        }else{
            strcpy(fb.b_content, txt.c_str());
            fseek(rfile, super.s_block_start + sizeof(CarpetaBloque), SEEK_SET);
            fwrite(&fb, sizeof(ArchivoBloque), 1, rfile);
            fclose(rfile);
        }

        cout <<  "Usuario creado" << endl;
    }
    catch (exception &e) {
        cout <<  "Usuario no creado" << endl;
    }
}

string Minuscula(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}

// Function to allocate memory to
// blocks as per First fit algorithm
void firstFit(int blockSize[], int m,
              int processSize[], int n)
{
    // Stores block id of the
    // block allocated to a process
    int allocation[n];

    // Initially no block is assigned to any process
    memset(allocation, -1, sizeof(allocation));

    // pick each process and find suitable blocks
    // according to its size ad assign to it
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (blockSize[j] >= processSize[i])
            {
                // allocate block j to p[i] process
                allocation[i] = j;

                // Reduce available memory in this block.
                blockSize[j] -= processSize[i];

                break;
            }
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock no.\n";
    for (int i = 0; i < n; i++)
    {
        cout << " " << i+1 << "\t\t"
             << processSize[i] << "\t\t";
        if (allocation[i] != -1)
            cout << allocation[i] + 1;
        else
            cout << "Not Allocated";
        cout << endl;
    }
}

// Function to allocate memory to
// blocks as per First fit algorithm
MBR firstFitParticion(MBR disco,vector<Traslado> t,vector<Partition> p,Partition nueva)
{
    // Stores block id of the
    // block allocated to a process
    int n = p.size();
    int m = t.size();
    //MBR actulizado=MBR();
    int contador=0;
    bool agregar=false;
    for (int j = 0; j < m; j++)
    {
        if (t[j].siguiente-t[j].fin >= nueva.part_s)
        {
            nueva.part_start=t[j].fin;
            contador=j;
            agregar=true;
            break;
        }
    }
    if(agregar){
        if(contador==0){
            Partition aux2=Partition ();
            aux2=disco.mbr_partition_3;
            disco.mbr_partition_4=aux2;
            Partition aux=Partition ();
            aux=disco.mbr_partition_2;
            disco.mbr_partition_3=aux;
            disco.mbr_partition_2=nueva;
            extendidaInicio=disco.mbr_partition_2.part_start;
        }else if(contador==1){
            Partition aux=Partition ();
            aux=disco.mbr_partition_3;
            disco.mbr_partition_4=aux;
            disco.mbr_partition_3=nueva;
            extendidaInicio=disco.mbr_partition_3.part_start;
        }else if(contador==2){
            disco.mbr_partition_4=nueva;
            extendidaInicio=disco.mbr_partition_4.part_start;
        }else{
            cout <<  "Particones llenas" << endl;
        }

    }else{
        cout <<  "Sin espacio para agregar particion" << endl;
    }
    return disco;
}

MBR AgregarParticionNueva(MBR mbr, Partition p, vector<Traslado> t, vector<Partition> ps, int u){//Disco, nueva particion, espacios, particones, cantidad
    if (u == 0) {
        p.part_start = sizeof(mbr);

        mbr.mbr_partition_1 = p;
        extendidaInicio=p.part_start;
        return mbr;
    } else {
        if(mbr.dsk_fit=='f'){
            return firstFitParticion(mbr,t,ps,p);
        }
        if(mbr.dsk_fit=='b'){
            return bestFitPartitton(mbr,t,ps,p);
        }
        if(mbr.dsk_fit=='w'){
            return worstFitPartitton(mbr,t,ps,p);
        }
    }
    return mbr;
}

void bestFit(int blockSize[], int m, int processSize[], int n)
{
    // Stores block id of the block allocated to a
    // process
    int allocation[n];

    // Initially no block is assigned to any process
    memset(allocation, -1, sizeof(allocation));

    // pick each process and find suitable blocks
    // according to its size ad assign to it
    for (int i=0; i<n; i++)
    {
        // Find the best fit block for current process
        int bestIdx = -1;
        for (int j=0; j<m; j++)
        {
            if (blockSize[j] >= processSize[i])
            {
                if (bestIdx == -1)
                    bestIdx = j;
                else if (blockSize[bestIdx] > blockSize[j])
                    bestIdx = j;
            }
        }

        // If we could find a block for current process
        if (bestIdx != -1)
        {
            // allocate block j to p[i] process
            allocation[i] = bestIdx;

            // Reduce available memory in this block.
            blockSize[bestIdx] -= processSize[i];
        }
    }
}

MBR bestFitPartitton(MBR disco,vector<Traslado> t,vector<Partition> p,Partition nueva)
{
    int n = p.size();
    int m = t.size();
    bool agregar=false;
    int bestIdx = -1;
    for (int j=0; j<m; j++)
    {
        if (t[j].siguiente-t[j].fin >= nueva.part_s)
        {
            if (bestIdx == -1)
                bestIdx = j;
            else if (t[bestIdx].siguiente-t[bestIdx].fin > t[j].siguiente-t[j].fin)
                bestIdx = j;
        }
    }


    if (bestIdx != -1)
    {
        if(bestIdx==0){
            nueva.part_start=t[bestIdx].fin;
            agregar=true;
            Partition aux=Partition ();
            aux=disco.mbr_partition_3;
            disco.mbr_partition_4=aux;
            Partition aux2=Partition ();
            aux2=disco.mbr_partition_2;
            disco.mbr_partition_3=aux2;
            disco.mbr_partition_2=nueva;
            extendidaInicio=nueva.part_start;
        }else if(bestIdx==1){
            nueva.part_start=t[bestIdx].fin;
            agregar=true;
            Partition aux=Partition ();
            aux=disco.mbr_partition_3;
            disco.mbr_partition_4=aux;
            disco.mbr_partition_3=nueva;
            extendidaInicio=nueva.part_start;
        }else if(bestIdx==2){
            nueva.part_start=t[bestIdx].fin;
            agregar=true;
            disco.mbr_partition_4=nueva;
            extendidaInicio=nueva.part_start;
        }
    }
    if (!agregar){
        cout <<  "Sin espacio para agregar particion" << endl;
    }
    return disco;
}

MBR worstFitPartitton(MBR disco,vector<Traslado> t,vector<Partition> p,Partition nueva)
{
    int n = p.size();
    int m = t.size();
    bool agregar=false;
    int bestIdx = -1;
    for (int j=0; j<m; j++)
    {
        if (t[j].siguiente-t[j].fin >= nueva.part_s)
        {
            if (bestIdx == -1)
                bestIdx = j;
            else if (t[bestIdx].siguiente-t[bestIdx].fin < t[j].siguiente-t[j].fin)
                bestIdx = j;
        }
    }


    if (bestIdx != -1)
    {
        if(bestIdx==0){
            nueva.part_start=t[bestIdx].fin;
            agregar=true;
            Partition aux=Partition ();
            aux=disco.mbr_partition_3;
            disco.mbr_partition_4=aux;
            Partition aux2=Partition ();
            aux2=disco.mbr_partition_2;
            disco.mbr_partition_3=aux2;
            disco.mbr_partition_2=nueva;
            extendidaInicio=nueva.part_start;
        }else if(bestIdx==1){
            nueva.part_start=t[bestIdx].fin;
            agregar=true;
            Partition aux=Partition ();
            aux=disco.mbr_partition_3;
            disco.mbr_partition_4=aux;
            disco.mbr_partition_3=nueva;
            extendidaInicio=nueva.part_start;
        }else if(bestIdx==2){
            nueva.part_start=t[bestIdx].fin;
            agregar=true;
            disco.mbr_partition_4=nueva;
            extendidaInicio=nueva.part_start;
        }
    }
    if (!agregar){
        cout <<  "Sin espacio para agregar particion" << endl;
    }
    return disco;
}

void dks(string p, string id){
    try {
        string path;
        PartitionMontada auxMontada;
        for (PartitionMontada actual :particionesMontadas) {
            if (actual.id==id){
                auxMontada=actual;
                path=auxMontada.ruta;
            }
        }

        FILE *file = fopen(path.c_str(), "rb+");
        if (file == NULL) {
            throw runtime_error("disco no existente");
        }

        MBR disk=MBR ();
        fseek(file, 0, SEEK_SET);
        fread(&disk, sizeof(MBR), 1, file);
        fclose(file);

        string pd = p.substr(0, p.find('.'));
        string tipo=p.substr(p.find('.')+1);
        bool pdf=false;
        bool png=false;
        bool jpg=false;
        if(tipo=="pdf"){
            pdf=true;
        }else if(tipo=="png"){
            png=true;
        }else if(tipo=="jpg"){
            jpg=true;
        }
        pd += ".dot";
        FILE *doc = fopen(pd.c_str(), "r");
        if (doc == NULL) {
            string cmm = "mkdir -p \"" + pd + "\"";
            string cmm2 = "rmdir \"" + pd + "\"";
            system(cmm.c_str());
            system(cmm2.c_str());
        } else {
            fclose(doc);
        }

        Partition partitions[4];
        partitions[0] = disk.mbr_partition_1;
        partitions[1] = disk.mbr_partition_2;
        partitions[2] = disk.mbr_partition_3;
        partitions[3] = disk.mbr_partition_4;
        Partition extended;
        bool ext = false;
        for (int i = 0; i < 4; ++i) {
            if (partitions[i].part_status == '1') {
                if (partitions[i].part_type == 'E') {
                    ext = true;
                    extended = partitions[i];
                }
            }
        }

        string content;
        content = "digraph G{\n"
                  "rankdir=TB;\n"
                  "forcelabels= true;\n"
                  "graph [ dpi = \"600\" ]; \n"
                  "node [shape = plaintext];\n";
        content += "nodo1 [label = <<table>\n";
        content += "<tr>\n";

        content += "<td ROWSPAN='2'> MBR </td>\n";

        vector<Traslado> auxParticiones;
        int cantidad=0;

                Partition extendida;
        if(disk.mbr_partition_1.part_s!=0){
            Traslado aux;
            aux.IdNum=cantidad+1;
            aux.comienzo=disk.mbr_partition_1.part_start;
            aux.fin=disk.mbr_partition_1.part_start+disk.mbr_partition_1.part_s;
            aux.anterior=disk.mbr_partition_1.part_start;
            if(disk.mbr_partition_2.part_s!=0)
            {
                //auxParticiones.at(cantidad-1).siguiente = aux.comienzo - (auxParticiones.at(cantidad-1).fin);
                aux.siguiente=disk.mbr_partition_2.part_start;
            }else{
                aux.siguiente=disk.mbr_tamano+disk.mbr_partition_1.part_start;
            }
            auxParticiones.push_back(aux);
            cantidad++;
            if(disk.mbr_partition_1.part_type=='e'){
                extendida=disk.mbr_partition_1;
            }
        }
        if(disk.mbr_partition_2.part_s!=0){
            Traslado aux;
            aux.IdNum=cantidad+1;
            aux.comienzo=disk.mbr_partition_2.part_start;
            aux.fin=disk.mbr_partition_2.part_start+disk.mbr_partition_2.part_s;
            aux.anterior=auxParticiones.at(0).fin;//discoAux.mbr_partition_2.part_start-libre;
            if(disk.mbr_partition_3.part_s!=0)
            {
                //auxParticiones.at(cantidad-1).siguiente = aux.comienzo - (auxParticiones.at(cantidad-1).fin);
                aux.siguiente=disk.mbr_partition_3.part_start;
            }else{
                aux.siguiente=disk.mbr_tamano+disk.mbr_partition_1.part_start;
            }
            auxParticiones.push_back(aux);
            cantidad++;
            if(disk.mbr_partition_2.part_type=='e'){
                extendida=disk.mbr_partition_2;
            }
        }
        if(disk.mbr_partition_3.part_s!=0){
            Traslado aux;
            aux.IdNum=cantidad+1;
            aux.comienzo=disk.mbr_partition_3.part_start;
            aux.fin=disk.mbr_partition_3.part_start+disk.mbr_partition_3.part_s;
            aux.anterior=auxParticiones.at(1).fin;//discoAux.mbr_partition_3.part_start-libre;
            if(disk.mbr_partition_4.part_s!=0)
            {
                //auxParticiones.at(cantidad-1).siguiente = aux.comienzo - (auxParticiones.at(cantidad-1).fin);
                aux.siguiente=disk.mbr_partition_4.part_start;
            }else{
                aux.siguiente=disk.mbr_tamano+disk.mbr_partition_1.part_start;
            }
            auxParticiones.push_back(aux);
            cantidad++;
            if(disk.mbr_partition_3.part_type=='e'){
                extendida=disk.mbr_partition_3;
            }
        }
        if(disk.mbr_partition_4.part_s!=0){
            Traslado aux;
            aux.IdNum=cantidad+1;
            aux.comienzo=disk.mbr_partition_4.part_start;
            aux.fin=disk.mbr_partition_4.part_start+disk.mbr_partition_4.part_s;
            aux.anterior=auxParticiones.at(2).fin;//discoAux.mbr_partition_4.part_start-libre;
            //if(cantidad !=0)
            //{
            //  auxParticiones.at(cantidad-1).siguiente = aux.comienzo - (auxParticiones.at(cantidad-1).fin);
            //}
            aux.siguiente=disk.mbr_tamano+disk.mbr_partition_1.part_start;
            auxParticiones.push_back(aux);
            cantidad++;
            if(disk.mbr_partition_4.part_type=='e'){
                extendida=disk.mbr_partition_4;
            }
        }

        for (int i = 0; i < cantidad; ++i) {
            if (partitions[i].part_s!=0){
                float num1=partitions[i].part_s;
                float num2=disk.mbr_tamano;
                float num=num1/num2;
                num=num*100;
                if(partitions[i].part_type=='p'){
                    content += "<td ROWSPAN='2'>Primaria "+ to_string(num)+"%</td>\n";
                }else if(partitions[i].part_type=='e'){
                    //content +="<tr>\n";
                    content += "<td ROWSPAN='2'>Extendida "+ to_string(num)+"%</td>\n";
                    //content += "</tr>\n";
                    vector<EBR> logicas=ObtenerLogicas(partitions[i],path.c_str());
                    float libreE=0;
                    for (EBR logica:logicas) {
                        float num1=logica.part_s;
                        float num2=partitions[i].part_s;
                        float num=num1/num2;
                        content += "<td ROWSPAN='2'>Logica "+ to_string(num)+"%</td>\n";
                        libreE=libreE+num;
                    }
                    if(libreE!=0){
                        float result=100-libreE;
                        content += "<td ROWSPAN='2'>Libre "+ to_string(result)+"%</td>\n";
                    }
                    if(logicas.size()==0){
                        //content +="<tr>\n";
                        content += "<td ROWSPAN='2'>EBR</td>\n";
                        //content += "</tr>\n";
                    }

                }
                if(auxParticiones[i].siguiente==auxParticiones[i].fin){

                }else{
                    float num1=auxParticiones[i].siguiente-auxParticiones[i].fin;
                    float num2=disk.mbr_tamano;
                    float num=num1/num2;
                    num=num*100;
                    content += "<td ROWSPAN='2'>Libre "+ to_string(num)+"%</td>\n";
                }
            }
        }





        content += "</tr>\n\n";

        content += "</table>>];\n}\n";
        ofstream outfile(pd);
        outfile << content.c_str() << endl;
        outfile.close();string function="";
        if(png){
            function = "dot -Tpng " + pd + " -o " + p;
        }else if(pdf){
            function = "dot -Tpdf " + pd + " -o " + p;
        }else if(jpg){
            function = "dot -Tjpg " + pd + " -o " + p;
        }
        system(function.c_str());
        //function = "rm \"" + pd + "\"";
        //system(function.c_str());
        //shared.response("REPORT", "generado correctamente");
    } catch (exception &e) {
        //shared.handler("REPORT", e.what());
    }
}

void Rmbr(string p, string id) {
    try {
        string path;
        PartitionMontada auxMontada;
        for (PartitionMontada actual :particionesMontadas) {
            if (actual.id==id){
                auxMontada=actual;
                path=auxMontada.ruta;
            }
        }

        if(!auxMontada.montada){
            cout <<  "Particion no montada" << endl;
            return;
        }

        FILE *file = fopen(path.c_str(), "rb+");
        if (file == NULL) {
            throw runtime_error("disco no existente");
        }

        MBR disco=MBR ();
        rewind(file);
        fread(&disco, sizeof(MBR), 1, file);
        fclose(file);

        string pd = p.substr(0, p.find('.'));
        string tipo=p.substr(p.find('.')+1);
        bool pdf=false;
        bool png=false;
        bool jpg=false;
        if(tipo=="pdf"){
            pdf=true;
        }else if(tipo=="png"){
            png=true;
        }else if(tipo=="jpg"){
            jpg=true;
        }
        pd += ".dot";
        FILE *doc = fopen(pd.c_str(), "r");
        if (doc == NULL) {
            string cmm = "mkdir -p \"" + pd + "\"";
            string cmm2 = "rmdir \"" + pd + "\"";
            system(cmm.c_str());
            system(cmm2.c_str());
        } else {
            fclose(doc);
        }

        Partition partitions[4];
        partitions[0] = disco.mbr_partition_1;
        partitions[1] = disco.mbr_partition_2;
        partitions[2] = disco.mbr_partition_3;
        partitions[3] = disco.mbr_partition_4;
        struct tm *tm;
        ofstream outfile(pd);
        tm = localtime(&disco.mbr_fecha_creacion);
        char mostrar_fecha [20];
        strftime(mostrar_fecha, 20, "%Y/%m/%d %H:%M:%S", tm);
        string content;
        content = "digraph G{\n"
                  "rankdir=TB;\n"
                  "graph [ dpi = \"600\" ]; \n"
                  "forcelabels= true;\n"
                  "node [shape = plaintext];\n"
                  "general [label = <<table>\n"
                  "<tr><td COLSPAN = '2' BGCOLOR=\"#800080\"><font color=\"white\">MBR</font></td></tr>\n"
                  "<tr>\n"
                  "<td>mbr_tamaño</td>\n"
                  "<td>" +
                  to_string(disco.mbr_tamano) + "</td>\n"
                                                "</tr>\n"
                                                "<tr>\n"
                                                "<td>mbr_fecha_creación</td>\n"
                                                "<td>" +
                  string(mostrar_fecha) + "</td>\n"
                                          "</tr>\n"
                                          "<tr>\n"
                                          "<td>mbr_disk_signature</td>\n"
                                          "<td>" +
                  to_string(disco.mbr_dsk_signature) + "</td>\n"
                                                        "</tr>\n"
                                                        "<tr>\n"
                                                        "<td>Disk_fit</td>\n"
                                                        "<td>" +
                  string(1, disco.dsk_fit) + "</td>\n"
                                              "</tr>\n";

        Partition extended;
        bool ext = false;
        string particiones="";
        outfile << content.c_str();
        for (int i = 0; i < 4; ++i) {
            if (partitions[i].part_s!=0) {
                if (partitions[i].part_type == 'e') {
                    ext = true;
                    extended = partitions[i];
                }
                particiones ="<tr><td COLSPAN = '2'>PARTICION</td></tr><tr>"
                           "<td>part_status" + to_string(i + 1) + "</td>"
                                                                   "<td>" +
                           partitions[i].part_status + " </td >\n"
                                                       "</tr>\n"
                                                       "<tr>\n"
                                                       "<td>part_type" + to_string(i + 1) + "</td>\n"
                                                                                             "<td>" +
                           partitions[i].part_type + "</td>\n"
                                                     "</tr>\n"
                                                     "<tr>\n"
                                                     "<td>part_fit" + to_string(i + 1) + "</td>\n"
                                                                                          "<td>" +
                           partitions[i].part_fit + "</td>\n"
                                                    "</tr>\n"
                                                    "<tr>\n"
                                                    "<td>part_start" + to_string(i + 1) + "</td>\n"
                                                                                           "<td>" +
                           to_string(partitions[i].part_start) + "</td>\n"
                                                                 "</tr>\n"
                                                                 "<tr>\n"
                                                                 "<td>part_s" + to_string(i + 1) + "</td>\n"
                                                                                                       "<td>" +
                           to_string(partitions[i].part_s) + "</td>\n"
                                                                "</tr>\n"
                                                                "<tr>\n"
                                                                "<td>part_name" + to_string(i + 1) + "</td>\n"
                                                                                                      "<td>" +
                           partitions[i].part_name + "</td>\n"
                                                     "</tr>\n";
                outfile << particiones.c_str();
            }
        }
        int count = 0;
        if (ext) {
            vector<EBR> ebrs =ObtenerLogicas(extended,path);
            string textoE="";
            for (EBR ebr : ebrs) {
                textoE= "<tr><td BORDER=\"0\"></td><td BORDER=\"0\"></td></tr>"
                        "<tr><td COLSPAN = '2'>LOGICA</td></tr>"
                           "<tr>\n"
                           "<td>part_status" + to_string(count + 1) + "</td>\n"
                                                                       "<td>" +
                           ebr.part_status + "</td>\n"
                                             "</tr>\n"
                                             "<tr>\n"
                                             "<td>part_fit" + to_string(count + 1) + "</td>\n"
                                                                                      "<td>" +
                           ebr.part_fit + "</td>\n"
                                          "</tr>\n"
                                          "<tr>\n"
                                          "<td>part_start" + to_string(count + 1) + "</td>\n"
                                                                                     "<td>" +
                           to_string(ebr.part_start) + "</td>\n"
                                                       "</tr>\n"
                                                       "<tr>\n"
                                                       "<td>part_s" + to_string(count + 1) + "</td>\n"
                                                                                                 "<td>" +
                           to_string(ebr.part_s) + "</td>\n"
                                                      "</tr>\n"
                                                      "<tr>\n"
                                                      "<td>part_next" + to_string(count + 1) + "</td>\n"
                                                                                               "<td>" +
                           to_string(ebr.part_next) + "</td>\n"
                                                      "</tr>\n"
                                                      "<tr>\n"
                                                      "<td>part_name" + to_string(count + 1) + "</td>\n"
                                                                                               "<td>" +
                           ebr.part_name + "</td>\n"
                                           "</tr>\n";
                count++;
                outfile << textoE.c_str();
            }

        }
        string t= "</table>>];";
        string n= "\n\n}\n";

        //string contenido=content;
        outfile <<t.c_str()<<n.c_str() << endl;
        outfile.close();
        string function="";
        if(png){
            function = "dot -Tpng " + pd + " -o " + p;
        }else if(pdf){
            function = "dot -Tpdf " + pd + " -o " + p;
        }else if(jpg){
            function = "dot -Tjpg " + pd + " -o " + p;
        }

        system(function.c_str());
        //function = "rm \"" + pd + "\"";
        //system(function.c_str());
        //shared.response("REPORT", "generado correctamente");
    } catch (exception &e) {
        //shared.handler("REPORT", e.what());
    }
}

void pausa(){
    cout <<  "Está en pausa " << endl;
    string ruta;
    getline(cin,ruta);
    cout <<  "Salio pausa en pausa " << endl;
    return;
}