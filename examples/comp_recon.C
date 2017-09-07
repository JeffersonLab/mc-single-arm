#include <TString.h>
#include <TLegend.h>
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
void comp_recon() {
 gROOT->Reset();
 gStyle->SetOptStat(0);
 gStyle->SetOptFit(11);
 gStyle->SetTitleOffset(1.,"Y");
 gStyle->SetTitleOffset(.7,"X");
 gStyle->SetLabelSize(0.04,"XY");
 gStyle->SetTitleSize(0.06,"XY");
 gStyle->SetPadLeftMargin(0.14);
  TString label1=" Compare Delta Recon/Inital";
   TString label2=" 2 GeV , 20cm target, wire ch, mscat";
   TString xlabel= label1 + " delta " + label2;
    TString basename = "shms_20cmtarg_20deg_2gev_wc_mscat_vac_cosy_6ord_for_and_rec_";
     TString outputpdf;
     // Change magnet name to "q1","q2" or "q3"
 TString magname="q1";
    outputpdf="outfiles/"+basename+magname+"_compare.pdf";
const UInt_t nftot=5;
TString q2name[nftot]={"ideal","_m0005","_m001","_m002","_m006"};
 TString lname[nftot];
 TString q1lname[nftot]={"Q1 ideal","Q1 at -0.05%","Q1 at -0.1%","Q1 at -0.2%","Q1 at -0.6%"};
 TString q2lname[nftot]={"Q2 ideal","Q2 at -0.05%","Q2 at -0.1%","Q2 at -0.2%","Q2 at -0.6%"};
 TString q3lname[nftot]={"Q3 ideal","Q3 at -0.05%","Q3 at -0.1%","Q3 at -0.2%","Q3 at -0.6%"};
  Int_t colind[nftot]={1,2,3,4,6};
 //
 TString h1name[4]={"hDeltadiff","hytardiff","hyptardiff","hxptardiff"};
 TString h2name[4]={"hDeltadiffvsDeltaini","hYtardiffvsYtarini","hYptardiffvsYptarini","hXptardiffvsXptarini"};
 Double_t sigmax[4]={.22,1.,3.,2.}; 
 Double_t sigmin[4]={0.,0.,0.,0.}; 
 Double_t rmsmax[4]={.2,1.,3.,2.}; 
 Double_t rmsmin[4]={-.2,-1,-3,-2.}; 
 Double_t meanmax[4]={.2,.2,3.,2.}; 
 Double_t meanmin[4]={-.2,-.2,-3.,-2.}; 
 //
  TString inputroot;
   TFile *fhistroot[nftot];
   TH2F *fhistsigma[nftot][4];
   TH2F *fhisttd[nftot][4];
   TH2F *fhistmean[nftot][4];
   TH2F *fhistrms[nftot][4];
   TH1F *fhistdiff[nftot][4];
   for (Int_t nf=0;nf<nftot;nf++) {
     if (nf==0) inputroot="worksim/"+basename+q2name[nf]+"_hist.root";
     if (nf>0) inputroot="worksim/"+basename+magname+q2name[nf]+"_hist.root";
     cout << " infile root = " << inputroot << endl;
   fhistroot[nf] =  new TFile(inputroot);
   if (magname == "q1") lname[nf]=q1lname[nf];
   if (magname == "q2") lname[nf]=q1lname[nf];
   if (magname == "q3") lname[nf]=q1lname[nf];
   for (Int_t nh=0;nh<4;nh++) {
   fhistsigma[nf][nh] = (TH2F*)fhistroot[nf]->Get(h2name[nh]+"_2");
   fhisttd[nf][nh] = (TH2F*)fhistroot[nf]->Get(h2name[nh]);
   fhistmean[nf][nh] = (TH2F*)fhistroot[nf]->Get(h2name[nh]+"_1");
   fhistrms[nf][nh] = (TH2F*)fhistroot[nf]->Get(h2name[nh]+"_px");
   fhistdiff[nf][nh] = (TH1F*)fhistroot[nf]->Get(h1name[nh]);
   }
   }
   //
   TCanvas *dcomp[4];
   TLegend *dleg[4];
   for (Int_t nc=0;nc<4;nc++) {
     dcomp[nc] =  new TCanvas(Form("dcomp_%d",nc),xlabel,1400,1000);
     dcomp[nc]->Divide(1,1);
     dcomp[nc]->cd(1);
     dleg[nc] = new TLegend(0.2,0.7,.4,.9,"");
   for (nf=0;nf<nftot;nf++) {
 fhistdiff[nf][nc]->SetLineColor(colind[nf]);
 dleg[nc]->AddEntry(fhistdiff[nf][nc],lname[nf],"l");
 if (nf==0)fhistdiff[nf][nc]->Draw();
 if (nf!=0)fhistdiff[nf][nc]->Draw("same");
   }
   dleg[nc]->Draw();
   }
   //
   TString ht[4]={" delta diff vs deltai "," Ytar diff vs Ytari "," Yptar diff vs Yptari "," Xptar diff vs Xptari "};
   TCanvas *ccomp[4];
   for (Int_t nc=0;nc<4;nc++) {
   TString xlabel= label1 + ht[nc] + label2;
   ccomp[nc] = new TCanvas(Form("ccomp_%d",nc),xlabel,1400,1000);
    ccomp[nc]->Divide(2,3);
   for (nf=0;nf<nftot;nf++) {
     ccomp[nc]->cd(nf+1);
     fhisttd[nf][nc]->SetTitle(ht[nc]+lname[nf]);
     fhisttd[nf][nc]->Draw("colz");
   }
   if (nc ==0) ccomp[nc]->Print(outputpdf+"(");
   if (nc !=0)ccomp[nc]->Print(outputpdf);
   }
   //
   //
   TCanvas *rmscomp[4];
   //
   for (Int_t nc=0;nc<4;nc++) {
   TString xlabel= label1 + ht[nc] + label2;
   rmscomp[nc] = new TCanvas(Form("rmscomp_%d",nc),xlabel,1400,1000);
   rmscomp[nc]->Divide(2,3);
   for (nf=0;nf<nftot;nf++) {
     rmscomp[nc]->cd(nf+1);
     fhistrms[nf][nc]->SetTitle(ht[nc]+lname[nf]);
     fhistrms[nf][nc]->SetMaximum(rmsmax[nc]);
     fhistrms[nf][nc]->SetMinimum(rmsmin[nc]);
     fhistrms[nf][nc]->Draw();
   }
   rmscomp[nc]->Print(outputpdf);
   }
   //
   //
   TCanvas *sigmacomp[4];
   TLegend *sigmaleg[4];
   for (Int_t nc=0;nc<4;nc++) {
     sigmacomp[nc] = new TCanvas(Form("sigmacomp_%d",nc),xlabel,1400,1000);
        sigmacomp[nc]->Divide(1,1);
    sigmaleg[nc]=new TLegend(0.2,0.7,.4,.9,"");
    sigmacomp[nc]->cd(1);
   for (nf=0;nf<nftot;nf++) {
 fhistsigma[nf][nc]->SetMaximum(sigmax[nc]);
 fhistsigma[nf][nc]->SetMinimum(sigmin[nc]);
 fhistsigma[nf][nc]->SetLineColor(colind[nf]);
 sigmaleg[nc]->AddEntry(fhistsigma[nf][nc],lname[nf],"l");
 if (nf==0)fhistsigma[nf][nc]->Draw();
 if (nf!=0)fhistsigma[nf][nc]->Draw("same");
   }
   sigmaleg[nc]->Draw();
   //
   sigmacomp[nc]->Print(outputpdf);
   }
   //
   TCanvas *meancomp[4];
   TLegend *meanleg[4];
   for (Int_t nc=0;nc<4;nc++) {
     meancomp[nc] = new TCanvas(Form("meancomp_%d",nc),xlabel,1400,1000);
     meancomp[nc]->Divide(1,1);
meanleg[nc]=new TLegend(0.2,0.7,.4,.9,"");
meancomp[nc]->cd(1);
   for (nf=0;nf<nftot;nf++) {
 fhistmean[nf][nc]->SetMaximum(meanmax[nc]);
 fhistmean[nf][nc]->SetMinimum(meanmin[nc]);
 fhistmean[nf][nc]->SetLineColor(colind[nf]);
 meanleg[nc]->AddEntry(fhistmean[nf][nc],lname[nf],"l");
 if (nf==0)fhistmean[nf][nc]->Draw();
 if (nf!=0)fhistmean[nf][nc]->Draw("same");
   }
   meanleg[nc]->Draw();
      if (nc !=3)meancomp[nc]->Print(outputpdf);
      if (nc ==3)meancomp[nc]->Print(outputpdf+")");
   }

}
