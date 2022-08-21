#include <iostream>
#include "fstream"
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
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_s;
    char part_name[16];
}Partition;

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

int main() {
    //mkdisk("mkdisk -s->5 -f->FF -path->\"/home/mis discos/Disco3.dsk\" -u->m");
    //mkdisk("mkdisk -s->10 -f->BF -u->m -path->/home/luis/Descargas/Gatos/p/g/Disco11.dsk");
    //mkdisk("mkdisk -s->30 -f->BF -u->k -path->/home/luis/Descargas/Disco3.dsk");
    //rmdisk("rmdisk -path->/home/luis/Descargas/Disco1.dsk");
    fdisk("fdisk -add->300 -t->l -name->\"Particion6\" -path->/home/luis/Descargas/Gatos/p/g/Disco11.dsk -delete->Full");
    //fdisk("fdisk -s->400 -name->\"Particion2\" -path->/home/luis/Descargas/Gatos/p/g/Disco10.dsk -delete->Full");
    //fdisk("fdisk -s->500 -name->\"Particion3\" -path->/home/luis/Descargas/Gatos/p/g/Disco10.dsk -delete->Full");
    //fdisk("fdisk -s->600 -name->\"Particion4\" -path->/home/luis/Descargas/Gatos/p/g/Disco10.dsk -delete->Full");
    //fdisk("fdisk -path->/home/Disco1.dsk -s->10 -add->-500 -name->\"Particion1\" -delete->full");
    //mount("mount -path->/home/Disco1.dsk -name->Part2");
    //unmount("unmount -id->061Disco1");
    //mkfs("mkfs -type->full -id->061Disco1 -fs->2fs"); exec -path->/home/luis/Descargas/prueba.mia
    //LeerDisco("/home/luis/Descargas/Gatos/p/g/Disco2.dsk");
    //cout <<  "Proyecto 1" << endl;
    //cout <<  "Luis Sánchez" << endl;
    //cout <<  "201700339" << endl;
    //cout <<  "Ingrese Comando" << endl;
    //string ruta;
    //getline(cin,ruta);
    //int espacio=ruta.find(" ");
    //int largoRuta=ruta.length();
    //string orden=ruta.substr(0,espacio);
    //if(orden=="exec"){
    //    string temporal=ruta.substr(espacio+1,largoRuta);
    //    ejecutar(ruta.substr(espacio+1,largoRuta));
    // }





    return 0;
}

void mkdisk(string linea){
    MkDiskEstruc disk;
    MBR disco;
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
        disk.ajuste='F';
    }else{
        int end = f.find(" ");
        //disk.ajuste=f.substr(2, end-2);
        string bf="BF";
        string ff="FF";
        string wf="WF";
        if(strcmp(f.substr(2, end-2).c_str(),bf.c_str())==0){
            //cout <<  "El ajuste BF" << endl;
            disk.ajuste='B';
            disco.dsk_fit='B';
        }else if(strcmp(f.substr(2, end-2).c_str(),ff.c_str())==0){
            //cout <<  "El ajuste FF" << endl;
            disk.ajuste='F';
            disco.dsk_fit='F';
        }else if(strcmp(f.substr(2, end-2).c_str(),wf.c_str())==0){
            //cout <<  "El ajuste WF" << endl;
            disk.ajuste='W';
            disco.dsk_fit='W';
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
    }else{
        int end = u.find(" ");
        cout <<  "La unidad es" << endl;
        disk.unidades=u.substr(2, end-2);
        string m="m";
        string k="k";
        if(strcmp(disk.unidades.c_str(),m.c_str())==0){
            //cout <<  "El ajuste BF" << endl;
            megas=true;
        }else if(strcmp(disk.unidades.c_str(),k.c_str())==0){
            //cout <<  "El ajuste FF" << endl;
        }else{
            cout <<  "La unidad es incorrecta" << endl;
            return;
        }
        if(megas){
            disco.mbr_tamano=disco.mbr_tamano*1024*1024;
        }else{
            disco.mbr_tamano=disco.mbr_tamano*1024;
        }
        cout <<  disk.unidades << endl;
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
        }
    }
};

