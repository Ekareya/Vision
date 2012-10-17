#ifndef _MASQUE_H
#define _MASQUE_H

#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
using namespace std;

class Masque
{
private:
	double coef;
	int size;
	vector<double> buffer;

public:
	Masque(double Coef, int Size,double* Buffer);
	Masque(string name);
	~Masque(void);
	void afficher(ostream &flux);
	double const & at( int col, int lig) const;
	double & at(int col,  int lig);
	
	double const & operator()( int col, int lig)const;
	double & operator()( int col, int lig);

	int getSize();
	double getCoef();
};

ostream& operator<<( ostream &flux, Masque & mas);

#endif 