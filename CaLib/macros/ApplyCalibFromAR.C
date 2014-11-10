void ApplyCalibFromAR() {
	// load CaLib
	gSystem->Load("libCaLib.so");
 
	// macro configuration: just change here for your beamtime and leave
	// the other parts of the code unchanged
	const Int_t firstRun            = 200;
	const Int_t lastRun             = 1490;
	const Char_t calibName[]        = "2013_11_G-E_Linturi";
	const Char_t calibDesc[]        = "Standard calibration for butanol November 2013 beamtime";
	const Char_t calibFile[]    = "/disk/user/afzal/Mainz/acqu/acqu_user/data/CB/PID_calib_nov_output7.dat";

	// remove the to be overwritten data sets first
	const Int_t runset = 0;
	TCMySQLManager::GetManager()->RemoveDataSet("Data.PID.E0", calibName, runset);
	TCMySQLManager::GetManager()->RemoveDataSet("Data.PID.E1", calibName, runset);

	
	// read AcquRoot calibration of TAPS
	TCMySQLManager::GetManager()->AddCalibAR(kDETECTOR_PID, calibFile,
	                                         calibName, calibDesc,
	                                         firstRun, lastRun);
 
}
