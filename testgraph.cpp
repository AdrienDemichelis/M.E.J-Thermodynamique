#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TRandom.h"
#include <tuple>
#include <iostream>
using namespace std;

void testgraph() {
    TCanvas* c = new TCanvas("c", "TH2D Graph Example", 1200, 800);
    
    tuple<int, int> taille(81, 81); // nombre de bins en x et y; NOMBRES IMPAIRS!!!

    tuple<int, int> centre((get<0>(taille)/2)+1, (get<1>(taille)/2)+1); //prend la moitie de x et y, et ajoute 1 pour etre au milieu

    // Arguments: name, title (with axis titles), nbinsX, xmin, xmax, nbinsY, ymin, ymax
    TH2D *h2 = new TH2D("h2", "Example TH2D Graph;X Axis [units];Y Axis [units];Z Axis [counts]", get<0>(taille), -5, 5, get<1>(taille), -5, 5);


    // 3. Fill the histogram with some data
    TRandom *r = new TRandom();
/*     for (int i = 0; i < 10000000; i++) {
        double x = r->Gaus(0, 1.5); // moyenne 0, ecart type 1.5
        double y = r->Gaus(0, 1.5); // moyenne 0, ecart type 1.5
        h2->Fill(x, y, 1); // incrémente la bin(x, y) de 1 en hauteur ou z
    
    }  */



    auto change_z = [h2](int x, int y) {
        double z = (h2->GetBinContent(x-1, y) + h2->GetBinContent(x+1, y) + h2->GetBinContent(x, y-1) + h2->GetBinContent(x, y+1)) - 4*h2->GetBinContent(x, y); 
    
        h2->SetBinContent(x, y, z);
        //grille[x][y] = z; // stocke la nouvelle valeur de z dans le tableau 2D
    };

    
    h2->SetBinContent(get<0>(centre), get<1>(centre), 1); // met la valeur 1 dans la bin du centre

    double grille[81][81] = {0}; // initialise un tableau 2D de 81x81 avec des zéros

    for (int i = 0; i <= 10; i++) {

        for (int x = 0; x<=get<0>(taille); x++) {
            for (int y = 0; y<=get<1>(taille); y++) {
                change_z(x, y);
                }
            }
        }





// on peut utiliser Int_t et Double_t c'est pareil que int et double


    /* for (int x = 1; x <= 80; x++) {
        for (int y = 1; y <= 80; y++) {
            if (x < 40 && y < 40) {
                h2->SetBinContent(x, y, 5); 
            } else if (x >= 40 && y >= 40) {
                h2->SetBinContent(x, y, 10); 
            } else {
                h2->SetBinContent(x, y, 0); 
            }
        }
    } */ 


    //h2->SetBinContent(get<0>(centre), get<0>(centre), 25);

/*     int global = h2->FindBin(10, 10);
    h2->AddBinContent(10, 10, 15);
    h2->AddBinContent(11, 11, 20);
    cout << global << endl; */

    //SetBinContent est différent de AddBinContent , add = incrémente

    // 4. Draw the histogram
    // "COLZ" option draws a color plot with a color palette on the right side
    h2->Draw("LEGO");
}