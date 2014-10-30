// some general utilies for AcquRoot
  TH1F *corrHist=NULL;

// Scaler dump functions
void ScalerCurr(int columns=1){				// all current buff to file
  scalerDump("stdout",0,0,columns,0);
}
void ScalerCurr(char *file, int columns=1){		// all current buff to file
  scalerDump(file,0,0,columns,0);
}
void ScalerCurr(int start, int nchans,int columns=1){	// all current buff to file
  scalerDump("stdout",start,nchans,columns,0);
}
void ScalerCurr(int start, int nchans, char *file ,int columns=1){	// all current buff to file
  scalerDump(file,start,nchans,columns,0);
}

void ScalerAcc(int columns=1){				// all accumulated buff to file
  scalerDump("stdout",0,0,columns,1);
}
void ScalerAcc(char *file, int columns=1){		// all accumulated buff to file
  scalerDump(file,0,0,columns,1);
}
void ScalerAcc(int start, int nchans,int columns=1){	// all accumulated buff to file
  scalerDump("stdout",start,nchans,columns,1);
}
void ScalerAcc(int start, int nchans, char *file ,int columns=1){	// all accumulated buff to file
  scalerDump(file,start,nchans,columns,1);
}


void scalerDump(char *file, int start, int chan, int cols, int buff){
  //general scaler dump (called by more specific ones (above)
  //file = output file name = "stdout" or any file name
  //start = index of first channel (0 if full buffer required)
  //chan = no of channels (0 if full buffer required)
  //cols = no of columns in the output file
  //buff = scaler buffer (0 = current, 1 = accumulated)

  FILE *fp;
  Double_t *accscaler= gAR->GetScalerSum();
  UInt_t *currscaler= gAR->GetScaler();
  Int_t last = gAR->GetMaxScaler();
  Int_t first = 0;
  Int_t counter=cols;

  if(chan){
    first=start;
    last=start+chan;
  }	

  if((strcmp(file,"stdout"))!=0){ // ie write to file
    fp=fopen(file,"w");
    fprintf(fp,"# Contents of current scaler buffer\n");
    fprintf(fp,"# Channels %d - %d\n",first,last-1);
    fprintf(fp,"# Chan\tContent\n");
    for(int n=first;n<last;n++){
      if(counter%cols==0)fprintf(fp,"%5d",n);
      if(buff==0) fprintf(fp,"\t%8d",currscaler[n]);
      else fprintf(fp,"\t%g",accscaler[n]);
      if((++counter)%cols==0)fprintf(fp,"\n");
    }
    fclose(fp);
  }
  else{
    fprintf(stdout,"# Contents of Accumulated scaler buffer\n");
    fprintf(stdout,"# Channels %d - %d\n",first,last-1);
    fprintf(stdout,"# Chan\t\tContent\n");
    for(int n=first;n<last;n++){
      if(counter%cols==0)fprintf(stdout,"%5d",n);
      if(buff==0) fprintf(stdout,"%8d",currscaler[n]);
      else fprintf(stdout,"\t%12g",accscaler[n]);
      if((++counter)%cols==0)fprintf(stdout,"\n");
    }
    if(last%cols!=0)fprintf(stdout,"\n");
    fflush(stdout);
  }
}

void PeakAlign(char *histBase, int index, int chans, char *outfile, int method=0, double range=3.0){
  //function to do the alignment of TDCs.for a range of histograms
  //eg .for Ladder_Time0 - Ladder_Time351
  //
  //histBase = base name of histograms eg "Ladder_Time"
  //index = index of first histogram (normally = 0)
  //chans = no of hostograms (eg 352 for the Ladder
  //outfile = file where the results go
  //method = way to find the peak (0 to take channel with most counts, 1 to fit a gaussian)
  //range = range used around channel with most counts for gaussian

  TCanvas *TDCAlignCanvas; //make a canvas specifically for this function
 
  char histname[50];
  float peak;
  FILE *fp;
  TH1F *hist = NULL;
  TF1 *fit = NULL;



  if((fp = fopen(outfile,"w")) == NULL){   //open file
    fprintf(stderr,"couldn't open file: %s\n",infile);
    return;
  }

  TDCAlignCanvas = new TCanvas("TDCAlignCanvas","TDCAlignCanvas",200,10,700,500); //create canvas
  for(int n=index;n<index+chans;n++){		   	//for all channels
    sprintf(histname,"%s%d",histBase,n);		//construct histogram name
    if((hist=(TH1F *)gDirectory->Get(histname))!=NULL){	//find the ptr to the hist
      peak=hist->GetBinCenter(hist->GetMaximumBin());	//get the value of the peak
      hist->SetAxisRange(peak-2*range,peak+2*range);	//set the draw range 
      hist->Draw();					//draw the hist
      if(method){					//if method is gaussian
	if(fit) delete fit;					//delete old fit
	fit = new TF1("fit1","gaus",peak-range,peak+range);	//create new fit over correct range	
	hist->Fit("fit1","R");					//fit the peak
	peak=fit->GetParameter(1);				//get the peak position
	TDCAlignCanvas->Update();						//update the canvas
	gSystem->Sleep(100);					//wait logn enoug to see the hist
      }
      fprintf(fp,"%d\t%3.1f\n",n,peak);			// print out the channel and peak position
    }
  }

  fclose(fp);				//close file
  if(TDCAlignCanvas) delete TDCAlignCanvas;	// tidy up
  if(fit) delete fit;
}

void LadderCorrected(char *file="none"){
  TA2Ladder *ladder=NULL;
  FILE *fp;
  int elements=0;
  

  ladder=(TA2Ladder *)gAN->GetGrandChild("Ladder");
  elements=ladder->GetNelem();
  if(!corrHist)corrHist=new TH1F("Ladder Corrected","Ladder: Prompt - Random",elements,0,elements);
  corrHist->Reset("ICE");
  corrHist->Add(Ladder_HitsPrompt,Ladder_HitsRand,1.0,-1.0);
  corrHist->Draw();
  if((strcmp(file,"none"))!=0){
    if((fp = fopen(file,"w")) == NULL){   //open file
      fprintf(stderr,"couldn't open file: %s\n",file);
      return;
    }
    for(int n=0;n<elements;n++){
      fprintf(fp,"%d\t%f\n",n,corrHist->GetBinContent(n+1));
    }
    fclose(fp);
  }
}
  
