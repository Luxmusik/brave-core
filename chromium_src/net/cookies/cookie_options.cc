/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "net/cookies/cookie_options.h"

#define CookieOptions CookieOptions_ChromiumImpl
#include "../../../../net/cookies/cookie_options.cc"
#undef CookieOptions

namespace net {

CookieOptions::CookieOptions() = default;
CookieOptions::CookieOptions(const CookieOptions&) = default;
CookieOptions::CookieOptions(CookieOptions&&) = default;
CookieOptions::~CookieOptions() = default;
CookieOptions& CookieOptions::operator=(const CookieOptions&) = default;
CookieOptions& CookieOptions::operator=(CookieOptions&&) = default;

CookieOptions::CookieOptions(const CookieOptions_ChromiumImpl& rhs)
    : CookieOptions_ChromiumImpl(rhs) {}
CookieOptions::CookieOptions(CookieOptions_ChromiumImpl&& rhs)
    : CookieOptions_ChromiumImpl(std::move(rhs)) {}

}  // namespace net
