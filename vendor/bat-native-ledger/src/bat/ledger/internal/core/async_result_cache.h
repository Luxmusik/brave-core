/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_CORE_ASYNC_RESULT_CACHE_H_
#define BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_CORE_ASYNC_RESULT_CACHE_H_

#include <list>
#include <map>
#include <type_traits>
#include <utility>

#include "base/memory/weak_ptr.h"
#include "base/optional.h"
#include "base/time/time.h"
#include "bat/ledger/internal/core/async_result.h"

namespace ledger {

// Caches operations that return async results. Operations can be keyed and
// results can be cached for a user-specified amount of time.
//
// Example:
//
//   AsyncResultCache<int> cache;
//
//   cache.GetResult([]() {
//     AsyncResult<int>::Resolver resolver;
//     resolver.Complete(42);
//     return resolver.result();
//   }).Then([](int value) {
//     LOG(INFO) << "Value is: " << value;
//   });
//
// The value returned from the function object supplied to |GetValue| can be one
// of:
//
//   - AsyncResult<T>
//   - AsyncResult<std::pair<T, base::TimeDelta>>
//
// When an |std::pair| is returned, the second value determines the length of
// time that the result will be cached for. Otherwise, the value is not cached
// after it has been completed and subsequent calls to |GetValue| will
// reexecute the function object.
template <typename T, typename Key = int>
class AsyncResultCache {
 private:
  static_assert(std::is_copy_constructible<T>::value,
                "AsyncResultCache<T> requires that T is copy constructible");

  using Resolver = typename AsyncResult<T>::Resolver;

  struct Entry {
    base::Optional<T> value;
    base::Time expires_at;
    std::list<Resolver> resolvers;
  };

 public:
  // Returns a cached AsyncResult using a default key. If the cache entry does
  // exist, then the specified function object is executed. The AsyncResult
  // returned by the function object is stored as the current cache entry. The
  // |Key| type must be default-constructible.
  template <typename F>
  AsyncResult<T> GetResult(F fn) {
    return GetResult(Key(), std::forward<F>(fn));
  }

  // Returns a cached AsyncResult for the specified key. If the cache entry does
  // exist, then the specified function object is executed. The AsyncResult
  // returned by the function object is stored as the current cache entry for
  // the given key.
  template <typename F>
  AsyncResult<T> GetResult(Key key, F fn) {
    MaybePurgeStaleEntries();

    Resolver resolver;

    auto iter = entries_.find(key);
    if (iter == entries_.end()) {
      auto pair = entries_.emplace(key, Entry());
      iter = pair.first;
    } else {
      Entry& entry = iter->second;
      if (entry.value && !EntryIsStale(entry)) {
        resolver.Complete(*entry.value);
        return resolver.result();
      }
    }

    Entry& entry = iter->second;
    entry.resolvers.push_back(resolver);
    if (entry.resolvers.size() == 1) {
      auto result = fn();
      using CompleteType = typename decltype(result)::CompleteType;
      result.Then(base::BindOnce(&AsyncResultCache::OnComplete<CompleteType>,
                                 weak_factory_.GetWeakPtr(), key));
    }

    return resolver.result();
  }

 private:
  template <typename U>
  void OnComplete(Key key, U value) {
    SetValue(key, std::move(value));
  }

  void SetValue(Key key, T value) {
    SetValue(key, std::make_pair(std::move(value), base::TimeDelta()));
  }

  void SetValue(Key key, std::pair<T, base::TimeDelta> pair) {
    auto iter = entries_.find(key);
    DCHECK(iter != entries_.end());

    Entry& entry = iter->second;
    entry.value = std::move(pair.first);
    entry.expires_at = base::Time::Now() + pair.second;

    std::list<Resolver> resolvers = std::move(entry.resolvers);
    for (auto& resolver : resolvers)
      resolver.Complete(*entry.value);
  }

  void MaybePurgeStaleEntries() {
    base::Time now = base::Time::Now();
    if (last_purge_ + base::TimeDelta::FromSeconds(30) < now)
      return;

    last_purge_ = now;

    for (auto iter = entries_.begin(); iter != entries_.end();) {
      if (EntryIsStale(iter->second))
        iter = entries_.erase(iter);
      else
        ++iter;
    }
  }

  bool EntryIsStale(const Entry& entry) {
    return entry.value && entry.expires_at <= base::Time::Now();
  }

  std::map<Key, Entry> entries_;
  base::Time last_purge_ = base::Time::Now();
  base::WeakPtrFactory<AsyncResultCache> weak_factory_{this};
};

}  // namespace ledger

#endif  // BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_CORE_ASYNC_RESULT_CACHE_H_