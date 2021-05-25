/* Copyright 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <string>

#include "base/rand_util.h"
#include "base/strings/string_number_conversions.h"
#include "brave/components/brave_stats/browser/brave_stats_updater_util.h"
#include "components/prefs/pref_service.h"
#include "services/network/public/cpp/simple_url_loader.h"
#include "brave/components/brave_federated_learning/trace_collection_service.h"

namespace brave {

namespace {

constexpr char federatedLearningUrl[] = "https://fl.brave.com/";
constexpr char traceCollectionWikiUrl[] = "https://#########";

constexpr uint64_t kDefaultFakeUpdateCalculationDuration = 5 * 60; // 5 minutes.

} // anonymous namespace

BraveTraceCollectionService::BraveTraceCollectionService(PrefService* pref_service)
        : pref_service_(pref_service) {}

BraveTraceCollectionService::~BraveTraceCollectionService() {}

void BraveTraceCollectionService::Init() {
    initialized_ = true;

    // Can override settings from command line here

    std::cerr << "FL-Trace-Collection: BraveTraceCollectionService::Init() Done!";
    std::cerr << ", last_checked_slot_ = " << last_check_slot_
              << ", collection_slot_size_ = " << collection_slot_size_
              << ", fake_computation_duration_ = " << fake_computation_duration_;
            

    // Init other components if needed
    if (isAdsEnabled() && isP3AEnabled() && isTraceCollectionEnabled()) {
        StartCollection();
    }
}

void BraveTraceCollectionService::StartCollection() {

    // Fake update timer for current slot
    DCHECK(!fake_update_activity_timer_);
    fake_update_activity_timer_ = std::make_unique<base::RetainingOneShotTimer>();
    fake_update_activity_timer_->Start(
        FROM_HERE,
        base::TimeDelta::FromSeconds(kDefaultFakeUpdateCalculationDuration),
        this, &BraveTraceCollectionService::OnFakeUpdateComputationTimerFired)
    );
    
    // Recurrent timer
    DCHECK(!collection_slot_periodic_timer_);
    collection_slot_periodic_timer_ = std::make_unique<base::RepeatingTimer>();
    collection_slot_periodic_timer_->Start(
        FROM_HERE,
        //delta,  TODO: Need some logic here
        this, &BraveTraceCollectionService::OnCollectionSlotStartTimerFired
    );
}

void BraveTraceCollectionService::OnCollectionSlotStartTimerFired() {
    DCHECK(fake_update_activity_timer_);
    std::cerr << "FL-Trace-Collection: Collection slot fired";
    fake_update_activity_timer_.reset();
}

void BraveTraceCollectionService::OnFakeUpdateComputationTimerFired() {
    SendCollectionSlot();
}

void BraveTraceCollectionService::SendCollectionSlot() {
    // Can we leverage P3A code here: need compressed_log_data
    // Post HTTP request to collection endpoint
    auto resource_request = std::make_unique<network::ResourceRequest>();
    resource_request->url = trace_collection_endpoint_;
    //resource_request->headers.SetHeader("X-Brave-Trace", "?1"); Needed??

    resource_request->credentials_mode = network::mojom::CredentialsMode::kOmit;
    resource_request->method = "POST";

    // Get payload information
    std::string ephemeral_ID = GetEphemeralID();
    std::string platform = GetPlatformIdentifier();
    int collection_slot = GetCurrentCollectionSlot();

    std::cerr << "FL-Trace-Collection: " << collection_slot;
}

int BraveTraceCollectionService::GetCurrentCollectionSlot() {
    base::Time::Exploded now;
    base::Time::Now().LocalExplode(&now);
     
    std::cerr << "FL-Trace-Collection: H:" << now.hour
              << ", M:" << now.minute;
    return now.hour*60/collection_slot_size_;
}
     
std::string BraveTraceCollectionService::GetPlatformIdentifier() {
    return brave_stats::GetPlatformIdentifier();
}

std::string BraveTraceCollectionService::GetEphemeralID() {
    return "XXXXXXXXXX";
}

bool BraveTraceCollectionService::isTraceCollectionEnabled() {
    // TODO: Hook up with Griffin feature switch

    return true;
}

bool BraveTraceCollectionService::isAdsEnabled() {
    return ProfileManager::GetPrimaryUserProfile()->GetPrefs()->GetBoolean(
      ads::prefs::kEnabled);
}

bool BraveTraceCollectionService::isP3AEnabled() {
    return pref_service_->GetBoolean(brave::kP3AEnabled);
}


} // namespace brave