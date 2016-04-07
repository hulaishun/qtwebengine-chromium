// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "fx_gif.h"
void CGifLZWDecoder::Input(uint8_t* src_buf, FX_DWORD src_size) {
  next_in = src_buf;
  avail_in = src_size;
}
FX_DWORD CGifLZWDecoder::GetAvailInput() {
  return avail_in;
}
void CGifLZWDecoder::InitTable(uint8_t code_len) {
  code_size = code_len;
  code_clear = 1 << code_size;
  code_end = code_clear + 1;
  bits_left = 0;
  code_store = 0;
  next_in = NULL;
  avail_in = 0;
  stack_size = 0;
  code_first = 0;
  ClearTable();
}
void CGifLZWDecoder::ClearTable() {
  code_size_cur = code_size + 1;
  code_next = code_end + 1;
  code_old = (FX_WORD)-1;
  FXSYS_memset(code_table, 0, sizeof(tag_Table) * GIF_MAX_LZW_CODE);
  FXSYS_memset(stack, 0, GIF_MAX_LZW_CODE);
  for (FX_WORD i = 0; i < code_clear; i++) {
    code_table[i].suffix = (uint8_t)i;
  }
}
void CGifLZWDecoder::DecodeString(FX_WORD code) {
  stack_size = 0;
  while (TRUE) {
    ASSERT(code <= code_next);
    if (code < code_clear || code > code_next) {
      break;
    }
    stack[GIF_MAX_LZW_CODE - 1 - stack_size++] = code_table[code].suffix;
    code = code_table[code].prefix;
  }
  stack[GIF_MAX_LZW_CODE - 1 - stack_size++] = (uint8_t)code;
  code_first = (uint8_t)code;
}
void CGifLZWDecoder::AddCode(FX_WORD prefix_code, uint8_t append_char) {
  if (code_next == GIF_MAX_LZW_CODE) {
    return;
  }
  code_table[code_next].prefix = prefix_code;
  code_table[code_next].suffix = append_char;
  if (++code_next < GIF_MAX_LZW_CODE) {
    if (code_next >> code_size_cur) {
      code_size_cur++;
    }
  }
}
int32_t CGifLZWDecoder::Decode(uint8_t* des_buf, FX_DWORD& des_size) {
  if (des_size == 0) {
    return 3;
  }
  FX_DWORD i = 0;
  if (stack_size != 0) {
    if (des_size < stack_size) {
      FXSYS_memcpy(des_buf, &stack[GIF_MAX_LZW_CODE - stack_size], des_size);
      stack_size -= (FX_WORD)des_size;
      return 3;
    }
    FXSYS_memcpy(des_buf, &stack[GIF_MAX_LZW_CODE - stack_size], stack_size);
    des_buf += stack_size;
    i += stack_size;
    stack_size = 0;
  }
  FX_WORD code = 0;
  while (i <= des_size && (avail_in > 0 || bits_left >= code_size_cur)) {
    if (code_size_cur > 12) {
      if (err_msg_ptr) {
        FXSYS_strncpy(err_msg_ptr, "Code Length Out Of Range",
                      GIF_MAX_ERROR_SIZE - 1);
      }
      return 0;
    }
    if (avail_in > 0) {
      code_store |= (*next_in++) << bits_left;
      avail_in--;
      bits_left += 8;
    }
    while (bits_left >= code_size_cur) {
      code = (FX_WORD)code_store & ((1 << code_size_cur) - 1);
      code_store >>= code_size_cur;
      bits_left -= code_size_cur;
      if (code == code_clear) {
        ClearTable();
        continue;
      } else if (code == code_end) {
        des_size = i;
        return 1;
      } else {
        if (code_old != (FX_WORD)-1) {
          if (code_next < GIF_MAX_LZW_CODE) {
            if (code == code_next) {
              AddCode(code_old, code_first);
              DecodeString(code);
            } else if (code > code_next) {
              if (err_msg_ptr) {
                FXSYS_strncpy(err_msg_ptr, "Decode Error, Out Of Range",
                              GIF_MAX_ERROR_SIZE - 1);
              }
              return 0;
            } else {
              DecodeString(code);
              uint8_t append_char = stack[GIF_MAX_LZW_CODE - stack_size];
              AddCode(code_old, append_char);
            }
          }
        } else {
          DecodeString(code);
        }
        code_old = code;
        if (i + stack_size > des_size) {
          FXSYS_memcpy(des_buf, &stack[GIF_MAX_LZW_CODE - stack_size],
                       des_size - i);
          stack_size -= (FX_WORD)(des_size - i);
          return 3;
        }
        FXSYS_memcpy(des_buf, &stack[GIF_MAX_LZW_CODE - stack_size],
                     stack_size);
        des_buf += stack_size;
        i += stack_size;
        stack_size = 0;
      }
    }
  }
  if (avail_in == 0) {
    des_size = i;
    return 2;
  }
  return 0;
}
static FX_BOOL _gif_grow_buf(uint8_t*& dst_buf,
                             FX_DWORD& dst_len,
                             FX_DWORD size) {
  if (dst_len < size) {
    FX_DWORD len_org = dst_len;
    while (dst_buf && dst_len < size) {
      dst_len <<= 1;
      dst_buf = FX_Realloc(uint8_t, dst_buf, dst_len);
    }
    if (dst_buf == NULL) {
      dst_len = size;
      dst_buf = FX_Realloc(uint8_t, dst_buf, dst_len);
      if (dst_buf == NULL) {
        return FALSE;
      }
    }
    FXSYS_memset(dst_buf + len_org, 0, dst_len - len_org);
    return dst_buf != NULL;
  }
  return TRUE;
}
static inline void _gif_cut_index(uint8_t& val,
                                  FX_DWORD index,
                                  uint8_t index_bit,
                                  uint8_t index_bit_use,
                                  uint8_t bit_use) {
  FX_DWORD cut = ((1 << (index_bit - index_bit_use)) - 1) << index_bit_use;
  val |= ((index & cut) >> index_bit_use) << bit_use;
}
static inline uint8_t _gif_cut_buf(const uint8_t* buf,
                                   FX_DWORD& offset,
                                   uint8_t bit_cut,
                                   uint8_t& bit_offset,
                                   FX_DWORD& bit_num) {
  if (bit_cut != 8) {
    FX_WORD index = 0;
    index |= ((1 << bit_cut) - 1) << (7 - bit_offset);
    uint8_t ret = ((index & buf[offset]) >> (7 - bit_offset));
    bit_offset += bit_cut;
    if (bit_offset >= 8) {
      if (bit_offset > 8) {
        ret |= ((index & (buf[offset + 1] << 8)) >> 8);
      }
      bit_offset -= 8;
      offset++;
    }
    bit_num += bit_cut;
    return ret;
  }
  bit_num += bit_cut;
  return buf[offset++];
}
CGifLZWEncoder::CGifLZWEncoder() {
  FXSYS_memset(this, 0, sizeof(CGifLZWEncoder));
}
CGifLZWEncoder::~CGifLZWEncoder() {}
void CGifLZWEncoder::ClearTable() {
  index_bit_cur = code_size + 1;
  index_num = code_end + 1;
  table_cur = code_end + 1;
  for (FX_WORD i = 0; i < GIF_MAX_LZW_CODE; i++) {
    code_table[i].prefix = 0;
    code_table[i].suffix = 0;
  }
}
void CGifLZWEncoder::Start(uint8_t code_len,
                           const uint8_t* src_buf,
                           uint8_t*& dst_buf,
                           FX_DWORD& offset) {
  code_size = code_len + 1;
  src_bit_cut = code_size;
  if (code_len == 0) {
    src_bit_cut = 1;
    code_size = 2;
  }
  code_clear = 1 << code_size;
  code_end = code_clear + 1;
  dst_buf[offset++] = code_size;
  bit_offset = 0;
  ClearTable();
  src_offset = 0;
  src_bit_offset = 0;
  src_bit_num = 0;
  code_table[index_num].prefix = _gif_cut_buf(src_buf, src_offset, src_bit_cut,
                                              src_bit_offset, src_bit_num);
  code_table[index_num].suffix = _gif_cut_buf(src_buf, src_offset, src_bit_cut,
                                              src_bit_offset, src_bit_num);
}
void CGifLZWEncoder::WriteBlock(uint8_t*& dst_buf,
                                FX_DWORD& dst_len,
                                FX_DWORD& offset) {
  if (!_gif_grow_buf(dst_buf, dst_len, offset + GIF_DATA_BLOCK + 1)) {
    longjmp(jmp, 1);
  }
  dst_buf[offset++] = index_buf_len;
  FXSYS_memcpy(&dst_buf[offset], index_buf, index_buf_len);
  offset += index_buf_len;
  FXSYS_memset(index_buf, 0, GIF_DATA_BLOCK);
  index_buf_len = 0;
}
void CGifLZWEncoder::EncodeString(FX_DWORD index,
                                  uint8_t*& dst_buf,
                                  FX_DWORD& dst_len,
                                  FX_DWORD& offset) {
  uint8_t index_bit_use;
  index_bit_use = 0;
  if (index_buf_len == GIF_DATA_BLOCK) {
    WriteBlock(dst_buf, dst_len, offset);
  }
  _gif_cut_index(index_buf[index_buf_len], index, index_bit_cur, index_bit_use,
                 bit_offset);
  if (index_bit_cur <= (8 - bit_offset)) {
    bit_offset += index_bit_cur;
  } else if (index_bit_cur <= (16 - bit_offset)) {
    index_bit_use += (8 - bit_offset);
    bit_offset = 0;
    index_buf_len++;
    if (index_buf_len == GIF_DATA_BLOCK) {
      WriteBlock(dst_buf, dst_len, offset);
    }
    _gif_cut_index(index_buf[index_buf_len], index, index_bit_cur,
                   index_bit_use, bit_offset);
    bit_offset = index_bit_cur - index_bit_use;
  } else {
    index_bit_use += (8 - bit_offset);
    bit_offset = 0;
    index_buf_len++;
    if (index_buf_len == GIF_DATA_BLOCK) {
      WriteBlock(dst_buf, dst_len, offset);
    }
    _gif_cut_index(index_buf[index_buf_len], index, index_bit_cur,
                   index_bit_use, bit_offset);
    index_bit_use += 8;
    bit_offset = 0;
    index_buf_len++;
    if (index_buf_len == GIF_DATA_BLOCK) {
      WriteBlock(dst_buf, dst_len, offset);
    }
    _gif_cut_index(index_buf[index_buf_len], index, index_bit_cur,
                   index_bit_use, bit_offset);
    bit_offset = index_bit_cur - index_bit_use;
  }
  if (bit_offset == 8) {
    bit_offset = 0;
    index_buf_len++;
    if (index_buf_len == GIF_DATA_BLOCK) {
      WriteBlock(dst_buf, dst_len, offset);
    }
  }
  if (index == code_end) {
    index_buf_len++;
    WriteBlock(dst_buf, dst_len, offset);
  }
  if (index_num++ >> index_bit_cur) {
    index_bit_cur++;
  }
}
FX_BOOL CGifLZWEncoder::Encode(const uint8_t* src_buf,
                               FX_DWORD src_len,
                               uint8_t*& dst_buf,
                               FX_DWORD& dst_len,
                               FX_DWORD& offset) {
  uint8_t suffix;
  if (setjmp(jmp)) {
    return FALSE;
  }
  while (src_bit_num < src_len) {
    if (!LookUpInTable(src_buf, src_offset, src_bit_offset)) {
      EncodeString(code_table[index_num].prefix, dst_buf, dst_len, offset);
      if (index_num == GIF_MAX_LZW_CODE) {
        suffix = code_table[index_num - 1].suffix;
        EncodeString(code_clear, dst_buf, dst_len, offset);
        ClearTable();
        code_table[index_num].prefix = suffix;
        code_table[index_num].suffix = _gif_cut_buf(
            src_buf, src_offset, src_bit_cut, src_bit_offset, src_bit_num);
      } else {
        code_table[index_num].prefix = code_table[index_num - 1].suffix;
        code_table[index_num].suffix = _gif_cut_buf(
            src_buf, src_offset, src_bit_cut, src_bit_offset, src_bit_num);
      }
    }
  }
  src_offset = 0;
  src_bit_offset = 0;
  src_bit_num = 0;
  return TRUE;
}
FX_BOOL CGifLZWEncoder::LookUpInTable(const uint8_t* buf,
                                      FX_DWORD& offset,
                                      uint8_t& bit_offset) {
  for (FX_WORD i = table_cur; i < index_num; i++) {
    if (code_table[i].prefix == code_table[index_num].prefix &&
        code_table[i].suffix == code_table[index_num].suffix) {
      code_table[index_num].prefix = i;
      code_table[index_num].suffix =
          _gif_cut_buf(buf, offset, src_bit_cut, bit_offset, src_bit_num);
      table_cur = i;
      return TRUE;
    }
  }
  table_cur = code_end + 1;
  return FALSE;
}
void CGifLZWEncoder::Finish(uint8_t*& dst_buf,
                            FX_DWORD& dst_len,
                            FX_DWORD& offset) {
  EncodeString(code_table[index_num].prefix, dst_buf, dst_len, offset);
  EncodeString(code_end, dst_buf, dst_len, offset);
  bit_offset = 0;
  ClearTable();
}
gif_decompress_struct_p _gif_create_decompress() {
  gif_decompress_struct_p gif_ptr =
      (gif_decompress_struct*)FX_Alloc(uint8_t, sizeof(gif_decompress_struct));
  if (gif_ptr == NULL) {
    return NULL;
  }
  FXSYS_memset(gif_ptr, 0, sizeof(gif_decompress_struct));
  gif_ptr->decode_status = GIF_D_STATUS_SIG;
  gif_ptr->img_ptr_arr_ptr = new CFX_ArrayTemplate<GifImage*>;
#ifdef GIF_SUPPORT_COMMENT_EXTENSION
  gif_ptr->cmt_data_ptr = new CFX_ByteString;
#endif
#ifdef GIF_SUPPORT_PLAIN_TEXT_EXTENSION
  gif_ptr->pt_ptr_arr_ptr = new CFX_ArrayTemplate<GifPlainText*>;
#endif
  return gif_ptr;
}
void _gif_destroy_decompress(gif_decompress_struct_pp gif_ptr_ptr) {
  if (gif_ptr_ptr == NULL || *gif_ptr_ptr == NULL) {
    return;
  }
  gif_decompress_struct_p gif_ptr = *gif_ptr_ptr;
  *gif_ptr_ptr = NULL;
  if (gif_ptr->global_pal_ptr != NULL) {
    FX_Free(gif_ptr->global_pal_ptr);
  }
  if (gif_ptr->img_decoder_ptr != NULL) {
    delete gif_ptr->img_decoder_ptr;
  }
  if (gif_ptr->img_ptr_arr_ptr != NULL) {
    int32_t size_img_arr = gif_ptr->img_ptr_arr_ptr->GetSize();
    for (int32_t i = 0; i < size_img_arr; i++) {
      GifImage* p = gif_ptr->img_ptr_arr_ptr->GetAt(i);
      if (p->image_info_ptr != NULL) {
        FX_Free(p->image_info_ptr);
      }
      if (p->image_gce_ptr != NULL) {
        FX_Free(p->image_gce_ptr);
      }
      if (p->image_row_buf != NULL) {
        FX_Free(p->image_row_buf);
      }
      if (p->local_pal_ptr != NULL &&
          p->local_pal_ptr != gif_ptr->global_pal_ptr) {
        FX_Free(p->local_pal_ptr);
      }
      FX_Free(p);
    }
    gif_ptr->img_ptr_arr_ptr->RemoveAll();
    delete gif_ptr->img_ptr_arr_ptr;
  }
#ifdef GIF_SUPPORT_APPLICATION_EXTENSION
  if (gif_ptr->app_data != NULL) {
    FX_Free(gif_ptr->app_data);
  }
#endif
#ifdef GIF_SUPPORT_COMMENT_EXTENSION
  if (gif_ptr->cmt_data_ptr != NULL) {
    delete gif_ptr->cmt_data_ptr;
  }
#endif
#ifdef GIF_SUPPORT_GRAPHIC_CONTROL_EXTENSION
  if (gif_ptr->gce_ptr != NULL) {
    FX_Free(gif_ptr->gce_ptr);
  }
#endif
#ifdef GIF_SUPPORT_PLAIN_TEXT_EXTENSION
  if (gif_ptr->pt_ptr_arr_ptr != NULL) {
    int32_t size_pt_arr = gif_ptr->pt_ptr_arr_ptr->GetSize();
    for (int32_t i = 0; i < size_pt_arr; i++) {
      GifPlainText* p = gif_ptr->pt_ptr_arr_ptr->GetAt(i);
      if (p->gce_ptr != NULL) {
        FX_Free(p->gce_ptr);
      }
      if (p->pte_ptr != NULL) {
        FX_Free(p->pte_ptr);
      }
      if (p->string_ptr != NULL) {
        delete p->string_ptr;
      }
    }
    gif_ptr->pt_ptr_arr_ptr->RemoveAll();
    delete gif_ptr->pt_ptr_arr_ptr;
  }
#endif
  FX_Free(gif_ptr);
}
gif_compress_struct_p _gif_create_compress() {
  gif_compress_struct_p gif_ptr =
      (gif_compress_struct*)FX_Alloc(uint8_t, sizeof(gif_compress_struct));
  if (gif_ptr == NULL) {
    return NULL;
  }
  FXSYS_memset(gif_ptr, 0, sizeof(gif_compress_struct));
  gif_ptr->img_encoder_ptr = new CGifLZWEncoder;
  gif_ptr->header_ptr = (GifHeader*)FX_Alloc(uint8_t, sizeof(GifHeader));
  if (gif_ptr->header_ptr == NULL) {
    delete (gif_ptr->img_encoder_ptr);
    FX_Free(gif_ptr);
    return NULL;
  }
  FXSYS_memcpy(gif_ptr->header_ptr->signature, GIF_SIGNATURE, 3);
  FXSYS_memcpy(gif_ptr->header_ptr->version, "89a", 3);
  gif_ptr->lsd_ptr = (GifLSD*)FX_Alloc(uint8_t, sizeof(GifLSD));
  if (gif_ptr->lsd_ptr == NULL) {
    FX_Free(gif_ptr->header_ptr);
    delete (gif_ptr->img_encoder_ptr);
    FX_Free(gif_ptr);
    return NULL;
  }
  FXSYS_memset(gif_ptr->lsd_ptr, 0, sizeof(GifLSD));
  gif_ptr->image_info_ptr =
      (GifImageInfo*)FX_Alloc(uint8_t, sizeof(GifImageInfo));
  if (gif_ptr->image_info_ptr == NULL) {
    FX_Free(gif_ptr->lsd_ptr);
    FX_Free(gif_ptr->header_ptr);
    delete (gif_ptr->img_encoder_ptr);
    FX_Free(gif_ptr);
    return NULL;
  }
  FXSYS_memset(gif_ptr->image_info_ptr, 0, sizeof(GifImageInfo));
#ifdef GIF_SUPPORT_APPLICATION_EXTENSION
  FXSYS_memcpy(gif_ptr->app_identify, "netscape", 8);
  FXSYS_memcpy(gif_ptr->app_authentication, "2.0", 3);
#endif
#ifdef GIF_SUPPORT_GRAPHIC_CONTROL_EXTENSION
  gif_ptr->gce_ptr = (GifGCE*)FX_Alloc(uint8_t, sizeof(GifGCE));
  if (gif_ptr->gce_ptr == NULL) {
    FX_Free(gif_ptr->image_info_ptr);
    FX_Free(gif_ptr->lsd_ptr);
    FX_Free(gif_ptr->header_ptr);
    delete (gif_ptr->img_encoder_ptr);
    FX_Free(gif_ptr);
    return NULL;
  }
#endif
#ifdef GIF_SUPPORT_PLAIN_TEXT_EXTENSION
  gif_ptr->pte_ptr = (GifPTE*)FX_Alloc(uint8_t, sizeof(GifPTE));
  if (gif_ptr->pte_ptr == NULL) {
    FX_Free(gif_ptr->gce_ptr);
    FX_Free(gif_ptr->image_info_ptr);
    FX_Free(gif_ptr->lsd_ptr);
    FX_Free(gif_ptr->header_ptr);
    delete (gif_ptr->img_encoder_ptr);
    FX_Free(gif_ptr);
    return NULL;
  }
  FXSYS_memset(gif_ptr->pte_ptr, 0, sizeof(GifPTE));
  gif_ptr->pte_ptr->block_size = 12;
#endif
  return gif_ptr;
}
void _gif_destroy_compress(gif_compress_struct_pp gif_ptr_ptr) {
  if (gif_ptr_ptr == NULL || *gif_ptr_ptr == NULL) {
    return;
  }
  gif_compress_struct_p gif_ptr = *gif_ptr_ptr;
  *gif_ptr_ptr = NULL;
  if (gif_ptr->header_ptr != NULL) {
    FX_Free(gif_ptr->header_ptr);
  }
  if (gif_ptr->lsd_ptr != NULL) {
    FX_Free(gif_ptr->lsd_ptr);
  }
  if (gif_ptr->global_pal != NULL) {
    FX_Free(gif_ptr->global_pal);
  }
  if (gif_ptr->image_info_ptr != NULL) {
    FX_Free(gif_ptr->image_info_ptr);
  }
  if (gif_ptr->local_pal != NULL) {
    FX_Free(gif_ptr->local_pal);
  }
  if (gif_ptr->img_encoder_ptr != NULL) {
    delete gif_ptr->img_encoder_ptr;
  }
#ifdef GIF_SUPPORT_APPLICATION_EXTENSION
  if (gif_ptr->app_data != NULL) {
    FX_Free(gif_ptr->app_data);
  }
#endif
#ifdef GIF_SUPPORT_GRAPHIC_CONTROL_EXTENSION
  if (gif_ptr->gce_ptr != NULL) {
    FX_Free(gif_ptr->gce_ptr);
  }
#endif
#ifdef GIF_SUPPORT_COMMENT_EXTENSION
  if (gif_ptr->cmt_data_ptr != NULL) {
    FX_Free(gif_ptr->cmt_data_ptr);
  }
#endif
#ifdef GIF_SUPPORT_PLAIN_TEXT_EXTENSION
  if (gif_ptr->pte_ptr != NULL) {
    FX_Free(gif_ptr->pte_ptr);
  }
#endif
  FX_Free(gif_ptr);
}
void _gif_error(gif_decompress_struct_p gif_ptr, const FX_CHAR* err_msg) {
  if (gif_ptr != NULL && gif_ptr->_gif_error_fn != NULL) {
    gif_ptr->_gif_error_fn(gif_ptr, err_msg);
  }
}
void _gif_warn(gif_decompress_struct_p gif_ptr, const FX_CHAR* err_msg) {}
int32_t _gif_read_header(gif_decompress_struct_p gif_ptr) {
  if (gif_ptr == NULL) {
    return 0;
  }
  FX_DWORD skip_size_org = gif_ptr->skip_size;
  ASSERT(sizeof(GifHeader) == 6);
  GifHeader* gif_header_ptr = NULL;
  if (_gif_read_data(gif_ptr, (uint8_t**)&gif_header_ptr, 6) == NULL) {
    return 2;
  }
  if (FXSYS_strncmp(gif_header_ptr->signature, GIF_SIGNATURE, 3) != 0 ||
      gif_header_ptr->version[0] != '8' || gif_header_ptr->version[2] != 'a') {
    _gif_error(gif_ptr, "Not A Gif Image");
    return 0;
  }
  ASSERT(sizeof(GifLSD) == 7);
  GifLSD* gif_lsd_ptr = NULL;
  if (_gif_read_data(gif_ptr, (uint8_t**)&gif_lsd_ptr, 7) == NULL) {
    gif_ptr->skip_size = skip_size_org;
    return 2;
  }
  if (((GifGF*)&gif_lsd_ptr->global_flag)->global_pal) {
    gif_ptr->global_pal_num = 2
                              << ((GifGF*)&gif_lsd_ptr->global_flag)->pal_bits;
    ASSERT(sizeof(GifPalette) == 3);
    int32_t global_pal_size = gif_ptr->global_pal_num * 3;
    uint8_t* global_pal_ptr = NULL;
    if (_gif_read_data(gif_ptr, &global_pal_ptr, global_pal_size) == NULL) {
      gif_ptr->skip_size = skip_size_org;
      return 2;
    }
    gif_ptr->global_sort_flag = ((GifGF*)&gif_lsd_ptr->global_flag)->sort_flag;
    gif_ptr->global_color_resolution =
        ((GifGF*)&gif_lsd_ptr->global_flag)->color_resolution;
    if (gif_ptr->global_pal_ptr != NULL) {
      FX_Free(gif_ptr->global_pal_ptr);
    }
    gif_ptr->global_pal_ptr = NULL;
    gif_ptr->global_pal_ptr = (GifPalette*)FX_Alloc(uint8_t, global_pal_size);
    GIF_PTR_NOT_NULL(gif_ptr->global_pal_ptr, gif_ptr);
    FXSYS_memcpy(gif_ptr->global_pal_ptr, global_pal_ptr, global_pal_size);
  }
  gif_ptr->width = (int)_GetWord_LSBFirst((uint8_t*)&gif_lsd_ptr->width);
  gif_ptr->height = (int)_GetWord_LSBFirst((uint8_t*)&gif_lsd_ptr->height);
  gif_ptr->bc_index = gif_lsd_ptr->bc_index;
  gif_ptr->pixel_aspect = gif_lsd_ptr->pixel_aspect;
  return 1;
}
int32_t _gif_get_frame(gif_decompress_struct_p gif_ptr) {
  if (gif_ptr == NULL) {
    return 0;
  }
  int32_t ret = 1;
  while (TRUE) {
    switch (gif_ptr->decode_status) {
      case GIF_D_STATUS_TAIL:
        return 1;
      case GIF_D_STATUS_SIG: {
        uint8_t* sig_ptr = NULL;
        if (_gif_read_data(gif_ptr, &sig_ptr, 1) == NULL) {
          return 2;
        }
        switch (*sig_ptr) {
          case GIF_SIG_EXTENSION:
            _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_EXT);
            continue;
          case GIF_SIG_IMAGE:
            _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_IMG_INFO);
            continue;
          case GIF_SIG_TRAILER:
            _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_TAIL);
            return 1;
          default:
            if (gif_ptr->avail_in) {
              _gif_warn(gif_ptr, "The Gif File has non_standard Tag!");
              _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_SIG);
              continue;
            }
            _gif_warn(gif_ptr, "The Gif File Doesn't have Trailer Tag!");
            return 1;
        }
      }
      case GIF_D_STATUS_EXT: {
        uint8_t* ext_ptr = NULL;
        if (_gif_read_data(gif_ptr, &ext_ptr, 1) == NULL) {
          return 2;
        }
        switch (*ext_ptr) {
#ifdef GIF_SUPPORT_APPLICATION_EXTENSION
          case GIF_BLOCK_AE:
            _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_EXT_AE);
            continue;
#endif
#ifdef GIF_SUPPORT_COMMENT_EXTENSION
          case GIF_BLOCK_CE:
            _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_EXT_CE);
            continue;
