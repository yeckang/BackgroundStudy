#include "BackgroundStudy/Analyser/plugins/MinBiasBackground.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

MinBiasBackground::MinBiasBackground(const edm::ParameterSet& pset) {
  rechit_token_ = consumes<GEMRecHitCollection>(pset.getParameter<edm::InputTag>("gemRecHits"));
  simhit_token_ = consumes<edm::PSimHitContainer>(pset.getParameter<edm::InputTag>("gemSimHits"));

  geomToken_ = esConsumes<GEMGeometry, MuonGeometryRecord>();
  geomTokenBeginRun_ = esConsumes<GEMGeometry, MuonGeometryRecord, edm::Transition::BeginRun>();
}

MinBiasBackground::~MinBiasBackground() {}

void MinBiasBackground::beginRun(const edm::Run& run, const edm::EventSetup& setup) {
  const GEMGeometry* gem = &setup.getData(geomTokenBeginRun_);

  for (const auto& station : gem->regions()[0]->stations()) {
    Int_t station_id = station->station();
    hist_rechit_global_pos_x_[station_id] 
      = fs_->make<TH1D>(Form("rechit_global_pos_x_st%d", station_id),"rechit_x",1000,0,1000);

    hist_rechit_global_pos_y_[station_id] 
      = fs_->make<TH1D>(Form("rechit_global_pos_y_st%d", station_id),"rechit_y",1000,0,1000);

    hist_rechit_global_pos_z_[station_id] 
      = fs_->make<TH1D>(Form("rechit_global_pos_z_st%d", station_id),"rechit_z",1000,0,1000);


    hist_simhit_global_pos_x_[station_id] 
      = fs_->make<TH1D>(Form("simhit_global_pos_x_st%d", station_id),"simhit_x",1000,0,1000);

    hist_simhit_global_pos_y_[station_id] 
      = fs_->make<TH1D>(Form("simhit_global_pos_y_st%d", station_id),"simhit_y",1000,0,1000);

    hist_simhit_global_pos_z_[station_id] 
      = fs_->make<TH1D>(Form("simhit_global_pos_z_st%d", station_id),"simhit_z",1000,0,1000);
  }
}


void MinBiasBackground::endRun(const edm::Run& run, const edm::EventSetup& setup) {
}

void MinBiasBackground::analyze(const edm::Event& event, const edm::EventSetup& setup) {
  const GEMGeometry* gem = &setup.getData(geomToken_);

  edm::Handle<edm::PSimHitContainer> simhit_container;
  event.getByToken(simhit_token_, simhit_container);

  edm::Handle<GEMRecHitCollection> rechit_collection;
  event.getByToken(rechit_token_, rechit_collection);

  for (const auto& rechit : *rechit_collection) {
    GEMDetId gem_id{rechit.gemId()};
    //Int_t region_id = gem_id.region();
    Int_t station_id = gem_id.station();
    //Int_t layer_id = gem_id.layer();
    //Int_t roll_id = gem_id.roll();

    const BoundPlane& surface = gem->idToDet(gem_id)->surface();
    GlobalPoint&& rechit_global_pos = surface.toGlobal(rechit.localPosition());

    Float_t rechit_g_abs_x = std::fabs(rechit_global_pos.x());
    Float_t rechit_g_abs_y = std::fabs(rechit_global_pos.y());
    Float_t rechit_g_abs_z = std::fabs(rechit_global_pos.z());

    hist_rechit_global_pos_x_[station_id]->Fill(rechit_g_abs_x);
    hist_rechit_global_pos_y_[station_id]->Fill(rechit_g_abs_y);
    hist_rechit_global_pos_z_[station_id]->Fill(rechit_g_abs_z);
  }

  // NOTE
  for (const auto& simhit : *simhit_container.product()) {

    GEMDetId simhit_gemid{simhit.detUnitId()};
    const BoundPlane& surface = gem->idToDet(simhit_gemid)->surface();

    //Int_t region_id = simhit_gemid.region();
    Int_t station_id = simhit_gemid.station();
    //Int_t layer_id = simhit_gemid.layer();
    //Int_t chamber_id = simhit_gemid.chamber();
    //Int_t roll_id = simhit_gemid.roll();

    const LocalPoint& simhit_local_pos = simhit.localPosition();
    const GlobalPoint& simhit_global_pos = surface.toGlobal(simhit_local_pos);

    Float_t simhit_g_abs_x = std::fabs(simhit_global_pos.x());
    Float_t simhit_g_abs_y = std::fabs(simhit_global_pos.y());
    Float_t simhit_g_abs_z = std::fabs(simhit_global_pos.z());

    hist_simhit_global_pos_x_[station_id]->Fill(simhit_g_abs_x);
    hist_simhit_global_pos_y_[station_id]->Fill(simhit_g_abs_y);
    hist_simhit_global_pos_z_[station_id]->Fill(simhit_g_abs_z);
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(MinBiasBackground);
