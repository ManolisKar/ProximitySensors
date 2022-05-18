int plotTrolleyTension_test(){

  // Vectors that will hold all data
  std::vector<double> tension_fish, position_fish, position_coax, position;
  
  // Iterate over all files
  for (int iFile=1; iFile<=5; iFile++){

    printf("Reading file#%d\n", iFile);
    // Open file and get data
    TFile *file = new TFile(Form("$G2QUADS/Alignment/VCNL4010_measurement/data/TrolleyRootfiles/FieldGraphOut0580%d_tier1.root", iFile),"r");
    TDirectory* PlotGalilTrolley = file->GetDirectory("PlotGalilTrolley");
    TGraph *gTension_fish=(TGraph*)PlotGalilTrolley->Get("gTension_fish");
    TGraph *gPosition_fish=(TGraph*)PlotGalilTrolley->Get("gPosition_fish");
    TGraph *gPosition_coax=(TGraph*)PlotGalilTrolley->Get("gPosition_coax");
    Double_t *tension_fish_arr=gTension_fish->GetY();
    Double_t *position_fish_arr=gPosition_fish->GetY();
    Double_t *position_coax_arr=gPosition_coax->GetY();
    int N = gTension_fish->GetN();
    
    // Append data into the vectors
    for (int i=0; i<N; i++){
      tension_fish.push_back(tension_fish_arr[i]);
      position_fish.push_back(position_fish_arr[i]);
      position_coax.push_back(position_coax_arr[i]);
      position.push_back(175.0+(position_fish_arr[i]-position_coax_arr[i])*180./432575.);;
    }

    file->Close();
  }
  
  // Plot 
  TGraph* gr=new TGraph(tension_fish.size(),&(position[0]),&(tension_fish[0]));
  gr->Draw("ACP");

  return 0;

}
