/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "foo.h"

int cantOk;
int cantUsuarioIncorrecto;
int cantPassIncorrecta;
int cantTotal;

int * autenticar_1_svc(autenticacion *argp, struct svc_req *rqstp)
{
	static int result;
	int n;
	char *nombreUsuario, *pass;
	int usuarioOk=0,i=0;

	nombreUsuario = malloc(128);
	pass = malloc(128);

	cantTotal++;
	
	if(freopen("usuarios.txt","r",stdin)==NULL)
	{
		result=2;
		return &result;
	}
	scanf("%d",&n);
	while(i<n)
	{
		scanf("%s",nombreUsuario);
		scanf("%s",pass);
		if(strcmp(nombreUsuario,argp->nombreUsuario)==0)
		{
			usuarioOk=1;
			if(strcmp(pass,argp->pass)==0)
			{
				cantOk++;
				result=0;
				return &result;
			}
		}
		i++;
	}
	if(usuarioOk) cantPassIncorrecta++;
	else cantUsuarioIncorrecto++;
	result=1;
	return &result;
}

estadisticas * getestadisticas_1_svc(void *argp, struct svc_req *rqstp)
{
	static estadisticas  result;

	result.cantOk=cantOk;
	result.cantUsuarioIncorrecto=cantUsuarioIncorrecto;
	result.cantPassIncorrecta=cantPassIncorrecta;
	result.cantTotal=cantTotal;

	return &result;
}
