#include "Image.h"

using namespace std;

//Dialogue debug=Dialogue(true);
//Dialogue talk=Dialogue(true);
ostream & debug=cout;
ostream & talk=cout;
////////////////////////////////////////////////////////////////////////////////////
// Constructs
	
Image::Image(unsigned int const col,unsigned int const lig):numcols(col),numrows(lig),vmax(1),name("defaut.pgm"),buffer(col*lig,0)
{}
Image::Image(unsigned int const col,unsigned int const lig,unsigned int max):numcols(col),numrows(lig),vmax(max),name("defaut.pgm"),buffer(col*lig,0)
{}
Image::Image(unsigned int const col,unsigned int const lig,unsigned int max, string nom):numcols(col),numrows(lig),vmax(max),name(nom),buffer(col*lig,0)
{}
//TODO handle comments
Image::Image(char* filename):name(filename)
{	

	ifstream file(filename);//flux representant le fichier
	
	string inputLine = "";

	/*
	Methode de lecture d'un fichier:
    Ligne par ligne en utilisant getline().
    Mot par mot en utilisant les chevrons >>.
    Caractère par caractère en utilisant get().
	*/
	
	getline(file,inputLine);
	if(inputLine.compare("P2") != 0) cerr << "Version error" << endl;
	else talk << "Version : " << inputLine << endl;

	getline(file,inputLine);
	talk << "Comment : " << inputLine << endl;
	
	file >> numcols >> numrows >> vmax;
	talk << numcols << " colonnes et " << numrows << " lignes" << endl << "valeur max:" << vmax << endl;

	int j=0;
	for(unsigned int i = 0; i < numrows*numcols; i++)
		{	file >> j;
			//assert(j<=(int)vmax);
			j=(j<=(int)vmax)? j:vmax;
			buffer.push_back(j);
			//debug << buffer[i];
		}
		
	debug <<endl << "creation du buffer OK" << endl;

	file.close();
		
	debug << "lecture du fichier OK" <<endl;
	
}

Image::~Image(void){}

////////////////////////////////////////////////////////////////////////////////////
// Getter

//get (3,5)
const int& Image::at(unsigned int col,unsigned int lig) const
{	assert(col<numcols);
	assert(lig<numrows);
	return buffer[lig*numcols+col];
}
unsigned int Image::getNumcols()const 
{	return numcols;}
unsigned int Image::getNumrows()const 
{	return numrows;}
unsigned int Image::getVmax()const 
{	return vmax;}
string Image::getName()const 
{	return name;}

vector<int> Image::getBuffer()const
{	return buffer;}
////////////////////////////////////////////////////////////////////////////////////
// Setter

//allow this.at(3,5)=3
int& Image::at(unsigned int col,unsigned int lig)
{	assert(col<numcols);
	assert(lig<numrows);
	return buffer[lig*numcols+col];
}
void Image::setVmax(unsigned int i)
{ vmax=i;}

////////////////////////////////////////////////////////////////////////////////////
// Surcharge operateur

//call at
int &Image::operator()(unsigned int col,unsigned int lig)
{	return this->at(col,lig);	
}
//call const at
 int const &Image::operator()(unsigned int col,unsigned int lig)const
{	return this->at(col,lig);	
}

std::ostream& operator<<( std::ostream &flux, Image & img )
{
	img.afficher(flux);
	return flux;
}
void Image::operator*= (Masque masque)
	 {
    	Image img = Image(numcols,numrows);
		int n = masque.getSize();
		double coef=masque.getCoef();
		cout << n << endl;
		//assert(n%2==1);
		for(unsigned int i=n/2; i<numcols-n/2;i++)
			for(unsigned int j=n/2; j<numrows-n/2;j++)	
				{	//cout <<"("<< i <<","<<j <<"):";
					for(int k=-n/2;k<=n/2;k++)
					{	for(int l=-n/2;l<=n/2;l++)
					 	{
					  		//cout <<"("<< k <<","<< l <<"):"; 
					 	 	img(i,j)+= coef*masque(k,l)*at(i+k,j+l);
					 		//cout << masque(k,l) << " | ";
					  }
					}
					//cout << at(i,j) << "," << img(i,j) <<endl;
				}
					  
    buffer= img.getBuffer();
	}
////////////////////////////////////////////////////////////////////////////////////
// Tool

void Image::afficher(std::ostream &flux)
{
    flux << "P2" << endl << "#" << name << endl << numrows << " " <<numcols << endl << vmax << endl;
	for(unsigned int lig = 0; lig < numrows; lig++)
		{for (unsigned int col = 0; col < numcols; col++) 
			flux << at(col,lig) <<" ";
		flux <<endl;
		}
}
void Image::save(const char* filename)
{
	name=filename;
	ofstream myfile (filename);
	if (myfile.is_open())
	{	
		myfile << *this;
		myfile.close();
		debug << "Sauvegarde du fichier OK";
	}
	else cout << "Unable to open file";
}
void Image::save()
{save(name.c_str());}

