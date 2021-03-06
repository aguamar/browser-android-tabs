// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/cssom/CSSMathNegate.h"

#include "core/css/cssom/CSSNumericSumValue.h"

namespace blink {

WTF::Optional<CSSNumericSumValue> CSSMathNegate::SumValue() const {
  auto maybe_sum = value_->SumValue();
  if (!maybe_sum)
    return WTF::nullopt;

  std::for_each(maybe_sum->terms.begin(), maybe_sum->terms.end(),
                [](auto& term) { term.value *= -1; });
  return maybe_sum;
}

void CSSMathNegate::BuildCSSText(Nested nested,
                                 ParenLess paren_less,
                                 StringBuilder& result) const {
  if (paren_less == ParenLess::kNo)
    result.Append(nested == Nested::kYes ? "(" : "calc(");

  result.Append("-");
  value_->BuildCSSText(Nested::kYes, ParenLess::kNo, result);

  if (paren_less == ParenLess::kNo)
    result.Append(")");
}

}  // namespace blink
