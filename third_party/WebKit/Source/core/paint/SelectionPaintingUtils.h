// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SelectionPaintingUtils_h
#define SelectionPaintingUtils_h

#include "base/memory/scoped_refptr.h"
#include "core/CoreExport.h"
#include "core/paint/PaintPhase.h"

namespace blink {

class Color;
class Document;
class ComputedStyle;
class Node;
struct TextPaintStyle;
struct PaintInfo;

class CORE_EXPORT SelectionPaintingUtils {
 public:
  static Color SelectionBackgroundColor(const Document&,
                                        const ComputedStyle&,
                                        Node*);
  static Color SelectionForegroundColor(const Document&,
                                        const ComputedStyle&,
                                        Node*,
                                        const GlobalPaintFlags);
  static Color SelectionEmphasisMarkColor(const Document&,
                                          const ComputedStyle&,
                                          Node*,
                                          const GlobalPaintFlags);
  static TextPaintStyle SelectionPaintingStyle(const Document&,
                                               const ComputedStyle&,
                                               Node*,
                                               bool have_selection,
                                               const TextPaintStyle& text_style,
                                               const PaintInfo&);
};

}  // namespace blink

#endif  // SelectionPaintingUtils_h
