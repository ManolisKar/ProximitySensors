int getCOLZplot(double subtract=0){
  tree->SetMarkerSize(0.8);
  tree->Draw("1000*r:theta:1000*z-50","","colz");
  c1->SaveAs("Top.png");
  exit();
}
