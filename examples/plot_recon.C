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
void plot_recon(TString basename="") {
   if (basename=="") {
     cout << " Input the basename of the root file (assumed to be in worksim)" << endl;
     cin >> basename;
   }
 gROOT->Reset();
gStyle->SetPalette(1,0);
 gStyle->SetOptStat(1);
 gStyle->SetOptFit(11);
 gStyle->SetTitleOffset(1.,"Y");
 gStyle->SetTitleOffset(.7,"X");
 gStyle->SetLabelSize(0.04,"XY");
 gStyle->SetTitleSize(0.06,"XY");
 gStyle->SetPadLeftMargin(0.12);
   TString inputroot;
   inputroot="worksim/"+basename+".root";
   TString outputhist;
   outputhist="worksim/"+basename+"_hist.root";
   //  TString outputpdf;
   // outputpdf="outfiles/"+basename+".pdf";
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
   //
 TH1F *hDeltadiff = new TH1F("hDeltadiff","Delta Diff %; Delta_recon- Delta_ini (%) ; Counts ",40,-.3,.3);
 TH2F *hDeltadiffvsDeltaini = new TH2F("hDeltadiffvsDeltaini","Delta Diff versus Delta_ini; Delta_ini (%) ; Delta_recon- Delta_ini (%) ",40,-10,30,40,-.3,.3);
 TH1F *hytardiff = new TH1F("hytardiff","Ytar diff;  Ytg_recon- Ytg_ini (cm); Counts",70,-2.,2.);
 TH2F *hYtardiffvsYtarini = new TH2F("hYtardiffvsYtarini","Ytar Diff versus Ytar_ini; Ytar_ini (cm) ; Ytar_recon- Ytar_ini (%) ",40,-5.,5.,70,-2.,2.);
 TH1F *hxptardiff = new TH1F("hxptardiff","Xptar diff;  Xptg_recon- XPtg_ini (mr); Counts",100,-5,5);
 TH2F *hXptardiffvsXptarini = new TH2F("hXptardiffvsXptarini","Xptar Diff versus Xptar_ini; Xptar_ini (cm) ; Xptar_recon- Xptar_ini (%) ",40,-50.,50.,50,-5.,5.);
 TH1F *hyptardiff = new TH1F("hyptardiff","Yptar diff;  Yptg_recon- Yptg_ini (mr); Counts ",100,-10,10);
 TH2F *hYptardiffvsYptarini = new TH2F("hYptardiffvsYptarini","Yptar Diff versus Yptar_ini; Yptar_ini (cm) ; Yptar_recon- Yptar_ini (%) ",40,-40.,40.,50,-10.,10.);
   TH2F* h_xfp_yfp;
   h_xfp_yfp = new TH2F("h_xfp_yfp",";x_fp;y_fp",100,-50,50,100,-40,40);
   TH2F* h_xptar_delta;
   h_xptar_delta = new TH2F("h_xptar_delta",";delta;xp_tg",100,-15,30,100,-.05,.05);
   TH2F* h_xptar_yptar;
   h_xptar_yptar = new TH2F("h_xptar_yptar",";yp_tg;xp_tg",100,-0.05,.05,100,-.05,.05);
   TH2F* h_ytar_yptar;
   h_ytar_yptar = new TH2F("h_ytar_yptar",";yp_tg;y_tg",100,-0.05,.05,100,-3,3);
   TH2F* h_yptar_delta;
   h_yptar_delta = new TH2F("h_yptar_delta","delta;xp_tg",100,-15,30,100,-.05,.05);
   TH2F* h_xfp_xpfp;
   h_xfp_xpfp = new TH2F("h_xfp_xpfp",";x_fp;xp_fp",100,-50,50,100,-.1,.1);
   TH2F* h_yfp_ypfp;
   h_yfp_ypfp = new TH2F("h_yfp_ypfp",";y_fp;yp_fp",100,-40,40,100,-.05,.05);
   TH2F* h_xpfp_ypfp;
   h_xpfp_ypfp = new TH2F("h_xpfp_ypfp",";xp_fp;yp_fp",100,-.1,.1,100,-.05,.05);
   TH1F* h_xfp;
   TH1F* h_yfp;
   h_xfp= new TH1F("h_xfp",";x_fp",100,-60,60);
   h_yfp = new TH1F("h_yfp",";Yfp",100,-20,20);
   TH1F* h_xpfp;
   TH1F* h_ypfp;
   h_xpfp= new TH1F("h_xpfp",";Xp_fp",100,-.1,.1);
   h_ypfp = new TH1F("h_ypfp",";Yp_fp",100,-.05,.05);
   //;ysieve
    Long64_t nentries = tsimc->GetEntries();
   for (int i = 0; i < nentries; i++) {
        tsimc->GetEntry(i);
        h_xfp_yfp->Fill(psxfp,psyfp,1.);
        h_xfp_xpfp->Fill(psxfp,psxpfp,1.);
        h_yfp_ypfp->Fill(psyfp,psypfp,1.);
        h_xpfp_ypfp->Fill(psxpfp,psypfp,1.);
        h_xptar_delta->Fill(psdelta,psxptar,1.);
        h_xptar_yptar->Fill(psyptar,psxptar,1.);
        h_ytar_yptar->Fill(psyptar,psytar,1.);
        h_yptar_delta->Fill(psdelta,psyptar,1.);
        hDeltadiffvsDeltaini->Fill(psdeltai,psdelta-psdeltai);
        h_xfp->Fill(psxfp);
        h_yfp->Fill(psyfp);
        h_xpfp->Fill(psxpfp);
        h_ypfp->Fill(psypfp);
	hDeltadiff->Fill(psdelta-psdeltai);
	hytardiff->Fill(psytar-psytari);
	hYtardiffvsYtarini->Fill(psytari,psytar-psytari);
	hyptardiff->Fill(1000*(psyptar-psyptari));
      	hYptardiffvsYptarini->Fill(1000*psyptari,1000*(psyptar-psyptari));
	hxptardiff->Fill(1000*(psxptar-psxptari));
	hXptardiffvsXptarini->Fill(1000*psxptari,1000*(psxptar-psxptari));
   }
   //
TCanvas *cfpdd = new TCanvas("cfpdd","Delta diff ",1400,900);
 gStyle->SetOptTitle(1);
 TPaveLabel *title = new TPaveLabel(.25,.85,.35,.99,htitle,"ndc");
cfpdd->Divide(1,4);
 cfpdd->cd(1);
  hDeltadiffvsDeltaini->Draw("colz");
 HList.Add(hDeltadiffvsDeltaini);
   title->Draw();
 cfpdd->cd(2);
 hDeltadiffvsDeltaini->FitSlicesY();
 hDeltadiffvsDeltaini_2->SetTitle(";Init Delta; Fitted Sigma of Delta diff (%)");
 hDeltadiffvsDeltaini_2->Draw();
 HList.Add(hDeltadiffvsDeltaini_2);
 cfpdd->cd(3);
 hDeltadiffvsDeltaini_1->SetTitle(";Init Delta; Fitted Mean of Delta diff (% )");
 hDeltadiffvsDeltaini_1->Draw();
 HList.Add(hDeltadiffvsDeltaini_1);
cfpdd->cd(4);
 TProfile *px =  hDeltadiffvsDeltaini->ProfileX("hDeltadiffvsDeltaini_px",1,-1,"s");//
 hDeltadiffvsDeltaini_px->SetTitle(";Init Delta; Mean +/- RMS of Delta diff (% )");
 hDeltadiffvsDeltaini_px->Draw();
 HList.Add(hDeltadiffvsDeltaini_px);
   //
TCanvas *cfpytar = new TCanvas("cfpytar","Ytar diff ",1400,900);
 gStyle->SetOptTitle(1);
 TPaveLabel *title = new TPaveLabel(.25,.85,.35,.99,htitle,"ndc");
cfpytar->Divide(1,4);
 cfpytar->cd(1);
  hYtardiffvsYtarini->Draw("colz");
 HList.Add(hYtardiffvsYtarini);
   title->Draw();
 cfpytar->cd(2);
 hYtardiffvsYtarini->FitSlicesY();
 hYtardiffvsYtarini_2->SetTitle(";Init Ytar; Fitted Sigma of Ytar diff (cm");
 hYtardiffvsYtarini_2->Draw();
 HList.Add(hYtardiffvsYtarini_2);
 cfpytar->cd(3);
 hYtardiffvsYtarini_1->SetTitle(";Init Ytar; Fitted Mean of Ytar diff (cm");
 hYtardiffvsYtarini_1->Draw();
 HList.Add(hYtardiffvsYtarini_1);
cfpytar->cd(4);
 TProfile *px =  hYtardiffvsYtarini->ProfileX("hYtardiffvsYtarini_px",1,-1,"s");//
 hYtardiffvsYtarini_px->SetTitle(";Init Ytar; Mean +/- RMS of Ytar diff (% )");
 hYtardiffvsYtarini_px->Draw();
 HList.Add(hYtardiffvsYtarini_px);
//
   //
TCanvas *cfpyptar = new TCanvas("cfpyptar","Yptar diff ",1400,900);
 gStyle->SetOptTitle(1);
 TPaveLabel *title = new TPaveLabel(.25,.85,.35,.99,htitle,"ndc");
cfpyptar->Divide(1,4);
 cfpyptar->cd(1);
  hYptardiffvsYptarini->Draw("colz");
 HList.Add(hYptardiffvsYptarini);
   title->Draw();
 cfpyptar->cd(2);
 hYptardiffvsYptarini->FitSlicesY();
 hYptardiffvsYptarini_2->SetTitle(";Init Yptar; Fitted Sigma of Yptar diff (mr)");
 hYptardiffvsYptarini_2->Draw();
 HList.Add(hYptardiffvsYptarini_2);
 cfpyptar->cd(3);
 hYptardiffvsYptarini_1->SetTitle(";Init Yptar; Fitted Mean of Yptar diff (mr)");
 hYptardiffvsYptarini_1->Draw();
 HList.Add(hYptardiffvsYptarini_1);
cfpyptar->cd(4);
 TProfile *px =  hYptardiffvsYptarini->ProfileX("hYptardiffvsYptarini_px",1,-1,"s");//
 hYptardiffvsYptarini_px->SetTitle(";Init Yptar; Mean +/- RMS of Yptar diff (% )");
 hYptardiffvsYptarini_px->Draw();
 HList.Add(hYptardiffvsYptarini_px);

//
   //
TCanvas *cfpxptar = new TCanvas("cfpxptar","Xptar diff ",1400,900);
 gStyle->SetOptTitle(1);
 TPaveLabel *title = new TPaveLabel(.25,.85,.35,.99,htitle,"ndc");
cfpxptar->Divide(1,4);
 cfpxptar->cd(1);
  hXptardiffvsXptarini->Draw("colz");
 HList.Add(hXptardiffvsXptarini);
   title->Draw();
 cfpxptar->cd(2);
 hXptardiffvsXptarini->FitSlicesY();
 hXptardiffvsXptarini_2->SetTitle(";Init Xptar; Fitted Sigma of Xptar diff (mr)");
 hXptardiffvsXptarini_2->Draw();
 HList.Add(hXptardiffvsXptarini_2);
 cfpxptar->cd(3);
 hXptardiffvsXptarini_1->SetTitle(";Init Xptar; Fitted Mean of Xptar diff (mr)");
 hXptardiffvsXptarini_1->Draw();
 HList.Add(hXptardiffvsXptarini_1);
cfpxptar->cd(4);
 TProfile *px =  hXptardiffvsXptarini->ProfileX("hXptardiffvsXptarini_px",1,-1,"s");//
 hXptardiffvsXptarini_px->SetTitle(";Init Xptar; Mean +/- RMS of Xptar diff (% )");
 hXptardiffvsXptarini_px->Draw();
 HList.Add(hXptardiffvsXptarini_px);
//
TCanvas *cfpd = new TCanvas("cfpd","Diff",1400,900);
 gStyle->SetOptTitle(1);
 TPaveLabel *title = new TPaveLabel(.15,.85,.35,.99,htitle,"ndc");
cfpd->Divide(2,2);
 cfpd->cd(1);
 hDeltadiff->Draw();
 HList.Add(hDeltadiff);
   title->Draw();
 cfpd->cd(2);
 hytardiff->Draw();
 HList.Add(hytardiff);
 cfpd->cd(3);
 hyptardiff->Draw();
 HList.Add(hyptardiff);
 cfpd->cd(4);
 hxptardiff->Draw();
 HList.Add(hxptardiff);
   //
 
//
 TFile hsimc(outputhist,"recreate");
 HList.Write();
 hsimc.Close();
 cout << " Plotted histograms put in root file = " << outputhist << endl;
}
