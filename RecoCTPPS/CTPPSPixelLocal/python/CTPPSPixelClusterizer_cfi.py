import FWCore.ParameterSet.Config as cms

clusterProd = cms.EDProducer("CTPPSPixelClusterProducer",
                              label=cms.untracked.string("RPixDetDigitizer"),
                              RPixVerbosity = cms.int32(0),
                              SeedADCThreshold = cms.int32(50),
                              ADCThreshold = cms.int32(10),
                              ElectronADCGain = cms.double(135.0),
                              VCaltoElectronOffset = cms.int32(-414),
			      VCaltoElectronGain = cms.int32(65),
                              CalibrationFile = cms.string("Gain_Fed_1294_Run_99.root"),
                              DAQCalibration = cms.bool(True)
)
