build_client:
	gcc -o obj/fooclient src/fooclient.c src/foo_clnt.c src/foo_xdr.c -lnsl
build_server:
	gcc -o obj/fooserver src/fooserv.c src/foo_svc.c src/foo_xdr.c -lrpcsvc -lnsl
run_client:
	obj/fooclient localhost
run_server:
	obj/fooserver
clean:
	rm -rf obj/*
