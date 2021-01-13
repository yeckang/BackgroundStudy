#ifndef MinBiasBackground_h
#define MinBiasBackground_h

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Event.h"

#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

class MinBiasBackground : public edm::EDAnalyzer {
public:
  explicit MinBiasBackground(const edm::ParameterSet&);
  ~MinBiasBackground();

private:
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void beginRun(const edm::Run&, const edm::EventSetup&) override;
  virtual void endRun(const edm::Run&, const edm::EventSetup&) override;

  // Parameter
  edm::EDGetTokenT<GEMRecHitCollection> rechit_token_;
  edm::EDGetTokenT<edm::PSimHitContainer> simhit_token_;
  edm::ESGetToken<GEMGeometry, MuonGeometryRecord> geomToken_;
  edm::ESGetToken<GEMGeometry, MuonGeometryRecord> geomTokenBeginRun_;

  // output definitions
  edm::Service<TFileService> fs_;

  std::map<Int_t, TH1D*> hist_rechit_global_pos_x_;
  std::map<Int_t, TH1D*> hist_rechit_global_pos_y_;
  std::map<Int_t, TH1D*> hist_rechit_global_pos_z_;

  std::map<Int_t, TH1D*> hist_simhit_global_pos_x_;
  std::map<Int_t, TH1D*> hist_simhit_global_pos_y_;
  std::map<Int_t, TH1D*> hist_simhit_global_pos_z_;
};

#endif  // MinBiasBackground_h
