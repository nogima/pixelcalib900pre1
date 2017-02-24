#include <string>
#include <iostream>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CondFormats/CTPPSObjects/interface/CTPPSPixelGainCalibrations.h"
#include "CondFormats/DataRecord/interface/CTPPSPixelGainCalibrationsRcd.h"
#include "TH2D.h"
#include "TFile.h"

namespace edmtest
{
  class myCTPPSPixGainCalibsESAnalyzer : public edm::EDAnalyzer
  {
  public:
    explicit  myCTPPSPixGainCalibsESAnalyzer(edm::ParameterSet const& p) { 
      std::cout<<"myCTPPSPixGainCalibsESAnalyzer"<<std::endl;
          }
    explicit  myCTPPSPixGainCalibsESAnalyzer(int i) {
      std::cout<<"myCTPPSPixGainCalibsESAnalyzer "<<i<<std::endl; 
    }
    virtual ~myCTPPSPixGainCalibsESAnalyzer() {  
      std::cout<<"~myCTPPSPixGainCalibsESAnalyzer "<<std::endl;
    }
        virtual void analyze(const edm::Event& e, const edm::EventSetup& c) override;
  };
  
  void
  myCTPPSPixGainCalibsESAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& context){
    std::cout<<"###myCTPPSPixGainCalibsESAnalyzer::analyze"<<std::endl;
    std::cout <<" I AM IN RUN NUMBER "<<e.id().run() <<std::endl;
    std::cout <<" ---EVENT NUMBER "<<e.id().event() <<std::endl;
    edm::eventsetup::EventSetupRecordKey recordKey(edm::eventsetup::EventSetupRecordKey::TypeTag::findType("CTPPSPixelGainCalibrationsRcd"));
    if( recordKey.type() == edm::eventsetup::EventSetupRecordKey::TypeTag()) {
      //record not found
      std::cout <<"Record \"CTPPSPixelGainCalibrationsRcd"<<"\" does not exist "<<std::endl;
    }
    //this part gets the handle of the event source and the record (i.e. the Database)
    edm::ESHandle<CTPPSPixelGainCalibrations> calhandle;
    std::cout<<"got eshandle"<<std::endl;
    context.get<CTPPSPixelGainCalibrationsRcd>().get(calhandle);
    std::cout<<"got context"<<std::endl;
    const CTPPSPixelGainCalibrations* pPixelGainCalibrations=calhandle.product();
    std::cout<<"got CTPPSPixelGainCalibrations* "<< std::endl;
    std::cout<< "print  pointer address : " ;
    std::cout << pPixelGainCalibrations << std::endl;

    TFile myfile("output.root","RECREATE");
    myfile.cd();
    TH2D  mypeds("pedsFromDB","peds; column; row",156,0.,156.,160,0.,160.);
    TH2D  mygains("gainsFromDB","gains; column; row",156,0.,156.,160,0.,160.);

    // the pPixelGainCalibrations object contains the map of detIds  to pixel gains and pedestals for current run
    // we get the map just to loop over the contents, but from here on it should be  as the code (reconstruction etc) needs. 
    // Probably best to check that the key (detid) is in the list before calling the data

    std::cout << "Size "  <<  pPixelGainCalibrations->size() << std::endl;
    const CTPPSPixelGainCalibrations::calibmap & mymap = pPixelGainCalibrations->getCalibmap(); //just to get the keys?
    
    for  (CTPPSPixelGainCalibrations::calibmap::const_iterator it = mymap.begin(); it !=mymap.end() ; ++it) {
      uint32_t detId = it->first;
      std::cout << "Content  of pPixelGainCalibrations for key: detId= " <<  detId  << std::endl;
      CTPPSPixelGainCalibration mycalibs=  pPixelGainCalibrations->getGainCalibration(detId);
      
      int ncols = mycalibs.getNCols();
      int npix  = mycalibs.getIEnd();
      int nrows = npix/ncols; //should be == 160
      for (int jrow=0; jrow < nrows ; ++jrow)
	for(int icol=0; icol < ncols ; ++icol){
	  bool isdead_g=false;
	  bool isnoisy_g=false;	
	  mygains.Fill(icol,jrow,mycalibs.getGain(icol,jrow,isdead_g,isnoisy_g));
	  if(isdead_g)
	    std::cout<<"Dead Pixel"<<std::endl;
	  if(isnoisy_g)
	    std::cout<<"Noisy Pixel"<<std::endl;

	  bool isdead_p=false;
	  bool isnoisy_p=false;	
	  mypeds.Fill(icol,jrow,mycalibs.getPed(icol,jrow,isdead_p,isnoisy_p));
	  if(isdead_p)
	    std::cout<<"Dead Pixel"<<std::endl;
	  if(isnoisy_p)
	    std::cout<<"Noisy Pixel"<<std::endl;

	  if(isdead_p!=isdead_g || isnoisy_p!=isnoisy_g)
	    std::cout<<"inconsistent information"<<std::endl;
	}
    }
    mypeds.Write();
    mygains.Write();
    myfile.Write();
    myfile.Close();
  }
  DEFINE_FWK_MODULE(myCTPPSPixGainCalibsESAnalyzer);
}
