// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/cssom/CSSStyleImageValue.h"

namespace blink {

double CSSStyleImageValue::intrinsicWidth(bool& is_null) const {
  const WTF::Optional<IntSize> size = IntrinsicSize();
  if (!size) {
    is_null = true;
    return 0;
  }
  return size.value().Width();
}

double CSSStyleImageValue::intrinsicHeight(bool& is_null) const {
  const WTF::Optional<IntSize> size = IntrinsicSize();
  if (!size) {
    is_null = true;
    return 0;
  }
  return size.value().Height();
}

double CSSStyleImageValue::intrinsicRatio(bool& is_null) const {
  const WTF::Optional<IntSize> size = IntrinsicSize();
  if (!size || size.value().Height() == 0) {
    is_null = true;
    return 0;
  }
  return static_cast<double>(size.value().Width()) / size.value().Height();
}

FloatSize CSSStyleImageValue::ElementSize(
    const FloatSize& default_object_size) const {
  bool not_used;
  return FloatSize(intrinsicWidth(not_used), intrinsicHeight(not_used));
}

}  // namespace blink
