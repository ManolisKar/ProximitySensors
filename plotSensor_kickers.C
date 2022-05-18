/* 

   Plot kicker plates location, as measured from the VCNL4010 sensors.
   Use approximations for the kicker azimuthal positions.
   
   Usage:
   .x plotSensor_kickers.C(<Kicker>)

   Author:
   Manolis Kargiantoulakis

*/



int plotSensor_kickers(Int_t Kicker=1 // Kicker id, 1-3
			  ){

  // These geometric factors are extracted from fits to teh known quad plates.
  Double_t trolley_sens0=39.26; // distance from center of the trolley to sensor0 (inner) in mm
  Double_t trolley_sens1=35.62; // distance from center of the trolley to sensor1 (outer) in mm
  //  Double_t trolley_sens0=36.78; // distance from center of the trolley to sensor0 (inner) in mm
  //  Double_t trolley_sens1=36.53; // distance from center of the trolley to sensor1 (outer) in mm
  
  
  Double_t theta_low;
  Double_t theta_high;
  Int_t run_low;
  Int_t run_high;
  // For the kickers we only have one dataset, with the trolley moving DS.
  // We were stopping and moving slowly when passing through the kickers in the US direction.

  TLine *line;

  if (Kicker==1){
    theta_low=76.7;
    theta_high=86.8;
    run_low=4801;
    run_high=4856;
  }

  else if (Kicker==2){
    theta_low=91.7;
    theta_high=101.9;
    run_low=4883;
    run_high=4938;
  }

  else if (Kicker==3){
    theta_low=107.4;
    theta_high=117.7;
    run_low=4964;
    run_high=5020;
  }
  
  // sensor tree
  TFile *f_sensor = new TFile("$G2QUADS/Alignment/VCNL4010_measurement/results/results_sample_20181212.root","r");
  TTree *tree_sensor=(TTree*)f_sensor->Get("tree");

  // Set aliases for theta and the middle of the trolley rails
  tree_sensor->SetAlias("theta",Form("(%f + (run-%d)*(%f-%f)/(%d-%d))",
				     theta_low, run_low, theta_high, theta_low, run_high,run_low) );


  // Trolley rails location, using only id==3 (and reduced rounding error):
  if (Kicker==1) tree_sensor->SetAlias("trolley_r0","-22800.8+984.674*theta+-3.53228*theta**2+-0.206407*theta**3+0.00277444*theta**4+-1.0442e-05*theta**5");
  else if (Kicker==2) tree_sensor->SetAlias("trolley_r0","96326.7+-3615.63*theta+54.9105*theta**2+-0.37036*theta**3+0.000936029*theta**4");
  else if (Kicker==3) tree_sensor->SetAlias("trolley_r0","737824+-25657.6*theta+337.756*theta**2+-1.97558*theta**3+0.00433215*theta**4");


  // Get avg <r> values for the inner and outer plates
  tree_sensor->Draw(Form("trolley_r0 - %f - dist0 >> hin_s", trolley_sens0),
		    Form("dist0>3 && dist0<17 && run>=%d && run<=%d", run_low,run_high) );
  TH1* hin_s = (TH1*)gPad->GetPrimitive("hin_s");
  printf("Sensors inner <r> = %5.2f +- %4.2f\n" , hin_s->GetMean(), hin_s->GetMeanError());
  tree_sensor->Draw(Form("trolley_r0 + %f + dist1 >> hout_s", trolley_sens1),
		    Form("dist0>3 && dist0<17 && run>%d && run<%d", run_low,run_high) );
  TH1* hout_s = (TH1*)gPad->GetPrimitive("hout_s");
  printf("Sensors outer <r> = %5.2f +- %4.2f\n" , hout_s->GetMean(), hout_s->GetMeanError());
 
  // Get avg <r> for each theta range on the plate from the laser scan files
  Double_t theta_range = theta_high-theta_low;
  Double_t theta_range_low, theta_range_high;
  int n_ranges=15;
  if (theta_range>20) // double the number of theta ranges for a long quad
    n_ranges*=2;

  Double_t r_range_in[40], r_range_out[40], err_r_range_in[40], err_r_range_out[40];
  Double_t theta_range_mean[40], r_range_diff[40], err_r_range_diff[40];
  int n_empty_ranges=0; int entries;
  for (int i_range=0; i_range<n_ranges; i_range++){
    theta_range_low = theta_low+(double)i_range*(theta_range/(double)n_ranges);
    theta_range_high = theta_low+((double)i_range+1)*(theta_range/(double)n_ranges);
    theta_range_mean[i_range-n_empty_ranges] = (theta_range_low+theta_range_high)/2.;
  }  
  
  // Plot sum of sensors -- should give distance between inner-outer plates
  TCanvas *c1 = new TCanvas("c_dist","c_dist",0,0,1100,800);
  tree_sensor->Draw(Form("%f + %f + dist1 + dist0 : theta", trolley_sens0, trolley_sens1),
		    Form("dist0>3 && dist0<17 && run>=%d && run<=%d", run_low,run_high) );
  TGraph *gr_sensor_sum = new TGraph(tree_sensor->GetSelectedRows(),tree_sensor->GetV2(),tree_sensor->GetV1());
  gr_sensor_sum->SetMarkerStyle(22); gr_sensor_sum->SetMarkerSize(0.7); gr_sensor_sum->SetMarkerColor(2);
  TMultiGraph *mg_sum = new TMultiGraph();
  mg_sum->Add(gr_sensor_sum);
  mg_sum->Draw("AP");
  mg_sum->SetTitle(Form("Distance between plates of K%d; #theta (degrees); d (mm)", Kicker));
  line = new TLine(theta_low,95,theta_high,95);
  line->Draw("same");

  gr_sensor_sum->Fit("pol0","Q");
  TF1* fit_sensor_sum=gr_sensor_sum->GetFunction("pol0");
  fit_sensor_sum->SetLineStyle(2); fit_sensor_sum->SetLineColor(kRed);
  printf("Sensor <Dr> = %5.2f +- %4.2f\n" , fit_sensor_sum->GetParameter(0), fit_sensor_sum->GetParError(0));
  

  
  // Plot inner plate sensor vs laser scan points
  TGraph *gr_sensor;  TGraph *gr_sensor_US;  TGraph *gr_laser;  TGraph *gr_laser_middle;
  TMultiGraph *mg;
  c1 = new TCanvas("c_location","c_location",100,100,2000,1200);
  c1->Divide(2,1);
  c1->cd(1);
  gPad->SetLeftMargin(0.16);  gPad->SetRightMargin(0.01);
  mg = new TMultiGraph();
  tree_sensor->Draw(Form("trolley_r0 - %f - dist0 : theta", trolley_sens0),
		    Form("dist0>3 && dist0<17 && run>=%d && run<=%d", run_low,run_high), "goff" );
  gr_sensor = new TGraph(tree_sensor->GetSelectedRows(),tree_sensor->GetV2(),tree_sensor->GetV1());
  gr_sensor->SetMarkerStyle(22); gr_sensor->SetMarkerSize(0.7); gr_sensor->SetMarkerColor(2);
  mg->Add(gr_sensor);
  mg->Draw("AP");
  mg->SetTitle(Form("Inner plate radial location, K%d; #theta (degrees); r (mm)", Kicker));
  line = new TLine(theta_low,7064.5,theta_high,7064.5);
  line->Draw("same");
 
  
  // Plot outer plate sensor vs laser scan points
  c1->cd(2);
  gPad->SetLeftMargin(0.16);  gPad->SetRightMargin(0.01);
  mg = new TMultiGraph();
  tree_sensor->Draw(Form("trolley_r0 + %f + dist1 : theta", trolley_sens1),
		    Form("dist0>3 && dist0<17 && run>=%d && run<=%d", run_low,run_high), "goff" );
  gr_sensor = new TGraph(tree_sensor->GetSelectedRows(),tree_sensor->GetV2(),tree_sensor->GetV1());
  gr_sensor->SetMarkerStyle(22); gr_sensor->SetMarkerSize(0.7); gr_sensor->SetMarkerColor(2);
  mg->Add(gr_sensor);
  mg->Draw("AP");
  mg->SetTitle(Form("Outer plate radial location, K%d; #theta (degrees); r (mm)", Kicker));
  line = new TLine(theta_low,7159.5,theta_high,7159.5);
  line->Draw("same");

  
  
  return 1;
  //  exit();
}
