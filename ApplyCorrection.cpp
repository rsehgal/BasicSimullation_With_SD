/*
 * ReadExactHitData.cpp
 *
 *  Created on: 17-Dec-2020
 *      Author: rsehgal
 */

#include <iostream>
#include <TFile.h>
#include <vector>
#include <iterator>
#include <TH1F.h>
#include <TH1D.h>
#include <TApplication.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TF1.h>
int main(int argc, char *argv[])
{

  TApplication *fApp = new TApplication("Test", NULL, NULL);
  // std::string outputFileName=argv[1];
  TFile *fp   = new TFile("acceptance.root", "READ");
  TH1F *zenithAngleCry = (TH1F *)fp->Get("HistCry");
  TH1F *zenithAngleDet = (TH1F *)fp->Get("DetCry");

  new TCanvas("BareZenithAngle_CRY","BareZenithAngle_CRY");
  zenithAngleCry->Draw();

  new TCanvas("BareZenithAngle_Det","BareZenithAngle_Det");
  zenithAngleDet->Draw();


  unsigned int nbins = zenithAngleDet->GetNbinsX();
  TH1F *acceptanceHist = new TH1F("AcceptanceHist", "AcceptanceHist", nbins, 0.0, M_PI / 2.);
  TH1F *acceptanceCorrHist = new TH1F("AcceptanceCorrHist", "AcceptanceCorrHist", nbins, 0.0, M_PI / 2.);
  


  // double startBinVal = solidAngleCorrectedHist->GetBinContent(0);
  for (int i = 0; i < nbins; i++) {
    int refVal     = zenithAngleCry->GetBinContent(i);
    int currentVal = zenithAngleDet->GetBinContent(i);
    if (refVal > 0. && currentVal > 0.) {
      double weightFactor = (1.0 * currentVal) / (1.0 * refVal);
      acceptanceHist->SetBinContent(i, weightFactor);

      std::cout << "Weight Factor : " << weightFactor << " : CurrentVal : " << currentVal << std::endl;

      if (weightFactor >= 0.) {
        acceptanceCorrHist->SetBinContent(i, currentVal / weightFactor);
      }
    }
  }

  TF1 *formula = new TF1("Cos Square", "[0]*pow(cos(x),[1])*sin(x)*cos(x)", 0, M_PI / 2.);
  //acceptanceCorrHist->Fit(formula, "r");


  new TCanvas("Acceptance_Can","Acceptance_Can");
  acceptanceHist->Draw();
  
  new TCanvas("AcceptanceCorr_Can","AcceptanceCorr_Can");
  acceptanceCorrHist->Draw();

  fApp->Run();
  return 0;
}
