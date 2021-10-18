#include <Windows.h>
#include "graphics.h"
#include "MathAdv.h"
#include <conio.h>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <time.h>
#include <stdlib.h>
#include <sstream>

using namespace std;

struct Coords
{
	int X;
	int Y;
};

//---------------------------------------------------------
struct console
{
	console(unsigned width, unsigned height)
	{
		SMALL_RECT r;
		COORD c;
		hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!GetConsoleScreenBufferInfo(hConOut, &csbi))
			throw runtime_error("You must be attached to a human.");
		r.Left = 0;
		r.Top = 0;
		r.Right = width - 1;
		r.Bottom = height - 1;
		SetConsoleWindowInfo(hConOut, TRUE, &r);

		c.X = width;
		c.Y = height;
		SetConsoleScreenBufferSize(hConOut, c);
	}

	~console()
	{
		SetConsoleTextAttribute(hConOut, csbi.wAttributes);
		SetConsoleScreenBufferSize(hConOut, csbi.dwSize);
		SetConsoleWindowInfo(hConOut, TRUE, &csbi.srWindow);
	}
	HANDLE hConOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
};

console con(80, 40);
//------------------------------------------------------------------------------

class PlanoCartesiano
{
	Coords Centro; // Center of the axis
	bool A; // variable for do a "do once"
	int XLineas; // useless variable for now

public:
	PlanoCartesiano(Coords Cor1, Coords Cor2, Coords CP1, Coords CP2); // another constructor is needed
	void CRectangle(Coords Cor1, Coords Cor2); // create the perimeter
	void eje(Coords Cor1, Coords Cor2); // create the axis and the measures
	void Vectores(Coords CP1, Coords CP2); // Graph the vectors and get their stats
	void NoSe(Coords CP1, Coords CP2, Coords P0, double A1, double A2); // I dont know
	// is a joke, adapt the arc radius acording to the vectors distance (needs to be fixed)


	/* the function format is like this: F(Start coords, End coords, number of iterations(Horizontal zoom of the function), 

	Identificator of the respective function... sinus id is 1, tangent id is 2, secant id is 3... if you want the 90° desviation of each function,
	only need to sum 90 + Id (for example, for get de cotangent the id is 92), for get the trigonometrig limit of each functions only need to concatenate
	"2" to the respective id (for get the limit of secant you need y}to type 32, and for get the limit of the cosinus tou need to type 912)*/
	void Function(Coords CP1, Coords CP2, float iterator, int ID);

	double CalDist(Coords CP1, Coords CP2); // calcule the distance between tho vectors
	double CalAng(Coords CP1, int x, int y, bool Ft); // cancule the angle between twho vectors acording to the point 0
}; 

PlanoCartesiano::PlanoCartesiano(Coords Cor1, Coords Cor2, Coords CP1, Coords CP2)
{
	XLineas = 0;

	PlanoCartesiano::CRectangle(Cor1, Cor2);
	PlanoCartesiano::eje(Cor1, Cor2);
	Centro.X = (Cor2.X / 2) + (Cor1.X / 2);
	Centro.Y = (Cor2.Y / 2) + (Cor1.Y / 2);
	//Vectores(CP1, CP2);

	//arc(Centro.X, Centro.Y, 0, 360, 100); 

	//sinus(Cor1, Cor2, 10, true, false, false, false);
	//a_sec(Cor1, Cor2, 10, true, false, false, false);
	//a_tan(Cor1, Cor2, 10, true, false, false, false);
}

void PlanoCartesiano::CRectangle(Coords Cor1, Coords Cor2)
{
	line(Cor1.X, Cor1.Y, Cor2.X, Cor1.Y);
	line(Cor1.X, Cor1.Y, Cor1.X, Cor2.Y);
	line(Cor2.X, Cor2.Y, Cor1.X, Cor2.Y);
	line(Cor2.X, Cor2.Y, Cor2.X, Cor1.Y);
}

