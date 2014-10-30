#ifndef __TA2GoAT_h__
#define __TA2GoAT_h__


#include "TFile.h"
#include "TTree.h"
#include "TA2AccessSQL.h"

#define TA2GoAT_MAX_TAGGER	1024
#define TA2GoAT_MAX_PARTICLE	128
#define TA2GoAT_MAX_HITS	860  
#define TA2GoAT_MAX_ERROR	300
#define TA2GoAT_NULL 1e6

enum {
    EG_OUTPUT_FOLDER = 30250,
    EG_INPUT_NAME,
    EG_FILE_NAME,
    EG_BEAM_HELICITY,
};

static const Map_t RootTreeConfigKeys[] = {
    // General keys
    {"RootTree-Output-Folder:"       	, EG_OUTPUT_FOLDER},
    {"RootTree-Input-Name:"           	, EG_INPUT_NAME},
    {"RootTree-File-Name:"           	, EG_FILE_NAME},
    //{"Beam-Helicity:"           	, EG_BEAM_HELICITY},
    // Termination
    {NULL       	 		, -1           }
};

class	TA2GoAT	: public TA2AccessSQL
{
private:
		TFile*		file;				// outFile
		TTree*		treeRawEvent;		// Raw particle information (filled each event)
		TTree*		treeTagger;			// Tagger information (filled each event)
		TTree*		treeLinPol;			// Tagger information (filled each event)		
		TTree* 		treeTrigger;		// Trigger information (filled each event)
		TTree* 		treeDetectorHits;	// Detector system hit patterns (filled each event)
		TTree*		treeScaler; 		// Scaler read information (filled each scaler read)

    	char        outputFolder[256];
    	char        inputName[64];
    	char        fileName[64];

    	//Particles    
    	Int_t		nParticles;		
    	Double_t*	Ek;
    	Double_t* 	Theta;
    	Double_t*	Phi;
    	Double_t*	time;
    	UChar_t*    clusterSize;
		Int_t*		centralCrys;
		Int_t*		centralVeto;

    	//Apparatus
    	UChar_t*	Apparatus;

    	//Charged detector energies
    	Double_t*	d_E;
    	Double_t*	WC0_E;
    	Double_t*	WC1_E;

		//Wire Chamber vertex reconstruction
    	Double_t* 	WC_Vertex_X;
    	Double_t* 	WC_Vertex_Y;
    	Double_t* 	WC_Vertex_Z;
    
    	//Tagger
    	Int_t		nTagged;
    	Double_t*	photonbeam_E;
    	Int_t*		tagged_ch;
    	Double_t*	tagged_t;
    	
    	//LinPol
    	Int_t 		plane;
    	Double_t	edge;
    	Double_t	edgeSetting;
             
    	//Hits
    	Int_t		nNaI_Hits;
    	Int_t*		NaI_Hits;
     	Int_t*		NaI_Cluster;
   	Int_t		nPID_Hits;
    	Int_t*		PID_Hits;
    	Int_t		nWC_Hits;
    	Int_t*		WC_Hits;
    	Int_t		nBaF2_PbWO4_Hits;
    	Int_t*		BaF2_PbWO4_Hits;
     	Int_t*		BaF2_PbWO4_Cluster;
   	Int_t		nVeto_Hits;
    	Int_t*		Veto_Hits;
    
    	//Trigger 
    	Double_t 	ESum;	// or Detector Energies
    	Int_t 		Mult; 	
    	Int_t 		nTriggerPattern;
		Int_t* 		TriggerPattern;	

		//Int_t  	 	nHelBits;
		//Bool_t  	Helicity;
		//Bool_t  	HelInver;
		//Int_t  	 	HelADC;

		//Char_t  	HelBits[8][8];
		//Bool_t  	HelInh[8];
		//Bool_t  	HelBeam[8];
		UChar_t     helicityBit;

    	Int_t 		nError; 	
    	Int_t* 		ErrModID; 	
    	Int_t* 		ErrModIndex; 	
    	Int_t* 		ErrCode; 	
    
    	//Scalers
    	Int_t		eventNumber;
    	Int_t		eventID;    

		// Display histograms
		TH2*		Check_CBdE_E;
		TH2*		Check_CBPhiCorr;
    
		TH2*		Check_CBdE_E_1PID;
		TH2*		Check_CBPhiCorr_1PID;
		
		TH2*		Check_CBdE_E_pi0;
		TH2*		Check_CBPhiCorr_pi0;

		TH2*		Check_TAPSdE_E;
		TH2*		Check_TAPSPhiCorr;
    
		TH2*		Check_TAPSdE_E_1Veto;
		TH2*		Check_TAPSPhiCorr_1Veto;
		
		TH2*		Check_CBHits;
		TH2*		Check_CBADCHits;
		TH2*		Check_CBTDCHits;
		
		TH2*		Check_PIDHits;
		TH2*		Check_PIDADCHits;
		TH2*		Check_PIDTDCHits;
				
		TH2*		Check_TAPSHits;
		TH2*		Check_TAPSADCHits;
		TH2*		Check_TAPSTDCHits;

		TH2*		Check_VetoHits;
		TH2*		Check_VetoADCHits;
		TH2*		Check_VetoTDCHits;


	protected:    	

		void TriggerReconstruction(); // Trigger reconstruction routine
    	void TriggerMC();    //Generates the trigger for the MC
    	void TriggerHW();    //Generates the trigger from hardware information
    	void DefineHistograms();	// Define histograms
    	void WriteHistograms();
		void DataCheckHistograms(); // Routine to fill data check histograms
    
	public:
	TA2GoAT(const char*, TA2Analysis*);
	~TA2GoAT();
	
	virtual void LoadVariable();            	//Creates histograms
    	virtual void SetConfig(Char_t*, Int_t); //Parses general information from configuration file
    	virtual void ParseMisc(char* line);     //Parses additional information from configuration file
    	virtual void Reconstruct();             //Event reconstruction
    	virtual void PostInit();                //Initialisation etc.
    	virtual void Finish();                	//Initialisation etc.

	ClassDef(TA2GoAT, 1)
};


#endif
