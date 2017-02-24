#include <string>
#include <iostream>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CondFormats/CTPPSObjects/interface/CTPPSRPPositions.h"
#include "CondFormats/DataRecord/interface/CTPPSRPPositionsRcd.h"

namespace edmtest
{
  class myCTPPSRPPosESAnalyzer : public edm::EDAnalyzer
  {
  public:
    explicit  myCTPPSRPPosESAnalyzer(edm::ParameterSet const& p) { 
      std::cout<<"myCTPPSRPPosESAnalyzer"<<std::endl;
          }
    explicit  myCTPPSRPPosESAnalyzer(int i) {
      std::cout<<"myCTPPSRPPosESAnalyzer "<<i<<std::endl; 
    }
    virtual ~myCTPPSRPPosESAnalyzer() {  
      std::cout<<"~myCTPPSRPPosESAnalyzer "<<std::endl;
    }
        virtual void analyze(const edm::Event& e, const edm::EventSetup& c) override;
  };
  
  void
  myCTPPSRPPosESAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& context){
    std::cout<<"###myCTPPSRPPosESAnalyzer::analyze"<<std::endl;
    std::cout <<" I AM IN RUN NUMBER "<<e.id().run() <<std::endl;
    std::cout <<" ---EVENT NUMBER "<<e.id().event() <<std::endl;
    edm::eventsetup::EventSetupRecordKey recordKey(edm::eventsetup::EventSetupRecordKey::TypeTag::findType("CTPPSRPPositionsRcd"));
    if( recordKey.type() == edm::eventsetup::EventSetupRecordKey::TypeTag()) {
      //record not found
      std::cout <<"Record \"CTPPSRPPositionsRcd"<<"\" does not exist "<<std::endl;
    }
    //this part gets the handle of the event source and the record (i.e. the Database)
    edm::ESHandle<CTPPSRPPositions> poshandle;
    std::cout<<"got eshandle"<<std::endl;
    context.get<CTPPSRPPositionsRcd>().get(poshandle);
    std::cout<<"got context"<<std::endl;
    const CTPPSRPPositions* pRPPositions=poshandle.product();
    std::cout<<"got CTPPSRPPositions* "<< std::endl;
    std::cout<< "print  pointer address : " ;
    std::cout << pRPPositions << std::endl;


    // the pRPPositions object contains the map of rpids to rp position parameters (distance to BP center and raw quantities) for current run
    // we get the map just to loop over the contents, but from here on it should be  as the code (reconstruction etc) needs. 
    // Probably best to check that the key (rpid) is in the list before calling the data

    std::cout << "Size "  <<  pRPPositions->size() << std::endl;
    const CTPPSRPPositions::posmap & mymap = pRPPositions->getPosmap();
   
   for  (CTPPSRPPositions::posmap::const_iterator it = mymap.begin(); it !=mymap.end() ; ++it) 
     std::cout<<"Content  of pRPPositions for key: "<< it->first <<std::endl 
	      <<" values dist: "<< pRPPositions->getRPDistanceToBeamPipeCenter(it->first)<<std::endl  
	      <<" offset "      << pRPPositions->getRPOffset(it->first)<<std::endl
	      <<" motor "       << pRPPositions->getRPRawMotor(it->first)<<std::endl
	      <<" lvdt    "     << pRPPositions->getRPRawLVDT(it->first)<<std::endl
	      <<" resolver "    << pRPPositions->getRPRawResolver(it->first)<<std::endl
	      <<std::endl;
   
  }
  DEFINE_FWK_MODULE(myCTPPSRPPosESAnalyzer);
}
