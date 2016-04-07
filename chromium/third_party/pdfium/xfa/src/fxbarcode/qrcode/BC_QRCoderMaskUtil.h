// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _BC_QRCODERMASKUTIL_H_
#define _BC_QRCODERMASKUTIL_H_
class CBC_CommonByteMatrix;
class CBC_QRCoderMaskUtil {
 public:
  CBC_QRCoderMaskUtil();
  virtual ~CBC_QRCoderMaskUtil();
  static FX_BOOL GetDataMaskBit(int32_t maskPattern,
                                int32_t x,
                                int32_t y,
                                int32_t& e);

  static int32_t ApplyMaskPenaltyRule1(CBC_CommonByteMatrix* matrix);
  static int32_t ApplyMaskPenaltyRule2(CBC_CommonByteMatrix* matrix);
  static int32_t ApplyMaskPenaltyRule3(CBC_CommonByteMatrix* matrix);
  static int32_t ApplyMaskPenaltyRule4(CBC_CommonByteMatrix* matrix);
  static int32_t ApplyMaskPenaltyRule1Internal(CBC_CommonByteMatrix* matrix,
                                               FX_BOOL isHorizontal);
};
#endif
