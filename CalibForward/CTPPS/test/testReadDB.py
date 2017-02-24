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
process.CondDB.connect = 'sqlite_file:myctpps00.db'

#process.load("CondCore.DBCommon.CondDBCommon_cfi")
# input database (in this case local sqlite file)
#process.CondDBCommon.connect = 'sqlite_file:MyPedestals.db'

process.PoolDBESSource = cms.ESSource("PoolDBESSource",
    process.CondDB,
    DumpStat=cms.untracked.bool(True),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('CTPPSRPPositionsRcd'),
        tag = cms.string("myctppsRPpos_00")
    )),
)

#process.PoolDBOutputService = cms.Service("PoolDBOutputService",
#    process.CondDB,
#    timetype = cms.untracked.string('runnumber'),
#    toPut = cms.VPSet(cms.PSet(
#        record = cms.string('CTPPSRPPositionsRcd'),
#        tag = cms.string('myctppsRPpos_test0')
#    ))
#)

#process.WriteInDB = cms.EDAnalyzer("myCTPPSRPPosMaker",
#                                   record= cms.string('CTPPSRPPositionsRcd'))

process.get = cms.EDAnalyzer("EventSetupRecordDataGetter",
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('CTPPSRPPositionsRcd'),
        data = cms.vstring('CTPPSRPPositions')
                      )),
    verbose = cms.untracked.bool(True)
 )

#process.p = cms.Path(process.get)

process.prod = cms.EDAnalyzer("myCTPPSRPPosESAnalyzer")
 
process.asciiprint = cms.OutputModule("AsciiOutputModule")
 
process.p = cms.Path(process.prod)
process.ep = cms.EndPath(process.asciiprint)