#endif
#ifdef GIF_SUPPORT_GRAPHIC_CONTROL_EXTENSION
          case GIF_BLOCK_GCE:
            _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_EXT_GCE);
            continue;
#endif
#ifdef GIF_SUPPORT_PLAIN_TEXT_EXTENSION
          case GIF_BLOCK_PTE:
            _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_EXT_PTE);
            continue;
#endif
          default: {
            int32_t status = GIF_D_STATUS_EXT_UNE;
#ifndef GIF_SUPPORT_PLAIN_TEXT_EXTENSION
            if (*ext_ptr == GIF_BLOCK_PTE) {
              status = GIF_D_STATUS_EXT_PTE;
            }
#endif
            _gif_save_decoding_status(gif_ptr, status);
            continue;
          }
        }
      }
      case GIF_D_STATUS_IMG_INFO: {
        ret = _gif_decode_image_info(gif_ptr);
        if (ret != 1) {
          return ret;
        }
        continue;
      }
      case GIF_D_STATUS_IMG_DATA: {
        uint8_t* data_size_ptr = NULL;
        uint8_t* data_ptr = NULL;
        FX_DWORD skip_size_org = gif_ptr->skip_size;
        if (_gif_read_data(gif_ptr, &data_size_ptr, 1) == NULL) {
          return 2;
        }
        while (*data_size_ptr != GIF_BLOCK_TERMINAL) {
          if (_gif_read_data(gif_ptr, &data_ptr, *data_size_ptr) == NULL) {
            gif_ptr->skip_size = skip_size_org;
            return 2;
          }
          _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_IMG_DATA);
          skip_size_org = gif_ptr->skip_size;
          if (_gif_read_data(gif_ptr, &data_size_ptr, 1) == NULL) {
            return 2;
          }
        }
        _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_SIG);
        continue;
      }
      default: {
        ret = _gif_decode_extension(gif_ptr);
        if (ret != 1) {
          return ret;
        }
        continue;
      }
    }
  }
  return 1;
}
void _gif_takeover_gce_ptr(gif_decompress_struct_p gif_ptr,
                           GifGCE** gce_ptr_ptr) {
  *gce_ptr_ptr = NULL;
#ifdef GIF_SUPPORT_GRAPHIC_CONTROL_EXTENSION
  if (gif_ptr->gce_ptr != NULL && gce_ptr_ptr != NULL) {
    *gce_ptr_ptr = gif_ptr->gce_ptr;
    gif_ptr->gce_ptr = NULL;
  }
#endif
}
int32_t _gif_decode_extension(gif_decompress_struct_p gif_ptr) {
  uint8_t* data_size_ptr = NULL;
  uint8_t* data_ptr = NULL;
  FX_DWORD skip_size_org = gif_ptr->skip_size;
  switch (gif_ptr->decode_status) {
#ifdef GIF_SUPPORT_APPLICATION_EXTENSION
    case GIF_D_STATUS_EXT_AE: {
      ASSERT(sizeof(GifAE) == 12);
      GifAE* gif_ae_ptr = NULL;
      if (_gif_read_data(gif_ptr, (uint8_t**)&gif_ae_ptr, 12) == NULL) {
        return 2;
      }
      CFX_ByteString gif_ae_data_str;
      if (_gif_read_data(gif_ptr, &data_size_ptr, 1) == NULL) {
        gif_ptr->skip_size = skip_size_org;
        return 2;
      }
      while (*data_size_ptr != GIF_BLOCK_TERMINAL) {
        uint8_t data_size = *data_size_ptr;
        if (_gif_read_data(gif_ptr, &data_ptr, *data_size_ptr) == NULL ||
            _gif_read_data(gif_ptr, &data_size_ptr, 1) == NULL) {
          gif_ptr->skip_size = skip_size_org;
          return 2;
        }
        gif_ae_data_str += CFX_ByteString((const uint8_t*)data_ptr, data_size);
      }
      FXSYS_memcpy(gif_ptr->app_identify, gif_ae_ptr->app_identify, 8);
      FXSYS_memcpy(gif_ptr->app_authentication, gif_ae_ptr->app_authentication,
                   3);
      gif_ptr->app_data_size = gif_ae_data_str.GetLength();
      if (gif_ptr->app_data != NULL) {
        FX_Free(gif_ptr->app_data);
        gif_ptr->app_data = NULL;
      }
      gif_ptr->app_data = FX_Alloc(uint8_t, gif_ptr->app_data_size);
      GIF_PTR_NOT_NULL(gif_ptr->app_data, gif_ptr);
      FXSYS_memcpy(gif_ptr->app_data, const uint8_t*(gif_ae_data_str),
                   gif_ptr->app_data_size);
    } break;
#endif
#ifdef GIF_SUPPORT_COMMENT_EXTENSION
    case GIF_D_STATUS_EXT_CE: {
      if (_gif_read_data(gif_ptr, &data_size_ptr, 1) == NULL) {
        gif_ptr->skip_size = skip_size_org;
        return 2;
      }
      gif_ptr->cmt_data_ptr->Empty();
      while (*data_size_ptr != GIF_BLOCK_TERMINAL) {
        uint8_t data_size = *data_size_ptr;
        if (_gif_read_data(gif_ptr, &data_ptr, *data_size_ptr) == NULL ||
            _gif_read_data(gif_ptr, &data_size_ptr, 1) == NULL) {
          gif_ptr->skip_size = skip_size_org;
          return 2;
        }
        *(gif_ptr->cmt_data_ptr) +=
            CFX_ByteString((const FX_CHAR*)data_ptr, data_size);
      }
    } break;
#endif
#ifdef GIF_SUPPORT_PLAIN_TEXT_EXTENSION
    case GIF_D_STATUS_EXT_PTE: {
      ASSERT(sizeof(GifPTE) == 13);
      GifPTE* gif_pte_ptr = NULL;
      if (_gif_read_data(gif_ptr, (uint8_t**)&gif_pte_ptr, 13) == NULL) {
        return 2;
      }
      GifPlainText* gif_pt_ptr = FX_Alloc(GifPlainText, 1);
      GIF_PTR_NOT_NULL(gif_pt_ptr, gif_ptr);
      FXSYS_memset(gif_pt_ptr, 0, sizeof(GifPlainText));
      _gif_takeover_gce_ptr(gif_ptr, &gif_pt_ptr->gce_ptr);
      gif_pt_ptr->pte_ptr = (GifPTE*)FX_Alloc(uint8_t, sizeof(GifPTE));
      GIF_PTR_NOT_NULL(gif_pt_ptr->pte_ptr, gif_ptr);
      gif_pt_ptr->string_ptr = new CFX_ByteString;
      GIF_PTR_NOT_NULL(gif_pt_ptr->string_ptr, gif_ptr);
      gif_pt_ptr->pte_ptr->block_size = gif_pte_ptr->block_size;
      gif_pt_ptr->pte_ptr->grid_left =
          _GetWord_LSBFirst((uint8_t*)&gif_pte_ptr->grid_left);
      gif_pt_ptr->pte_ptr->grid_top =
          _GetWord_LSBFirst((uint8_t*)&gif_pte_ptr->grid_top);
      gif_pt_ptr->pte_ptr->grid_width =
          _GetWord_LSBFirst((uint8_t*)&gif_pte_ptr->grid_width);
      gif_pt_ptr->pte_ptr->grid_height =
          _GetWord_LSBFirst((uint8_t*)&gif_pte_ptr->grid_height);
      gif_pt_ptr->pte_ptr->char_width = gif_pte_ptr->char_width;
      gif_pt_ptr->pte_ptr->char_height = gif_pte_ptr->char_height;
      gif_pt_ptr->pte_ptr->fc_index = gif_pte_ptr->fc_index;
      gif_pt_ptr->pte_ptr->bc_index = gif_pte_ptr->bc_index;
      if (_gif_read_data(gif_ptr, &data_size_ptr, 1) == NULL) {
        gif_ptr->skip_size = skip_size_org;
        if (gif_pt_ptr != NULL) {
          if (gif_pt_ptr->gce_ptr != NULL) {
            FX_Free(gif_pt_ptr->gce_ptr);
          }
          if (gif_pt_ptr->pte_ptr != NULL) {
            FX_Free(gif_pt_ptr->pte_ptr);
          }
          if (gif_pt_ptr->string_ptr != NULL) {
            delete gif_pt_ptr->string_ptr;
          }
          FX_Free(gif_pt_ptr);
        }
        return 2;
      }
      while (*data_size_ptr != GIF_BLOCK_TERMINAL) {
        uint8_t data_size = *data_size_ptr;
        if (_gif_read_data(gif_ptr, &data_ptr, *data_size_ptr) == NULL ||
            _gif_read_data(gif_ptr, &data_size_ptr, 1) == NULL) {
          gif_ptr->skip_size = skip_size_org;
          if (gif_pt_ptr != NULL) {
            if (gif_pt_ptr->gce_ptr != NULL) {
              FX_Free(gif_pt_ptr->gce_ptr);
            }
            if (gif_pt_ptr->pte_ptr != NULL) {
              FX_Free(gif_pt_ptr->pte_ptr);
            }
            if (gif_pt_ptr->string_ptr != NULL) {
              delete gif_pt_ptr->string_ptr;
            }
            FX_Free(gif_pt_ptr);
          }
          return 2;
        }
        *(gif_pt_ptr->string_ptr) +=
            CFX_ByteString((const FX_CHAR*)data_ptr, data_size);
      }
      gif_ptr->pt_ptr_arr_ptr->Add(gif_pt_ptr);
    } break;
#endif
#ifdef GIF_SUPPORT_GRAPHIC_CONTROL_EXTENSION
    case GIF_D_STATUS_EXT_GCE: {
      ASSERT(sizeof(GifGCE) == 5);
      GifGCE* gif_gce_ptr = NULL;
      if (_gif_read_data(gif_ptr, (uint8_t**)&gif_gce_ptr, 6) == NULL) {
        return 2;
      }
      if (gif_ptr->gce_ptr == NULL) {
        gif_ptr->gce_ptr = (GifGCE*)FX_Alloc(uint8_t, sizeof(GifGCE));
        GIF_PTR_NOT_NULL(gif_ptr->gce_ptr, gif_ptr);
      }
      gif_ptr->gce_ptr->block_size = gif_gce_ptr->block_size;
      gif_ptr->gce_ptr->gce_flag = gif_gce_ptr->gce_flag;
      gif_ptr->gce_ptr->delay_time =
          _GetWord_LSBFirst((uint8_t*)&gif_gce_ptr->delay_time);
      gif_ptr->gce_ptr->trans_index = gif_gce_ptr->trans_index;
    } break;
#endif
    default: {
#ifndef GIF_SUPPORT_PLAIN_TEXT_EXTENSION
      if (gif_ptr->decode_status == GIF_D_STATUS_EXT_PTE) {
#ifdef GIF_SUPPORT_GRAPHIC_CONTROL_EXTENSION
        if (gif_ptr->gce_ptr != NULL) {
          FX_Free(gif_ptr->gce_ptr);
          gif_ptr->gce_ptr = NULL;
        }
#endif
      }
#endif
      if (_gif_read_data(gif_ptr, &data_size_ptr, 1) == NULL) {
        return 2;
      }
      while (*data_size_ptr != GIF_BLOCK_TERMINAL) {
        if (_gif_read_data(gif_ptr, &data_ptr, *data_size_ptr) == NULL ||
            _gif_read_data(gif_ptr, &data_size_ptr, 1) == NULL) {
          gif_ptr->skip_size = skip_size_org;
          return 2;
        }
      }
    }
  }
  _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_SIG);
  return 1;
}
int32_t _gif_decode_image_info(gif_decompress_struct_p gif_ptr) {
  if (gif_ptr->width == 0 || gif_ptr->height == 0) {
    _gif_error(gif_ptr, "No Image Header Info");
    return 0;
  }
  FX_DWORD skip_size_org = gif_ptr->skip_size;
  ASSERT(sizeof(GifImageInfo) == 9);
  GifImageInfo* gif_img_info_ptr = NULL;
  if (_gif_read_data(gif_ptr, (uint8_t**)&gif_img_info_ptr, 9) == NULL) {
    return 2;
  }
  GifImage* gif_image_ptr = (GifImage*)FX_Alloc(uint8_t, sizeof(GifImage));
  GIF_PTR_NOT_NULL(gif_image_ptr, gif_ptr);
  FXSYS_memset(gif_image_ptr, 0, sizeof(GifImage));
  gif_image_ptr->image_info_ptr =
      (GifImageInfo*)FX_Alloc(uint8_t, sizeof(GifImageInfo));
  GIF_PTR_NOT_NULL(gif_image_ptr->image_info_ptr, gif_ptr);
  gif_image_ptr->image_info_ptr->left =
      _GetWord_LSBFirst((uint8_t*)&gif_img_info_ptr->left);
  gif_image_ptr->image_info_ptr->top =
      _GetWord_LSBFirst((uint8_t*)&gif_img_info_ptr->top);
  gif_image_ptr->image_info_ptr->width =
      _GetWord_LSBFirst((uint8_t*)&gif_img_info_ptr->width);
  gif_image_ptr->image_info_ptr->height =
      _GetWord_LSBFirst((uint8_t*)&gif_img_info_ptr->height);
  gif_image_ptr->image_info_ptr->local_flag = gif_img_info_ptr->local_flag;
  if (gif_image_ptr->image_info_ptr->left +
              gif_image_ptr->image_info_ptr->width >
          gif_ptr->width ||
      gif_image_ptr->image_info_ptr->top +
              gif_image_ptr->image_info_ptr->height >
          gif_ptr->height) {
    if (gif_image_ptr->image_info_ptr != NULL) {
      FX_Free(gif_image_ptr->image_info_ptr);
    }
    if (gif_image_ptr->image_row_buf != NULL) {
      FX_Free(gif_image_ptr->image_row_buf);
    }
    FX_Free(gif_image_ptr);
    _gif_error(gif_ptr, "Image Data Out Of LSD, The File May Be Corrupt");
    return 0;
  }
  GifLF* gif_img_info_lf_ptr = (GifLF*)&gif_img_info_ptr->local_flag;
  if (gif_img_info_lf_ptr->local_pal) {
    ASSERT(sizeof(GifPalette) == 3);
    int32_t loc_pal_size = (2 << gif_img_info_lf_ptr->pal_bits) * 3;
    uint8_t* loc_pal_ptr = NULL;
    if (_gif_read_data(gif_ptr, &loc_pal_ptr, loc_pal_size) == NULL) {
      gif_ptr->skip_size = skip_size_org;
      if (gif_image_ptr->image_info_ptr != NULL) {
        FX_Free(gif_image_ptr->image_info_ptr);
      }
      if (gif_image_ptr->image_row_buf != NULL) {
        FX_Free(gif_image_ptr->image_row_buf);
      }
      FX_Free(gif_image_ptr);
      return 2;
    }
    gif_image_ptr->local_pal_ptr =
        (GifPalette*)gif_ptr->_gif_ask_buf_for_pal_fn(gif_ptr, loc_pal_size);
    if (gif_image_ptr->local_pal_ptr != NULL) {
      FXSYS_memcpy((uint8_t*)gif_image_ptr->local_pal_ptr, loc_pal_ptr,
                   loc_pal_size);
    }
  }
  uint8_t* code_size_ptr = NULL;
  if (_gif_read_data(gif_ptr, &code_size_ptr, 1) == NULL) {
    gif_ptr->skip_size = skip_size_org;
    if (gif_image_ptr->image_info_ptr != NULL) {
      FX_Free(gif_image_ptr->image_info_ptr);
    }
    if (gif_image_ptr->local_pal_ptr != NULL) {
      FX_Free(gif_image_ptr->local_pal_ptr);
    }
    if (gif_image_ptr->image_row_buf != NULL) {
      FX_Free(gif_image_ptr->image_row_buf);
    }
    FX_Free(gif_image_ptr);
    return 2;
  }
  gif_image_ptr->image_code_size = *code_size_ptr;
  gif_ptr->_gif_record_current_position_fn(gif_ptr,
                                           &gif_image_ptr->image_data_pos);
  gif_image_ptr->image_data_pos += gif_ptr->skip_size;
  _gif_takeover_gce_ptr(gif_ptr, &gif_image_ptr->image_gce_ptr);
  gif_ptr->img_ptr_arr_ptr->Add(gif_image_ptr);
  _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_IMG_DATA);
  return 1;
}
int32_t _gif_load_frame(gif_decompress_struct_p gif_ptr, int32_t frame_num) {
  if (gif_ptr == NULL || frame_num < 0 ||
      frame_num >= gif_ptr->img_ptr_arr_ptr->GetSize()) {
    return 0;
  }
  uint8_t* data_size_ptr = NULL;
  uint8_t* data_ptr = NULL;
  FX_DWORD skip_size_org = gif_ptr->skip_size;
  GifImage* gif_image_ptr = gif_ptr->img_ptr_arr_ptr->GetAt(frame_num);
  FX_DWORD gif_img_row_bytes = gif_image_ptr->image_info_ptr->width;
  if (gif_ptr->decode_status == GIF_D_STATUS_TAIL) {
    if (gif_image_ptr->image_row_buf) {
      FX_Free(gif_image_ptr->image_row_buf);
      gif_image_ptr->image_row_buf = NULL;
    }
    gif_image_ptr->image_row_buf = FX_Alloc(uint8_t, gif_img_row_bytes);
    GIF_PTR_NOT_NULL(gif_image_ptr->image_row_buf, gif_ptr);
    GifGCE* gif_img_gce_ptr = gif_image_ptr->image_gce_ptr;
    int32_t loc_pal_num =
        ((GifLF*)&gif_image_ptr->image_info_ptr->local_flag)->local_pal
            ? (2 << ((GifLF*)&gif_image_ptr->image_info_ptr->local_flag)
                        ->pal_bits)
            : 0;
    gif_ptr->avail_in = 0;
    if (gif_img_gce_ptr == NULL) {
      FX_BOOL bRes = gif_ptr->_gif_get_record_position_fn(
          gif_ptr, gif_image_ptr->image_data_pos,
          gif_image_ptr->image_info_ptr->left,
          gif_image_ptr->image_info_ptr->top,
          gif_image_ptr->image_info_ptr->width,
          gif_image_ptr->image_info_ptr->height, loc_pal_num,
          gif_image_ptr->local_pal_ptr, 0, 0, -1, 0,
          (FX_BOOL)((GifLF*)&gif_image_ptr->image_info_ptr->local_flag)
              ->interlace);
      if (!bRes) {
        FX_Free(gif_image_ptr->image_row_buf);
        gif_image_ptr->image_row_buf = NULL;
        _gif_error(gif_ptr, "Error Read Record Position Data");
        return 0;
      }
    } else {
      FX_BOOL bRes = gif_ptr->_gif_get_record_position_fn(
          gif_ptr, gif_image_ptr->image_data_pos,
          gif_image_ptr->image_info_ptr->left,
          gif_image_ptr->image_info_ptr->top,
          gif_image_ptr->image_info_ptr->width,
          gif_image_ptr->image_info_ptr->height, loc_pal_num,
          gif_image_ptr->local_pal_ptr,
          (int32_t)gif_image_ptr->image_gce_ptr->delay_time,
          (FX_BOOL)((GifCEF*)&gif_image_ptr->image_gce_ptr->gce_flag)
              ->user_input,
          ((GifCEF*)&gif_image_ptr->image_gce_ptr->gce_flag)->transparency
              ? (int32_t)gif_image_ptr->image_gce_ptr->trans_index
              : -1,
          (int32_t)((GifCEF*)&gif_image_ptr->image_gce_ptr->gce_flag)
              ->disposal_method,
          (FX_BOOL)((GifLF*)&gif_image_ptr->image_info_ptr->local_flag)
              ->interlace);
      if (!bRes) {
        FX_Free(gif_image_ptr->image_row_buf);
        gif_image_ptr->image_row_buf = NULL;
        _gif_error(gif_ptr, "Error Read Record Position Data");
        return 0;
      }
    }
    if (gif_ptr->img_decoder_ptr == NULL) {
      gif_ptr->img_decoder_ptr = new CGifLZWDecoder(gif_ptr->err_ptr);
      GIF_PTR_NOT_NULL(gif_ptr->img_decoder_ptr, gif_ptr);
    }
    gif_ptr->img_decoder_ptr->InitTable(gif_image_ptr->image_code_size);
    gif_ptr->img_row_offset = 0;
    gif_ptr->img_row_avail_size = 0;
    gif_ptr->img_pass_num = 0;
    gif_image_ptr->image_row_num = 0;
    _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_IMG_DATA);
  }
  CGifLZWDecoder* img_decoder_ptr = gif_ptr->img_decoder_ptr;
  if (gif_ptr->decode_status == GIF_D_STATUS_IMG_DATA) {
    if (_gif_read_data(gif_ptr, &data_size_ptr, 1) == NULL) {
      return 2;
    }
    if (*data_size_ptr != GIF_BLOCK_TERMINAL) {
      if (_gif_read_data(gif_ptr, &data_ptr, *data_size_ptr) == NULL) {
        gif_ptr->skip_size = skip_size_org;
        return 2;
      }
      img_decoder_ptr->Input(data_ptr, *data_size_ptr);
      _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_IMG_DATA);
      gif_ptr->img_row_offset += gif_ptr->img_row_avail_size;
      gif_ptr->img_row_avail_size = gif_img_row_bytes - gif_ptr->img_row_offset;
      int32_t ret = img_decoder_ptr->Decode(
          gif_image_ptr->image_row_buf + gif_ptr->img_row_offset,
          gif_ptr->img_row_avail_size);
      if (ret == 0) {
        FX_Free(gif_image_ptr->image_row_buf);
        gif_image_ptr->image_row_buf = NULL;
        _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_TAIL);
        _gif_error(gif_ptr, "Decode Image Data Error");
        return 0;
      }
      while (ret != 0) {
        if (ret == 1) {
          gif_ptr->_gif_get_row_fn(gif_ptr, gif_image_ptr->image_row_num,
                                   gif_image_ptr->image_row_buf);
          FX_Free(gif_image_ptr->image_row_buf);
          gif_image_ptr->image_row_buf = NULL;
          _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_TAIL);
          return 1;
        }
        if (ret == 2) {
          ASSERT(img_decoder_ptr->GetAvailInput() == 0);
          skip_size_org = gif_ptr->skip_size;
          if (_gif_read_data(gif_ptr, &data_size_ptr, 1) == NULL) {
            return 2;
          }
          if (*data_size_ptr != GIF_BLOCK_TERMINAL) {
            if (_gif_read_data(gif_ptr, &data_ptr, *data_size_ptr) == NULL) {
              gif_ptr->skip_size = skip_size_org;
              return 2;
            }
            img_decoder_ptr->Input(data_ptr, *data_size_ptr);
            _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_IMG_DATA);
            gif_ptr->img_row_offset += gif_ptr->img_row_avail_size;
            gif_ptr->img_row_avail_size =
                gif_img_row_bytes - gif_ptr->img_row_offset;
            ret = img_decoder_ptr->Decode(
                gif_image_ptr->image_row_buf + gif_ptr->img_row_offset,
                gif_ptr->img_row_avail_size);
          }
        }
        if (ret == 3) {
          if (((GifLF*)&gif_image_ptr->image_info_ptr->local_flag)->interlace) {
            gif_ptr->_gif_get_row_fn(gif_ptr, gif_image_ptr->image_row_num,
                                     gif_image_ptr->image_row_buf);
            gif_image_ptr->image_row_num +=
                s_gif_interlace_step[gif_ptr->img_pass_num];
            if (gif_image_ptr->image_row_num >=
                (int32_t)gif_image_ptr->image_info_ptr->height) {
              gif_ptr->img_pass_num++;
              gif_image_ptr->image_row_num =
                  s_gif_interlace_step[gif_ptr->img_pass_num] / 2;
            }
          } else {
            gif_ptr->_gif_get_row_fn(gif_ptr, gif_image_ptr->image_row_num++,
                                     gif_image_ptr->image_row_buf);
          }
          gif_ptr->img_row_offset = 0;
          gif_ptr->img_row_avail_size = gif_img_row_bytes;
          ret = img_decoder_ptr->Decode(
              gif_image_ptr->image_row_buf + gif_ptr->img_row_offset,
              gif_ptr->img_row_avail_size);
        }
        if (ret == 0) {
          FX_Free(gif_image_ptr->image_row_buf);
          gif_image_ptr->image_row_buf = NULL;
          _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_TAIL);
          _gif_error(gif_ptr, "Decode Image Data Error");
          return 0;
        }
      }
    }
    _gif_save_decoding_status(gif_ptr, GIF_D_STATUS_TAIL);
  }
  _gif_error(gif_ptr, "Decode Image Data Error");
  return 0;
}
void _gif_save_decoding_status(gif_decompress_struct_p gif_ptr,
                               int32_t status) {
  gif_ptr->decode_status = status;
  gif_ptr->next_in += gif_ptr->skip_size;
  gif_ptr->avail_in -= gif_ptr->skip_size;
  gif_ptr->skip_size = 0;
}
uint8_t* _gif_read_data(gif_decompress_struct_p gif_ptr,
                        uint8_t** des_buf_pp,
                        FX_DWORD data_size) {
  if (gif_ptr == NULL || gif_ptr->avail_in < gif_ptr->skip_size + data_size) {
    return NULL;
  }
  *des_buf_pp = gif_ptr->next_in + gif_ptr->skip_size;
  gif_ptr->skip_size += data_size;
  return *des_buf_pp;
}
void _gif_input_buffer(gif_decompress_struct_p gif_ptr,
                       uint8_t* src_buf,
                       FX_DWORD src_size) {
  gif_ptr->next_in = src_buf;
  gif_ptr->avail_in = src_size;
  gif_ptr->skip_size = 0;
}
FX_DWORD _gif_get_avail_input(gif_decompress_struct_p gif_ptr,
                              uint8_t** avial_buf_ptr) {
  if (avial_buf_ptr != NULL) {
    *avial_buf_ptr = NULL;
    if (gif_ptr->avail_in > 0) {
      *avial_buf_ptr = gif_ptr->next_in;
    }
  }
  return gif_ptr->avail_in;
}
int32_t _gif_get_frame_num(gif_decompress_struct_p gif_ptr) {
  return gif_ptr->img_ptr_arr_ptr->GetSize();
}
static FX_BOOL _gif_write_header(gif_compress_struct_p gif_ptr,
                                 uint8_t*& dst_buf,
                                 FX_DWORD& dst_len) {
  if (gif_ptr->cur_offset) {
    return TRUE;
  }
  dst_len = sizeof(GifHeader) + sizeof(GifLSD) + sizeof(GifGF);
  dst_buf = FX_TryAlloc(uint8_t, dst_len);
  if (dst_buf == NULL) {
    return FALSE;
  }
  FXSYS_memset(dst_buf, 0, dst_len);
  FXSYS_memcpy(dst_buf, gif_ptr->header_ptr, sizeof(GifHeader));
  gif_ptr->cur_offset += sizeof(GifHeader);
  _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset, gif_ptr->lsd_ptr->width);
  gif_ptr->cur_offset += 2;
  _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset, gif_ptr->lsd_ptr->height);
  gif_ptr->cur_offset += 2;
  dst_buf[gif_ptr->cur_offset++] = gif_ptr->lsd_ptr->global_flag;
  dst_buf[gif_ptr->cur_offset++] = gif_ptr->lsd_ptr->bc_index;
  dst_buf[gif_ptr->cur_offset++] = gif_ptr->lsd_ptr->pixel_aspect;
  if (gif_ptr->global_pal) {
    FX_WORD size = sizeof(GifPalette) * gif_ptr->gpal_num;
    if (!_gif_grow_buf(dst_buf, dst_len, gif_ptr->cur_offset + size)) {
      return FALSE;
    }
    FXSYS_memcpy(&dst_buf[gif_ptr->cur_offset], gif_ptr->global_pal, size);
    gif_ptr->cur_offset += size;
  }
  return TRUE;
}
void interlace_buf(const uint8_t* buf, FX_DWORD pitch, FX_DWORD height) {
  CFX_ArrayTemplate<uint8_t*> pass[4];
  int i, j;
  FX_DWORD row;
  row = 0;
  uint8_t* temp;
  while (row < height) {
    if (row % 8 == 0) {
      j = 0;
    } else if (row % 4 == 0) {
      j = 1;
    } else if (row % 2 == 0) {
      j = 2;
    } else {
      j = 3;
    }
    temp = FX_Alloc(uint8_t, pitch);
    if (temp == NULL) {
      return;
    }
    FXSYS_memcpy(temp, &buf[pitch * row], pitch);
    pass[j].Add(temp);
    row++;
  }
  for (i = 0, row = 0; i < 4; i++) {
    for (j = 0; j < pass[i].GetSize(); j++, row++) {
      FXSYS_memcpy((uint8_t*)&buf[pitch * row], pass[i].GetAt(j), pitch);
      FX_Free(pass[i].GetAt(j));
    }
  }
}
static void _gif_write_block_data(const uint8_t* src_buf,
                                  FX_DWORD src_len,
                                  uint8_t*& dst_buf,
                                  FX_DWORD& dst_len,
                                  FX_DWORD& dst_offset) {
  FX_DWORD src_offset = 0;
  while (src_len > GIF_DATA_BLOCK) {
    dst_buf[dst_offset++] = GIF_DATA_BLOCK;
    FXSYS_memcpy(&dst_buf[dst_offset], &src_buf[src_offset], GIF_DATA_BLOCK);
    dst_offset += GIF_DATA_BLOCK;
    src_offset += GIF_DATA_BLOCK;
    src_len -= GIF_DATA_BLOCK;
  }
  dst_buf[dst_offset++] = (uint8_t)src_len;
  FXSYS_memcpy(&dst_buf[dst_offset], &src_buf[src_offset], src_len);
  dst_offset += src_len;
}
static FX_BOOL _gif_write_data(gif_compress_struct_p gif_ptr,
                               uint8_t*& dst_buf,
                               FX_DWORD& dst_len) {
  if (!_gif_grow_buf(dst_buf, dst_len, gif_ptr->cur_offset + GIF_DATA_BLOCK)) {
    return FALSE;
  }
#ifdef GIF_SUPPORT_GRAPHIC_CONTROL_EXTENSION
  if (FXSYS_memcmp(gif_ptr->header_ptr->version, "89a", 3) == 0) {
    dst_buf[gif_ptr->cur_offset++] = GIF_SIG_EXTENSION;
    dst_buf[gif_ptr->cur_offset++] = GIF_BLOCK_GCE;
    gif_ptr->gce_ptr->block_size = 4;
    dst_buf[gif_ptr->cur_offset++] = gif_ptr->gce_ptr->block_size;
    gif_ptr->gce_ptr->gce_flag = 0;
    dst_buf[gif_ptr->cur_offset++] = gif_ptr->gce_ptr->gce_flag;
    gif_ptr->gce_ptr->delay_time = 10;
    _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset,
                      gif_ptr->gce_ptr->delay_time);
    gif_ptr->cur_offset += 2;
    gif_ptr->gce_ptr->trans_index = 0;
    dst_buf[gif_ptr->cur_offset++] = gif_ptr->gce_ptr->trans_index;
    dst_buf[gif_ptr->cur_offset++] = 0;
  }
