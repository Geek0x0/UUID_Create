all: uuid_create

uuid_create:main.c 
	gcc main.c libsqlite3.a libuuid.a -o uuid_create -lpthread -ldl

clean:
	rm uuid_create uuid.db
