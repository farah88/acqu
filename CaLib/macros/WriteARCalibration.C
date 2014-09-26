// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// WriteARCalibration.C                                                 //
//                                                                      //
// Write AcquRoot calibration files using CaLib.                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void WriteARCalibration()
{
    // load CaLib
    gSystem->Load("libCaLib.so");
    
    // write tagger calibration file
    //TCWriteARCalib w(kDETECTOR_TAGG, "/disk/user/afzal/Mainz/acqu/acqu_user/data/Tagger/Tagger.dat");
    //w.Write("/disk/user/afzal/Mainz/acqu/acqu_user/data/Tagger/new_Tagger.dat", "2013_11_G-E_Linturi", 13840);

    // write CB calibration file
    TCWriteARCalib w1(kDETECTOR_CB, "/disk/user/afzal/Mainz/acqu/acqu_user/data/CB/CB_NaI.dat");
    w1.Write("/disk/user/afzal/Mainz/acqu/acqu_user/data/CB/new_CB_NaI.dat", "2013_11_G-E_Linturi",467);

    // write TAPS calibration file
    //TCWriteARCalib w2(kDETECTOR_TAPS, "/disk/user/afzal/Mainz/acqu/acqu_user/data/TAPS/TAPS_BaF2-PbWO4.dat");
    //w2.Write("/disk/user/afzal/Mainz/acqu/acqu_user/data/TAPS/new_TAPS_BaF2-PbWO4.dat", "2013_11_G-E_Linturi", 13090);

    // write PID calibration file
    //TCWriteARCalib w3(kDETECTOR_PID, "PID.dat");
    //w3.Write("new_PID.dat", "2013_11_G-E_Linturi", 13840);

    // write Veto calibration file
    //TCWriteARCalib w4(kDETECTOR_VETO, "Veto.dat");
    //  w4.Write("new_Veto.dat", "2013_11_G-E_Linturi", 13840);

    gSystem->Exit(0);
}

