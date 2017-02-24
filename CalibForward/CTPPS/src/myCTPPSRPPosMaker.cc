// -*- C++ -*-
//
// Package:    CalibForward/CTPPS
// Class:      myCTPPSRPPosMaker
// 
/**\class myCTPPSRPPosMaker myCTPPSRPPosMaker.cc CalibForward/CTPPS/plugins/myCTPPSRPPosMaker.cc

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

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class myCTPPSRPPosMaker : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit myCTPPSRPPosMaker(const edm::ParameterSet&);
      ~myCTPPSRPPosMaker();

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
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
myCTPPSRPPosMaker::myCTPPSRPPosMaker(const edm::ParameterSet& iConfig):
  m_record(iConfig.getParameter<std::string>("record")),
  m_rpid(iConfig.getParameter<std::vector<unsigned int>>("RPIDvect")),
  m_rpdist(iConfig.getParameter<std::vector<double>>("RPDistvect"))
{
   //now do what ever initialization is needed
   usesResource("TFileService");

}


myCTPPSRPPosMaker::~myCTPPSRPPosMaker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
myCTPPSRPPosMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;


   CTPPSRPPositions* pRPPositions = new CTPPSRPPositions();
   CTPPSRPPositions* pRPPositions0 = new CTPPSRPPositions();
   std::cout<<"Size of pRPPositions obj  "<<pRPPositions->size() <<std::endl;
   std::cout<<"Size of pRPPositions0 obj "<<pRPPositions0->size() <<std::endl;

   pRPPositions->setRPPosition(0,41.287);
   pRPPositions->setRPPosition(1,125.9);

   CTPPSRPPosition posexample(10.1,0.9,1.,2.,3.);
   pRPPositions0->setRPPosition(5,20.);
   pRPPositions0->setRPPosition(1,posexample);
   pRPPositions0->setRPPosition(2);


   std::cout<<"Size of pRPPositions obj  "<<pRPPositions->size() <<std::endl;
   std::cout<<"Size of pRPPositions0 obj "<<pRPPositions0->size() <<std::endl;


   const CTPPSRPPositions::posmap & mymap = pRPPositions->getPosmap();
   const CTPPSRPPositions::posmap & mymap0 = pRPPositions0->getPosmap();


   CTPPSRPPositions::posmap::const_iterator it = mymap.begin(); 
   std::cout<<"Content  of pRPPositions keys: "<< it->first <<std::endl 
	    <<" values dist: "<< pRPPositions->getRPDistanceToBeamPipeCenter(it->first)<<std::endl  
	    <<" offset " << (it->second).getOffset()<<std::endl
	    <<" motor " << (it->second).getRawMotor()<<std::endl
	    <<" lvdt    " << (it->second).getRawLVDT()<<std::endl
	    <<" resolver " << (it->second).getRawResolver()<<std::endl
	    <<std::endl;
   

   std::cout<<"Content of pRPPositions0 "<<std::endl;
   for(it = mymap0.begin(); it != mymap0.end() ; ++it)
     std::cout<<"keys :" << it->first <<std::endl
	      << " values: distance " << pRPPositions0->getRPDistanceToBeamPipeCenter(it->first)<<std::endl
	      <<" offset " << (it->second).getOffset()<<std::endl
	      <<" motor " << (it->second).getRawMotor()<<std::endl
	      <<" lvdt    " << (it->second).getRawLVDT()<<std::endl
	      <<" resolver " << (it->second).getRawResolver()<<std::endl
	      <<std::endl;

   std::cout<<"testing "<< pRPPositions->getRPDistanceToBeamPipeCenter(1) <<std::endl;
   std::cout<<"Size of mymap "<<mymap.size() <<std::endl<<std::endl;

   /////// set from a posmap

   CTPPSRPPositions* pRPPositions1 = new CTPPSRPPositions();
   std::cout<<"Size of pRPPositions1 obj  "<<pRPPositions1->size() <<std::endl<<std::endl;

   CTPPSRPPosition posexample1(50.6,-8.9,0.1,0.2,0.3);
   CTPPSRPPositions::posmap mymap1;
   mymap1[20]=CTPPSRPPosition(26.,62.,0.11,0.22,0.33);
   mymap1[6]=posexample1;

   pRPPositions1->setRPPositions(mymap1);   

   std::cout<<"Size of pRPPositions1 obj  "<<pRPPositions1->size() <<std::endl;
   const CTPPSRPPositions::posmap & mymapc1 = pRPPositions1->getPosmap();

   std::cout<<"Content of pRPPositions1 "<<std::endl;
   for(it = mymapc1.begin(); it != mymapc1.end() ; ++it)
     std::cout<<"keys :" << it->first <<std::endl
	      << " values: distance " << pRPPositions1->getRPDistanceToBeamPipeCenter(it->first)<<std::endl
	      <<" offset " << (it->second).getOffset()<<std::endl
	      <<" motor " << (it->second).getRawMotor()<<std::endl
	      <<" lvdt    " << (it->second).getRawLVDT()<<std::endl
	      <<" resolver " << (it->second).getRawResolver()<<std::endl
	      <<std::endl;
   
   edm::Service<cond::service::PoolDBOutputService> poolDbService;
   if( poolDbService.isAvailable() ){
     poolDbService->writeOne( pRPPositions, poolDbService->beginOfTime(),
			      m_record  );
     poolDbService->writeOne( pRPPositions1, poolDbService->currentTime(),
			      m_record  );
   }
   
   
   if (m_rpid.size()!=0){
     /////// set from a pair of vectors (rpids and rpdistances) --> from xml

     CTPPSRPPositions* pRPPositions2 = new CTPPSRPPositions();
     std::cout<<"Size of pRPPositions2 obj  "<<pRPPositions2->size() <<std::endl<<std::endl;

   
     pRPPositions2->setRPPositions(m_rpid,m_rpdist);   

     std::cout<<"Size of pRPPositions2 obj  "<<pRPPositions2->size() <<std::endl;
     const CTPPSRPPositions::posmap & mymapc2 = pRPPositions2->getPosmap();

     std::cout<<"Content of pRPPositions2 "<<std::endl;
     for(it = mymapc2.begin(); it != mymapc2.end() ; ++it)
       std::cout<<"keys :" << it->first <<std::endl
		<< " values: distance " << pRPPositions2->getRPDistanceToBeamPipeCenter(it->first)<<std::endl
		<<" offset " << (it->second).getOffset()<<std::endl
		<<" motor " << (it->second).getRawMotor()<<std::endl
		<<" lvdt    " << (it->second).getRawLVDT()<<std::endl
		<<" resolver " << (it->second).getRawResolver()<<std::endl
		<<std::endl;

   

     // Form the data here

     //  edm::Service<cond::service::PoolDBOutputService> poolDbService;
     if( poolDbService.isAvailable() ){
       //  poolDbService->writeOne( pRPPositions, poolDbService->beginOfTime(),
       //			   m_record  );
       cond::Time_t valid_time = 5;
       poolDbService->writeOne( pRPPositions2,valid_time, m_record  );
     }
   }
   ///should I use PopCon instead (template service)


// #ifdef THIS_IS_AN_EVENT_EXAMPLE
//    Handle<ExampleData> pIn;
//    iEvent.getByLabel("example",pIn);
// #endif
// #ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
//    ESHandle<SetupData> pSetup;
//    iSetup.get<SetupRecord>().get(pSetup);
// #endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
myCTPPSRPPosMaker::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
myCTPPSRPPosMaker::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
myCTPPSRPPosMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(myCTPPSRPPosMaker);
