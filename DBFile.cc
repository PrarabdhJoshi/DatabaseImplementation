#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include "string.h"
#include "stdio.h"
#include <iostream>
using namespace std;

// stub file .. replace it with your own DBFile.cc

DBFile::DBFile() {

	heapfile = new File;
	fpath = NULL;
	readbuffer = new Page;
	writebuffer = new Page;
	indicator = 0;
	verify = false;
}


DBFile::~DBFile() {
	delete(heapfile);
	delete(readbuffer);
	delete(writebuffer);
}

int DBFile::Create(const char *f_path, fType f_type, void *startup) {

	if (verify)
		return 0;
	fpath = strdup(f_path);
	heapfile->Open(0, fpath);
	verify = true;
	return 1;
}

void DBFile::Load(Schema &f_schema, const char *loadpath) {
	Record temprecord;
	FILE *tempfile = new FILE;
	tempfile = fopen(loadpath, "r");
	if (!verify)
		return;
	while (temprecord.SuckNextRecord(&f_schema, tempfile))
	{
		Add(temprecord);
	}
}

int DBFile::Open(const char *f_path) {
	if (verify)
		return 0;
	fpath = strdup(f_path);
	heapfile->Open(1, fpath);
	verify = true;
	return 1;

}

void DBFile::MoveFirst() {
	if (readbuffer->totalrecords() != 0) {
		heapfile->AddPage(readbuffer, heapfile->Getlast());
		readbuffer->EmptyItOut();
	}
	writebuffer->EmptyItOut();
	heapfile->GetPage(writebuffer, indicator);
}

int DBFile::Close() {
	if (!verify)
		return 0;
	if (readbuffer->totalrecords() != 0)
	{
		heapfile->AddPage(readbuffer, heapfile->Getlast());
		readbuffer->EmptyItOut();
	}
	heapfile->Close();
	verify = false;
	return 1;

}

void DBFile::Add(Record &rec) {
	int temp = readbuffer->Append(&rec);
	if (temp == 0)
	{
		heapfile->AddPage(readbuffer, heapfile->Getlast());
		readbuffer->EmptyItOut();
		readbuffer->Append(&rec);
	}

}

int DBFile::GetNext(Record &fetchme) {

	if (readbuffer->totalrecords() != 0) {
		heapfile->AddPage(readbuffer, heapfile->Getlast());
		readbuffer->EmptyItOut();
	}
	while (!writebuffer->GetFirst(&fetchme))
	{
		if (++indicator == heapfile->Getlast())
			return 0;
		heapfile->GetPage(writebuffer, indicator);


	}
	return 1;
}



int DBFile::GetNext(Record &fetchme, CNF &cnf, Record &literal) {

	ComparisonEngine *compeng = new ComparisonEngine();
	while (GetNext(fetchme)) {
		if (compeng->Compare(&fetchme, &literal, &cnf) != 0)
			return 1;
	}
	return 0;
}

