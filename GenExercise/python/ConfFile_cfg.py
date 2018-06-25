import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
options.parseArguments()

process = cms.Process("BTagTest")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.printDecay = cms.EDAnalyzer("ParticleDecayDrawer",
                                   src = cms.InputTag("prunedGenParticles"),
                                   printP4 = cms.untracked.bool(False),
                                   printPtEtaPhi = cms.untracked.bool(False),
                                   printVertex = cms.untracked.bool(False)
                                   )


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

inputFiles=[]
if len(options.inputFiles)==1 and (".root" not in options.inputFiles[0]):
    flist = open(options.inputFiles[0])
    inputFiles = flist.readlines()
    flist.close()
else:
    inputFiles = options.inputFiles

process.source = cms.Source(
    "PoolSource",
    fileNames  = cms.untracked.vstring(inputFiles)
)

process.TFileService = cms.Service("TFileService", fileName = cms.string('GenW.root'))

process.btagtest = cms.EDAnalyzer('GenParticleList',
                              srcGenParticles = cms.InputTag('prunedGenParticles'),
                              srcPatJets = cms.InputTag('slimmedJets')
)

#process.p = cms.Path(process.printDecay*process.btagtest)

process.p = cms.Path(process.btagtest)
