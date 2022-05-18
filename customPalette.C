//Draw color plots using different color palettes.
//Author:: Olivier Couet

#include "TStyle.h"
#include "TColor.h"
#include "TF2.h"
#include "TExec.h"
#include "TCanvas.h"


TCanvas* customPalette() {   
   TCanvas *c1  = new TCanvas("c1","c1",0,0,600,400);   
   TH2D *h2 = new TH2D("h2","h2",20,0.,1.,200,0.,1.);
  

  Int_t fPalette[40];
  fPalette[0]  = 6;
  fPalette[1]  = 6;
  fPalette[2]  = 6;
  fPalette[3]  = 6;
  fPalette[4]  = 6;
  fPalette[5]  = 6;
  fPalette[6]  = 2;
  fPalette[7]  = 2;
  fPalette[8]  = 2;
  fPalette[9]  = 2;
  fPalette[10] = 2;
  fPalette[11] = 2;
  fPalette[12] = 2;
  fPalette[13] = 5;
  fPalette[14] = 5;
  fPalette[15] = 5;
  fPalette[16] = 5;
  fPalette[17] = 5;
  fPalette[18] = 3;
  fPalette[19] = 3;
  fPalette[20] = 3;
  fPalette[21] = 3;
  fPalette[22] = 5;
  fPalette[23] = 5;
  fPalette[24] = 5;
  fPalette[25] = 5;
  fPalette[26] = 5;
  fPalette[27]  = 2;
  fPalette[28]  = 2;
  fPalette[29]  = 2;
  fPalette[30] = 2;
  fPalette[31] = 2;
  fPalette[32] = 2;
  fPalette[33] = 2;
  fPalette[34]  = 6;
  fPalette[35]  = 6;
  fPalette[36]  = 6;
  fPalette[37]  = 6;
  fPalette[38]  = 6;
  fPalette[39]  = 6;
  gStyle->SetPalette(40,fPalette);
  
  h2->SetBinContent(1,1,0);
  h2->SetBinContent(1,2,200);
  h2->SetTitle("Without checking maximum");
  int j=1;
  for (int i=18; i< 23; i++) {    
    h2->SetBinContent(j,i,i*10);
    h2->Draw("colz");
    c1->Update();
    gSystem->Sleep(1000);
    j = j+1;
    if (j>20){
    	j=1;
    }    
  }
  
  
  h2->Reset();
  h2->SetTitle("Checking maximum");  
  h2->SetBinContent(1,1,0);
  h2->SetBinContent(1,2,200);
  int j=1;
  for (int i=18; i< 23; i++) {    
    
    int y=i*10;
    if (y > 200) {
    	y = 200;
    }
    
    h2->SetBinContent(j,i,y);
    h2->Draw("colz");
    c1->Update();
    gSystem->Sleep(1000);
    j = j+1;
    if (j>20)
    	j=1;    
  }
  
  h2->Reset();
  //h2->SetTitle("Logz");
  h2->SetBinContent(1,1,0);
  h2->SetBinContent(1,2,200);
  //gPad->SetLogz();
    int j=1;
  for (int i=18; i< 23; i++) {    
    
    int y=i*10;
    if (y > 200) {
    	y = 200;
    }
    
    h2->SetBinContent(j,i,y);
    h2->Draw("colz");
    c1->Update();
    gSystem->Sleep(1000);
    j = j+1;
    if (j>20)
    	j=1;    
  }
  
  
  
   return c1;
}