#endif
  dst_buf[gif_ptr->cur_offset++] = GIF_SIG_IMAGE;
  _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset,
                    gif_ptr->image_info_ptr->left);
  gif_ptr->cur_offset += 2;
  _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset,
                    gif_ptr->image_info_ptr->top);
  gif_ptr->cur_offset += 2;
  _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset,
                    gif_ptr->image_info_ptr->width);
  gif_ptr->cur_offset += 2;
  _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset,
                    gif_ptr->image_info_ptr->height);
  gif_ptr->cur_offset += 2;
  GifLF& lf = (GifLF&)gif_ptr->image_info_ptr->local_flag;
  dst_buf[gif_ptr->cur_offset++] = gif_ptr->image_info_ptr->local_flag;
  if (gif_ptr->local_pal) {
    FX_DWORD pal_size = sizeof(GifPalette) * gif_ptr->lpal_num;
    if (!_gif_grow_buf(dst_buf, dst_len, pal_size + gif_ptr->cur_offset)) {
      return FALSE;
    }
    FXSYS_memcpy(&dst_buf[gif_ptr->cur_offset], gif_ptr->local_pal, pal_size);
    gif_ptr->cur_offset += pal_size;
  }
  if (lf.interlace) {
    interlace_buf(gif_ptr->src_buf, gif_ptr->src_pitch,
                  gif_ptr->image_info_ptr->height);
  }
  uint8_t code_bit = lf.pal_bits;
  if (lf.local_pal == 0) {
    GifGF& gf = (GifGF&)gif_ptr->lsd_ptr->global_flag;
    code_bit = gf.pal_bits;
  }
  gif_ptr->img_encoder_ptr->Start(code_bit, gif_ptr->src_buf, dst_buf,
                                  gif_ptr->cur_offset);
  FX_DWORD i;
  for (i = 0; i < gif_ptr->src_row; i++) {
    if (!gif_ptr->img_encoder_ptr->Encode(
            &gif_ptr->src_buf[i * gif_ptr->src_pitch],
            gif_ptr->src_width * (code_bit + 1), dst_buf, dst_len,
            gif_ptr->cur_offset)) {
      return FALSE;
    }
  }
  gif_ptr->img_encoder_ptr->Finish(dst_buf, dst_len, gif_ptr->cur_offset);
  dst_buf[gif_ptr->cur_offset++] = 0;
