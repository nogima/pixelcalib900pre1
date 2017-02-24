// -*- C++ -*-
//
// Package:    CalibForward/CTPPS
// Class:      CTPPSRPPosMaker
// 
/**\class CTPPSRPPosMaker CTPPSRPPosMaker.cc CalibForward/CTPPS/plugins/CTPPSRPPosMaker.cc

 Description: class to make RP position conditions

*/
//
// Original Author:  Clemencia Mora Herrera
//         Created:  Mon, 27 Jun 2016 21:04:36 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/CTPPSObjects/interface/CTPPSRPPositions.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
//
// class declaration
//


class CTPPSRPPosMaker : public edm::one::EDAnalyzer<>  {
   public:
      explicit CTPPSRPPosMaker(const edm::ParameterSet&);
      ~CTPPSRPPosMaker();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
  std::string m_record;
  std::vector<unsigned int> m_rpid;
  std::vector<double> m_rpdist;
  
};


//
// constructors and destructor
//
CTPPSRPPosMaker::CTPPSRPPosMaker(const edm::ParameterSet& iConfig):
  m_record(iConfig.getParameter<std::string>("record")),
  m_rpid(iConfig.getParameter<std::vector<unsigned int>>("RPIDvect")),
  m_rpdist(iConfig.getParameter<std::vector<double>>("RPDistvect"))
{

}


CTPPSRPPosMaker::~CTPPSRPPosMaker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
CTPPSRPPosMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
}


// ------------ method called once each job just before starting event loop  ------------
void 
CTPPSRPPosMaker::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CTPPSRPPosMaker::endJob() 
{
  using namespace edm;


  if (m_rpid.size()!=0){
    /////// set from a pair of vectors (rpids and rpdistances) --> from xml

    CTPPSRPPositions* pRPPositions2 = new CTPPSRPPositions();
    std::cout<<"Size of pRPPositions2 obj  "<<pRPPositions2->size() <<std::endl<<std::endl;
   
    pRPPositions2->setRPPositions(m_rpid,m_rpdist);   

    std::cout<<"Size of pRPPositions2 obj  "<<pRPPositions2->size() <<std::endl;
    const CTPPSRPPositions::posmap & mymapc2 = pRPPositions2->getPosmap();

    std::cout<<"Content of pRPPositions2 "<<std::endl;
    for(CTPPSRPPositions::posmap::const_iterator it = mymapc2.begin(); it != mymapc2.end() ; ++it)
      std::cout<<"keys :" << it->first <<std::endl
	       << " values: distance " << pRPPositions2->getRPDistanceToBeamPipeCenter(it->first)<<std::endl
	       <<" offset " << (it->second).getOffset()<<std::endl
	       <<" motor " << (it->second).getRawMotor()<<std::endl
	       <<" lvdt    " << (it->second).getRawLVDT()<<std::endl
	       <<" resolver " << (it->second).getRawResolver()<<std::endl
	       <<std::endl;

  
    // Form the data here
    edm::Service<cond::service::PoolDBOutputService> poolDbService;
    if( poolDbService.isAvailable() ){
      cond::Time_t valid_time = poolDbService->currentTime(); 
      // this writes the payload to begin in current run defined in cfg
      poolDbService->writeOne( pRPPositions2,valid_time, m_record  );
    }
  }
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CTPPSRPPosMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CTPPSRPPosMaker);
