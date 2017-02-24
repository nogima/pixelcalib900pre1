import FWCore.ParameterSet.Config as cms

process = cms.Process("ProcessOne")

process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(10099),  ## these runs  will take data for IOV 1 to 10100
    lastValue = cms.uint64(10110),   ## these runs will take data fror IOV10101 to EOT
    interval = cms.uint64(1)
)
#Database output service

process.load("CondCore.CondDB.CondDB_cfi")
# input database (in this case local sqlite file)
process.CondDB.connect = 'sqlite_file:myctppsnew_xml.db'  


process.PoolDBESSource = cms.ESSource("PoolDBESSource",
    process.CondDB,
    DumpStat=cms.untracked.bool(True),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('CTPPSRPPositionsRcd'),
        tag = cms.string("ctppsRPpos_v1.0_test")
    )),
)



process.myprodtest = cms.EDAnalyzer("myCTPPSRPPosESAnalyzer") 
 
process.p = cms.Path(process.myprodtest)


