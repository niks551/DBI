#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <fstream>
#include <fcntl.h>
#include <iostream>

// stub file .. replace it with your own DBFile.cc

int fileExists(const char *fileName)
{
    ifstream infile(fileName);
    return infile.good();
}

DBFile::DBFile () {

	this->currentRecord = new (std::nothrow) Record();
	theFile = File();
	currentPage = Page();

}
DBFile::~DBFile () {

	delete this->currentRecord;

}

int DBFile::Create (char *f_path, fType f_type, void *startup) {

//	// Open the actual file
//	fileStream.open(f_path,ios::binary);
//	fileType = heap;
//
//	// Check if the file is readable
//	if(this->fileStream.good())
//	{
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
	File newfile = File();
	newfile = this->theFile;
	newfile.Open(0,f_path);

}

void DBFile::Load (Schema &f_schema, char *loadpath) {

	   FILE *loadFile;

	   loadFile = fopen (loadpath , "r");
	   if (loadFile == NULL)
		   perror ("Error opening .tbl file to be loaded");

	   int pageNumber = 0;
	   // Get the Record from the .tbl File
    while((*(this->currentRecord)).SuckNextRecord(&f_schema,loadFile) == TRUE)
    {
    	// Try to Append a record to the currentPage
    	if(currentPage.Append(this->currentRecord) == TRUE)
    	{
    		// Keep Adding records
    	}
    	// If page is full add it to theFile, empty it and start creating a new page
    	else
    	{
    		theFile.AddPage(&currentPage, pageNumber);
    		pageNumber++;
    		currentPage.EmptyItOut();
    		currentPage.Append(this->currentRecord);
    	}
    }

}

int DBFile::Open (char *f_path) {

	// Open the actual file
//	this->fileStream.open(f_path,ios::binary);
//
//	if(this->fileStream.good() && this->fileStream.is_open())
//		return 1;
//	else
//		return 0;
	// ** Should read the metadata and save the fileType **
	off_t length;
	length = theFile.GetLength();
	theFile.Open(length,f_path);
	if(fileExists(f_path))
		return 1;
	else
		return 0;

}

void DBFile::MoveFirst () {

	Page *firstPage;
	firstPage = new (std::nothrow) Page();
	// Get the first page from the file
	theFile.GetPage(firstPage,0);
	firstPage->GetFirst(currentRecord);

	curPageNumber =0;
	curRecordNumber =0;
	delete firstPage;
}

int DBFile::Close () {

	if(theFile.GetLength() == theFile.Close())
		return 1;
	else
		return 0;
}

void DBFile::Add (Record &rec) {

	Page *lastPage;
	lastPage = new (std::nothrow) Page();
	// Get and Remove the Last Page from the file
	theFile.GetPage(lastPage,theFile.GetLength());

	// Check if the Page has space for one more record
	if(lastPage->Append(&rec))
	{
		// Removing the last page from File
		theFile.AddPage(lastPage,theFile.GetLength());
	} else
	{
		lastPage->EmptyItOut();
		lastPage->Append(&rec);
		theFile.AddPage(lastPage,theFile.GetLength());
	}
	this->currentRecord = &rec;
	delete &rec;

}

int DBFile::GetNext (Record &fetchme) {
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
}