#ifdef GIF_SUPPORT_COMMENT_EXTENSION
  if (FXSYS_memcmp(gif_ptr->header_ptr->version, "89a", 3) == 0 &&
      gif_ptr->cmt_data_ptr) {
    dst_buf[gif_ptr->cur_offset++] = GIF_SIG_EXTENSION;
    dst_buf[gif_ptr->cur_offset++] = GIF_BLOCK_CE;
    _gif_write_block_data(gif_ptr->cmt_data_ptr, gif_ptr->cmt_data_len, dst_buf,
                          dst_len, gif_ptr->cur_offset);
    dst_buf[gif_ptr->cur_offset++] = 0;
  }
#endif
#ifdef GIF_SUPPORT_PLAIN_TEXT_EXTENSION
  if (FXSYS_memcmp(gif_ptr->header_ptr->version, "89a", 3) == 0 &&
      gif_ptr->pte_data_ptr) {
    dst_buf[gif_ptr->cur_offset++] = GIF_SIG_EXTENSION;
    dst_buf[gif_ptr->cur_offset++] = GIF_BLOCK_PTE;
    dst_buf[gif_ptr->cur_offset++] = gif_ptr->pte_ptr->block_size;
    _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset,
                      gif_ptr->pte_ptr->grid_left);
    gif_ptr->cur_offset += 2;
    _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset,
                      gif_ptr->pte_ptr->grid_top);
    gif_ptr->cur_offset += 2;
    _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset,
                      gif_ptr->pte_ptr->grid_width);
    gif_ptr->cur_offset += 2;
    _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset,
                      gif_ptr->pte_ptr->grid_height);
    gif_ptr->cur_offset += 2;
    _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset,
                      gif_ptr->pte_ptr->char_width);
    gif_ptr->cur_offset += 2;
    _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset,
                      gif_ptr->pte_ptr->char_height);
    gif_ptr->cur_offset += 2;
    _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset,
                      gif_ptr->pte_ptr->fc_index);
    gif_ptr->cur_offset += 2;
    _SetWord_LSBFirst(dst_buf + gif_ptr->cur_offset,
                      gif_ptr->pte_ptr->bc_index);
    gif_ptr->cur_offset += 2;
    _gif_write_block_data(gif_ptr->pte_data_ptr, gif_ptr->pte_data_len, dst_buf,
                          dst_len, gif_ptr->cur_offset);
    gif_ptr->cur_offset += gif_ptr->pte_data_len;
    dst_buf[gif_ptr->cur_offset++] = 0;
  }
