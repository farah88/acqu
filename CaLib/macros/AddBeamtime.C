// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AddBeamtime.C                                                        //
//                                                                      //
// Add a beamtime including raw data files and initial calibrations     //
// from AcquRoot configuration files to a CaLib database.               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void AddBeamtime()
{
    // load CaLib
    gSystem->Load("libCaLib.so");
 
    // macro configuration: just change here for your beamtime and leave
    // the other parts of the code unchanged
//     const Int_t firstRun            = 4921;
//     const Int_t lastRun             = 6008;
//     const Char_t calibName[]        = "2014-07_EPT_Prod_Neiser";
//     const Char_t calibDesc[]        = "Calibration for July 2014 EPT Beamtime";
//     const Char_t calibFileTagger[]  = "../acqu_user/data/Detector-EPT.dat";
//     const Char_t calibFileCB[]      = "../acqu_user/data/Detector-NaI.dat";
//     const Char_t calibFileTAPS[]    = "../acqu_user/data/Detector-BaF2-PbWO4.dat";
//     const Char_t calibFilePID[]     = "../acqu_user/data/Detector-PID.dat";
//     const Char_t calibFileVeto[]    = "../acqu_user/data/Detector-Veto.dat";
//     const Int_t firstRun            = 8607;
//     const Int_t lastRun             = 9204;
//     const Char_t calibName[]        = "2015-05_G_E_Proton";
//     const Char_t calibDesc[]        = "Calibration for May 2015 G/E-Proton Beamtime";
//     const Char_t calibFileTagger[]  = "../acqu_user/data.2015.05/AR-Analysis-Tagger-Ladder.dat";
//     const Char_t calibFileCB[]      = "../acqu_user/data.2015.05/AR-Analysis-CB-NaI.dat";
//     const Char_t calibFileTAPS[]    = "../acqu_user/data.2015.05/AR-Analysis-TAPS-BaF2.dat";
//     const Char_t calibFilePID[]     = "../acqu_user/data.2015.05/AR-Analysis-CB-PID.dat";
//     const Char_t calibFileVeto[]    = "../acqu_user/data.2015.05/AR-Analysis-TAPS-Veto.dat";

    const Int_t firstRun            = 11159;
    const Int_t lastRun             = 12013;
    const Char_t calibName[]        = "2015-09_G_E_Proton";
    const Char_t calibDesc[]        = "Calibration for Sep 2015 G/E-Proton Beamtime";
    const Char_t calibFileTagger[]  = "../acqu_user/data.2015.09/AR-Analysis-Tagger-Ladder.dat";
    const Char_t calibFileCB[]      = "../acqu_user/data.2015.09/AR-Analysis-CB-NaI.dat";
    const Char_t calibFileTAPS[]    = "../acqu_user/data.2015.09/AR-Analysis-TAPS-BaF2.dat";
    const Char_t calibFilePID[]     = "../acqu_user/data.2015.09/AR-Analysis-CB-PID.dat";
    const Char_t calibFileVeto[]    = "../acqu_user/data.2015.09/AR-Analysis-TAPS-Veto.dat";

    // Adding the runfiles should be done by maintainers only!
    // add raw files to the database
    //const Char_t rawfilePath[]      = "/kernph/data/A2/LD2/May_09";
    //const Char_t target[]           = "LD2";    
    //TCMySQLManager::GetManager()->AddRunFiles(rawfilePath, target);
    
    // read AcquRoot calibration of tagger
    TCMySQLManager::GetManager()->AddCalibAR(kDETECTOR_TAGG, calibFileTagger,
                                             calibName, calibDesc,
                                             firstRun, lastRun);
    
    // init tagging efficiency table
    TCMySQLManager::GetManager()->AddSet("Type.Tagger.Eff", calibName, calibDesc,
                                         firstRun, lastRun, 0);
      
    // read AcquRoot calibration of CB
    TCMySQLManager::GetManager()->AddCalibAR(kDETECTOR_CB, calibFileCB,
                                             calibName, calibDesc,
                                             firstRun, lastRun);
    
    // init CB time walk calibration
    TCMySQLManager::GetManager()->AddSet("Type.CB.Time.Walk", calibName, calibDesc,
                                         firstRun, lastRun, 0);
     
    // init CB quadratic energy correction
    TCMySQLManager::GetManager()->AddSet("Type.CB.Energy.Quad", calibName, calibDesc,
                                         firstRun, lastRun, 0);
    
    // init CB LED calibration
    TCMySQLManager::GetManager()->AddSet("Type.CB.LED", calibName, calibDesc,
                                         firstRun, lastRun, 0);
  
    // read AcquRoot calibration of TAPS
    TCMySQLManager::GetManager()->AddCalibAR(kDETECTOR_TAPS, calibFileTAPS,
                                             calibName, calibDesc,
                                             firstRun, lastRun);
    
    // init TAPS quadratic energy correction
    TCMySQLManager::GetManager()->AddSet("Type.TAPS.Energy.Quad", calibName, calibDesc,
                                         firstRun, lastRun, 0);
 
    // init TAPS LED calibration
    TCMySQLManager::GetManager()->AddSet("Type.TAPS.LED1", calibName, calibDesc,
                                         firstRun, lastRun, 0);
    TCMySQLManager::GetManager()->AddSet("Type.TAPS.LED2", calibName, calibDesc,
                                         firstRun, lastRun, 0);
 
    // read AcquRoot calibration of PID
    TCMySQLManager::GetManager()->AddCalibAR(kDETECTOR_PID, calibFilePID,
                                             calibName, calibDesc,
                                             firstRun, lastRun);
    
    // read AcquRoot calibration of Veto 
    TCMySQLManager::GetManager()->AddCalibAR(kDETECTOR_VETO, calibFileVeto,
                                             calibName, calibDesc,
                                             firstRun, lastRun);
     
    gSystem->Exit(0);
}

