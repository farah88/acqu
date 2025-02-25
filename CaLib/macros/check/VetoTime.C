// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// VetoTime.C                                                           //
//                                                                      //
// Check calibration of runsets.                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void Fit(TH1* h, Double_t* outPos, Double_t* outFWHM)
{
    // Perform fit.
    
    Char_t tmp[256];

    // delete old function
    TF1* func = new TF1(tmp, "gaus");
    func->SetLineColor(2);
    
    // estimate peak position
    Double_t fPi0Pos = h->GetBinCenter(h->GetMaximumBin());

    // configure fitting function
    func->SetRange(fPi0Pos - 3, fPi0Pos + 3);
    func->SetLineColor(2);
    func->SetParameters( h->GetMaximum(), 0, 1);
    Int_t fitres = h->Fit(func, "RB0Q");
    
    // get position and FWHM
    fPi0Pos = func->GetParameter(1);
    *outPos = fPi0Pos;
    *outFWHM = 2.35*func->GetParameter(2);

    // indicator line
    TLine* line = new TLine();
    line->SetX1(fPi0Pos);
    line->SetX2(fPi0Pos);
    line->SetY1(0);
    line->SetY2(h->GetMaximum());

    // draw 
    h->GetXaxis()->SetRangeUser(-30, 30);
    h->Draw();
    func->Draw("same");
    line->Draw("same");
}

//______________________________________________________________________________
void VetoTime()
{
    // Main method.
    
    Char_t tmp[256];
    
    // load CaLib
    gSystem->Load("libCaLib.so");
    
    // general configuration
    const Char_t* data = "Data.Veto.T0";
    const Char_t* hName = "CaLib_Veto_Time";

    // configuration (December 2007)
    //const Char_t calibration[] = "LD2_Dec_07";
    //const Char_t filePat[] = "/usr/puma_scratch0/werthm/A2/Dec_07/AR/out/ARHistograms_CB_RUN.root";

    // configuration (February 2009)
    const Char_t calibration[] = "LD2_Feb_09";
    const Char_t filePat[] = "/usr/puma_scratch0/werthm/A2/Feb_09/AR/out/ARHistograms_CB_RUN.root";
    
    // configuration (May 2009)
    //const Char_t calibration[] = "LD2_May_09";
    //const Char_t filePat[] = "/usr/puma_scratch0/werthm/A2/May_09/AR/out/ARHistograms_CB_RUN.root";
    
    // get number of sets
    Int_t nSets = TCMySQLManager::GetManager()->GetNsets(data, calibration);

    // create canvas
    Int_t n = TMath::Sqrt(nSets);
    TCanvas* cOverview = new TCanvas();
    cOverview->Divide(n, nSets / n + 1);
    
    // create arrays
    Double_t* pos = new Double_t[nSets+1];
    Double_t* fwhm = new Double_t[nSets+1];
    
    // total sum histogram
    TH1* hTot = 0;

    // loop over sets
    for (Int_t i = 0; i < nSets; i++)
    { 
        // create file manager
        TCFileManager m(data, calibration, 1, &i, filePat);
        
        // get histo
        TH2* h2 = (TH2*) m.GetHistogram(hName);
        
        // skip empty histo
        if (!h2) continue;

        // project histo
        sprintf(tmp, "Proj_%d", i);
        TH1* h = (TH1*) h2->ProjectionX(tmp);
        
        // add to total histogram
        if (!hTot) hTot = (TH1*) h->Clone();
        else hTot->Add(h);
 
        // fit histo
        cOverview->cd(i+1);
        Fit(h, &pos[i], &fwhm[i]);
    }
    
    // show total histogram
    TCanvas* cTot = new TCanvas();
    Fit(hTot, &pos[nSets], &fwhm[nSets]);

    // show results
    for (Int_t i = 0; i < nSets; i++)
        printf("Set %02d:   Pos: %5.2f ns   FWHM: %.2f ns\n", i, pos[i], fwhm[i]);
    printf("Total :   Pos: %5.2f ns   FWHM: %.2f ns\n", pos[nSets], fwhm[nSets]);
}

