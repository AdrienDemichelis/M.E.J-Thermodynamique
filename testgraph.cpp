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

    double lowerLimit = 1e-4; // valeur minimale de l'axe z

    h2->SetMaximum(1); // fixe la valeur maximale de l'axe z à 1 
    h2->SetMinimum(lowerLimit); // fixe la valeur minimale de l'axe z
    h2->SetBinContent(get<0>(taille)/2 + 1, get<1>(taille)/2 + 1, 1e-4); // initialise la valeur du centre à 1
    
    double grille[81][81] = {}; //tableau des valeurs de h2 au temps t-1, h2 étant au temps t

    while (h2->GetBinContent(get<0>(taille)/2 + 1, get<1>(taille)/2 + 1) < 1) {
        h2->SetBinContent(get<0>(taille)/2 + 1, get<1>(taille)/2 + 1, h2->GetBinContent(get<0>(taille)/2 + 1, get<1>(taille)/2 + 1 ) * 2); // initialise la valeur du centre à 1 dans la grille aussi (temps t-1)
        h2->Draw("LEGO");
        gPad->SetLogz(); // Met l'échelle de l'axe z en échelle logarithmique
        c->Update(); // Met à jour le canvas pour afficher les
        this_thread::sleep_for(chrono::milliseconds(35)); // pause de 100 ms entre chaque mise à jour pour voir l'évolution
    }

    
    this_thread::sleep_for(chrono::milliseconds(65)); // pause de 3 secondes avant de commencer l'évolution pour voir le point de départ
    
    grille[get<0>(taille)/2 + 1][get<1>(taille)/2 + 1] = 1; // initialise la valeur du centre à 1 dans la grille aussi (temps t-1)
    h2->SetBinContent(get<0>(taille)/2 + 1, get<1>(taille)/2 + 1, 1); // initialise la valeur du centre à 1 dans h2 aussi (temps t)

    double diffusionCoefficient = 0.9; // coefficient de diffusion pour faire évoluer h2

    //for (int i = 0; i <= 1000; i++) { // nombre d'itérations pour faire évoluer h2
    while (grille[get<0>(taille)/2 + 1][get<1>(taille)/2 + 1] > lowerLimit*2) { // tant que la valeur du centre est supérieure à la valeur minimale de l'axe z    
        
        //mettre à jour la grille avec les valeurs de h2
        for (int x = 0; x <= get<0>(taille); x++) {
            for (int y = 0; y <= get<1>(taille); y++) {
                grille[x][y] = h2->GetBinContent(x, y);
            }
        }

        // Mettre à jour les valeurs de h2 en fonction de la moyenne des voisins dans la grille (d'avant, au temps t-1)
        for (int x = 1; x < get<0>(taille); x++) {  
            for (int y = 1; y < get<1>(taille); y++) {
                h2->SetBinContent(x, y, grille[x][y] + diffusionCoefficient * ((grille[x-1][y] + grille[x+1][y] + grille[x][y-1] + grille[x][y+1]) / 4.0 - grille[x][y])); // relaxation vers la moyenne des voisins
                if (h2->GetBinContent(x, y) < lowerLimit) { // si la valeur de h2 est inférieure à la valeur minimale de l'axe z, la fixer à cette valeur minimale
                    h2->SetBinContent(x, y, lowerLimit);
                }
            }
        }




        h2->Draw("LEGO");
        gPad->SetLogz(); // Met l'échelle de l'axe z en échelle logarithmique
        c->Update(); // Met à jour le canvas pour afficher les changements
        //c->SaveAs(Form("frame_%02d.png", i)); // Sauvegarde chaque frame
    //}

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