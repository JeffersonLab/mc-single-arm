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
void comp_fp() {
 gROOT->Reset();
 gStyle->SetOptStat(0);
 gStyle->SetOptFit(11);
 gStyle->SetTitleOffset(1.,"Y");
 gStyle->SetTitleOffset(.7,"X");
 gStyle->SetLabelSize(0.04,"XY");
 gStyle->SetTitleSize(0.06,"XY");
 gStyle->SetPadLeftMargin(0.14);
    TString basename = "shms_pointtarg_15deg_3gev_wc_mscat_vac_";
    TString basename2 = "/group/hallc/mkj/shms_study/shms_replay_production_";
     TString outputpdf;
    outputpdf="outfiles/"+basename+"_q1_compare.pdf";
const UInt_t nftot=5;
const UInt_t nplots=6;
 TString q2name[nftot]={"ideal","q1_m10","q2_m015","dipole_rotx_m5deg_q2_m015","481"};
 TString lname[nftot]={"ideal"," Q1 at 90%"," Q2 at 98.5"," Dip rot -5 deg Q2 at 98.5 "," Run 481 data"};
 //
 TString h1name[nplots]={"h_xfp_yfp","h_xfp_xpfp","h_xpfp_ypfp","h_yfp_ypfp","h_xfp_ypfp","h_yfp_xpfp"};
 TString h2name[nplots]={"pdc_xfp_vs_yfp","pdc_xfp_vs_xpfp","pdc_xpfp_vs_ypfp","pdc_yfp_vs_ypfp","pdc_xfp_vs_ypfp","pdc_yfp_vs_xpfp"};
 //
  TString inputroot;
   TFile *fhistroot[nftot];
   TH2F *fhist[nftot][6];
  for (Int_t nf=0;nf<nftot;nf++) {
     inputroot="worksim/"+basename+q2name[nf]+"_hist.root";
     if (nf==nftot-1) inputroot=basename2+q2name[nf]+"_1000000.root";
     cout << " infile root = " << inputroot << endl;
   fhistroot[nf] =  new TFile(inputroot);
   for (Int_t nh=0;nh<nplots;nh++) {
     if (nf!=(nftot-1)) fhist[nf][nh] = (TH2F*)fhistroot[nf]->Get(h1name[nh]);
     if (nf==(nftot-1)) fhist[nf][nh] = (TH2F*)fhistroot[nf]->Get(h2name[nh]);
   }
   }
   //
   TCanvas *ccomp[6];
   for (Int_t nc=0;nc<nplots;nc++) {
   TString xlabel= h1name[nc];
   ccomp[nc] = new TCanvas(Form("ccomp_%d",nc),xlabel,1400,1000);
    ccomp[nc]->Divide(2,3);
   for (nf=0;nf<nftot;nf++) {
     ccomp[nc]->cd(nf+1);
   gPad->SetLogz();
   gPad->SetGridx();
   gPad->SetGridy();
     fhist[nf][nc]->SetTitle(lname[nf]);
     fhist[nf][nc]->SetMinimum(1);
     fhist[nf][nc]->SetMaximum(2000);
     if (nf == (nftot-1) && nc==0) fhist[nf][nc]->SetNdivisions(-508,"Y");
     if (nf == (nftot-1) && nc!=0) fhist[nf][nc]->SetNdivisions(-510,"Y");
     fhist[nf][nc]->Draw("colz");
   }
   ccomp[nc]->cd(6);
   gPad->SetLogz();
   gPad->SetGridx();
   gPad->SetGridy();
     fhist[nftot-1][nc]->Draw("colz");
   if (nc ==0) ccomp[nc]->Print(outputpdf+"(");
   if (nc >0 )ccomp[nc]->Print(outputpdf);
   }
   //
   cleg = new TLegend(0.3,0.1,.7,.5,"") ;
   Int_t colind[4]={2,3,4,6};
   ccomp1 = new TCanvas("ccomp1"," 1d projection",1400,1000);
   ccomp1->Divide(2,2);
   ccomp1->cd(1);
   TH1D *projxfp_data = fhist[4][0]->ProjectionX("projxfp_data_px");
   cleg->AddEntry(projxfp_data,lname[4],"l");
   projxfp_data->Draw();
   projxfp_data->SetTitle(" Data vs MC");
   TH1D *projxfp_mc[4];
     for (Int_t i=0;i<4;i++) {
       projxfp_mc[i]= fhist[i][0]->ProjectionX(Form("projxfp_mc_px_%d",i));
   projxfp_mc[i]->Draw("same");
   projxfp_mc[i]->SetLineColor(colind[i]);
   cleg->AddEntry(projxfp_mc[i],lname[i],"l");
     }
   cleg->Draw();
   ccomp1->cd(2);
   TH1D *projyfp_data = fhist[4][0]->ProjectionY("projyfp_data_py");
   projyfp_data->Draw();
   projyfp_data->SetTitle(" Data vs MC");
   TH1D *projyfp_mc[4];
     for (Int_t i=0;i<4;i++) {
   projyfp_mc[i] = fhist[i][0]->ProjectionY(Form("projyfp_mc_py_%d",i));
   projyfp_mc[i]->Draw("same");
   projyfp_mc[i]->SetLineColor(colind[i]);
     }
  ccomp1->cd(3);
   TH1D *projxpfp_data = fhist[4][2]->ProjectionX("projxpfp_data_px");
   projxpfp_data->Draw();
   projxpfp_data->SetTitle(" Data vs MC");
   TH1D *projxpfp_mc[4];
     for (Int_t i=0;i<4;i++) {
   projxpfp_mc[i] = fhist[i][2]->ProjectionX(Form("projxpfp_mc_px_%d",i));
   projxpfp_mc[i]->Draw("same");
   projxpfp_mc[i]->SetLineColor(colind[i]);
     }
   ccomp1->cd(4);
   TH1D *projypfp_data = fhist[4][2]->ProjectionY("projypfp_data_py");
   projypfp_data->Draw();
   projypfp_data->SetTitle(" Data vs MC");
   TH1D *projypfp_mc[4];
     for (Int_t i=0;i<4;i++) {
   projypfp_mc[i] = fhist[i][2]->ProjectionY(Form("projypfp_mc_py_%d",i));
   projypfp_mc[i]->Draw("same");
   projypfp_mc[i]->SetLineColor(colind[i]);
     }
   ccomp1->Print(outputpdf+")");
   //

}
