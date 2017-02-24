import FWCore.ParameterSet.Config as cms

process = cms.Process("ProcessOne")


#writes payload for IOV 1--eot 
process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)
#Database output service

process.load("CondCore.CondDB.CondDB_cfi")
# output database (in this case local sqlite file)
process.CondDB.connect = 'sqlite_file:myctppsnew_xml.db'


process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDB,
    timetype = cms.untracked.string('runnumber'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('CTPPSRPPositionsRcd'),
        tag = cms.string('ctppsRPpos_v1.0_test')
    ))
)

from ParseMyXML import RPIDAndDistFromXML

RPinfo=RPIDAndDistFromXML("RP_Dist_Beam_Cent.xml")
#print RPinfo.vdist[0]
# modify the first distance by 1 unit just to test the saving of multiple (and different) payloads in same tag
RPinfo.vdist[0]+=1.0


## this takes the "cleaned up" version of the conditions maker, just using xml source and including the printout
process.WriteInDB = cms.EDAnalyzer("CTPPSRPPosMaker",
                                   record= cms.string('CTPPSRPPositionsRcd'),
                                   RPIDvect = cms.vuint32(RPinfo.vrpn),
                                   RPDistvect =cms.vdouble(RPinfo.vdist)
)

process.p = cms.Path(process.WriteInDB)