void PlanoCartesiano::eje(Coords Cor1, Coords Cor2)
{
	line(Cor1.X, (Cor2.Y / 2) + (Cor1.Y / 2), Cor2.X, (Cor2.Y / 2) + (Cor1.Y / 2));
	line((Cor2.X / 2) + (Cor1.X / 2), Cor1.Y, (Cor2.X / 2) + (Cor1.X / 2), Cor2.Y);

	for (int i = Cor1.X; i < Cor2.X + (Cor1.X / 2); i += 10)
	{
		line(i, ((Cor2.Y / 2) + (Cor1.Y / 2)) + 3, i, ((Cor2.Y / 2) + (Cor1.Y / 2)) - 3); // graph the medition lines of the horizontal line (each 10 points)
		XLineas++;
	}

	for (int i = Cor1.Y; i < Cor2.Y + (Cor1.Y / 2); i += 10)
	{
		line(((Cor2.X / 2) + (Cor1.X / 2)) + 3, i, ((Cor2.X / 2) + (Cor1.X / 2)) - 3, i); // graph the medition lines of the vertical line (each 10 points)
	}
}

void PlanoCartesiano::Vectores(Coords CP1, Coords CP2)
{
	// graph the vectors
	line(Centro.X, Centro.Y, CP1.X * 10 + 405, CP1.Y * -10 + 305);
	line(Centro.X, Centro.Y, CP2.X * 10 + 405, CP2.Y * -10 + 305);
	circle(CP1.X * 10 + 405, CP1.Y * -10 + 305, 10);
	circle(CP2.X * 10 + 405, CP2.Y * -10 + 305, 10);

	Coords P0; P0.X = 0; P0.Y = 0;

	double CP1A = CalAng(CP1, 11, 0, true), CP2A = CalAng(CP2, 11, 0, true); // get the angle acording to 0

	if (CP1A < CP2A)
	{
		NoSe(CP1, CP2, P0, CP1A, CP2A);
	}
	else
	{
		NoSe(CP1, CP2, P0, CP2A, CP1A);
	}


	if (A != true)
	{

		double L1 = CP1.X * CP2.X, L2 = CP1.Y * CP2.Y;
		double Cos1 = L1 + L2;
		double Cos2 = (sqrt((pow(CP1.X, 2) + pow(CP1.Y, 2))) * sqrt((pow(CP2.X, 2) + pow(CP2.Y, 2))));
		double Cos = Cos1 / Cos2;
		
		double Res = acos(Cos) * 180 / 3.141592653589793;

		// stats
		string CoordsP1 = "P1: " + std::to_string(CP1.X) + ", " + std::to_string(CP1.Y);
		string CoordsP2 = "       P2: " + std::to_string(CP2.X) + ", " + std::to_string(CP2.Y);
		string Angle = "       Angle: " + std::to_string(Res);
		string Dis = "       Distance: " + std::to_string(CalDist(CP2, P0));

		outtextxy(10, 650, CoordsP1);
		outtextxy(10, 670, CoordsP2);
		outtextxy(10, 690, Angle);
		outtextxy(10, 710, Dis);

		A = true;
	}
}

void PlanoCartesiano::NoSe(Coords CP1, Coords CP2, Coords P0, double A1, double A2)
{
	if (CalDist(CP1, P0) < 5.6 || CalDist(CP2, P0) < 5.6)
	{
		if (CalDist(CP1, P0) < CalDist(CP2, P0))
		{
			if (CP1.X > CP1.Y)
			{
				arc(Centro.X, Centro.Y, A1, A2, 10 * CP1.X);
			}
			else {
				arc(Centro.X, Centro.Y, A1, A2, 10 * CP1.Y);
			}
		}
		else {
			if (CP2.X > CP2.Y)
			{
				arc(Centro.X, Centro.Y, A1, A2, 10 * CP2.X);
			}
			else {
				arc(Centro.X, Centro.Y, A1, A2, 10 * CP2.Y);
			}
		}
	}
	else {
		arc(Centro.X, Centro.Y, A1, A2, 100);
	}
}

double PlanoCartesiano::CalDist(Coords CP1, Coords CP2)
{

	// pitagoras theorem
	double Cat1 = CP1.X - CP2.X, Cat2 = CP2.Y - CP1.Y;

	if (Cat1 < 0)
	{
		Cat1 *= -1;
	}

	if (Cat2 < 0)
	{
		Cat2 *= -1;
	}

	double Dist = sqrt(pow(Cat1, 2) + pow(Cat2, 2));

	return Dist;
}

