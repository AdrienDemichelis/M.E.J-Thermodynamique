#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TRandom.h"
#include <tuple>

void testgraph() {
    TCanvas* c = new TCanvas("c", "TH2D Graph Example", 1200, 800);
    
    tuple<int, int> taille(80, 80); // nombre de bins en x et y

    // Arguments: name, title (with axis titles), nbinsX, xmin, xmax, nbinsY, ymin, ymax
    TH2D *h2 = new TH2D("h2", "Example TH2D Graph;X Axis [units];Y Axis [units];Z Axis [counts]", get<0>(taille), -5, 5, get<1>(taille), -5, 5);


    // 3. Fill the histogram with some data
    TRandom *r = new TRandom();
    for (int i = 0; i < 100; i++) {
        double x = r->Gaus(0, 2); // moyenne 0, ecart type 2
        double y = r->Gaus(0, 2);
        h2->Fill(x, y, 1); // incrémente la bin(x, y) de 1 en hauteur ou z
    
    } 

// on peut utiliser Int_t et Double_t c'est pareil que int et double


    for (int x = 1; x <= 80; x++) {
        for (int y = 1; y <= 80; y++) {
            if (x < 40 && y < 40) {
                h2->SetBinContent(x, y, 5); 
            } else if (x >= 40 && y >= 40) {
                h2->SetBinContent(x, y, 10); 
            } else {
                h2->SetBinContent(x, y, 0); 
            }
        }
    } 




/*     int global = h2->FindBin(10, 10);
    h2->AddBinContent(10, 10, 15);
    h2->AddBinContent(11, 11, 20);
    cout << global << endl; */

    //SetBinContent est différent de AddBinContent , add = incrémente

    // 4. Draw the histogram
    // "COLZ" option draws a color plot with a color palette on the right side
    h2->Draw("LEGO");
}