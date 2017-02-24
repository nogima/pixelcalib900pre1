#include "CondFormats/CTPPSObjects/src/headers.h"

namespace CondFormats_CTPPSObjects{
  struct dictionary {
    std::map<unsigned int,CTPPSRPPosition> myposmap;
    std::vector<float>::iterator p1;
    std::vector<float>::const_iterator p2;
    std::vector< CTPPSPixelGainCalibration::DetRegistry >::iterator p3;
    std::vector< CTPPSPixelGainCalibration::DetRegistry >::const_iterator p4;
    std::map<uint32_t,CTPPSPixelGainCalibration> mycalibmap; 


  };
  
}