#endif
#ifdef GIF_SUPPORT_APPLICATION_EXTENSION
  if (FXSYS_memcmp(gif_ptr->header_ptr->version, "89a", 3) == 0 &&
      gif_ptr->app_data) {
    dst_buf[gif_ptr->cur_offset++] = GIF_SIG_EXTENSION;
    dst_buf[gif_ptr->cur_offset++] = GIF_BLOCK_AE;
    dst_buf[gif_ptr->cur_offset++] = 11;
    FXSYS_memcpy(&dst_buf[gif_ptr->cur_offset], gif_ptr->app_identify, 8);
    gif_ptr->cur_offset += 8;
    FXSYS_memcpy(&dst_buf[gif_ptr->cur_offset], gif_ptr->app_authentication, 8);
    gif_ptr->cur_offset += 3;
    FXSYS_memcpy(&dst_buf[gif_ptr->cur_offset], gif_ptr->app_data,
                 gif_ptr->app_data_size);
    gif_ptr->cur_offset += gif_ptr->app_data_size;
    dst_buf[gif_ptr->cur_offset++] = 0;
  }
#endif
  dst_buf[gif_ptr->cur_offset++] = GIF_SIG_TRAILER;
  return TRUE;
}
FX_BOOL _gif_encode(gif_compress_struct_p gif_ptr,
                    uint8_t*& dst_buf,
                    FX_DWORD& dst_len) {
  if (!_gif_write_header(gif_ptr, dst_buf, dst_len)) {
    return FALSE;
  }
  FX_DWORD cur_offset = gif_ptr->cur_offset;
  FX_BOOL res = TRUE;
  if (gif_ptr->frames) {
    gif_ptr->cur_offset--;
  }
  if (!_gif_write_data(gif_ptr, dst_buf, dst_len)) {
    gif_ptr->cur_offset = cur_offset;
    res = FALSE;
  }
  dst_len = gif_ptr->cur_offset;
  dst_buf[dst_len - 1] = GIF_SIG_TRAILER;
  if (res) {
    gif_ptr->frames++;
  }
  return res;
}
