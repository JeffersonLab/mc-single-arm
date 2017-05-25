#include <iostream>
#include <TString.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TMath.h>
#include <TProfile.h>
#include <TObjArray.h>
void plot_coll_fp(TString inputroot) {
 gROOT->Reset();
gStyle->SetPalette(1,0);
 gStyle->SetOptStat(0);
 gStyle->SetOptFit(11);
 gStyle->SetTitleOffset(1.,"Y");
 gStyle->SetTitleOffset(.7,"X");
 gStyle->SetLabelSize(0.04,"XY");
 gStyle->SetTitleSize(0.06,"XY");
 gStyle->SetPadLeftMargin(0.12);
 //   inputroot="rootfiles/"+basename+".root";
TFile *fsimc = new TFile(inputroot);
TTree *tsimc = (TTree*)fsimc->Get("h1411");
 Float_t         hsxfp; // position at focal plane ,+X is pointing down
 Float_t         hsyfp; // X x Y = Z so +Y pointing central ray left
 Float_t         hsxpfp; // dx/dz at focal plane
 Float_t         hsypfp; //  dy/dz at focal plane
 Float_t         hsztari; // thrown position along the beam direction
 Float_t         hsytari;  //thrown  horizontal position X x Y = Z so +Y pointing central ray left at plane perpendicular to SHMS at z=0
 Float_t         hsdeltai; // thrown  100*(p - pc)/pc with pc = central SHMS momentum 
 Float_t         hsyptari; // thrown target dy/dz horizontal slope
 Float_t         hsxptari; // thrown target dx/dz vertical slope
 Float_t         hsztar; // reconstructed position along the beam directio
 Float_t         hsytar; //reconstructed horizontal position
 Float_t         hsdelta;//reconstructed
   Float_t         hsyptar;//reconstructed
   Float_t         hsxptar;//reconstructed
   Float_t         hsxtari;// from thrown kinematics , the calculated vertical position at plane perpendicular to SHMS at z=0
   Float_t         yrast;// vertical raster position
   Float_t         xsnum;
   Float_t         ysnum;
   Float_t         xsieve;
   Float_t         ysieve;
   // Set branch addresses.
   tsimc->SetBranchAddress("hsxfp",&hsxfp);
   tsimc->SetBranchAddress("hsyfp",&hsyfp);
   tsimc->SetBranchAddress("hsxpfp",&hsxpfp);
   tsimc->SetBranchAddress("hsypfp",&hsypfp);
   tsimc->SetBranchAddress("hsztari",&hsztari);
   tsimc->SetBranchAddress("hsytari",&hsytari);
   tsimc->SetBranchAddress("hsdeltai",&hsdeltai);
   tsimc->SetBranchAddress("hsyptari",&hsyptari);
   tsimc->SetBranchAddress("hsxptari",&hsxptari);
   tsimc->SetBranchAddress("hsztar",&hsztar);
   tsimc->SetBranchAddress("hsytar",&hsytar);
   tsimc->SetBranchAddress("hsdelta",&hsdelta);
   tsimc->SetBranchAddress("hsyptar",&hsyptar);
   tsimc->SetBranchAddress("hsxptar",&hsxptar);
   tsimc->SetBranchAddress("hsxtari",&hsxtari);
   tsimc->SetBranchAddress("yrast",&yrast);
   tsimc->SetBranchAddress("xsnum",&xsnum);
   tsimc->SetBranchAddress("ysnum",&ysnum);
   tsimc->SetBranchAddress("xsieve",&xsieve);
   tsimc->SetBranchAddress("ysieve",&ysieve);
   //;ysieve
 TH1F *hxsnum = new TH1F("hxsnum",";xsnum ",44,-10.,12.);
 TH1F *hysnum = new TH1F("hysnum",";ysnum ",44,-10.,12.);
 TH1F *hxsieve = new TH1F("hxsieve",";xsieve (cm) ",100,-15.,15.);
 TH1F *hysieve = new TH1F("hysieve",";ysieve (cm)",100,-7.5,7.5);
TH2F *hysieveVSxsieve = new TH2F("hysieveVSxsieve",";ysieve (cm);xsieve (cm) ",200,-13.,13.,200,-18.,18.);
TH2F *hxfpVSxpfp = new TH2F("hxfpVSxpfp",";xpfp (mr);xfp (cm) ",100,-80.,80.,100,-40.,40.);
TH2F *hxfpVSypfp = new TH2F("hxfpVSypfp",";ypfp (mr);xfp (cm) ",100,-80.,80.,100,-40.,40.);
TH2F *hyfpVSypfp = new TH2F("hyfpVSypfp",";ypfp (mr);yfp (cm) ",100,-40.,40.,100,-25.,25.);
TH2F *hyfpVSxfp = new TH2F("hyfpVSxfp",";yfp (cm);xfp (cm) ",100,-40.,40.,100,-40.,40.);
TH2F *hypfpVSxpfp = new TH2F("hypfpVSxpfp",";ypfp (mr);xpfp (mr) ",100,-80.,80.,100,-80.,80.);
TH2F *hyptarVShsdelta = new TH2F("hyptarVShsdelta",";delta (%);Yp tar (mr) ",100,-17.,25.,100,-30.,30);
TH2F *hxptarVShsdelta = new TH2F("hxptarVShsdelta",";delta (%);Xp tar (mr) ",100,-17.,25.,100,-55.,55.);
TH2F *hyfpVSdel = new TH2F("hyfpVSdel",";yfp (cm);delta (%) ",100,-40.,40.,100,-20.,20.);
 const int numx = 11;
 const int numy = 9;
 Int_t ColIndx[numx]= {1,1,2,2,3,3,4,4,6,6,7};
 Int_t ColIndy[numy]= {1,1,2,2,3,3,4,4,6};
 TH2F *hyfpVSypfpperhole[numx][numy];
 TH2F *hxfpVSxpfpperhole[numx][numy];
 TH2F *hxfpVSyfpperhole[numx][numy];
 TH2F *hysieveVSxsieveperhole[numx][numy];
 TH2F *hyfpVSypfpperhole_xcut[numx][numy];
 TH2F *hxfpVSxpfpperhole_xcut[numx][numy];
 TH2F *hxfpVSyfpperhole_xcut[numx][numy];
 TH2F *hysieveVSxsieveperhole_xcut[numx][numy];
 for ( int ix = 0; ix < numx ; ix++) {
 for ( int iy = 0; iy < numy ; iy++) {
   //  hyfpVSypfpperhole[ix][iy] = new TH2F(Form("hyVyp_x%02d_y%02d ", ix,iy),Form("Hole x%02d_y%02d;ypfp (mr);yfp (cm) ", ix,iy),160,-40.,40.,120,-30.,30.);
     hyfpVSypfpperhole[ix][iy] = new TH2F(Form("hyVyp_x%02d_y%02d ", ix,iy),";ypfp (mr);yfp (cm) ",160,-40.,40.,120,-30.,30.);
   hxfpVSxpfpperhole[ix][iy] = new TH2F(Form("hxVxp_x%02d_y%02d ", ix,iy),";xpfp (mr);xfp (cm)  ",100,-100.,100.,160,-40.,40.);
   hxfpVSyfpperhole[ix][iy] = new TH2F(Form("hxVy_x%02d_y%02d ", ix,iy),";yfp (cm);xfp (cm)  ",120,-30.,30.,160,-40.,40.);
   hysieveVSxsieveperhole[ix][iy] = new TH2F(Form("hxs%02d_ys%02d ", ix,iy),";ysieve (cm);xsieve(cm) ",400,-8.,8.,300,-15.,15.);
     hyfpVSypfpperhole_xcut[ix][iy] = new TH2F(Form("hyVyp_x%02d_y%02d_xcut ", ix,iy),";ypfp (mr);yfp (cm) ",100,-40.,40.,100,-30.,30.);
   hxfpVSxpfpperhole_xcut[ix][iy] = new TH2F(Form("hxVxp_x%02d_y%02d_xcut ", ix,iy),";xpfp (mr);xfp (cm)  ",100,-100.,100.,100,-40.,40.);
   hxfpVSyfpperhole_xcut[ix][iy] = new TH2F(Form("hxVy_x%02d_y%02d_xcut ", ix,iy),";yfp (cm);xfp (cm)  ",100,-30.,30.,100,-40.,40.);
   hysieveVSxsieveperhole_xcut[ix][iy] = new TH2F(Form("hxs%02d_ys%02d_xcut ", ix,iy),";ysieve (cm);xsieve(cm) ",400,-8.,8.,300,-15.,15.);
 }
 }
 //
 int numxsh,numysh;
   Long64_t nentries = tsimc->GetEntries();
   for (int i = 0; i < nentries; i++) {
        tsimc->GetEntry(i);
	numxsh=xsnum+5;
	numysh=ysnum+4;
        if (TMath::Abs(hsdeltai) <= 20) {
	 	  if ( (numysh == 0 ||  numysh == 2 ||  numysh == 4 ||  numysh == 6  ||  numysh == 8) && 
	       (numxsh == 1 ||  numxsh == 3 ||  numxsh == 5 ||  numxsh == 7  ||  numxsh == 9 ||  numxsh == 11)) {
 	if ( numxsh < numx && numysh <numy ) {
	hyfpVSypfpperhole[numxsh][numysh]->Fill(hsypfp*1000.,hsyfp);
	hxfpVSxpfpperhole[numxsh][numysh]->Fill(hsxpfp*1000.,hsxfp);
	hxfpVSyfpperhole[numxsh][numysh]->Fill(hsyfp,hsxfp);
	hysieveVSxsieveperhole[numxsh][numysh]->Fill(ysieve,xsieve);
	hyfpVSypfpperhole_xcut[numxsh][numysh]->Fill(hsypfp*1000.,hsyfp);
	hxfpVSxpfpperhole_xcut[numxsh][numysh]->Fill(hsxpfp*1000.,hsxfp);
	hxfpVSyfpperhole_xcut[numxsh][numysh]->Fill(hsyfp,hsxfp);
	hysieveVSxsieveperhole_xcut[numxsh][numysh]->Fill(ysieve,xsieve);
	}
	  }
        hxsieve->Fill(xsieve);
        hysieve->Fill(ysieve);
        hxsnum->Fill(xsnum);
        hysnum->Fill(ysnum);
        hysieveVSxsieve->Fill(ysieve,xsieve);
        hxfpVSxpfp->Fill(hsxpfp*1000.,hsxfp);
        hxptarVShsdelta->Fill(hsdeltai,hsxptari*1000.);
        hyptarVShsdelta->Fill(hsdeltai,hsyptari*1000.);
        hxfpVSypfp->Fill(hsypfp*1000.,hsxfp);
        hyfpVSypfp->Fill(hsypfp*1000.,hsyfp);
        hyfpVSxfp->Fill(hsyfp,hsxfp);
        hypfpVSxpfp->Fill(hsypfp*1000.,hsxpfp*1000.);
        hyfpVSdel->Fill(hsyfp,hsdeltai);
        
        }
   }
   //
TCanvas *ctar = new TCanvas("ctar","sieve",1400,1000);
ctar->Divide(2,2);
ctar->cd(1);
 hxptarVShsdelta->Draw("colz");
 // hxsnum->Draw();
ctar->cd(2);
 hyptarVShsdelta->Draw("colz");
ctar->cd(3);
 hxsnum->Draw();
ctar->cd(4);
 hysnum->Draw();
 //
   //
TCanvas *cfpall = new TCanvas("cfpall","focal plane for all events",1400,1000);
cfpall->Divide(2,2);
cfpall->SetLogz(1);
cfpall->cd(1);
 gPad->SetLogz(1);
 hxfpVSypfp->Draw("colz");
cfpall->cd(2);
 gPad->SetLogz(1);
 hyfpVSypfp->Draw("colz");
cfpall->cd(3);
 gPad->SetLogz(1);
 hyfpVSxfp->Draw("colz");
cfpall->cd(4);
 gPad->SetLogz(1);
 hxfpVSxpfp->Draw("colz");
 //

TCanvas *cfp = new TCanvas("cfp","focal plane",1400,1000);
cfp->Divide(2,3);
cfp->cd(1);
 int ifirst=1;
 for ( int iy = 0; iy < numy ; iy++) {
 for ( int ix = 0; ix < numx ; ix++) {
   if (hyfpVSypfpperhole[ix][iy]->Integral() != 0) {
  if (ifirst==1) {
     hyfpVSypfpperhole[ix][iy]->Draw("box");
     ifirst=2;
  } else {
 hyfpVSypfpperhole[ix][iy]->Draw("box SAME");
 hyfpVSypfpperhole[ix][iy]->SetLineColor(ColIndy[iy]);
  }
   }
 }
 }
cfp->cd(3);
 ifirst=1;
 for ( int iy = 0; iy < numy ; iy++) {
 for ( int ix = 0; ix < numx ; ix++) {
   if (hxfpVSyfpperhole[ix][iy]->Integral() != 0) {
  if (ifirst==1) {
     hxfpVSyfpperhole[ix][iy]->Draw("box");
     ifirst=2;
  } else {
 hxfpVSyfpperhole[ix][iy]->Draw("box SAME");
 hxfpVSyfpperhole[ix][iy]->SetLineColor(ColIndy[iy]);
  }
   }
 }
 }
cfp->cd(5);
 ifirst=1;
 for ( int iy = 0; iy < numy ; iy++) {
 for ( int ix = 0; ix < numx ; ix++) {
   if (hysieveVSxsieveperhole[ix][iy]->Integral() != 0) {
  if (ifirst==1) {
     hysieveVSxsieveperhole[ix][iy]->Draw("box");
     ifirst=2;
  } else {
 hysieveVSxsieveperhole[ix][iy]->Draw("box SAME");
 hysieveVSxsieveperhole[ix][iy]->SetLineColor(ColIndy[iy]);
  }
   }
 }
 }
cfp->cd(2);
 ifirst=1;
 for ( int ix = 0; ix < numx ; ix++) {
 for ( int iy = 0; iy < numy ; iy++) {
   if (hxfpVSxpfpperhole_xcut[ix][iy]->Integral() != 0) {
  if (ifirst==1) {
     hxfpVSxpfpperhole_xcut[ix][iy]->Draw("box");
     ifirst=2;
  } else {
 hxfpVSxpfpperhole_xcut[ix][iy]->Draw("box SAME");
 hxfpVSxpfpperhole_xcut[ix][iy]->SetLineColor(ColIndx[ix]);
  }
   }
 }
 }
cfp->cd(4);
 ifirst=1;
 for ( int ix = 0; ix < numx ; ix++) {
 for ( int iy = 0; iy < numy ; iy++) {
   if (hxfpVSyfpperhole_xcut[ix][iy]->Integral() != 0) {
  if (ifirst==1) {
     hxfpVSyfpperhole_xcut[ix][iy]->Draw("box");
     ifirst=2;
  } else {
 hxfpVSyfpperhole_xcut[ix][iy]->Draw("box SAME");
 hxfpVSyfpperhole_xcut[ix][iy]->SetLineColor(ColIndx[ix]);
  }
   }
 }
 }
 

cfp->cd(6);
 ifirst=1;
 for ( int ix = 0; ix < numx ; ix++) {
 for ( int iy = 0; iy < numy ; iy++) {
   if (hysieveVSxsieveperhole_xcut[ix][iy]->Integral() != 0) {
  if (ifirst==1) {
     hysieveVSxsieveperhole_xcut[ix][iy]->Draw("box");
     ifirst=2;
  } else {
 hysieveVSxsieveperhole_xcut[ix][iy]->Draw("box SAME");
 hysieveVSxsieveperhole_xcut[ix][iy]->SetLineColor(ColIndx[ix]);
  }
   }
 }
 }
 
 //
}
