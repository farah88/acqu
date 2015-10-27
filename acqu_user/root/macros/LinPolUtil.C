void DumpLinPolRefs(char *file){
  Int_t RunNumber=0;
  Char_t runNumString[20];                                              //temp string for getting run number
  Char_t CurrentRunFileName[200];
  Char_t ScalerFileName[200];
  Char_t TDCFileName[200];

  TH1F *corrHist=NULL;

  TA2KensLadder *ladder=NULL;
  TA2KensTagger *tagger=NULL;
  Double_t beamEnergy;
  Double_t *ladderEnergy;
  FILE *fp;
  int elements=0;
  Double_t *scalerAcc;

  tagger=(TA2KensTagger *)gAN->GetChild("Tagger");
  beamEnergy=tagger->GetBeamEnergy();

  ladder=(TA2KensLadder *)gAN->GetGrandChild("Ladder");
  elements=ladder->GetNelem();
  scalerAcc=ladder->GetScalerAcc();
  ladderEnergy=ladder->GetECalibration();


  strcpy(CurrentRunFileName,gAR->GetFileName());                        //save curent file name
  strcpy(runNumString,strrchr(CurrentRunFileName,'_')+1);               //copy file name after last "_"
  strcpy(strstr(runNumString,".dat")," ");                              //replace ".dat" with " "
  sscanf(runNumString,"%d",&RunNumber);                                 //scan the run number
  sprintf(ScalerFileName,"%s_scaler_%d.dat",file,RunNumber);           	//construct outfile names
  sprintf(TDCFileName,"%s_TDC_%d.dat",file,RunNumber);             	//construct outfile names

  //first the accummulated scalers
  if((fp = fopen(ScalerFileName,"w")) == NULL){   //open file
    fprintf(stderr,"couldn't open file: %s\n",ScalerFileName);
    return;
  }
  
  for(int n=0;n<elements;n++){
    fprintf(fp,"%f\t%f\n",beamEnergy-ladderEnergy[(elements-1)-n],scalerAcc[(elements-1)-n]);
  }
  fclose(fp);

  //now the random subtracted TDCs
  if((fp = fopen(TDCFileName,"w")) == NULL){   //open file
    fprintf(stderr,"couldn't open file: %s\n",TDCFileName);
    return;
  }
  if(!corrHist)corrHist=new TH1F("Ladder Corrected","Ladder: Prompt - Random",elements,0,elements);
  corrHist->Reset("ICE");
  corrHist->Add(Ladder_HitsPrompt,Ladder_HitsRand,1.0,-1.0*ladder->GetPromptRand());
  //corrHist->Draw();
  for(int n=0;n<elements;n++){
    fprintf(fp,"%f\t%f\n",beamEnergy-ladderEnergy[(elements-1)-n],corrHist->GetBinContent(elements-n));
  }
  fclose(fp);
}
  
