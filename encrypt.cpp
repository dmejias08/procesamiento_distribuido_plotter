/*
  Encriptado de textos.
  (C) Noviembre de 2.000 
  Jorge Eduardo Cordero Rodriguez 
*/

#include <stdio.h>
#include <conio.h>
#include <iostream.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    FILE *arch;
    char encriptado[100],desencriptado[100], r, pas[11];
    int longitud,i;


    arch=fopen("Datos.txt","w");

    clrscr();

    cout<<"Escribe el texto que deseas encriptar : ";
    cin.getline(encriptado,100);

    longitud = strlen(encriptado);

    for ( i = 0;i<longitud;i++)
    {
       encriptado[i] = char(toascii(encriptado[i])+122);
    }
    fprintf(arch,"%s",encriptado);
    cout<<"\n\nEl texto encriptado es "<<encriptado;

    cout<<"\n\nDeseas desencriptar el texto s/n ";
    cin>>r;
    if (toupper(r)=='S')
    {
       cout<<"\n\nDame la clave de acceso para desencriptar : ";
       cin>>pas;
       if (strcmp(pas,"C-con-Clase")==0)
       {
          cout<<"\n\nEl texto desencriptado es ";
          for(i=0;i<longitud;i++)
          {
             desencriptado[i]=char(encriptado[i])-122;
             cout<<desencriptado[i];
          }
       }
       else
       {
          cout<<"\n\n\n  Lo sentimos mucho, no esta autorizado para ver el archivo....";
          getch();
       }
    }
    else cout<<"\n\nGracias por utiliza nuestro programa...";
    getchar();
    fclose(arch);

    return(0);
}