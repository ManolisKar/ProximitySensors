int getMinMax(){
  Double_t min,max;

  TFile *fb = new TFile("Bot-quad.root","r");
  TTree *treeb=(TTree*)fb->Get("tree");
  min = 1000*treeb->GetMinimum("r");
  max = 1000*treeb->GetMaximum("r");
  //printf("Bottom plate r range: %5.2f - %5.2f\n", min, max);
  printf("Bottom <r> = %5.2f\n" , (min+max)/2-7112);
  fb->Close();

  TFile *ft = new TFile("Top-quad.root","r");
  TTree *treet=(TTree*)ft->Get("tree");
  min = 1000*treet->GetMinimum("r");
  max = 1000*treet->GetMaximum("r");
  //printf("Top plate r range: %5.2f - %5.2f\n", min, max);
  printf("Top <r> = %5.2f\n" , (min+max)/2-7112);
  ft->Close();

  TFile *fi = new TFile("Inner-quad.root","r");
  TTree *treei=(TTree*)fi->Get("tree");
  min = 1000*treei->GetMinimum("z");
  max = 1000*treei->GetMaximum("z");
  //printf("Inner plate z range: %5.2f - %5.2f\n", min, max);
  printf("Inner <z> = %5.2f\n" , (min+max)/2);
  fi->Close();

  TFile *fo = new TFile("Outer-quad.root","r");
  TTree *treeo=(TTree*)fo->Get("tree");
  min = 1000*treeo->GetMinimum("z");
  max = 1000*treeo->GetMaximum("z");
  //printf("Outer plate z range: %5.2f - %5.2f\n", min, max);
  printf("Outer <z> = %5.2f\n" , (min+max)/2);
  fo->Close();

  
  exit();
}
