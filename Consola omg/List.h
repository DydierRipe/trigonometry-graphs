#pragma once
#include "graphics.h"

struct Data
{
	int Coordinates;
	std::shared_ptr<Data> Next;
};

class Cola
{
public:
	void Insert(std::shared_ptr<Data> &Front, std::shared_ptr<Data> &End, int Number);
	int  ShowAndDelete(std::shared_ptr<Data> &Front, std::shared_ptr<Data> &End);

	bool voidData(std::shared_ptr<Data>  Front);

	~Cola();
};

void Cola::Insert(std::shared_ptr<Data> &Front,std::shared_ptr<Data> &End, int Number)
{
	std::shared_ptr<Data>  NewData (new Data());

	NewData->Coordinates = Number;
	NewData->Next = NULL;

	if (voidData(Front))
	{
		Front = NewData;
	}
	else {
		End->Next = NewData;
	}

	End = NewData;
}

int Cola::ShowAndDelete(std::shared_ptr<Data> &Front,std::shared_ptr<Data> &End)
{
	int Number = Front->Coordinates;

	if (Front == End)
	{
		End = NULL;
		Front = NULL;
	}
	else {
		Front = Front->Next;
	}

	return Number;
}

bool Cola::voidData(std::shared_ptr<Data>  Front) { return (Front == NULL) ? true : false;}

Cola::~Cola() {}

