// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TEST_TRANSFER_CACHE_TEST_HELPER_H_
#define CC_TEST_TRANSFER_CACHE_TEST_HELPER_H_

#include <map>
#include <vector>

#include "base/containers/span.h"
#include "cc/paint/transfer_cache_deserialize_helper.h"
#include "cc/paint/transfer_cache_serialize_helper.h"

class GrContext;

namespace cc {

class TransferCacheTestHelper : public TransferCacheDeserializeHelper,
                                public TransferCacheSerializeHelper {
 public:
  explicit TransferCacheTestHelper(GrContext* context = nullptr);
  ~TransferCacheTestHelper() override;

  void SetGrContext(GrContext* context);
  void SetCachedItemsLimit(size_t limit);

  // Direct Access API (simulates ContextSupport methods).
  bool LockEntryDirect(const EntryKey& key);

  void CreateEntryDirect(const EntryKey& key, base::span<uint8_t> data);
  void UnlockEntriesDirect(const std::vector<EntryKey>& keys);
  void DeleteEntryDirect(const EntryKey& key);

 protected:
  // Deserialization helpers.
  ServiceTransferCacheEntry* GetEntryInternal(TransferCacheEntryType type,
                                              uint32_t id) override;

  // Serialization helpers.
  bool LockEntryInternal(const EntryKey& key) override;
  void CreateEntryInternal(const ClientTransferCacheEntry& entry) override;
  void FlushEntriesInternal(std::set<EntryKey> keys) override;

 private:
  // Helper functions.
  void EnforceLimits();

  std::map<EntryKey, std::unique_ptr<ServiceTransferCacheEntry>> entries_;
  std::set<EntryKey> locked_entries_;

  GrContext* context_ = nullptr;
  size_t cached_items_limit_ = std::numeric_limits<size_t>::max();
};

}  // namespace cc

#endif  // CC_TEST_TRANSFER_CACHE_TEST_HELPER_H_
