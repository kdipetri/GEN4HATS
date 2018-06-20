// Draw genjet pT for TTBar nominal and systematics scale up samples
{
  // Open the nominal file and get the histograms
  TFile *_file0 = TFile::Open("root://cmseos.fnal.gov//store/mc/RunIISpring16MiniAODv2/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext3-v2/70000/00287FF4-0E40-E611-8D06-00266CFE78EC.root");
  TTree* events0 = (TTree*)_file0->Get("Events");
  TH1F* hpt0 = new TH1F("hpt0","hpt0",30,0.,1200.);
  events0->Draw("recoGenJets_slimmedGenJets__PAT.obj.pt()>>hpt0","","goff");
  hpt0->SetLineColor(kMagenta+1);
  int nbins0 = hpt0->GetNbinsX();
  double yield0 = hpt0->Integral(0,nbins0+1);
  hpt0->Scale(1/yield0);

  TH1F* hnj0 = new TH1F("hnj0","hnj0",30,0,30);
  events0->Draw("recoGenJets_slimmedGenJets__PAT.@obj.size()>>hnj0","","goff");
  hnj0->SetLineColor(kMagenta+1);
  int nbins0nj = hnj0->GetNbinsX();
  double yield0nj = hnj0->Integral(0,nbins0nj+1);
  hnj0->Scale(1/yield0nj);

  // Open the scale up file and get the histograms
  TFile *_file1 = TFile::Open("root://cmseos.fnal.gov//store/mc/RunIISpring16MiniAODv2/TT_TuneCUETP8M1_13TeV-powheg-scaleup-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/50000/04E7DF1C-E11C-E611-BD6F-003048C7B87C.root");
  TTree* events1 = (TTree*)_file1->Get("Events");
  TH1F* hpt1 = new TH1F("hpt1","hpt1",30,0.,1200.);
  events1->Draw("recoGenJets_slimmedGenJets__PAT.obj.pt()>>hpt1","","goff");
  int nbins1 = hpt1->GetNbinsX();
  double yield1 = hpt1->Integral(0,nbins1+1);
  hpt1->Scale(1/yield1);

  TH1F* hnj1 = new TH1F("hnj1","hnj1",30,0,30);
  events1->Draw("recoGenJets_slimmedGenJets__PAT.@obj.size()>>hnj1","","goff");
  int nbins1nj = hnj1->GetNbinsX();
  double yield1nj = hnj1->Integral(0,nbins1nj+1);
  hnj1->Scale(1/yield1nj);


  gStyle->SetOptStat(0);
  TCanvas* c = new TCanvas("c", "c", 900, 900);
  c->Divide(2, 2);
  c->cd(1);
  c->cd(1)->SetLogy(1);
  hpt0->Draw();
  hpt1->Draw("same");
  TLegend *l = new TLegend(0.60, 0.65, 0.90, 0.85);
  l->SetBorderSize(0);
  l->SetFillStyle(0000);
  l->SetTextSize(0.042);
  l->AddEntry(hpt0,"Nominal", "l");
  l->AddEntry(hpt1,"Scale up", "l");
  l->Draw("same");

  c->cd(2);
  TH1F* hptsub = (TH1F*)hpt0->Clone();
  hptsub->Divide(hpt1);
  hptsub->SetLineColor(kRed);
  hptsub->SetLineWidth(2);
  hptsub->SetTitle("GenJetPt(TT) / GenJetPt(TT_scaleup)");
  hptsub->Draw();

  c->cd(3);
  c->cd(3)->SetLogy(1);
  hnj0->Draw();
  hnj1->Draw("same");
  l->Draw("same");

  c->cd(4);
  TH1F* hnjsub = (TH1F*)hnj0->Clone();
  hnjsub->Divide(hnj1);
  hnjsub->SetLineColor(kRed);
  hnjsub->SetLineWidth(2);
  hnjsub->SetTitle("#GenJet(TT) / #GenJet(TT_scaleup)");
  hnjsub->SetMaximum(4);
  hnjsub->Draw();


}
