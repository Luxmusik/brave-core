/* Copyright 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <string> 

#include "base/timer/timer.h"
#include "components/prefs/pref_service.h"
#include "url/gurl.h"

namespace brave {

class BraveTraceCollectionService {
    public:
     explicit BraveTraceCollectionService(PrefService* pref_service);
     ~BraveTraceCollectionService();

     void Init();

     //static void LoadPrefs(PrefRegistrySimple* registry, bool first_run);
     //static void StorePrefs(PrefRegistrySimple* registry, bool first_run);

     void StartCollection();

    private:

     void OnCollectionSlotStartTimerFired();
     void OnFakeUpdateComputationTimerFired();

     void SendCollectionSlot();

     int GetCurrentCollectionSlot();
     std::string GetPlatformIdentifier();
     std::string GetEphemeralID();

     bool isP3AEnabled();
     bool isAdsEnabled();
     bool isTraceCollectionEnabled();
     //bool hasCheckedInCurrentSlot();

     PrefService* pref_service_;
     GURL trace_collection_endpoint_;
     std::unique_ptr<base::RepeatingTimer> collection_slot_periodic_timer_;
     std::unique_ptr<base::RetainingOneShotTimer> fake_update_activity_timer_;

     int last_check_slot_;
     int collection_slot_size_;
     int fake_computation_duration_;

     // Do we need a callback to update the last collected slot
};

} // namespace brave