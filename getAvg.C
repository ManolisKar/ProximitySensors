int getAvg(){
  TFile *fb = new TFile("Bot-quad.root","r");
  TTree *treeb=(TTree*)fb->Get("tree");
  treeb->Draw("1000*z + 50 >> hb","","goff");
  printf("Bottom <z> = %5.2f +- %4.2f\n" , hb->GetMean(), hb->GetMeanError());
  fb->Close();

  TFile *ft = new TFile("Top-quad.root","r");
  TTree *treet=(TTree*)ft->Get("tree");
  treet->Draw("1000*z - 50 >> ht","","goff");
  printf("Top   <z> = %5.2f +- %4.2f\n" , ht->GetMean(), ht->GetMeanError());
  ft->Close();
  
  TFile *fi = new TFile("Inner-quad.root","r");
  TTree *treei=(TTree*)fi->Get("tree");
  treei->Draw("1000*r - 7062 >> hi","","goff");
  printf("Inner <r> = %5.2f +- %4.2f\n" , hi->GetMean(), hi->GetMeanError());
  fi->Close();
  
  TFile *fo = new TFile("Outer-quad.root","r");
  TTree *treeo=(TTree*)fo->Get("tree");
  treeo->Draw("1000*r - 7162 >> ho","","goff");
  printf("Outer <r> = %5.2f +- %4.2f\n" , ho->GetMean(), ho->GetMeanError());
  fo->Close();

  exit();
}
