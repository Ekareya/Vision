#ifndef _IMAGE_H
#define _IMAGE_H

#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <sstream> // stringstream
#include <cmath>
#include <cassert>
#include <algorithm>
#include <string>
#include <vector>
//#include "Dialogue.h"
#include "Masque.h"
using namespace std;



class Image
{

private:
////////////////////////////////////////////////////////////////////////////////////
// Attributs

	unsigned int numcols;
	unsigned int numrows;
	unsigned int vmax;
	string name;
	vector<int> buffer;

public:

////////////////////////////////////////////////////////////////////////////////////
// Constructs

	Image(unsigned int const col,unsigned int const lig);
	Image(unsigned int const col,unsigned int const lig,unsigned int const vmax);
	Image(unsigned int const col,unsigned int const lig,unsigned int max, string nom);
	Image(char* filename);
	~Image(void);

////////////////////////////////////////////////////////////////////////////////////
// Getter

	int const & at(unsigned int col, unsigned int lig) const;
	unsigned int getNumcols()const ;
	unsigned int getNumrows()const ;
	unsigned int getVmax()const ;
	string getName()const;
	vector<int> getBuffer()const;

////////////////////////////////////////////////////////////////////////////////////
// Setter

	int & at(unsigned int col, unsigned int lig);
	void setVmax(unsigned int i);

////////////////////////////////////////////////////////////////////////////////////
// Surcharge operateur

	int const & operator()(unsigned int col, unsigned int lig)const;
	int & operator()(unsigned int col, unsigned int lig);
	void operator*= (Masque masque);
	
	
////////////////////////////////////////////////////////////////////////////////////
// Tool

	void afficher(ostream &flux);
	void afficher();
	void save(const char* filename);
	void save();
	void reset();

////////////////////////////////////////////////////////////////////////////////////
// Method

	void seuil(int inf,int sup);
	void negatif();
	void binaire(int seuil);
	vector<int> getContour(int comp);

	

	vector<int> getCoord(int pixel);
	//vector<unsigned int> voisin( unsigned int pixel);

		
	
////////////////////////////////////////////////////////////////////////////////////
// 

	Image compConnexe();




};

ostream& operator<<( ostream &flux, Image & img);
vector<int> getVoisin(int i);

#endif
