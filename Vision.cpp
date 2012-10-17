#include <iostream>
#include "Image.h"
#include "Masque.h"
using namespace std;

#include <string>
#include <locale>
#include <algorithm>


int main(int argc, char *argv[])
{
/*
   double G[]={-1.,0.,1.,-2.,0.,2.,-1.,0.,1.};
  Masque masque= Masque(1.,3,G);
  masque.afficher(cout);

  Image img = Image("bruit.pnm");
  img*=masque;
  img.save("gradx.pgm");

  //cout << img << endl;
 
 */
 Image img =Image("test.pgm");
 //cout << img <<  endl;
 vector<int> truc = img.getContour(1);
 for(int i=0;i<truc.size();i++)
  cout << truc[i] << ",";
 cout << endl;
 

}


