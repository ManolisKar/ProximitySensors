int plotTrolleyTension(){
  TFile *file1 = new TFile("$G2QUADS/Alignment/VCNL4010_measurement/data/TrolleyRootfiles/FieldGraphOut05801_tier1.root","r");
  file1->cd("PlotGalilTrolley"); // cd to the relevant dir in the file
  Double_t *tension_fish=gTension_fish->GetY();
  Double_t *position_fish=gPosition_fish->GetY();
  Double_t *position_coax=gPosition_coax->GetY();
  const Int_t N=gTension_fish->GetN();
  Double_t position[N];
  for (int i=0; i<N; i++){
    position[i] = 175.0+(position_fish[i]-position_coax[i])*180./432575.;
  }
  TGraph* gr=new TGraph(N,position,tension_fish);
  gr->Draw("ACP");

}
