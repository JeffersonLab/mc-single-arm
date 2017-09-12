#include <iostream>
#include <TString.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TROOT.h>
#include <TMath.h>
#include <TLegend.h>
#include <TProfile.h>
#include <TObjArray.h>
void plot_fp(TString basename="") {
   if (basename=="") {
     cout << " Input the basename of the root file (assumed to be in worksim)" << endl;
     cin >> basename;
   }
 gROOT->Reset();
gStyle->SetPalette(1,0);
 gStyle->SetOptStat(0);
 gStyle->SetOptFit(11);
 gStyle->SetTitleOffset(1.,"Y");
 gStyle->SetTitleOffset(.7,"X");
 gStyle->SetLabelSize(0.04,"XY");
 gStyle->SetTitleSize(0.06,"XY");
 gStyle->SetPadLeftMargin(0.12);
 TString htitle="";
 TPaveLabel *title = new TPaveLabel(.15,.90,0.95,.99,htitle,"ndc");
   TString inputroot;
   inputroot="worksim/"+basename+".root";
   TString outputhist;
   outputhist="worksim/"+basename+"_hist.root";
 TObjArray HList(0);
     TString outputpdf;
    outputpdf="outfiles/"+basename+".pdf";
 //   inputroot="rootfiles/"+basename+".root";
 TString htitle=basename;
TFile *fsimc = new TFile(inputroot);
TTree *tsimc = (TTree*)fsimc->Get("h1411");
 TObjArray HList(0);
 Float_t         psxfp; // position at focal plane ,+X is pointing down
 Float_t         psyfp; // X x Y = Z so +Y pointing central ray left
 Float_t         psxpfp; // dx/dz at focal plane
 Float_t         psypfp; //  dy/dz at focal plane
 Float_t         psztari; // thrown position along the beam direction
 Float_t         psytari;  //thrown  horizontal position X x Y = Z so +Y pointing central ray left at plane perpendicular to SHMS at z=0
 Float_t         psdeltai; // thrown  100*(p - pc)/pc with pc = central SHMS momentum 
 Float_t         psyptari; // thrown target dy/dz horizontal slope
 Float_t         psxptari; // thrown target dx/dz vertical slope
 Float_t         psztar; // reconstructed position along the beam directio
 Float_t         psytar; //reconstructed horizontal position
 Float_t         psdelta;//reconstructed
   Float_t         psyptar;//reconstructed
   Float_t         psxptar;//reconstructed
   Float_t         psxtari;// from thrown kinematics , the calculated vertical position at plane perpendicular to SHMS at z=0
   Float_t normfac;
   // Set branch addresses.
   tsimc->SetBranchAddress("psxfp",&psxfp);
   tsimc->SetBranchAddress("psyfp",&psyfp);
   tsimc->SetBranchAddress("psxpfp",&psxpfp);
   tsimc->SetBranchAddress("psypfp",&psypfp);
   tsimc->SetBranchAddress("psztari",&psztari);
   tsimc->SetBranchAddress("psytari",&psytari);
   tsimc->SetBranchAddress("psdeltai",&psdeltai);
   tsimc->SetBranchAddress("psyptari",&psyptari);
   tsimc->SetBranchAddress("psxptari",&psxptari);
   tsimc->SetBranchAddress("psztar",&psztar);
   tsimc->SetBranchAddress("psytar",&psytar);
   tsimc->SetBranchAddress("psdelta",&psdelta);
   tsimc->SetBranchAddress("psyptar",&psyptar);
   tsimc->SetBranchAddress("psxptar",&psxptar);
   tsimc->SetBranchAddress("psxtari",&psxtari);
   tsimc->SetBranchAddress("stop_id",&normfac);
   TH2F* h_xfp_yfp;
   h_xfp_yfp = new TH2F("h_xfp_yfp",";x_fp;y_fp",100,-50,50,100,-40,40);
   TH2F* h_xfp_ypfp;
   h_xfp_ypfp = new TH2F("h_xfp_ypfp",";x_fp;y_pfp",100,-50,50,100,-.05,.05);
   TH2F* h_xfp_xpfp;
   h_xfp_xpfp = new TH2F("h_xfp_xpfp",";x_fp;xp_fp",100,-50,50,100,-.1,.1);
   TH2F* h_yfp_ypfp;
   h_yfp_ypfp = new TH2F("h_yfp_ypfp",";y_fp;yp_fp",100,-40,40,100,-.05,.05);
   TH2F* h_yfp_xpfp;
   h_yfp_xpfp = new TH2F("h_yfp_xpfp",";y_fp;xp_fp",100,-40,40,100,-.1,.1);
   TH2F* h_xpfp_ypfp;
   h_xpfp_ypfp = new TH2F("h_xpfp_ypfp",";xp_fp;yp_fp",100,-.1,.1,100,-.05,.05);
   //
   Float_t cfac = 731.6*.5073*(3800/20.)*(.80)*1.12;
   Float_t weight;
    Long64_t nentries = tsimc->GetEntries();
   for (int i = 0; i < nentries; i++) {
        tsimc->GetEntry(i);
        weight=normfac*cfac;
        h_xfp_yfp->Fill(psxfp,psyfp,weight);
        h_xfp_ypfp->Fill(psxfp,psypfp,weight);
        h_xfp_xpfp->Fill(psxfp,psxpfp,weight);
        h_yfp_ypfp->Fill(psyfp,psypfp,weight);
        h_yfp_xpfp->Fill(psyfp,psxpfp,weight);
        h_xpfp_ypfp->Fill(psxpfp,psypfp,weight);
   }
   //
TCanvas *cfp = new TCanvas("cfp","Focal plane ",1400,900);
 cfp->Divide(2,3);
 gStyle->SetGridStyle(1);
 cfp->cd(1);
   cfp_1->SetLogz();
   cfp_1->SetGridx();
   cfp_1->SetGridy();
 h_xfp_yfp->Draw("colz");
 HList.Add(h_xfp_yfp);
 title->Draw();
 cfp->cd(2);
   cfp_2->SetLogz();
   cfp_2->SetGridx();
   cfp_2->SetGridy();
 h_xfp_xpfp->Draw("colz");
 HList.Add(h_xfp_xpfp);
 cfp->cd(3);
   cfp_3->SetLogz();
   cfp_3->SetGridx();
   cfp_3->SetGridy();
 h_xpfp_ypfp->Draw("colz");
 HList.Add(h_xpfp_ypfp);
 cfp->cd(4);
    cfp_4->SetLogz();
   cfp_4->SetGridx();
   cfp_4->SetGridy();
h_yfp_ypfp->Draw("colz");
 HList.Add(h_yfp_ypfp);
 cfp->cd(5);
    cfp_5->SetLogz();
   cfp_5->SetGridx();
   cfp_5->SetGridy();
h_yfp_xpfp->Draw("colz");
 HList.Add(h_yfp_xpfp);
 cfp->cd(6);
    cfp_6->SetLogz();
   cfp_6->SetGridx();
   cfp_6->SetGridy();
 h_xfp_ypfp->Draw("colz");
 HList.Add(h_xfp_ypfp);
 cfp->Print(outputpdf);
//
 TFile hsimc(outputhist,"recreate");
 HList.Write();
 cout << " Plotted histograms put in root file = " << outputhist << endl;

}
