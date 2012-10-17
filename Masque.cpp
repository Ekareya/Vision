#include "Masque.h"

using namespace std;

Masque::Masque(double Coef, int Size,double* Buffer):coef(Coef),size(Size),buffer(Size*Size,0)
{
	cout << "creation d'un masque" <<endl << "coef: " << Coef <<endl << "Taille: "<<Size <<endl << "Buffer: ";
	for(int i =0; i<size*size;i++)
		buffer[i]=Buffer[i];

	cout << "size:" << size << endl << "coef:" << coef << endl;
}

Masque::Masque(string name)
{


	if(!name.compare("gaussian"))
	{
	  double G[]={1.,2.,1.,2.,4.,2.,1.,2.,1.};
      Masque(1./16.,3,G);
	}
	else if(!name.compare("gradx"))
	{ 
	  double G[]={-1.,0.,1.,-2.,0.,2.,-1.,0.,1.};
      Masque(1.,3,G);
	}
	else
	{coef=1;size=3;buffer.assign(9,0);}
}

Masque::~Masque(void)
{
}
	double const & Masque::at(int col, int lig) const
	{
	assert(col<=size/2 && col >= -size/2);
	assert(lig<=size/2 && lig >= -size/2);
	return buffer[(lig+size/2)*size+col+size/2];
	}

	double & Masque::at(int col, int lig)
	{
	assert(col<=size/2 && col >= -size/2);
	assert(lig<=size/2 && lig >= -size/2);
	return  buffer[(lig+size/2)*size+col+size/2];
	}
	double const & Masque::operator()(int col,int lig)const
	{return this->at(col,lig);}
	double & Masque::operator()(int col,int lig)
	{return this->at(col,lig);}


void Masque::afficher(ostream &flux)
{

    flux << "size:" << size << endl << "coef:" << coef << endl;
	for(int lig = -size/2; lig <= size/2; lig++)
		{for (int col = -size/2; col <= size/2; col++) 
			flux << at(col,lig) <<" ";
		flux <<endl;
		}
}

int Masque::getSize()
{return size;}

double Masque::getCoef()
{return coef;}

ostream& operator<<( std::ostream &flux, Masque & mas )
{
	mas.afficher(flux);
	return flux;
}