void Image::reset()
{}
////////////////////////////////////////////////////////////////////////////////////
// Method

void Image::seuil(int inf,int sup)
{
	assert(inf<=(int)vmax);
	assert(sup<=(int)vmax);
	assert(inf<=sup);
	for(unsigned int i=0; i < buffer.size();i++)
		buffer[i] = (buffer[i]<inf) ? inf : ((buffer[i]>sup) ? sup : buffer[i]);
}
void Image::negatif()
{
	for(unsigned int i=0; i < buffer.size();i++)
		buffer[i] = vmax-buffer[i];
}

void Image::binaire(int seuil)
{
	for(unsigned int i=0; i < buffer.size();i++)
		buffer[i] = (buffer[i]<seuil) ? 0 : 1;
	vmax=1;
}
vector<int> Image::getCoord(int pixel)
{
	vector <int> coord(2,0);
	coord[0]=pixel%numcols;
	coord[1]=pixel/numcols;
	return coord;
}

vector<int> getVoisin(int i)
{	vector<int> coord(2,0);
	switch(i%8)
	{
		case 0: coord[0]=1;coord[1]=0;break;
		case 1: coord[0]=1;coord[1]=-1;break;
		case 2: coord[0]=0;coord[1]=-1;break;
		case 3: coord[0]=-1;coord[1]=-1;break;
		case 4: coord[0]=-1;coord[1]=0;break;
		case 5: coord[0]=-1;coord[1]=1;break;
		case 6: coord[0]=0;coord[1]=1;break;
		case 7: coord[0]=1;coord[1]=1;break;
	}
	return coord;
}

vector<int> Image::getContour(int comp)
{
//assert(cadre autour de l'image)
vector<int> coord;
for(unsigned int i=0; i < buffer.size();i++)
	{if(buffer[i]==comp)
		{
		cout << "premier pixel: " << i << endl;
		coord=getCoord((int)i);
		cout << coord[0] << " | " << coord[1] << endl;
		break;
		}
	}
	vector<int> Contour = coord;
	vector<int> voisin(2,0),oldcoord(2,0);
	int maj[]={6,6,0,0,2,2,4,4};
	int k=4;

	
	for(int j=k;j<k+8;j++)
		{	
			voisin=getVoisin(j);
			if(at(coord[0]+voisin[0],coord[1]+voisin[1])==comp)
			{
				Contour.push_back(j%8);
				coord[0]+=voisin[0];coord[1]+=voisin[1];
				oldcoord=coord;
				cout << coord[0] << " | " << coord[1] << " | " << j%8 << endl;
				k=j%8;
				break;
			}
		}
	do
	{	
		k=maj[k];
		for(int j=k;j<k+8;j++)
		{	
			voisin=getVoisin(j);
			if(at(coord[0]+voisin[0],coord[1]+voisin[1])==comp)
			{
				Contour.push_back(j%8);
				oldcoord=coord;
				coord[0]+=voisin[0];coord[1]+=voisin[1];
				cout << coord[0] << " | " << coord[1] << " | " << j%8 << endl;
				k=j%8;
				break;
			}
		}
	}
	while(!(coord[0]==oldcoord[0] && coord[1]==oldcoord[1] && k==Contour[2]) );
	
	return Contour;
}
/*
vector<unsigned int> Image::voisin( unsigned int pixel)
{
	vector<unsigned int>p=getCoord(pixel);
	unsigned int px=p[0];
	unsigned int py=p[1];
	unsigned int nx=numcols-1;
	unsigned int ny=numrows-1;
	vector<unsigned int> liste;
	if(px > 0 && px <nx && py >0 && py < ny) //centre
	{
		liste.push_back(at(px-1,py));//W
		//liste.push_back(at(px-1,py-1));//NW
		liste.push_back(at(px,py-1));//N
		//liste.push_back(at(px+1,py-1));//NE

		liste.push_back(at(px+1,py));//E
		//liste.push_back(at(px+1,py+1));//SE
		liste.push_back(at(px,py+1));//S
		//liste.push_back(at(px-1,py+1));//SW
		

	}

	else if((px==0 || px == nx) && (py == 0 || py == ny)) //coin
	{
		if(px==0)
			liste.push_back(at(px+1,py));//E
		else
			liste.push_back(at(px-1,py));//W
		if(py==0)
			liste.push_back(at(px,py+1));//S
		else
			liste.push_back(at(px,py-1));//N
	}

	else //bord
	{
		if(px==0)
		{		
			liste.push_back(at(px,py-1));//N
			//liste.push_back(at(px+1,py-1));//NE
			liste.push_back(at(px+1,py));//E
			//liste.push_back(at(px+1,py+1));//SE
			liste.push_back(at(px,py+1));//S
		}
		else if(py==0)
		{		
			liste.push_back(at(px-1,py));//W
			liste.push_back(at(px+1,py));//E
			//liste.push_back(at(px+1,py+1));//SE
			liste.push_back(at(px,py+1));//S
			//liste.push_back(at(px-1,py+1));//SW
		}
		else if(px==nx)
		{		
			liste.push_back(at(px-1,py));//W
			//liste.push_back(at(px-1,py-1));//NW
			liste.push_back(at(px,py-1));//N
			liste.push_back(at(px,py+1));//S
			//liste.push_back(at(px-1,py+1));//SW
		}
		else if(py=ny)
		{		
			liste.push_back(at(px-1,py));//W
			//liste.push_back(at(px-1,py-1));//NW
			liste.push_back(at(px,py-1));//N
			//liste.push_back(at(px+1,py-1));//NE
			liste.push_back(at(px+1,py));//E
		}
	}
	 return liste;
}
*/
////////////////////////////////////////////////////////////////////////////////////
// 