double PlanoCartesiano::CalAng(Coords CP1, int x, int y, bool Ft)
{
	double L1 = CP1.X * x, L2 = CP1.Y * y;
	double Cos1 = L1 + L2;
	double Cos2 = (sqrt((pow(CP1.X, 2) + pow(CP1.Y, 2))) * sqrt((pow(11, 2) + pow(0, 2))));
	double Cos = Cos1 / Cos2;

	double Res = acos(Cos) * 180 / 3.141592653589793;

	if (((CP1.X > 0 && CP1.Y < 0) || (CP1.X < 0 && CP1.Y < 0)) && Ft)
	{
		Res = 180 + CalAng(CP1, -11, 0, false);
	}

	return Res;
}

void PlanoCartesiano::Function(Coords CP1, Coords CP2, float iterator, int ID)
{
	
	double Pos[1000]; // save each result 
	float Conti = -380; int J = 0; // this wrong

	for (int i = -38; i <= 39; i++)
	{
		switch (ID)
		{
			case 1:
				Pos[J] = sin(Conti * 3.141592653589793 / 180); // calcule all on radians
			break;

			case 91:
				Pos[J] = cos(Conti * 3.141592653589793 / 180); // calcule all on radians
			break;

			case 12:
				Pos[J] = (sin(Conti * 3.141592653589793 / 180)) / (Conti * 3.141592653589793 / 180);
			break;

			case 912:
				Pos[J] = (cos(Conti * 3.141592653589793 / 180)) / (Conti * 3.141592653589793 / 180);
			break;

			case 2:
				Pos[J] = tan(Conti * 3.141592653589793 / 180); // calcule all on radians
			break;

			case 92:
				Pos[J] = cotan(Conti * 3.141592653589793 / 180); // calcule all on radians
			break;

			case 22:
				Pos[J] = (tan(Conti * 3.141592653589793 / 180)) / (Conti * 3.141592653589793 / 180);
			break;

			case 922:
				Pos[J] = (cotan(Conti * 3.141592653589793 / 180)) / (Conti * 3.141592653589793 / 180);
			break;

			case 3:
				Pos[J] = sec(Conti * 3.141592653589793 / 180); // calcule all on radians
			break;

			case 93:
				Pos[J] = csc(Conti * 3.141592653589793 / 180); // calcule all on radians
			break;

			case 32:
				Pos[J] = (sec(Conti * 3.141592653589793 / 180)) / (Conti * 3.141592653589793 / 180);
			break;

			case 932:
				Pos[J] = (csc(Conti * 3.141592653589793 / 180)) / (Conti * 3.141592653589793 / 180);
			break;
		}

		Conti += iterator;
		J++;
	}

	J = 0;
	for (float i = CP1.X; i < CP2.X; i+=10)
	{
		line(i, ((CP2.Y / 2) + (CP1.Y / 2)) + (Pos[J] * 100), i + 10, ((CP2.Y / 2) + (CP1.Y / 2)) + (Pos[J + 1] * 100));
		//circle(i, (CP2.Y / 2) + (Pos[J] * 100), 10); // a graph with each point
		J++;
	}
}

// needed functions of graphics.h
void line(int x1, int y1, int x2, int y2);
void initgraph();
void circle(int x, int y, int radius);
void arc(int x, int y, int stangle, int enangle, int radius);
void outtextxy(int, int, string);

int main()
{

	initgraph();

	Coords CP1, CP2;
	srand(time(NULL));
	
	// Generate random coords between -38 and 38 (x),  -28 and 28 (y)
	CP1.X = -38 + rand() % (77);
	CP1.Y = -28 + rand() % (57);
	CP2.X = -38 + rand() % (77);
	CP2.Y = -28 + rand() % (57);
	
	//system("pause");

	do
	{
		
		Coords C1, C2;
		C1.X = 20; C1.Y = 20; C2.X = 780; C2.Y = 580; // 780 = (38 * 2 * 100) + 20......580 = (28 * 2 * 100) + 20
		PlanoCartesiano Plano(C1, C2, CP1, CP2);
		
	
	} while (true != false); // infinite bucle lol


	return 0;
}



