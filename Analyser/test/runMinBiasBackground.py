import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process('MinBiasBackground')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.Geometry.GeometryExtended2026D67Reco_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1),
)

process.source = cms.Source("PoolSource", fileNames=cms.untracked.vstring('file:step3.root'))
process.source.skipEvents = cms.untracked.uint32(0)

process.options = cms.untracked.PSet(
)

process.TFileService = cms.Service("TFileService",fileName = cms.string("histo.root"))

process.MinBiasBackground = cms.EDAnalyzer('MinBiasBackground',
  gemRecHits = cms.InputTag('gemRecHits'),
  gemSimHits = cms.InputTag('g4SimHits', "MuonGEMHits"),
)

process.p = cms.Path(process.MinBiasBackground)

process.schedule = cms.Schedule(process.p)
