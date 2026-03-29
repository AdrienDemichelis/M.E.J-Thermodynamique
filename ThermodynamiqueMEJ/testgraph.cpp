#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TRandom.h"
#include "TImage.h"
#include <tuple>
#include <iostream>
#include "TSystem.h"
using namespace std;



void testgraph() {
    static TCanvas* c = new TCanvas("c1", "TH2D", 1200, 800);
    TImage *img = TImage::Create();
    tuple<int, int> taille(61, 61); // nombre de bins en x et y; NOMBRES IMPAIRS!!!

    tuple<int, int> centre((get<0>(taille)/2)+1, (get<1>(taille)/2)+1); //prend la moitie de x et y, et ajoute 1 pour etre au milieu

    // Arguments: name, title (with axis titles), nbinsX, xmin, xmax, nbinsY, ymin, ymax
    TH2D *h2 = new TH2D("h21", "Example TH2D Graph;X Axis [units];Y Axis [units];Z Axis [counts]", get<0>(taille), -4, 4, get<1>(taille), -4, 4);

    double lowerLimit = 1e-4; // valeur minimale de l'axe z

    h2->SetMaximum(1); // fixe la valeur maximale de l'axe z à 1 
    h2->SetMinimum(lowerLimit); // fixe la valeur minimale de l'axe z
    h2->SetBinContent(get<0>(taille)/2 + 1, get<1>(taille)/2 + 1, 1e-4); // initialise la valeur du centre à 1
    

    int N = get<0>(taille); //taille x
    int M = get<1>(taille); //taille y
    vector<vector<double>> grille(N, vector<double>(M, 0.0)); //on converti les valeurs N et M pour définir la taille de grille = taille de h2

    while (h2->GetBinContent(get<0>(taille)/2 + 1, get<1>(taille)/2 + 1) < 1) {
        h2->SetBinContent(get<0>(taille)/2 + 1, get<1>(taille)/2 + 1, h2->GetBinContent(get<0>(taille)/2 + 1, get<1>(taille)/2 + 1 ) * 2); // initialise la valeur du centre à 1 dans la grille aussi (temps t-1)
        h2->Draw("LEGO");
        gPad->SetLogz(); // Met l'échelle de l'axe z en échelle logarithmique
        c->Update(); // Met à jour le canvas pour afficher les
        this_thread::sleep_for(chrono::milliseconds(35)); // pause de 100 ms entre chaque mise à jour pour voir l'évolution
    }

    
    this_thread::sleep_for(chrono::milliseconds(65)); // pause de 3 secondes avant de commencer l'évolution pour voir le point de départ
    
    h2->SetBinContent(get<0>(taille)/2 + 1, get<1>(taille)/2 + 1, 1); // initialise la valeur du centre à 1 dans h2 aussi (temps t)

    double diffusionCoefficient = 0.2; // coefficient de diffusion pour faire évoluer h2

    int pas = 0;

    //for (int i = 0; i <= 1000; i++) { // nombre d'itérations pour faire évoluer h2
    //while (grille[get<0>(taille)/2 + 1][get<1>(taille)/2 + 1] > lowerLimit*2) { // tant que la valeur du centre est supérieure à la valeur minimale de l'axe z    
    while (pas < 1000){
        //mettre à jour la grille avec les valeurs de h2
        for (int x = 1; x <= get<0>(taille); x++) {
            for (int y = 1; y <= get<1>(taille); y++) {
                grille[x-1][y-1] = h2->GetBinContent(x, y);
            }
        }

        // Mettre à jour les valeurs de h2 en fonction de la moyenne des voisins dans la grille (d'avant, au temps t-1)
        for (int x = 0; x < get<0>(taille); x++) {  
            for (int y = 0; y < get<1>(taille); y++) {
                double somme = 0.0;
                int casesDispo = 0;

                //On regarde si les cases ont des voisins pour faire la moyenne
                if (x > 0) {
                    somme += grille[x-1][y];
                    casesDispo++;
                }
                if (x < get<0>(taille)-1) {
                     somme += grille[x+1][y];
                    casesDispo++;
                }
                if (y > 0) {
                    somme += grille[x][y-1];
                    casesDispo++;
                }
                if (y < get<1>(taille)-1) {
                    somme += grille[x][y+1];
                    casesDispo++;
                }

                if (casesDispo == 0) continue;
                h2->SetBinContent(x+1, y+1, grille[x][y] + diffusionCoefficient * ((somme/casesDispo) - grille[x][y])); //On reprend le calcul d'avant

                if (h2->GetBinContent(x+1, y+1) < lowerLimit) { // si la valeur de h2 est inférieure à la valeur minimale de l'axe z, la fixer à cette valeur minimale
                    h2->SetBinContent(x+1, y+1, lowerLimit);
                }
            }
        }

        pas ++;
        //double valeurCentre;
        //if ((pas / 200) % 2 == 0) {valeurCentre = 1.0;} else {valeurCentre = lowerLimit;}
        

        grille[get<0>(taille)/2][get<1>(taille)/2] = 1; // On dit que le centre est toujours éguale a 1
        h2->SetBinContent(get<0>(taille)/2 + 1, get<1>(taille)/2 + 1, 1);

        h2->Draw("LEGO");
        gPad->SetLogz(); // Met l'échelle de l'axe z en échelle logarithmique
        c->Update(); // Met à jour le canvas pour afficher les changements
        h2->Draw("LEGO");
        gSystem->ProcessEvents(); // l'animation fait que on peut pas intéragir de base donc j'ai mis ca
        //c->SaveAs(Form("frame_%02d.png", i)); // Sauvegarde chaque frame
    //}

    }   
    
    //h2->Draw("LEGO");
    //c->Update(); 
    //cout << "z au centre: " << grille[get<0>(taille)/2 + 1][get<1>(taille)/2 + 1] << endl;

// on peut utiliser Int_t et Double_t c'est pareil que int et double




// gSystem-> cd("ThermodynamiqueMEJ")


/*     int global = h2->FindBin(10, 10);
    h2->AddBinContent(10, 10, 15);
    h2->AddBinContent(11, 11, 20);
    cout << global << endl; */

    //SetBinContent est différent de AddBinContent , add = incrémente

    // 4. Draw the histogram
    // "COLZ" option draws a color plot with a color palette on the right side

}