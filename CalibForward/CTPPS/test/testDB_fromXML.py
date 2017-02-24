import FWCore.ParameterSet.Config as cms

process = cms.Process("ProcessOne")

process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)
#Database output service

process.load("CondCore.CondDB.CondDB_cfi")
# output database (in this case local sqlite file)
process.CondDB.connect = 'sqlite_file:myctpps_xml.db'


process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDB,
    timetype = cms.untracked.string('runnumber'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('CTPPSRPPositionsRcd'),
        tag = cms.string('myctppsRPpos_test0')
    ))
)

from ParseMyXML import RPIDAndDistFromXML

RPinfo=RPIDAndDistFromXML("/afs/cern.ch/work/c/cmora/CTPPSDB/CMSSW_8_1_0_pre8/src/RP_Dist_Beam_Cent.xml")

process.WriteInDB = cms.EDAnalyzer("myCTPPSRPPosMaker",
                                   record= cms.string('CTPPSRPPositionsRcd'),
                                   RPIDvect = cms.vuint32(RPinfo.vrpn),
                                   RPDistvect =cms.vdouble(RPinfo.vdist)
)

process.p = cms.Path(process.WriteInDB)


