#include "CondCore/ESSources/interface/registration_macros.h"
#include "CondFormats/CTPPSObjects/interface/CTPPSRPPositions.h"
#include "CondFormats/CTPPSObjects/interface/CTPPSPixelGainCalibrations.h"
#include "CondFormats/DataRecord/interface/CTPPSRPPositionsRcd.h"
#include "CondFormats/DataRecord/interface/CTPPSPixelGainCalibrationsRcd.h"

REGISTER_PLUGIN(CTPPSRPPositionsRcd,CTPPSRPPositions);
REGISTER_PLUGIN(CTPPSPixelGainCalibrationsRcd,CTPPSPixelGainCalibrations);