//TODO verif imgout + debug equiv + deuxième passage

Image Image::compConnexe()
{
	int west,nWest,north,nEast,etiwest,etinWest,etinorth,etinEast;
	int etiquette=0;
	
	Image imgout=Image(numcols,numrows);
	Image equiv=Image(numcols*numrows/2+1,numrows*numcols/2+1,1,"equiv.pgm");
	debug << "taille de la matrice d'equivalence:" << ceil(numcols/2) << endl;
	
	for(unsigned int lig = 1; lig < numrows-1; lig++)
	{	for (unsigned int col = 1; col < numcols-1; col++) 
		{				
			
			if(at(col,lig)==1)
			{	debug <<"("<< col <<","<<lig<<"):";

				west=at(col-1,lig);
				nWest=at(col-1,lig-1);
				north=at(col,lig-1);
				nEast=at(col+1,lig-1);
				etiwest=imgout(col-1,lig);
				etinWest=imgout(col-1,lig-1);
				etinorth=imgout(col,lig-1);
				etinEast=imgout(col+1,lig-1);				
				if(west+nWest+north+nEast==0)
				{	
					debug << "pas de voisin"<<endl;
					etiquette++;
					imgout(col,lig)=etiquette;
					equiv(etiquette,etiquette)=1;
				}
				
				else if(west+nWest+north+nEast==1)
				{
					debug << "un voisin"<<endl;
					imgout(col,lig)=etiwest+etinWest+etinorth+etinEast;			
				}
				else
				{
					debug << "plusieur voisin";
					if(west)
					{	imgout(col,lig)=etiwest;
						debug<<":W";
					}
					if(nWest)
					{	debug<<":NW";
						if(!imgout(col,lig))
							imgout(col,lig)=etinWest;
						else
							{equiv(imgout(col,lig),etinWest)=1;
							equiv(etinWest,imgout(col,lig))=1;}
					}
					if(north)
					{	debug<<":N";
						if(!imgout(col,lig))
							imgout(col,lig)=etinorth;
						else
							{equiv(imgout(col,lig),etinorth)=1;
							equiv(etinorth,imgout(col,lig))=1;}
					}
					if(nEast)
					{	debug<<":NE";
						if(!imgout(col,lig))
							imgout(col,lig)=etinEast;
						else
							{equiv(imgout(col,lig),etinEast)=1;
							equiv(etinEast,imgout(col,lig))=1;}
					}
					debug <<endl;
				}		
			}			
		}	debug << endl;			
	}
	debug << "end for" << endl;
	//talk << equiv << endl;
	for(unsigned int lig = 1; lig < equiv.getNumrows(); lig++)
		for (unsigned int col = 1; col < equiv.getNumrows(); col++) 
			if(equiv(col,lig))
				for(unsigned int k = 1;k < equiv.getNumrows();k++)
					equiv(col,k)=equiv(col,k)||equiv(lig,k);
					
	string filename = "equiv.pgm";
	int maxv=0;
	for (unsigned int col = 1; col < numcols-1; col++)
		for(unsigned int lig = 1; lig < numrows-1; lig++)
			{	if(imgout(col,lig))
				{for(unsigned int i=1;i<equiv.getNumrows();i++)
					if(equiv(i,imgout(col,lig)))
					{	imgout(col,lig)=i;
						maxv=max((int)i,maxv);
						i=equiv.getNumrows();
					}
				}
			}
			
	imgout.setVmax(maxv);
	//talk << equiv << endl;
	//equiv.save();
	return imgout;
	
}
