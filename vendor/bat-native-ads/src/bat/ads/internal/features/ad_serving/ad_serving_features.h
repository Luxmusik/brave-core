/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FEATURES_AD_SERVING_AD_SERVING_FEATURES_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FEATURES_AD_SERVING_AD_SERVING_FEATURES_H_

#include "base/feature_list.h"

namespace ads {
namespace features {

extern const base::Feature kAdServing;

bool IsAdServingEnabled();

// TODO(tmancey): Decouple to ad_notification_serving.cc
int GetDefaultAdNotificationsPerHour();
int GetMaximumAdNotificationsPerDay();

// TODO(tmancey): Decouple to inline_content_ad_serving.cc
int GetMaximumInlineContentAdsPerHour();
int GetMaximumInlineContentAdsPerDay();

// TODO(tmancey): Decouple to new_tab_page_ad_serving.cc
int GetMaximumNewTabPageAdsPerHour();
int GetMaximumNewTabPageAdsPerDay();

// TODO(tmancey): Decouple to inline_content_ad_serving.cc
int GetMaximumPromotedContentAdsPerHour();
int GetMaximumPromotedContentAdsPerDay();

int GetBrowsingHistoryMaxCount();
int GetBrowsingHistoryDaysAgo();

}  // namespace features
}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FEATURES_AD_SERVING_AD_SERVING_FEATURES_H_
