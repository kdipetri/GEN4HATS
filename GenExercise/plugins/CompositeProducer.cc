// -*- C++ -*-
//
// Package:    CompositeProducer
// Class:      CompositeProducer
// 
/**\class CompositeProducer CompositeProducer.cc Analysis/CompositeProducer/src/CompositeProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  "Salvatore Rappoccio"
// Modified by    :  "Stephen Mrenna "
//         Created:  Mon Sep 28 12:53:57 CDT 2009
// $Id: CompositeProducer.cc,v 1.1.1.1 2012/01/09 15:21:02 mrenna Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "PhysicsTools/CandUtils/interface/AddFourMomenta.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/LorentzVector.h"


#include <TLorentzVector.h>


#include <vector>

//
// class declaration
//

class CompositeProducer : public edm::global::EDProducer<> {
  public:
    explicit CompositeProducer(const edm::ParameterSet&);
    ~CompositeProducer();

  private:
    void produce(edm::StreamID, edm::Event&, const edm::EventSetup&) const override;
      
    // ----------member data ---------------------------

    edm::InputTag dauSrc_;
    edm::EDGetTokenT<reco::CandidateCollection> dauTok_;
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
CompositeProducer::CompositeProducer(const edm::ParameterSet& iConfig) :
  dauSrc_ ( iConfig.getParameter<edm::InputTag>("dauSrc") ),
  dauTok_ ( consumes<reco::CandidateCollection>(dauSrc_) )
{
  produces<std::vector<reco::CompositeCandidate> > ();
}


CompositeProducer::~CompositeProducer()
{
 
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
CompositeProducer::produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup& iSetup) const
{

  auto jpsiCands = std::make_unique<std::vector<reco::CompositeCandidate>>();
  edm::Handle<reco::CandidateCollection> h_daus;

  iEvent.getByToken( dauTok_, h_daus );

  reco::Particle::LorentzVector temp(0,0,0,0);
	
  if ( h_daus.isValid() && h_daus->size() > 0 ) {

    reco::CompositeCandidate jpsi;

    for ( const auto& imuon : *(h_daus.product()) ) {
	  jpsi.addDaughter( imuon );
      temp += reco::Particle::LorentzVector(imuon.px(),imuon.py(),0.0,imuon.pt());
    }

    jpsi.setP4( temp );
    jpsiCands->push_back( jpsi );

  }

  iEvent.put( std::move(jpsiCands) );
}

//define this as a plug-in
DEFINE_FWK_MODULE(CompositeProducer);
