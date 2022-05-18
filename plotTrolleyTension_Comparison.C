const int NMax=200000;

int plotTrolleyTension_Comparison(double rangemin=-100., double rangemax=270){

  const char *dataDir_Name="/Users/ekargian/Documents/g-2/Magnets/ElectrostaticQuads/Alignment/VCNL4010_measurement/data/TrolleyRootfiles/";
  const char *extension=".root";

  TSystemDirectory dataDir(dataDir_Name, dataDir_Name);
  TList *files = dataDir.GetListOfFiles();
  if (!files) {
    printf("\n\n Uh oh! Empty dir:\n %s\n\n\tExiting...\n\n", dataDir_Name);
    exit(1);
  }

  
  // Define the graphs that will be filled from each file
  TGraph *gr_fish[2];
  TGraph *gr_coax[2];
  TMultiGraph* mg_fish = new TMultiGraph();
  TMultiGraph* mg_coax = new TMultiGraph();

  std::vector<double> tension_fish_sens, tension_coax_sens,
    tension_fish_noSens, tension_coax_noSens,
    pos_sens, pos_noSens;
    
  // Iterate over all files in the data dir
  TSystemFile *sfile;
  TString file_name;
  TIter next(files);
  int nfiles=0;
  int id_Sensor, id_noSensor;
  while ((sfile=(TSystemFile*)next())) {
    file_name = sfile->GetName();
    printf("\n Processing file: %s", file_name.Data());
    // Skip files that are not relevant
    if (sfile->IsDirectory() || !file_name.EndsWith(extension) || file_name.Contains("05805")
	|| file_name.Contains("05806") || file_name.Contains("05807") || file_name.Contains("05808") || file_name.Contains("05809")) {
      printf("\n Skipping...\n\n");
      continue;
    }
    // file accepted, increment file count
    nfiles++;
    
    // Open file and get data
    TFile *file = new TFile(Form("%s/%s",dataDir_Name, file_name.Data()));
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

    double position;
    // Append data to vectors
    if (file_name.Contains("0580")) {// data from run with sensor
    //if (file_name.Contains("0499")) {// old data before sensor
      for (int i=0; i<N; i++){
	position = 175.0+(position_fish[i]-position_coax[i])*180./432575.;
	if (position>rangemin && position<rangemax) {
	  tension_fish_sens.push_back(tension_fish[i]);
	  tension_coax_sens.push_back(tension_coax[i]);
	  pos_sens.push_back(position);
	}
      }
    }
    else if (file_name.Contains("0589")) { // data from run w/o sensor
      for (int i=0; i<N; i++){
	position = 175.0+(position_fish[i]-position_coax[i])*180./432575.;
	if (position>rangemin && position<rangemax) {
	  tension_fish_noSens.push_back(tension_fish[i]);
	  tension_coax_noSens.push_back(tension_coax[i]);
	  pos_noSens.push_back( 175.0+(position_fish[i]-position_coax[i])*180./432575. );
	}
      }
    }

    file->Close();
  }

  
  // Fill the vector data in two TGraphs

  // The #0 member holds data with the sensor
  gr_fish[0]=new TGraph(tension_fish_sens.size(),&pos_sens[0],&tension_fish_sens[0]);
  gr_fish[0]->SetMarkerStyle(20);
  gr_fish[0]->SetMarkerSize(0.1);
  gr_fish[0]->SetMarkerColor(kBlue);
  gr_coax[0]=new TGraph(tension_coax_sens.size(),&pos_sens[0],&tension_coax_sens[0]);
  gr_coax[0]->SetMarkerStyle(20);
  gr_coax[0]->SetMarkerSize(0.1);
  gr_coax[0]->SetMarkerColor(kBlue);

  // The #1 member holds data w/o the sensor
  gr_fish[1]=new TGraph(tension_fish_noSens.size(),&pos_noSens[0],&tension_fish_noSens[0]);
  gr_fish[1]->SetMarkerStyle(20);
  gr_fish[1]->SetMarkerSize(0.1);
  gr_fish[1]->SetMarkerColor(kRed);
  gr_coax[1]=new TGraph(tension_coax_noSens.size(),&pos_noSens[0],&tension_coax_noSens[0]);
  gr_coax[1]->SetMarkerStyle(20);
  gr_coax[1]->SetMarkerSize(0.1);
  gr_coax[1]->SetMarkerColor(kRed);

  mg_fish->Add(gr_fish[0],"p");  mg_fish->Add(gr_fish[1],"p");
  mg_coax->Add(gr_coax[0],"p");  mg_coax->Add(gr_coax[1],"p");

  
  // Plot
  gStyle->SetTitleW(0.7);
  TCanvas * c1 = new TCanvas("c1", "c1", 1200, 2000);
  c1->Clear();  c1->Divide(2,1);

  TVirtualPad *pad = c1->cd(1); pad->DrawFrame(0,0,10,10);
  pad->SetLeftMargin(0.14);  pad->SetTopMargin(0.08);  pad->SetRightMargin(0.02);
  mg_fish->SetTitle("Trolley cable tension (fish); Azimuthal position; Tension (lbf)");
  mg_fish->Draw("AC");

  
  pad = c1->cd(2); pad->DrawFrame(0,0,10,10);
  pad->SetLeftMargin(0.14);  pad->SetTopMargin(0.08);  pad->SetRightMargin(0.02);
  mg_coax->SetTitle("Trolley cable tension (coax); Azimuthal position; Tension (lbf)");
  mg_coax->Draw("AC");


  TLegend* legend = new TLegend(0.0,0.79,0.45,0.92);
  TLegendEntry *le; TString le_text;
  le = legend->AddEntry(gr_fish[0], "With VCNL4010 Sensor","lp");
  //le = legend->AddEntry(gr_fish[0], "Old data","lp");
  le->SetTextColor(kBlue);
  le->SetTextSize(0.03);
  le = legend->AddEntry(gr_fish[1], "W/o VCNL4010 Sensor","lp");
  le->SetTextColor(kRed);
  le->SetTextSize(0.03);

  legend->Draw();
  gPad->Update();
  

  c1->SaveAs("~/Desktop/CableTension.png");
  
  return 0;

}
