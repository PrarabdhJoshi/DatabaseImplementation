#ifndef DBFILE_H
#define DBFILE_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "string.h"
#include "stdio.h"
typedef enum {heap, sorted, tree} fType;

// stub DBFile header..replace it with your own DBFile.h

class DBFile {

File *heapfile;
char *fpath;
Page *readbuffer; //reading files from user given as input to be sent into dbfile 
Page *writebuffer; //for getting records from dbfile, writing them as output 
int indicator; //indicator to the current page
bool verify;  //boolean operator

public:
	DBFile ();
	~DBFile();

	int Create (const char *fpath, fType file_type, void *startup);
	int Open (const char *fpath);
	int Close ();

	void Load (Schema &myschema, const char *loadpath);

	void MoveFirst ();
	void Add (Record &addme);
	int GetNext (Record &fetchme);
	int GetNext (Record &fetchme, CNF &cnf, Record &literal);

};
#endif
