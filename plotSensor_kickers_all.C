
/* 

   Plot kicker plates location, as measured from the VCNL4010 sensors.
   Use approximations for the kicker azimuthal positions.
   In the _all script, plot all kicker plats together.
   
   Usage:
   .x plotSensor_kickers_all.C

   Author:
   Manolis Kargiantoulakis


*/

# include "TLine.h"
# include "TFile.h"
# include "TGraph.h"
# include "TMultiGraph.h"
# include "TTree.h"
# include "TCanvas.h"



int plotSensor_kickers_all(){

  // These geometric factors are extracted from fits to teh known quad plates.
  Double_t trolley_sens0=39.26; // distance from center of the trolley to sensor0 (inner) in mm
  Double_t trolley_sens1=35.62; // distance from center of the trolley to sensor1 (outer) in mm
  //  Double_t trolley_sens0=36.78; // distance from center of the trolley to sensor0 (inner) in mm
  //  Double_t trolley_sens1=36.53; // distance from center of the trolley to sensor1 (outer) in mm
  
  
  Double_t theta_low[3];
  Double_t theta_high[3];
  Int_t run_low[3];
  Int_t run_high[3];
  // For the kickers we only have one dataset, with the trolley moving DS.
  // We were stopping and moving slowly when passing through the kickers in the US direction.

  theta_low[0]=76.7;
  theta_high[0]=86.8;
  run_low[0]=4801;
  run_high[0]=4856;

  theta_low[1]=91.7;
  theta_high[1]=101.9;
  run_low[1]=4883;
  run_high[1]=4938;

  theta_low[2]=107.4;
  theta_high[2]=117.7;
  run_low[2]=4964;
  run_high[2]=5020;

  TLine* line;

  
  // sensor tree
  TFile *f_sensor = new TFile("$G2QUADS/Alignment/VCNL4010_measurement/results/results_sample_20181212.root","r");
  TTree *tree_sensor=(TTree*)f_sensor->Get("tree");

  // Set aliases for theta and the middle of the trolley rails
  for (int iK=0; iK<3; iK++)
    tree_sensor->SetAlias(Form("theta_K%d", iK+1),
			  Form("(%f + (run-%d)*(%f-%f)/(%d-%d))",
			       theta_low[iK], run_low[iK], theta_high[iK], theta_low[iK], run_high[iK],run_low[iK]) );

  // Trolley rails location, using only id==3 (and reduced rounding error):
  tree_sensor->SetAlias("trolley_r0_K1","-22800.8+984.674*theta_K1+-3.53228*theta_K1**2+-0.206407*theta_K1**3+0.00277444*theta_K1**4+-1.0442e-05*theta_K1**5");
  tree_sensor->SetAlias("trolley_r0_K2","96326.7+-3615.63*theta_K2+54.9105*theta_K2**2+-0.37036*theta_K2**3+0.000936029*theta_K2**4");
  tree_sensor->SetAlias("trolley_r0_K3","737824+-25657.6*theta_K3+337.756*theta_K3**2+-1.97558*theta_K3**3+0.00433215*theta_K3**4");



  
  // **** Plot sum of sensors -- should give distance between inner-outer plates ****
  TGraph* gr_sensor_sum[3];
  TMultiGraph* mg_sum = new TMultiGraph();
  TCanvas *c1 = new TCanvas("c_dist","c_dist",0,0,1100,800);
  for (int iK=0; iK<3; iK++){
    tree_sensor->Draw(Form("%f + %f + dist1 + dist0 : theta_K%d", trolley_sens0, trolley_sens1, iK+1),
		      Form("dist0>3 && dist0<17 && run>=%d && run<=%d", run_low[iK],run_high[iK]) );
    gr_sensor_sum[iK] = new TGraph(tree_sensor->GetSelectedRows(),tree_sensor->GetV2(),tree_sensor->GetV1());
    gr_sensor_sum[iK]->SetMarkerStyle(22); gr_sensor_sum[iK]->SetMarkerSize(0.7); gr_sensor_sum[iK]->SetMarkerColor(2);
    mg_sum->Add(gr_sensor_sum[iK]);
  }
  mg_sum->Draw("AP");
  mg_sum->SetTitle("Distance between kicker plates; #theta (degrees); d (mm)");
  line = new TLine(theta_low[0]-1,95,theta_high[2]+1,95);
  line->Draw("same");

  mg_sum->Fit("pol0","Q");
  TF1* fit_sum=mg_sum->GetFunction("pol0");
  fit_sum->SetLineStyle(2); fit_sum->SetLineColor(kRed);
  printf("Sensor <Dr> = %5.2f +- %4.2f\n" , fit_sum->GetParameter(0), fit_sum->GetParError(0));
  

  
  // **** Plot plate sensor vs laser scan pointsin ring coordinates ****
  TGraph *gr_sensor[3];  
  TMultiGraph *mg;
  c1 = new TCanvas("c_location","c_location",100,100,2000,1200);
  c1->Divide(1,2);

  // inner plate
  c1->cd(1);
  gPad->SetLeftMargin(0.06);  gPad->SetRightMargin(0.02);
  mg = new TMultiGraph();
  for (int iK=0; iK<3; iK++){
    tree_sensor->Draw(Form("trolley_r0_K%d + %f + dist1 : theta_K%d", iK+1, trolley_sens1, iK+1),
		      Form("dist0>3 && dist0<17 && run>=%d && run<=%d", run_low[iK],run_high[iK]), "goff" );
    gr_sensor[iK] = new TGraph(tree_sensor->GetSelectedRows(),tree_sensor->GetV2(),tree_sensor->GetV1());
    gr_sensor[iK]->SetMarkerStyle(22); gr_sensor[iK]->SetMarkerSize(0.7); gr_sensor[iK]->SetMarkerColor(kBlue);
    mg->Add(gr_sensor[iK]);
  }
  mg->Draw("AP");
  mg->SetTitle("Outer kicker plate radial location; #theta (degrees); r (mm)");
  line = new TLine(theta_low[0]-1,7159.5,theta_high[2]+1,7159.5);
  line->Draw("same");
  mg->Fit("pol0","Q");
  TF1* fit_outer=mg->GetFunction("pol0");
  fit_outer->SetLineStyle(2); fit_outer->SetLineColor(kBlue);
  printf("Inner plate <r> = %5.2f +- %4.2f\n" , fit_outer->GetParameter(0), fit_outer->GetParError(0));

  // outer plate 
  c1->cd(2);
  gPad->SetLeftMargin(0.06);  gPad->SetRightMargin(0.02);
  mg = new TMultiGraph();
  for (int iK=0; iK<3; iK++){
    tree_sensor->Draw(Form("trolley_r0_K%d - %f - dist0 : theta_K%d", iK+1, trolley_sens0, iK+1),
		      Form("dist0>3 && dist0<17 && run>=%d && run<=%d", run_low[iK],run_high[iK]), "goff" );
    gr_sensor[iK] = new TGraph(tree_sensor->GetSelectedRows(),tree_sensor->GetV2(),tree_sensor->GetV1());
    gr_sensor[iK]->SetMarkerStyle(22); gr_sensor[iK]->SetMarkerSize(0.7); gr_sensor[iK]->SetMarkerColor(kRed);
    mg->Add(gr_sensor[iK]);
  }
  mg->Draw("AP");
  mg->SetTitle("Inner kicker plate radial location; #theta (degrees); r (mm)");
  line = new TLine(theta_low[0]-1,7064.5,theta_high[2]+1,7064.5);
  line->Draw("same");
  mg->Fit("pol0","Q");
  TF1* fit_inner=mg->GetFunction("pol0");
  fit_inner->SetLineStyle(2); fit_inner->SetLineColor(kRed);
  printf("Inner plate <r> = %5.2f +- %4.2f\n" , fit_inner->GetParameter(0), fit_inner->GetParError(0));

  
  
  return 1;
  //  exit();
}



