#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TRandom.h"
#include "TImage.h"
#include <tuple>
#include <iostream>
using namespace std;



void testgraph() {
    TCanvas* c = new TCanvas("c", "TH2D Graph Example", 1200, 800);
    TImage *img = TImage::Create();
    tuple<int, int> taille(61, 61); // nombre de bins en x et y; NOMBRES IMPAIRS!!!

    tuple<int, int> centre((get<0>(taille)/2)+1, (get<1>(taille)/2)+1); //prend la moitie de x et y, et ajoute 1 pour etre au milieu

    // Arguments: name, title (with axis titles), nbinsX, xmin, xmax, nbinsY, ymin, ymax
    TH2D *h2 = new TH2D("h2", "Example TH2D Graph;X Axis [units];Y Axis [units];Z Axis [counts]", get<0>(taille), -4, 4, get<1>(taille), -4, 4);



    h2->SetBinContent(get<0>(taille)/2 + 1, get<1>(taille)/2 + 1, 1); // initialise la valeur du centre à 1
    
    double grille[81][81] = {}; //tableau des valeurs de h2 au temps t-1, h2 étant au temps t



    for (int i = 0; i <= 1000; i++) { // nombre d'itérations pour faire évoluer h2
        
        //mettre à jour la grille avec les valeurs de h2
        for (int x = 0; x < get<0>(taille); x++) {
            for (int y = 0; y < get<1>(taille); y++) {
                grille[x][y] = h2->GetBinContent(x, y);
        }
    }

        // Mettre à jour les valeurs de h2 en fonction de la moyenne des voisins dans la grille (d'avant, au temps t-1)
        for (int x = 1; x <=get<0>(taille); x++) {  
            for (int y = 1; y <= get<1>(taille); y++) {
                if (x != get<0>(taille)/2 + 1 || y != get<1>(taille)/2 + 1) {// ne pas mettre à jour la valeur du centre
                    h2->SetBinContent(x, y, ((grille[x-1][y] + grille[x+1][y] + grille[x][y-1] + grille[x][y+1]) / 4.0));
                }
            }
        }




        h2->Draw("LEGO");
        c->Update(); // Met à jour le canvas pour afficher les changements
        //c->SaveAs(Form("frame_%02d.png", i)); // Sauvegarde chaque frame
    }


    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //h2->Draw("LEGO");
    //c->Update(); 
    cout << "z au centre: " << grille[get<0>(taille)/2 + 1][get<1>(taille)/2 + 1] << endl;

// on peut utiliser Int_t et Double_t c'est pareil que int et double







/*     int global = h2->FindBin(10, 10);
    h2->AddBinContent(10, 10, 15);
    h2->AddBinContent(11, 11, 20);
    cout << global << endl; */

    //SetBinContent est différent de AddBinContent , add = incrémente

    // 4. Draw the histogram
    // "COLZ" option draws a color plot with a color palette on the right side

}