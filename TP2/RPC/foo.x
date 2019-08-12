struct autenticacion{
	string nombreUsuario<128>;
	string pass<128>;
};

struct estadisticas{
	int cantOk;
	int cantUsuarioIncorrecto;
	int cantPassIncorrecta;
	int cantTotal;
};

program UIDPROG{
	version UIDVERS{
		int autenticar(autenticacion) = 1;
		estadisticas getEstadisticas() = 2;
	} = 1;
} = 0x20000001;