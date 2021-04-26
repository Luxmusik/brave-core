/* Copyright 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "net/url_request/url_request_http_job.h"

#include "../../../../net/url_request/url_request_http_job.cc"

namespace net {

CookieOptions URLRequestHttpJob::CreateCookieOptions(
    CookieOptions::SameSiteCookieContext same_site_context,
    CookieOptions::SamePartyCookieContextType same_party_context,
    const IsolationInfo& isolation_info,
    bool is_in_nontrivial_first_party_set) const {
  CookieOptions cookie_options =
      ::CreateCookieOptions(same_site_context, same_party_context,
                            isolation_info, is_in_nontrivial_first_party_set);
  cookie_options.set_top_frame_origin(isolation_info.top_frame_origin());
  cookie_options.set_site_for_cookies(request_->site_for_cookies());
  if (const CookieAccessDelegate* cookie_access_delegate =
          request_->context()->cookie_store()->cookie_access_delegate()) {
    cookie_options.set_should_use_ephemeral_storage(
        cookie_access_delegate->ShouldUseEphemeralStorage(
            request_->url(), cookie_options.site_for_cookies(),
            cookie_options.top_frame_origin()));
  }
  return cookie_options;
}

}  // namespace net
