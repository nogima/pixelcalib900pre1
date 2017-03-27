// -*- C++ -*-
//
// Package:     CondFormats/CTPPSReadoutObjects
// Class  :     CTPPSPixelDAQCalibration
// 
// Implementation:
//     [Notes on implementation]
//
// Original Author:  Helio Nogima
//         Created:  Wed, 15 Mar 2017 02:15:07 GMT
//
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelDAQCalibration.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelIndices.h"
#include <iostream>
#include "TFile.h"
#include "TH2F.h"

CTPPSPixelDAQCalibration::CTPPSPixelDAQCalibration(edm::ParameterSet const& conf)
{
  CalibrationFile_ = conf.getParameter<std::string>("CalibrationFile");

// To be removed
//  CalibrationFile_ = std::string("Gain_Fed_1294_Run_99.root");

  fp = new TFile(CalibrationFile_.c_str());
}

// CTPPSPixelDAQCalibration::CTPPSPixelDAQCalibration(const CTPPSPixelDAQCalibration& rhs)
// {
//    // do actual copying here;
// }

CTPPSPixelDAQCalibration::~CTPPSPixelDAQCalibration()
{
}

void CTPPSPixelDAQCalibration::getDAQCalibration(unsigned int detid, int row, int col, float &gain, float &pedestal){

  CTPPSPixelIndices * modulepixels = new CTPPSPixelIndices(156,160);
  int plane = int((detid>>16) & 0X7);
  int arm = int((detid>>24)& 0X1);
  int station = int((detid>>22)& 0X3);
  int pot = int((detid>>19)& 0X7);
  int roc = 0;
  int sector=0;
  int colROC;
  int rowROC;
  if (arm==0) sector=45;
  if (arm==1) sector=56;
//  std::cout << "arm = " << arm << "  rp = " << pot  << "  station = "  << station << "  plane = " << plane << std::endl;

// To be removed - test file including only arm==0. pot==2 and plane<5
  if (arm==1) sector=45;
  if (plane==5) plane=4;
  if (pot==3) pot=2;

  if (modulepixels->transformToROC(col,row,roc,colROC,rowROC)==0){
   sprintf(pathgains,"CTPPS/CTPPS_SEC%d/CTPPS_SEC%d_RP00%d/CTPPS_SEC%d_RP00%d_PLN%d/CTPPS_SEC%d_RP00%d_PLN%d_ROC%d_Slope2D",sector,sector,pot,sector,pot,plane,sector,pot,plane,roc);
   sprintf(pathpedestals,"CTPPS/CTPPS_SEC%d/CTPPS_SEC%d_RP00%d/CTPPS_SEC%d_RP00%d_PLN%d/CTPPS_SEC%d_RP00%d_PLN%d_ROC%d_Intercept2D",sector,sector,pot,sector,pot,plane,sector,pot,plane,roc);

   if(!(gainshisto = (TH2F*)fp->Get(pathgains))) {
    std::cout << pathgains << " not found." << std::endl;
   }

   if(!(pedestalshisto = (TH2F*)fp->Get(pathpedestals))) {
    std::cout << pathpedestals << " not found." << std::endl;
    return;
  }

 gain = float(gainshisto->GetBinContent(colROC+1,rowROC+1));
 pedestal = float(pedestalshisto->GetBinContent(colROC+1,rowROC+1));

 }else{
  std::cout << "** Module to ROC pixel transformation failed!! **" << std::endl;
 }

}
//DEFINE_FWK_MODULE( CTPPSPixelDAQCalibration);
