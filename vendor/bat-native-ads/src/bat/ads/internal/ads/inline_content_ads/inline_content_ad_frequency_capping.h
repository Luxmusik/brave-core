/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_INLINE_CONTENT_ADS_INLINE_CONTENT_AD_FREQUENCY_CAPPING_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_INLINE_CONTENT_ADS_INLINE_CONTENT_AD_FREQUENCY_CAPPING_H_

#include "bat/ads/internal/ad_events/ad_event_info.h"
#include "bat/ads/internal/frequency_capping/frequency_capping_aliases.h"

namespace ads {

struct CreativeAdInfo;

namespace ad_targeting {
namespace geographic {
class SubdivisionTargeting;
}  // namespace geographic
}  // namespace ad_targeting

namespace resource {
class AntiTargeting;
}  // namespace resource

namespace inline_content_ads {

class FrequencyCapping {
 public:
  FrequencyCapping(
      ad_targeting::geographic::SubdivisionTargeting* subdivision_targeting,
      resource::AntiTargeting* anti_targeting,
      const AdEventList& ad_events,
      const BrowsingHistoryList& history);

  ~FrequencyCapping();

  FrequencyCapping(const FrequencyCapping&) = delete;
  FrequencyCapping& operator=(const FrequencyCapping&) = delete;

  bool IsAdAllowed();

  bool ShouldExcludeAd(const CreativeAdInfo& ad);

 private:
  ad_targeting::geographic::SubdivisionTargeting* subdivision_targeting_;

  resource::AntiTargeting* anti_targeting_;

  AdEventList ad_events_;

  BrowsingHistoryList history_;
};

}  // namespace inline_content_ads
}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_INLINE_CONTENT_ADS_INLINE_CONTENT_AD_FREQUENCY_CAPPING_H_