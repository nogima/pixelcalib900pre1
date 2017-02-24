#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
//CTPPS Gain Calibration Conditions Object 
#include "CondFormats/CTPPSObjects/interface/CTPPSPixelGainCalibrations.h"
//CTPPS tracker DetId
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "TFile.h"
#include "TH2F.h"

//
// class declaration
//


class myCTPPSPixGainCalibsMaker : public edm::one::EDAnalyzer<>  
{
 public:
  explicit myCTPPSPixGainCalibsMaker(const edm::ParameterSet&);
  ~myCTPPSPixGainCalibsMaker();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
 private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  void getHistos();
  void fillDB();
  // ----------member data ---------------------------
  std::string m_record;
  std::string m_inputHistosFileName;
  TFile * m_inputRootFile;
};


//
// constructors and destructor
//
myCTPPSPixGainCalibsMaker::myCTPPSPixGainCalibsMaker(const edm::ParameterSet& iConfig):   
  m_record(iConfig.getUntrackedParameter<std::string>("record","CTPPSPixelGainCalibrationsRcd")),
  m_inputHistosFileName(iConfig.getUntrackedParameter<std::string>("inputrootfile","inputfile.root"))
{

}


myCTPPSPixGainCalibsMaker::~myCTPPSPixGainCalibsMaker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ method called for each event  ------------
void
myCTPPSPixGainCalibsMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //   using namespace edm;

}


// ------------ method called once each job just before starting event loop  ------------
void 
myCTPPSPixGainCalibsMaker::beginJob()
{
}


// ------------ method called once each job just after ending the event loop  ------------
void 
myCTPPSPixGainCalibsMaker::endJob() 
{
  getHistos();
  fillDB();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
myCTPPSPixGainCalibsMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


void 
myCTPPSPixGainCalibsMaker::getHistos() 
{
  std::cout <<"Parsing file " <<m_inputHistosFileName << std::endl;
  m_inputRootFile = new TFile(m_inputHistosFileName.c_str());
  m_inputRootFile->cd();
  TDirectory *dir = m_inputRootFile->GetDirectory("siPixelGainCalibrationAnalysis");
  TList *list = dir->GetListOfKeys();
  list->Print();
}


void 
myCTPPSPixGainCalibsMaker::fillDB() 
{
  
  
  //hardcoded values for test only
  CTPPSPixelGainCalibrations * gainCalibsTest = new CTPPSPixelGainCalibrations();
  
  CTPPSPixelDetId myid020(/*arm*/0,/*station*/0,/*pot*/2,/*plane*/0);

  std::vector<float> fakegains,fakepeds;

  TH2F * testgains = (TH2F*)m_inputRootFile->Get("siPixelGainCalibrationAnalysis/Pixel/Barrel/Shell_pO/Layer_1/Ladder_07F/Module_1/Gain2d_siPixelCalibDigis_302058516");
  TH2F * testpeds  = (TH2F*)m_inputRootFile->Get("siPixelGainCalibrationAnalysis/Pixel/Barrel/Shell_pO/Layer_1/Ladder_07F/Module_1/Pedestal2d_siPixelCalibDigis_302058516");
  int ncols = testgains->GetNbinsX();
  int nrows = testgains->GetNbinsY();
  if (nrows != 160)
    std::cout<<"Something wrong ncols = "<< ncols << " and nrows = " << nrows <<std::endl;

  ncols = ncols>156 ? 156:nrows;  // the example is from central pixels, adapt it to CTPPS pixels 160x156 max
  
  for (int jrow = 1; jrow <= nrows ; ++jrow) // when scanning through the 2d histo make sure to avoid underflow bin i or j ==0
    for (int icol = 1 ; icol <= ncols ; ++icol){
      fakegains.push_back(testgains->GetBinContent(icol,jrow));
      fakepeds.push_back(testpeds->GetBinContent(icol,jrow));
    }

  gainCalibsTest->setGainCalibration(myid020.rawId(),fakepeds,fakegains);
  
  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  if(!mydbservice.isAvailable() ){
    edm::LogError("db service unavailable");
    return;
  }
  mydbservice->writeOne( gainCalibsTest, mydbservice->currentTime(), m_record  );
  
  //test also saving directly with putData and initializing from vectors, etc


}


//define this as a plug-in
DEFINE_FWK_MODULE(myCTPPSPixGainCalibsMaker);
