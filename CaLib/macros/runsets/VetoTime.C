// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// VetoTime.C                                                           //
//                                                                      //
// Make run sets depending on the stability in time of a calibration.   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TFile.h"
#include "TList.h"
#include "/disk/user/afzal/Mainz/acqu/CaLib/include/TCReadARCalib.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TF2.h"
#include "TH2.h"
#include "TSystem.h"
#include "/disk/user/afzal/Mainz/acqu/CaLib/include/TCMySQLManager.h"
#include "TLine.h"
#include "TGraph.h"
#include "/disk/user/afzal/Mainz/acqu/CaLib/include/TCFitTools.h"

TCanvas* gCFit;
TH1* gHOverview;
TH1* gH;
TH2* gH2;
TFile* f1;
TF1* gFitFunc;
TLine* gLine;


//______________________________________________________________________________
void Fit(Int_t run)
{
    // Perform fit.
    
    Char_t tmp[256];

    // delete old function
    if (gFitFunc) delete gFitFunc;
    sprintf(tmp, "fTime_%i", run);
    gFitFunc = new TF1(tmp, "gaus");
    gFitFunc->SetLineColor(2);
    
    // estimate peak position
    Double_t fPi0Pos = gH->GetBinCenter(gH->GetMaximumBin());

    // configure fitting function
    gFitFunc->SetRange(fPi0Pos - 3, fPi0Pos + 3);
    gFitFunc->SetLineColor(2);
    gFitFunc->SetParameters(gH->GetMaximum(), 0, 5);
    Int_t fitres = gH->Fit(gFitFunc, "RB0Q");
    
    // get position
    fPi0Pos = gFitFunc->GetParameter(1);

    // check failed fits
    if (fitres) 
    {
        printf("Run %d: fit failed\n", run);
        return;
    }

    // indicator line
    gLine->SetX1(fPi0Pos);
    gLine->SetX2(fPi0Pos);
    gLine->SetY1(0);
    gLine->SetY2(gH->GetMaximum());

    // draw 
    gCFit->cd();
    gH->GetXaxis()->SetRangeUser(-20, 20);
    gH->Draw();
    gFitFunc->Draw("same");
    gLine->Draw("same");

    // fill overview histogram
    gHOverview->SetBinContent(run+1, fPi0Pos);
    gHOverview->SetBinError(run+1, 0.0001);
}

//______________________________________________________________________________
void VetoTime()
{
    // Main method.
    
    Char_t tmp[256];
    
    // load CaLib
    gSystem->Load("libCaLib.so");
    
    // general configuration
    Bool_t watch = kFALSE;
    const Char_t* data = "Data.Veto.T0";
    const Char_t* hName = "CaLib_Veto_Time";
    Double_t yMin = -20;
    Double_t yMax = 20;

    // configuration (December 2007)
    const Char_t calibration[] = "2013_11_G-E_Linturi";
    const Char_t* fLoc = "/hiskp2/afzal/Mainz/Nov13/calibration/time/vetocorr";

    // configuration (February 2009)
    //const Char_t calibration[] = "LD2_Feb_09";
    //const Char_t* fLoc = "/usr/panther_scratch0/werthm/A2/Feb_09/AR/out/ADC";
    
    // configuration (May 2009)
    //const Char_t calibration[] = "LD2_May_09";
    //const Char_t* fLoc = "/usr/puma_scratch0/werthm/A2/May_09/AR/out";

    // create histogram
    gHOverview = new TH1F("Overview", "Overview", 40000, 0, 40000);
    TCanvas* cOverview = new TCanvas();
    gHOverview->GetYaxis()->SetRangeUser(yMin, yMax);
    gHOverview->Draw("E1");
    
    // create line
    gLine = new TLine();
    gLine->SetLineColor(kBlue);
    gLine->SetLineWidth(2);

    // init fitting function
    gFitFunc = 0;
    
    // create fitting canvas
    gCFit = new TCanvas();
    
    // get number of sets
    Int_t nSets = TCMySQLManager::GetManager()->GetNsets(data, calibration);
    
    // total number of runs
    Int_t nTotRuns = 0;

    // first and last runs
    Int_t first_run, last_run;

    // loop over sets
    for (Int_t i = 0; i < nSets; i++)
    {
        // get runs of set
        Int_t nRuns;
        Int_t* runs = TCMySQLManager::GetManager()->GetRunsOfSet(data, calibration, i, &nRuns);
    
        // loop over runs
        for (Int_t j = 0; j < nRuns; j++)
        {
            // save first and last runs
            if (i == 0 && j == 0) first_run = runs[j];
            if (i == nSets-1 && j == nRuns-1) last_run = runs[j];

            // clean-up
            if (gH) delete gH;
            if (gH2) delete gH2;
            if (f1) delete f1;
            gH = 0;
            gH2 = 0;
            f1 = 0;

            // load ROOT file
            sprintf(tmp, "%s/ARHist_CBTaggTAPS_%d.root", fLoc, runs[j]);
            f1 = new TFile(tmp);

            // check file
            if (!f1) continue;
            if (f1->IsZombie()) continue;

            // load histogram
            gH2 = (TH2*) f1->Get(hName);
            if (!gH2) continue;
            if (!gH2->GetEntries()) continue;

            // project histogram
            sprintf(tmp, "Proj_%d", runs[j]);
            gH = gH2->ProjectionX(tmp);

            // fit the histogram
            Fit(runs[j]);
            
            // update canvases and sleep
            if (watch)
            {
                cOverview->Update();
                gCFit->Update();
                gSystem->Sleep(100);
            }
     
            // count run
            nTotRuns++;
        }

        // clean-up
        delete runs;

        // draw runset markers
        cOverview->cd();
        
        // get first run of set
        Int_t frun = TCMySQLManager::GetManager()->GetFirstRunOfSet(data, calibration, i);

        // draw line
        TLine* aLine = new TLine(frun, yMin, frun, yMax);
        aLine->SetLineColor(kBlue);
        aLine->SetLineWidth(2);
        aLine->Draw("same");
    }
    
    // adjust axis
    gHOverview->GetXaxis()->SetRangeUser(first_run-10, last_run+10);

    TFile* fout = new TFile("/hiskp2/afzal/Mainz/Nov13/calibration/time/runset_overview_time_veto.root", "recreate");
    cOverview->Write();
    delete fout;

    printf("%d runs analyzed.\n", nTotRuns);

    gSystem->Exit(0);
}

