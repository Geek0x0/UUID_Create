#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uuid.h"
#include "sqlite3.h"

#define UUDI_TOTAL 10000

sqlite3 *DB;

int create_database()
{
	int err;
	char *dberr = NULL;
	char *exec = "create table uuid_table(uuid text);";

	err = sqlite3_open("./uuid.db", &DB);
	if (err) 
    {
		fprintf(stderr, "Can't Open Database, err=%d\n", err);
		return -1;
	}
	sqlite3_exec(DB, "PRAGMA synchronous = OFF; ", 0, 0, &dberr);
    err = sqlite3_exec(DB, exec, 0, 0, &dberr);
    if(err)
    {
        printf("create table uuid error, %d\n", err);
        printf("err: %s\n", dberr);
        sqlite3_free(dberr);
        return (-1);
    }
}

int serach_database(char *uuid_string)
{
    int err, nrow = 0, ncolumn = 0;
    char *dberr = NULL, **result = NULL;
    char *exec_f = "select * from uuid_table where uuid='%s';";
    char *exec = calloc(1024, sizeof(char));

    sprintf(exec, exec_f, uuid_string);
    
    err = sqlite3_get_table(DB, exec, &result, &nrow, &ncolumn, &dberr);
    sqlite3_free_table(result);
    free(exec);
    if(err)
    {
        printf("select table uuid error, %d\n", err);
        printf("err: %s\n", dberr);
        sqlite3_free(dberr);
        return (-1);
    }

    if(nrow && ncolumn)
        return 1;
    else
        return 0;
}

int insert_database(char *uuid_string)
{
    int err;
    char *dberr = NULL;
    char *exec_f = "insert into uuid_table (uuid) values('%s');";
    char *exec = calloc(1024, sizeof(char));

    sprintf(exec, exec_f, uuid_string);

    err = sqlite3_exec(DB, exec, 0, 0, &dberr);
    free(exec);
    if(err)
    {
        printf("insert table uuid error, %d\n", err);
        printf("err: %s\n", dberr);
        sqlite3_free(dberr);
        return (-1);
    }

    return 0;
}

void create_random_uuid(char *uuid_string)
{
	char buff[2] = { 0 };
	long int i, n;
	uuid_t uuid;

	memset(uuid_string, 0, sizeof(uuid));

	uuid_generate_random(uuid);

	for (i = 0; i < sizeof(uuid); i++) {
		sprintf(buff, "%X", uuid[i]);
		n = strtol(buff, NULL, 16);
		if (n < 16)
			sprintf(buff, "0%X", n);
		strncat(uuid_string, buff, 2);
	}
}

int main(void)
{
    int err, loop, count = UUDI_TOTAL;
	char uuid[32] = { 0 };

    err = create_database();
    if(err < 0)
        return (-1);

    loop = 0;
    while(count)
    {
        loop++;
	    create_random_uuid(uuid);
        err = serach_database(uuid);
        if(!err)
        {
            insert_database(uuid);
            count--;
        }
    }
    sqlite3_close(DB);

    return 0;
}