void fdisk(string linea){
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


    //-add
    string add=split(limpio,"-add");
    if(strcmp(limpio.c_str(),add.c_str())==0){
        cout <<  "El parametro -add no esta" << endl;
    }else{
        int end = add.find(" ");
        part.add=stoi(add.substr(2, end-2));
        cout <<  "ADD" << endl;
        cout << part.add << endl;
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
        }else if(strcmp(part.unidades.c_str(),k.c_str())==0){
            //cout <<  "El ajuste FF" << endl;
            part.tamanio=part.tamanio*1024;
        }else if(strcmp(part.unidades.c_str(),b.c_str())==0){
            //cout <<  "El ajuste FF" << endl;
        }else{
            cout <<  "La unidad es incorrecta" << endl;
            return;
        }
        cout <<  part.unidades << endl;
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
        part.ajuste='F';
    }else{
        int end = f.find(" ");
        part.ajuste=f.substr(2, end-2);
        string bf="BestFit";
        string ff="FirstFit";
        string wf="WorstFit";
        if(strcmp(f.substr(2, end-2).c_str(),bf.c_str())==0){
            //cout <<  "El ajuste BF" << endl;
            part.ajuste="BestFit";
        }else if(strcmp(f.substr(2, end-2).c_str(),ff.c_str())==0){
            //cout <<  "El ajuste FF" << endl;
            part.ajuste="BestFit";
        }else if(strcmp(f.substr(2, end-2).c_str(),wf.c_str())==0){
            //cout <<  "El ajuste WF" << endl;
            part.ajuste="WorstFit";
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
        string completo="Full";
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
                    strcpy(logica.part_name, part.name.substr(1,16).c_str());
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
            if(discoAux.mbr_partition_1.part_s==0){
                discoAux.mbr_partition_1.part_status='n';
                discoAux.mbr_partition_1.part_type=part.tipo.c_str()[0];
                discoAux.mbr_partition_1.part_fit=part.ajuste.c_str()[0];
                if(discoAux.mbr_partition_2.part_s+discoAux.mbr_partition_3.part_s+discoAux.mbr_partition_4.part_s==0){
                    discoAux.mbr_partition_1.part_start=0;
                }//else if()

                discoAux.mbr_partition_1.part_s=part.tamanio;
                strcpy(discoAux.mbr_partition_1.part_name, part.name.substr(0,15).c_str());
            }else if(discoAux.mbr_partition_2.part_s==0){
                discoAux.mbr_partition_2.part_status='n';
                discoAux.mbr_partition_2.part_type=part.tipo.c_str()[0];
                discoAux.mbr_partition_2.part_fit=part.ajuste.c_str()[0];
                discoAux.mbr_partition_2.part_start=0;
                discoAux.mbr_partition_2.part_s=part.tamanio;
                strcpy(discoAux.mbr_partition_2.part_name, part.name.substr(0,15).c_str());
            }else if(discoAux.mbr_partition_3.part_s==0){
                discoAux.mbr_partition_3.part_status='n';
                discoAux.mbr_partition_3.part_type=part.tipo.c_str()[0];
                discoAux.mbr_partition_3.part_fit=part.ajuste.c_str()[0];
                discoAux.mbr_partition_3.part_start=0;
                discoAux.mbr_partition_3.part_s=part.tamanio;
                strcpy(discoAux.mbr_partition_3.part_name, part.name.substr(0,15).c_str());
            }else if(discoAux.mbr_partition_4.part_s==0){
                discoAux.mbr_partition_4.part_status='n';
                discoAux.mbr_partition_4.part_type=part.tipo.c_str()[0];
                discoAux.mbr_partition_4.part_fit=part.ajuste.c_str()[0];
                discoAux.mbr_partition_4.part_start=0;
                discoAux.mbr_partition_4.part_s=part.tamanio;
                strcpy(discoAux.mbr_partition_4.part_name, part.name.substr(0,15).c_str());
            }else{
                cout <<  "Particiones llenas" << endl;
                return;
            }



            FILE *file;
            file = fopen(part.ruta.c_str(), "rb+");
            int posicion=0;
            int existe=1;
            bool primero=false;
            //MBR aux;
            fseek(file, 0, SEEK_SET);
            //int contador=0;
            //while(existe==1){
            //    fread(&aux, sizeof(MBR), 1, file);
            //    if (aux.mbr_dsk_signature==0){
            //        existe=0;
            //    }
            //    contador++;
            //}
            //fseek(file, ftell(file)-sizeof(MBR), SEEK_SET);


            fwrite(&discoAux, sizeof(MBR), 1, file);

            fclose(file);
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
        }
    }else{
        cout <<  "El disco no existe" << endl;
    }
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
}

void unmount(string linea){
    Particion part;
    size_t pos = linea.find(" ");
    string limpio = linea.substr (pos+1);
    //-f
    string f=split(limpio,"-id");
    if(strcmp(limpio.c_str(),f.c_str())==0){
        cout <<  "No esta el parametro id" << endl;
        return;
    }else{
        int end = f.find(" ");
        part.ajuste=f.substr(2, end-2);

        cout <<  "El ID es" << endl;
        cout <<  part.ajuste << endl;
    }

}

void mkfs(string linea){
    Particion part;
    size_t pos = linea.find(" ");
    string limpio = linea.substr (pos+1);
    //-f
    string f=split(limpio,"-id");
    if(strcmp(limpio.c_str(),f.c_str())==0){
        cout <<  "No esta el parametro id" << endl;
        return;
    }else{
        int end = f.find(" ");
        part.ajuste=f.substr(2, end-2);

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
        }else{
            cout <<  "El fs  no es disponible" << endl;
        }
    }
}

MBR LeerDisco(string ruta){

    FILE *file;
    file = fopen(ruta.c_str(), "rb");

    fseek(file, 0, SEEK_SET);


    MBR aux;
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
    // Obtener línea de archivo, y almacenar contenido en "linea"
    while (getline(archivo, linea)) {
        int espacio=linea.find(" ");
        string temporal=linea.substr(0,espacio);
        if(temporal=="mkdisk"){
            string tem=linea.substr(espacio+1);
            mkdisk(linea);
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
    EBR aux;
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