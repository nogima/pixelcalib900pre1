// -*- C++ -*-
//
// Package:     RecoCTPPS/CTPPSPixelLocal
// Class  :     CTPPSPixelGainCalibrationDBService
// 
// Implementation:
//     [Notes on implementation]
//
// Original Author:  Helio Nogima
//         Created:  Thu, 23 Feb 2017 17:37:50 GMT
//

#include "RecoCTPPS/CTPPSPixelLocal/interface/CTPPSPixelGainCalibrationDBService.h"
//#include "CalibForward/CTPPS/interface/CTPPSPixelGainCalibrationDBService.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CondFormats/CTPPSObjects/interface/CTPPSPixelGainCalibrations.h"
#include "CondFormats/DataRecord/interface/CTPPSPixelGainCalibrationsRcd.h"

CTPPSPixelGainCalibrationDBService::CTPPSPixelGainCalibrationDBService()
{
}

CTPPSPixelGainCalibrationDBService::~CTPPSPixelGainCalibrationDBService()
{
}

void CTPPSPixelGainCalibrationDBService::getDB(const edm::Event& iEvent, const edm::EventSetup& iSetup){

 edm::eventsetup::EventSetupRecordKey recordKey(edm::eventsetup::EventSetupRecordKey::TypeTag::findType("CTPPSPixelGainCalibrationsRcd"));
 if( recordKey.type() == edm::eventsetup::EventSetupRecordKey::TypeTag()) {
  //record not found
  std::cout <<"Record \"CTPPSPixelGainCalibrationsRcd"<<"\" does not exist "<<std::endl;
 }
 edm::ESHandle<CTPPSPixelGainCalibrations> calhandle;
 iSetup.get<CTPPSPixelGainCalibrationsRcd>().get(calhandle);
 pPixelGainCalibrations=calhandle.product();
}
