const int NMax=200000;

int plotTrolleyTension(){

  // Define the graphs that will be filled from each file
  TGraph *gr_fish[5];
  TGraph *gr_coax[5];
  TMultiGraph* mg_fish = new TMultiGraph();
  TMultiGraph* mg_coax = new TMultiGraph();
  
  // Iterate over all files
  for (int iFile=1; iFile<=5; iFile++){

    // Open file and get data
    printf("Reading file#%d\n", iFile);
    TFile *file = new TFile(Form("$G2QUADS/Alignment/VCNL4010_measurement/data/TrolleyRootfiles/FieldGraphOut0580%d_tier1.root", iFile),"r");
    TDirectory* PlotGalilTrolley = file->GetDirectory("PlotGalilTrolley");
    TGraph *gTension_fish=(TGraph*)PlotGalilTrolley->Get("gTension_fish");
    TGraph *gTension_coax=(TGraph*)PlotGalilTrolley->Get("gTension_coax");
    TGraph *gPosition_fish=(TGraph*)PlotGalilTrolley->Get("gPosition_fish");
    TGraph *gPosition_coax=(TGraph*)PlotGalilTrolley->Get("gPosition_coax");
    Double_t *tension_fish=gTension_fish->GetY();
    Double_t *tension_coax=gTension_coax->GetY();
    Double_t *position_fish=gPosition_fish->GetY();
    Double_t *position_coax=gPosition_coax->GetY();

    int N = gTension_fish->GetN();
    printf("  -- #%d data points\n", N);
    
    if (N>NMax) N=NMax;
    Double_t position[NMax];
    for (int i=0; i<N; i++){
      position[i] = 175.0+(position_fish[i]-position_coax[i])*180./432575.;
    }

    // Fill the data in a TGraph and add it to the mg
    gr_fish[iFile-1]=new TGraph(N,position,tension_fish);
    gr_fish[iFile-1]->SetMarkerStyle(20);
    gr_fish[iFile-1]->SetMarkerSize(0.1);
    if (iFile==5) gr_fish[iFile-1]->SetMarkerColor(6);
    else gr_fish[iFile-1]->SetMarkerColor(iFile);
    mg_fish->Add(gr_fish[iFile-1],"p");

    gr_coax[iFile-1]=new TGraph(N,position,tension_coax);
    gr_coax[iFile-1]->SetMarkerStyle(20);
    gr_coax[iFile-1]->SetMarkerSize(0.1);
    if (iFile==5) gr_coax[iFile-1]->SetMarkerColor(6);
    else gr_coax[iFile-1]->SetMarkerColor(iFile);
    mg_coax->Add(gr_coax[iFile-1],"p");

    file->Close();
  }

  
  // Plot
  TCanvas * c1 = new TCanvas("c1", "c1", 1200, 2000);
  c1->Clear();  c1->Divide(2,1);

  c1->cd(1);
  mg_fish->SetTitle("Trolley cable tension (fish) for VCNL4010 run; Azimuthal position; Tension (lbf)");
  mg_fish->Draw("AC");

  
  c1->cd(2);
  mg_coax->SetTitle("Trolley cable tension (coax) for VCNL4010 run; Azimuthal position; Tension (lbf)");
  mg_coax->Draw("AC");


  TLegend* legend = new TLegend(0.01,0.7,0.45,0.9);
  TLegendEntry *le; TString le_text;
  for (int iFile=0; iFile<5; iFile++){
    if (iFile==0) le_text = "Garage to DSQ1";
    else if (iFile==1) le_text = "DSQ1 to USQ4";
    else if (iFile==2) le_text = "USQ4 to USQ2";
    else if (iFile==3) le_text = "USQ2 to DSQ3";
    else le_text = "DSQ3 to garage";
    le = legend->AddEntry(gr_fish[iFile],Form("0580%d, %s",iFile, le_text.Data()),"lp");
    if (iFile==4) le->SetTextColor(6);
    else le->SetTextColor(iFile+1);
    le->SetTextSize(0.03);
  }
  legend->Draw();
  gPad->Update();
  

  
  return 0;

}
