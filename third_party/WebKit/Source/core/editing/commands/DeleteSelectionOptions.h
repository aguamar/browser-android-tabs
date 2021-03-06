// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DeleteSelectionOptions_h
#define DeleteSelectionOptions_h

#include "base/macros.h"
#include "core/CoreExport.h"
#include "platform/wtf/Allocator.h"

namespace blink {

// DeleteSelectionOptions of |DeleteSelectionCommand|.
class CORE_EXPORT DeleteSelectionOptions final {
  DISALLOW_NEW();

 public:
  class Builder;

  DeleteSelectionOptions(const DeleteSelectionOptions&);

  bool IsExpandForSpecialElements() const;
  bool IsMergeBlocksAfterDelete() const;
  bool IsSanitizeMarkup() const;
  bool IsSmartDelete() const;

  static DeleteSelectionOptions NormalDelete();
  static DeleteSelectionOptions SmartDelete();

 private:
  DeleteSelectionOptions();

  bool is_expand_for_special_elements_ = false;
  bool is_merge_blocks_after_delete_ = false;
  bool is_sanitize_markup_ = false;
  bool is_smart_delete_ = false;
};

// Build |DeleteSelectionCommand::Options|.
class CORE_EXPORT DeleteSelectionOptions::Builder final {
  DISALLOW_NEW();

 public:
  Builder();

  DeleteSelectionOptions Build() const;

  Builder& SetExpandForSpecialElements(bool);
  Builder& SetMergeBlocksAfterDelete(bool);
  Builder& SetSanitizeMarkup(bool);
  Builder& SetSmartDelete(bool);

 private:
  DeleteSelectionOptions options_;

  DISALLOW_COPY_AND_ASSIGN(Builder);
};

}  // namespace blink

#endif  // DeleteSelectionOptions_h
