//--Author	JRM Annand   19th Oct 2005  Skeleton user version
//--Rev
//--Update
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data
//
// TMCUserParticle
//
// Skeleton User Particle specification and kinematics generation for
// the Monte Carlo event generator TMCGenerator.
// Users can redefine the behaviour of TMCParticle here

#ifndef __TMCUserParticle_h__
#define __TMCUserParticle_h__
#include "TRandom.h"
#include "TMCParticle.h"
//#include "TDatabasePDG.h"

class TMCUserParticle : public TMCParticle {
 protected:
 public:
  TMCUserParticle( const Char_t*, TRandom*, Double_t, Bool_t = kFALSE, Bool_t = kTRUE );
  //TMCUserParticle( const Char_t*, TRandom*, TDatabasePDG*, Int_t, Bool_t = kFALSE, Bool_t = kTRUE );
  TMCUserParticle( const Char_t*, TMCUserParticle* );
  virtual ~TMCUserParticle();
  virtual void Init();
  virtual void PostInit();
  //  virtual Bool_t GenDecay( );
  //  virtual Bool_t Gen2Decay();
  //  virtual Bool_t GenNDecay();
  // Getters
  //  virtual Double_t Mass( ){ return fMass; }
  //
  ClassDef(TMCUserParticle,1)

};

#endif
