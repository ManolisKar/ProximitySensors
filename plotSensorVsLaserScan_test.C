// Plug in the function for the trolley location by hand for each chamber.

int plotSensorVsLaserScan_test(Int_t VC=1 // vacuum chamber id
			  ){

  Double_t trolley_sens0=39.7; // distance from center of the trolley to sensor0 in mm
  Double_t trolley_sens1=35.5; // distance from center of the trolley to sensor1 in mm
  
  
  Double_t theta_low;
  Double_t theta_high;
  Double_t r_survey[4];
  Double_t theta_survey[4];
  Int_t run_low;
  Int_t run_high;
  Int_t run_low_US=0;
  Int_t run_high_US=0;

  TLine *line;

  if (VC==1){
    theta_low=30.97;
    theta_high=43.87;
    theta_survey[0]=40.231; theta_survey[1]=40.358; theta_survey[2]=40.134; theta_survey[3]=40.260;
    r_survey[0]=7061.804; r_survey[1]=7061.58; r_survey[2]=7061.883; r_survey[3]=7061.743;
    run_low=4547;
    run_high=4617;
    run_low_US=3435;
    run_high_US=3506;
  }

  else if (VC==2){
    theta_low=47.95;
    theta_high=73.86;
    theta_survey[0]=70.167; theta_survey[1]=70.295; theta_survey[2]=70.224; theta_survey[3]=70.351;
    r_survey[0]=7061.903; r_survey[1]=7061.730; r_survey[2]=7061.664; r_survey[3]=7061.459;
    run_low=4640;
    run_high=4781;
    run_low_US=3269;
    run_high_US=3412;
  }

  else if (VC==4){
    theta_low=120.92;
    theta_high=133.85;
    theta_survey[0]=130.236; theta_survey[1]=130.365; theta_survey[2]=130.104; theta_survey[3]=130.233;
    r_survey[0]=7061.764; r_survey[1]=7061.257; r_survey[2]=7061.703; r_survey[3]=7061.287;
    run_low=5038;
    run_high=5108;
  }

  else if (VC==5){
    theta_low=138.;
    theta_high=163.8;
    theta_survey[0]=160.23; theta_survey[1]=160.359; theta_survey[2]=160.127; theta_survey[3]=160.255;
    r_survey[0]=7062.036; r_survey[1]=7061.79; r_survey[2]=7062.054; r_survey[3]=7061.743;
    run_low=5131;
    run_high=5270;
  }

  else if (VC==7){
    theta_low=210.94;
    theta_high=223.55;
    theta_survey[0]=0; theta_survey[1]=0; theta_survey[2]=0; theta_survey[3]=0;
    r_survey[0]=0; r_survey[1]=0; r_survey[2]=0; r_survey[3]=0;
    run_low=5567;
    run_high=5634;
    run_low_US=5978;
    run_high_US=6047;
  }

  else if (VC==8){
    theta_low=228.2;
    theta_high=253.8;
    theta_survey[0]=360-109.891; theta_survey[1]=360-109.762; theta_survey[2]=360-109.794; theta_survey[3]=360-109.666;
    r_survey[0]=7061.238; r_survey[1]=7061.0; r_survey[2]=7061.093; r_survey[3]=7060.971;
    run_low=5659;
    run_high=5799;
    run_low_US=5816;
    run_high_US=5957;
  }

  else if (VC==10){
    theta_low=300.9;
    theta_high=313.86;
    theta_survey[0]=0; theta_survey[1]=0; theta_survey[2]=0; theta_survey[3]=0;
    r_survey[0]=0; r_survey[1]=0; r_survey[2]=0; r_survey[3]=0;
    run_low=4053;
    run_high=4124;
    run_low_US=3931;
    run_high_US=4006;
  }

  else if (VC==11){
    theta_low=318.25;
    theta_high=343.4;
    theta_survey[0]=360-19.885; theta_survey[1]=360-19.758; theta_survey[2]=360-19.825; theta_survey[3]=360-19.698;
    r_survey[0]=7063.608; r_survey[1]=7062.723; r_survey[2]=7063.644; r_survey[3]=7063.587;
    run_low=4146;
    run_high=4288;
    run_low_US=3766;
    run_high_US=3908;
  }
  
  // sensor tree
  TFile *f_sensor = new TFile("$G2QUADS/Alignment/VCNL4010_measurement/results/results_sample_20181212.root","r");
  TTree *tree_sensor=(TTree*)f_sensor->Get("tree");

  // Quad inner plate tree
  TFile *f_in = new TFile(Form("$G2QUADS/Alignment/20170222 Alignment data/VC Export All replacing VC09 info/VC%02d Export/Inner-quad.root",
			       VC),
			  "r");
  TTree *tree_in=(TTree*)f_in->Get("tree");
  tree_in->SetMarkerStyle(20);  tree_in->SetMarkerSize(0.1);
  // Quad outer plate tree
  TFile *f_out = new TFile(Form("$G2QUADS/Alignment/20170222 Alignment data/VC Export All replacing VC09 info/VC%02d Export/Outer-quad.root",
				VC),
			  "r");
  TTree *tree_out=(TTree*)f_out->Get("tree");
  tree_out->SetMarkerStyle(20);  tree_out->SetMarkerSize(0.1);

  // Set aliases for theta and the middle of the trolley rails
  tree_sensor->SetAlias("theta",Form("(%f + (run-%d)*(%f-%f)/(%d-%d))",
				     theta_low, run_low, theta_high, theta_low, run_high,run_low) );

  // // Trolley rails location, using average of all trolley points:
  // if (VC==1) tree_sensor->SetAlias("trolley_r0","22036.3+-2007.54*theta+107.614*theta**2+-2.87387*theta**3+0.0382403*theta**4+-0.000202859*theta**5");
  // else if (VC==2) tree_sensor->SetAlias("trolley_r0","12569.2+-462.422*theta+15.5747*theta**2+-0.26074*theta**3+0.00217057*theta**4+-7.19049e-06*theta**5");
  // else if (VC==4) tree_sensor->SetAlias("trolley_r0","9539.13+-56.9307*theta+0.445295*theta**2+-0.00116177*theta**3");
  // else if (VC==5) tree_sensor->SetAlias("trolley_r0","1137.4+158.131*theta-1.56525*theta**2+0.00686682*theta**3-1.12646e-05*theta**4");
  // else if (VC==7) tree_sensor->SetAlias("trolley_r0","20496.7+-185.577*theta+0.857627*theta**2+-0.00132105*theta**3");
  // else if (VC==8) tree_sensor->SetAlias("trolley_r0","5148.5+15.3565*theta+0.0133587*theta**2+-0.000341958*theta**3+6.73985e-07*theta**4");
  // else if (VC==10) tree_sensor->SetAlias("trolley_r0","-118384+1679.12*theta+-8.42606*theta**2+0.0187951*theta**3+-1.57234e-05*theta**4");
  // else if (VC==11) tree_sensor->SetAlias("trolley_r0","42802.4+-422.299*theta+1.87317*theta**2+-0.00369172*theta**3+2.7278e-06*theta**4");

  // Trolley rails location, using only id==3 (and reduced rounding error):
  if (VC==1) tree_sensor->SetAlias("trolley_r0","8416.5663+-137.25317*theta+5.3823139*theta**2+-0.093194268*theta**3+0.00060115324*theta**4");
  else if (VC==2) tree_sensor->SetAlias("trolley_r0","6842.7542+16.671970*theta+-0.38869091*theta**2+0.0040527647*theta**3+-1.5964441e-05*theta**4");
  else if (VC==4) tree_sensor->SetAlias("trolley_r0","-14812.182+716.82535*theta+-8.7633470*theta**2+0.047490400*theta**3+-9.6284309e-05*theta**4");
  else if (VC==5) tree_sensor->SetAlias("trolley_r0","6355.2742+22.808127*theta+-0.25063955*theta**2+0.0011975142*theta**3+-2.1073778e-06*theta**4");
  else if (VC==7) tree_sensor->SetAlias("trolley_r0","181876.84+-3111.7315*theta+20.748141*theta**2+-0.061394195*theta**3+6.8016131e-05*theta**4");
  else if (VC==8) tree_sensor->SetAlias("trolley_r0","-92855.152+1642.7435*theta+-10.117048*theta**2+0.027675831*theta**3+-2.8374460e-05*theta**4");
  else if (VC==10) tree_sensor->SetAlias("trolley_r0","51108.423+-434.27844*theta+1.4287167*theta**2+-0.0015665715*theta**3");
  else if (VC==11) tree_sensor->SetAlias("trolley_r0","19103.130+-108.22890*theta+0.32545620*theta**2+-0.00032606652*theta**3");



  // Get avg <r> values for each plate, from sensors and from laser scan
  tree_sensor->Draw(Form("trolley_r0 - %f - dist0 >> hin_s", trolley_sens0),
		    Form("dist0>3 && dist0<17 && run>%d && run<%d", run_low,run_high) );
  TH1* hin_s = (TH1*)gPad->GetPrimitive("hin_s");
  printf("Sensors inner <r> = %5.2f +- %4.2f\n" , hin_s->GetMean(), hin_s->GetMeanError());
  tree_in->Draw("1000*r >> hin_l","");
  TH1* hin_l = (TH1*)gPad->GetPrimitive("hin_l");
  printf("LaseScan inner <r> = %5.2f +- %4.2f\n" , hin_l->GetMean(), hin_l->GetMeanError());
  tree_sensor->Draw(Form("trolley_r0 + %f + dist1 >> hout_s", trolley_sens1),
		    Form("dist0>3 && dist0<17 && run>%d && run<%d", run_low,run_high) );
  TH1* hout_s = (TH1*)gPad->GetPrimitive("hout_s");
  printf("Sensors outer <r> = %5.2f +- %4.2f\n" , hout_s->GetMean(), hout_s->GetMeanError());
  tree_out->Draw("1000*r >> hout_l","");
  TH1* hout_l = (TH1*)gPad->GetPrimitive("hout_l");
  printf("LaseScan inner <r> = %5.2f +- %4.2f\n" , hout_l->GetMean(), hout_l->GetMeanError());

  
  // Plot sum of sensors -- should give distance between inner-outer plates
  TCanvas *c1 = new TCanvas("c_dist","c_dist");
  tree_sensor->Draw(Form("%f + %f + dist1 + dist0 : theta", trolley_sens0, trolley_sens1),
		    Form("dist0>3 && dist0<17 && run>%d && run<%d", run_low,run_high) );
  line = new TLine(theta_low,100,theta_high,100);
  line->Draw("same");


  TGraph *gr_sensor;  TGraph *gr_sensor_US;  TGraph *gr_laser;  TGraph *gr_laser_middle;
  TMultiGraph *mg;

  
  // Plot inner plate sensor vs laser scan points
  c1 = new TCanvas("c_comparison","c_comparison",0,0,2000,1200);
  c1->Divide(2,1);
  c1->cd(1);
  gPad->SetLeftMargin(0.16);  gPad->SetRightMargin(0.01);
  mg = new TMultiGraph();
  tree_sensor->Draw(Form("trolley_r0 - %f - dist0 : theta", trolley_sens0),
		    Form("dist0>3 && dist0<17 && run>%d && run<%d", run_low,run_high), "goff" );
  gr_sensor = new TGraph(tree_sensor->GetSelectedRows(),tree_sensor->GetV2(),tree_sensor->GetV1());
  gr_sensor->SetMarkerStyle(22); gr_sensor->SetMarkerSize(0.7); gr_sensor->SetMarkerColor(2);
  mg->Add(gr_sensor);
  if (run_low_US!=0) { // Data exists with the trolley moving upstream. Plot it with differnet marker.
    // Needs redefinition of theta in terms of run number:
    tree_sensor->SetAlias("theta",Form("%f + (%d-run)*(%f-%f)/(%d-%d)",
				       theta_low, run_high_US, theta_high, theta_low, run_high_US,run_low_US) );
    tree_sensor->Draw(Form("trolley_r0 - %f - dist0 : theta", trolley_sens0),
		      Form("dist0>3 && dist0<17 && run>%d && run<%d", run_low_US,run_high_US), "goff" );
    gr_sensor_US = new TGraph(tree_sensor->GetSelectedRows(),tree_sensor->GetV2(),tree_sensor->GetV1());
    gr_sensor_US->SetMarkerStyle(23); gr_sensor_US->SetMarkerSize(0.7); gr_sensor_US->SetMarkerColor(2);
    mg->Add(gr_sensor_US);
    tree_sensor->SetAlias("theta",Form("%f + (run-%d)*(%f-%f)/(%d-%d)",
				       theta_low, run_low, theta_high, theta_low, run_high,run_low) );
  }
  
  if (VC<6) tree_in->Draw("1000*r:-1*theta","","goff");
  else tree_in->Draw("1000*r:360-theta","","goff");
  gr_laser = new TGraph(tree_in->GetSelectedRows(),tree_in->GetV2(),tree_in->GetV1());
  gr_laser->SetMarkerStyle(20); gr_laser->SetMarkerSize(0.15); gr_laser->SetMarkerColor(1);
  mg->Add(gr_laser);
  if (VC<6) tree_in->Draw("1000*r:-1*theta","abs(z)<0.005","goff");
  else tree_in->Draw("1000*r:360-theta","abs(z)<0.005","goff");
  gr_laser_middle = new TGraph(tree_in->GetSelectedRows(),tree_in->GetV2(),tree_in->GetV1());
  gr_laser_middle->SetMarkerStyle(20); gr_laser_middle->SetMarkerSize(0.15); gr_laser_middle->SetMarkerColor(3);
  mg->Add(gr_laser_middle);
  mg->Draw("AP");
  mg->SetTitle("Inner plate radial location; #theta (degrees); r (mm)");
  line = new TLine(theta_low,7062,theta_high,7062);
  line->Draw("same");

  // Superimpose also the measurements from the through-flange survey
  TGraph* gr_survey = new TGraph(4,theta_survey,r_survey);
  gr_survey->SetMarkerStyle(34);  gr_survey->SetMarkerSize(2);  gr_survey->SetMarkerColor(9);
  gr_survey->Draw("P same");

  
  // Plot outer plate sensor vs laser scan points
  c1->cd(2);
  gPad->SetLeftMargin(0.16);  gPad->SetRightMargin(0.01);
  mg = new TMultiGraph();
  tree_sensor->Draw(Form("trolley_r0 + %f + dist1 : theta", trolley_sens1),
		    Form("dist0>3 && dist0<17 && run>%d && run<%d", run_low,run_high), "goff" );
  gr_sensor = new TGraph(tree_sensor->GetSelectedRows(),tree_sensor->GetV2(),tree_sensor->GetV1());
  gr_sensor->SetMarkerStyle(22); gr_sensor->SetMarkerSize(0.7); gr_sensor->SetMarkerColor(2);
  mg->Add(gr_sensor);
  if (run_low_US!=0) { // Data exists with the trolley moving upstream. Plot it with differnet marker.
    // Needs redefinition of theta in terms of run number:
    tree_sensor->SetAlias("theta",Form("%f + (%d-run)*(%f-%f)/(%d-%d)",
				       theta_low, run_high_US, theta_high, theta_low, run_high_US,run_low_US) );
    tree_sensor->Draw(Form("trolley_r0 + %f + dist1 : theta", trolley_sens1),
		      Form("dist0>3 && dist0<17 && run>%d && run<%d", run_low_US,run_high_US), "goff" );
    gr_sensor_US = new TGraph(tree_sensor->GetSelectedRows(),tree_sensor->GetV2(),tree_sensor->GetV1());
    gr_sensor_US->SetMarkerStyle(23); gr_sensor_US->SetMarkerSize(0.7); gr_sensor_US->SetMarkerColor(2);
    mg->Add(gr_sensor_US);
    tree_sensor->SetAlias("theta",Form("%f + (run-%d)*(%f-%f)/(%d-%d)",
				       theta_low, run_low, theta_high, theta_low, run_high,run_low) );
  }
  
  if (VC<6) tree_out->Draw("1000*r:-1*theta","","goff");
  else tree_out->Draw("1000*r:360-theta","","goff");
  gr_laser = new TGraph(tree_out->GetSelectedRows(),tree_out->GetV2(),tree_out->GetV1());
  gr_laser->SetMarkerStyle(20); gr_laser->SetMarkerSize(0.15); gr_laser->SetMarkerColor(1);
  mg->Add(gr_laser);
  if (VC<6) tree_out->Draw("1000*r:-1*theta","abs(z)<0.005","goff");
  else tree_out->Draw("1000*r:360-theta","abs(z)<0.005","goff");
  gr_laser_middle = new TGraph(tree_out->GetSelectedRows(),tree_out->GetV2(),tree_out->GetV1());
  gr_laser_middle->SetMarkerStyle(20); gr_laser_middle->SetMarkerSize(0.15); gr_laser_middle->SetMarkerColor(3);
  mg->Add(gr_laser_middle);
  mg->Draw("AP");
  mg->SetTitle("Outer plate radial location; #theta (degrees); r (mm)");
  line = new TLine(theta_low,7162,theta_high,7162);
  line->Draw("same");

  
  
  return 1;
  //  exit();
}
