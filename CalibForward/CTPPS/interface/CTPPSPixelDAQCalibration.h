#ifndef CondFormats_CTPPSReadoutObjects_CTPPSPixelDAQCalibration_h
#define CondFormats_CTPPSReadoutObjects_CTPPSPixelDAQCalibration_h
// -*- C++ -*-
//
// Package:     CalibForward/CTPPSPixelCalibration
// Class  :     CTPPSPixelDAQCalibration
// 
/**\class CTPPSPixelDAQCalibration CTPPSPixelDAQCalibration.h "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelDAQCalibration.h"

 Description: [one line class summary]

 Usage:
    <usage>

*/
//
// Original Author:  Helio Nogima
//         Created:  Wed, 15 Mar 2017 02:15:33 GMT
//
//
#include "FWCore/ParameterSet/interface/ParameterSet.h"
class TFile;
class TH2F;
class CTPPSPixelDAQCalibration
{

   public:
      CTPPSPixelDAQCalibration(edm::ParameterSet const& conf);
      virtual ~CTPPSPixelDAQCalibration();
      virtual void getDAQCalibration(unsigned int, int, int, float&, float&);
      void setDAQCalibrationFile(std::string DAQFile){CalibrationFile_ = DAQFile;}
      char pathgains[200];
      char pathpedestals[200];

   private:
//      CTPPSPixelDAQCalibration(const CTPPSPixelDAQCalibration&); 
      std::string CalibrationFile_;
      const CTPPSPixelDAQCalibration& operator=(const CTPPSPixelDAQCalibration&); // stop default
      TFile * fp;
      TH2F * gainshisto;
      TH2F * pedestalshisto;
};


#endif
