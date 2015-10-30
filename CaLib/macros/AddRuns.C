
//______________________________________________________________________________
void AddRuns()
{
    // load CaLib
    gSystem->Load("libCaLib.so");
 
    // Adding the runfiles should be done by maintainers only!
    // add raw files to the database
    const Char_t rawfilePath[]      = "/disk/nobackup/001/mlang/Mainz/2015-09_G_E_Proton/";
    const Char_t target[]           = "HButanol";    
    TCMySQLManager::GetManager()->AddRunFiles(rawfilePath, target);
    
    gSystem->Exit(0);
}

