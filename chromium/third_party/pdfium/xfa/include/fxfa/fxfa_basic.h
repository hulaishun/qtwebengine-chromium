// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _FXFA_BASIC_H
#define _FXFA_BASIC_H

#include "xfa/include/fxjse/fxjse.h"

class CXFA_Measurement;
#define XFA_HASHCODE_Xfa 0xc56b9ff
#define XFA_HASHCODE_Config 0x4e1e39b6
#define XFA_HASHCODE_Template 0x803550fc
#define XFA_HASHCODE_Datasets 0x99b95079
#define XFA_HASHCODE_Data 0xbde9abda
#define XFA_HASHCODE_Form 0xcd309ff4
#define XFA_HASHCODE_LocaleSet 0x5473b6dc
#define XFA_HASHCODE_ConnectionSet 0xe14c801c
#define XFA_HASHCODE_SourceSet 0x811929d
#define XFA_HASHCODE_Xdc 0xc56afbf
#define XFA_HASHCODE_Pdf 0xb843dba
#define XFA_HASHCODE_Xfdf 0x48d004a8
#define XFA_HASHCODE_Xmpmeta 0x132a8fbc
#define XFA_HASHCODE_Signature 0x8b036f32
#define XFA_HASHCODE_Stylesheet 0x6038580a
#define XFA_HASHCODE_XDP 0xc56afcc
#define XFA_HASHCODE_Record 0x5779d65f
#define XFA_HASHCODE_DataWindow 0x83a550d2
#define XFA_HASHCODE_Host 0xdb075bde
#define XFA_HASHCODE_Log 0x0b1b3d22
#define XFA_HASHCODE_Event 0x185e41e2
#define XFA_HASHCODE_Layout 0x7e7e845e
#define XFA_HASHCODE_Occur 0xf7eebe1c
#define XFA_HASHCODE_This 0x2d574d58
#define XFA_HASHCODE_DataDescription 0x2b5df51e
#define XFA_HASHCODE_Name 0x31b19c1
enum XFA_PACKET {
  XFA_PACKET_USER,
  XFA_PACKET_SourceSet,
  XFA_PACKET_Pdf,
  XFA_PACKET_Xdc,
  XFA_PACKET_XDP,
  XFA_PACKET_Xmpmeta,
  XFA_PACKET_Xfdf,
  XFA_PACKET_Config,
  XFA_PACKET_LocaleSet,
  XFA_PACKET_Stylesheet,
  XFA_PACKET_Template,
  XFA_PACKET_Signature,
  XFA_PACKET_Datasets,
  XFA_PACKET_Form,
  XFA_PACKET_ConnectionSet,
};
enum XFA_XDPPACKET {
  XFA_XDPPACKET_UNKNOWN = 0,
  XFA_XDPPACKET_Config = 1 << XFA_PACKET_Config,
  XFA_XDPPACKET_Template = 1 << XFA_PACKET_Template,
  XFA_XDPPACKET_Datasets = 1 << XFA_PACKET_Datasets,
  XFA_XDPPACKET_Form = 1 << XFA_PACKET_Form,
  XFA_XDPPACKET_LocaleSet = 1 << XFA_PACKET_LocaleSet,
  XFA_XDPPACKET_ConnectionSet = 1 << XFA_PACKET_ConnectionSet,
  XFA_XDPPACKET_SourceSet = 1 << XFA_PACKET_SourceSet,
  XFA_XDPPACKET_Xdc = 1 << XFA_PACKET_Xdc,
  XFA_XDPPACKET_Pdf = 1 << XFA_PACKET_Pdf,
  XFA_XDPPACKET_Xfdf = 1 << XFA_PACKET_Xfdf,
  XFA_XDPPACKET_Xmpmeta = 1 << XFA_PACKET_Xmpmeta,
  XFA_XDPPACKET_Signature = 1 << XFA_PACKET_Signature,
  XFA_XDPPACKET_Stylesheet = 1 << XFA_PACKET_Stylesheet,
  XFA_XDPPACKET_USER = 1 << XFA_PACKET_USER,
  XFA_XDPPACKET_XDP = 1 << XFA_PACKET_XDP,
};
enum XFA_XDPPACKET_FLAGS {
  XFA_XDPPACKET_FLAGS_COMPLETEMATCH = 1,
  XFA_XDPPACKET_FLAGS_PREFIXMATCH = 2,
  XFA_XDPPACKET_FLAGS_NOMATCH = 4,
  XFA_XDPPACKET_FLAGS_SUPPORTONE = 8,
  XFA_XDPPACKET_FLAGS_SUPPORTMANY = 16,
};
typedef struct _XFA_PACKETINFO {
  uint32_t uHash;
  const FX_WCHAR* pName;
  XFA_XDPPACKET eName;
  const FX_WCHAR* pURI;
  FX_DWORD eFlags;
} XFA_PACKETINFO, *XFA_LPPACKETINFO;
typedef XFA_PACKETINFO const* XFA_LPCPACKETINFO;
XFA_LPCPACKETINFO XFA_GetPacketByName(const CFX_WideStringC& wsName);
XFA_LPCPACKETINFO XFA_GetPacketByID(FX_DWORD dwPacket);
extern const XFA_PACKETINFO g_XFAPacketData[];
inline XFA_LPCPACKETINFO XFA_GetPacketByIndex(XFA_PACKET ePacket) {
  return g_XFAPacketData + ePacket;
}
enum XFA_ATTRIBUTEENUM {
  XFA_ATTRIBUTEENUM_Asterisk,
  XFA_ATTRIBUTEENUM_Slash,
  XFA_ATTRIBUTEENUM_Backslash,
  XFA_ATTRIBUTEENUM_On,
  XFA_ATTRIBUTEENUM_Tb,
  XFA_ATTRIBUTEENUM_Up,
  XFA_ATTRIBUTEENUM_MetaData,
  XFA_ATTRIBUTEENUM_Delegate,
  XFA_ATTRIBUTEENUM_PostSubmit,
  XFA_ATTRIBUTEENUM_Name,
  XFA_ATTRIBUTEENUM_Cross,
  XFA_ATTRIBUTEENUM_Next,
  XFA_ATTRIBUTEENUM_None,
  XFA_ATTRIBUTEENUM_ShortEdge,
  XFA_ATTRIBUTEENUM_1mod10_1mod11,
  XFA_ATTRIBUTEENUM_Height,
  XFA_ATTRIBUTEENUM_CrossDiagonal,
  XFA_ATTRIBUTEENUM_All,
  XFA_ATTRIBUTEENUM_Any,
  XFA_ATTRIBUTEENUM_ToRight,
  XFA_ATTRIBUTEENUM_MatchTemplate,
  XFA_ATTRIBUTEENUM_Dpl,
  XFA_ATTRIBUTEENUM_Invisible,
  XFA_ATTRIBUTEENUM_Fit,
  XFA_ATTRIBUTEENUM_Width,
  XFA_ATTRIBUTEENUM_PreSubmit,
  XFA_ATTRIBUTEENUM_Ipl,
  XFA_ATTRIBUTEENUM_FlateCompress,
  XFA_ATTRIBUTEENUM_Med,
  XFA_ATTRIBUTEENUM_Odd,
  XFA_ATTRIBUTEENUM_Off,
  XFA_ATTRIBUTEENUM_Pdf,
  XFA_ATTRIBUTEENUM_Row,
  XFA_ATTRIBUTEENUM_Top,
  XFA_ATTRIBUTEENUM_Xdp,
  XFA_ATTRIBUTEENUM_Xfd,
  XFA_ATTRIBUTEENUM_Xml,
  XFA_ATTRIBUTEENUM_Zip,
  XFA_ATTRIBUTEENUM_Zpl,
  XFA_ATTRIBUTEENUM_Visible,
  XFA_ATTRIBUTEENUM_Exclude,
  XFA_ATTRIBUTEENUM_MouseEnter,
  XFA_ATTRIBUTEENUM_Pair,
  XFA_ATTRIBUTEENUM_Filter,
  XFA_ATTRIBUTEENUM_MoveLast,
  XFA_ATTRIBUTEENUM_ExportAndImport,
  XFA_ATTRIBUTEENUM_Push,
  XFA_ATTRIBUTEENUM_Portrait,
  XFA_ATTRIBUTEENUM_Default,
  XFA_ATTRIBUTEENUM_StoredProc,
  XFA_ATTRIBUTEENUM_StayBOF,
  XFA_ATTRIBUTEENUM_StayEOF,
  XFA_ATTRIBUTEENUM_PostPrint,
  XFA_ATTRIBUTEENUM_UsCarrier,
  XFA_ATTRIBUTEENUM_Right,
  XFA_ATTRIBUTEENUM_PreOpen,
  XFA_ATTRIBUTEENUM_Actual,
  XFA_ATTRIBUTEENUM_Rest,
  XFA_ATTRIBUTEENUM_TopCenter,
  XFA_ATTRIBUTEENUM_StandardSymbol,
  XFA_ATTRIBUTEENUM_Initialize,
  XFA_ATTRIBUTEENUM_JustifyAll,
  XFA_ATTRIBUTEENUM_Normal,
  XFA_ATTRIBUTEENUM_Landscape,
  XFA_ATTRIBUTEENUM_NonInteractive,
  XFA_ATTRIBUTEENUM_MouseExit,
  XFA_ATTRIBUTEENUM_Minus,
  XFA_ATTRIBUTEENUM_DiagonalLeft,
  XFA_ATTRIBUTEENUM_SimplexPaginated,
  XFA_ATTRIBUTEENUM_Document,
  XFA_ATTRIBUTEENUM_Warning,
  XFA_ATTRIBUTEENUM_Auto,
  XFA_ATTRIBUTEENUM_Below,
  XFA_ATTRIBUTEENUM_BottomLeft,
  XFA_ATTRIBUTEENUM_BottomCenter,
  XFA_ATTRIBUTEENUM_Tcpl,
  XFA_ATTRIBUTEENUM_Text,
  XFA_ATTRIBUTEENUM_Grouping,
  XFA_ATTRIBUTEENUM_SecureSymbol,
  XFA_ATTRIBUTEENUM_PreExecute,
  XFA_ATTRIBUTEENUM_DocClose,
  XFA_ATTRIBUTEENUM_Keyset,
  XFA_ATTRIBUTEENUM_Vertical,
  XFA_ATTRIBUTEENUM_PreSave,
  XFA_ATTRIBUTEENUM_PreSign,
  XFA_ATTRIBUTEENUM_Bottom,
  XFA_ATTRIBUTEENUM_ToTop,
  XFA_ATTRIBUTEENUM_Verify,
  XFA_ATTRIBUTEENUM_First,
  XFA_ATTRIBUTEENUM_ContentArea,
  XFA_ATTRIBUTEENUM_Solid,
  XFA_ATTRIBUTEENUM_Pessimistic,
  XFA_ATTRIBUTEENUM_DuplexPaginated,
  XFA_ATTRIBUTEENUM_Round,
  XFA_ATTRIBUTEENUM_Remerge,
  XFA_ATTRIBUTEENUM_Ordered,
  XFA_ATTRIBUTEENUM_Percent,
  XFA_ATTRIBUTEENUM_Even,
  XFA_ATTRIBUTEENUM_Exit,
  XFA_ATTRIBUTEENUM_ToolTip,
  XFA_ATTRIBUTEENUM_OrderedOccurrence,
  XFA_ATTRIBUTEENUM_ReadOnly,
  XFA_ATTRIBUTEENUM_Currency,
  XFA_ATTRIBUTEENUM_Concat,
  XFA_ATTRIBUTEENUM_Thai,
  XFA_ATTRIBUTEENUM_Embossed,
  XFA_ATTRIBUTEENUM_Formdata,
  XFA_ATTRIBUTEENUM_Greek,
  XFA_ATTRIBUTEENUM_Decimal,
  XFA_ATTRIBUTEENUM_Select,
  XFA_ATTRIBUTEENUM_LongEdge,
  XFA_ATTRIBUTEENUM_Protected,
  XFA_ATTRIBUTEENUM_BottomRight,
  XFA_ATTRIBUTEENUM_Zero,
  XFA_ATTRIBUTEENUM_ForwardOnly,
  XFA_ATTRIBUTEENUM_DocReady,
  XFA_ATTRIBUTEENUM_Hidden,
  XFA_ATTRIBUTEENUM_Include,
  XFA_ATTRIBUTEENUM_Dashed,
  XFA_ATTRIBUTEENUM_MultiSelect,
  XFA_ATTRIBUTEENUM_Inactive,
  XFA_ATTRIBUTEENUM_Embed,
  XFA_ATTRIBUTEENUM_Static,
  XFA_ATTRIBUTEENUM_OnEntry,
  XFA_ATTRIBUTEENUM_Cyrillic,
  XFA_ATTRIBUTEENUM_NonBlank,
  XFA_ATTRIBUTEENUM_TopRight,
  XFA_ATTRIBUTEENUM_Hebrew,
  XFA_ATTRIBUTEENUM_TopLeft,
  XFA_ATTRIBUTEENUM_Center,
  XFA_ATTRIBUTEENUM_MoveFirst,
  XFA_ATTRIBUTEENUM_Diamond,
  XFA_ATTRIBUTEENUM_PageOdd,
  XFA_ATTRIBUTEENUM_1mod10,
  XFA_ATTRIBUTEENUM_Korean,
  XFA_ATTRIBUTEENUM_AboveEmbedded,
  XFA_ATTRIBUTEENUM_ZipCompress,
  XFA_ATTRIBUTEENUM_Numeric,
  XFA_ATTRIBUTEENUM_Circle,
  XFA_ATTRIBUTEENUM_ToBottom,
  XFA_ATTRIBUTEENUM_Inverted,
  XFA_ATTRIBUTEENUM_Update,
  XFA_ATTRIBUTEENUM_Isoname,
  XFA_ATTRIBUTEENUM_Server,
  XFA_ATTRIBUTEENUM_Position,
  XFA_ATTRIBUTEENUM_MiddleCenter,
  XFA_ATTRIBUTEENUM_Optional,
  XFA_ATTRIBUTEENUM_UsePrinterSetting,
  XFA_ATTRIBUTEENUM_Outline,
  XFA_ATTRIBUTEENUM_IndexChange,
  XFA_ATTRIBUTEENUM_Change,
  XFA_ATTRIBUTEENUM_PageArea,
  XFA_ATTRIBUTEENUM_Once,
  XFA_ATTRIBUTEENUM_Only,
  XFA_ATTRIBUTEENUM_Open,
  XFA_ATTRIBUTEENUM_Caption,
  XFA_ATTRIBUTEENUM_Raised,
  XFA_ATTRIBUTEENUM_Justify,
  XFA_ATTRIBUTEENUM_RefAndDescendants,
  XFA_ATTRIBUTEENUM_Short,
  XFA_ATTRIBUTEENUM_PageFront,
  XFA_ATTRIBUTEENUM_Monospace,
  XFA_ATTRIBUTEENUM_Middle,
  XFA_ATTRIBUTEENUM_PrePrint,
  XFA_ATTRIBUTEENUM_Always,
  XFA_ATTRIBUTEENUM_Unknown,
  XFA_ATTRIBUTEENUM_ToLeft,
  XFA_ATTRIBUTEENUM_Above,
  XFA_ATTRIBUTEENUM_DashDot,
  XFA_ATTRIBUTEENUM_Gregorian,
  XFA_ATTRIBUTEENUM_Roman,
  XFA_ATTRIBUTEENUM_MouseDown,
  XFA_ATTRIBUTEENUM_Symbol,
  XFA_ATTRIBUTEENUM_PageEven,
  XFA_ATTRIBUTEENUM_Sign,
  XFA_ATTRIBUTEENUM_AddNew,
  XFA_ATTRIBUTEENUM_Star,
  XFA_ATTRIBUTEENUM_Optimistic,
  XFA_ATTRIBUTEENUM_Rl_tb,
  XFA_ATTRIBUTEENUM_MiddleRight,
  XFA_ATTRIBUTEENUM_Maintain,
  XFA_ATTRIBUTEENUM_Package,
  XFA_ATTRIBUTEENUM_SimplifiedChinese,
  XFA_ATTRIBUTEENUM_ToCenter,
  XFA_ATTRIBUTEENUM_Back,
  XFA_ATTRIBUTEENUM_Unspecified,
  XFA_ATTRIBUTEENUM_BatchOptimistic,
  XFA_ATTRIBUTEENUM_Bold,
  XFA_ATTRIBUTEENUM_Both,
  XFA_ATTRIBUTEENUM_Butt,
  XFA_ATTRIBUTEENUM_Client,
  XFA_ATTRIBUTEENUM_2mod10,
  XFA_ATTRIBUTEENUM_ImageOnly,
  XFA_ATTRIBUTEENUM_Horizontal,
  XFA_ATTRIBUTEENUM_Dotted,
  XFA_ATTRIBUTEENUM_UserControl,
  XFA_ATTRIBUTEENUM_DiagonalRight,
  XFA_ATTRIBUTEENUM_ConsumeData,
  XFA_ATTRIBUTEENUM_Check,
  XFA_ATTRIBUTEENUM_Data,
  XFA_ATTRIBUTEENUM_Down,
  XFA_ATTRIBUTEENUM_SansSerif,
  XFA_ATTRIBUTEENUM_Inline,
  XFA_ATTRIBUTEENUM_TraditionalChinese,
  XFA_ATTRIBUTEENUM_Warn,
  XFA_ATTRIBUTEENUM_RefOnly,
  XFA_ATTRIBUTEENUM_InteractiveForms,
  XFA_ATTRIBUTEENUM_Word,
  XFA_ATTRIBUTEENUM_Unordered,
  XFA_ATTRIBUTEENUM_Required,
  XFA_ATTRIBUTEENUM_ImportOnly,
  XFA_ATTRIBUTEENUM_BelowEmbedded,
  XFA_ATTRIBUTEENUM_Japanese,
  XFA_ATTRIBUTEENUM_Full,
  XFA_ATTRIBUTEENUM_Rl_row,
  XFA_ATTRIBUTEENUM_Vietnamese,
  XFA_ATTRIBUTEENUM_EastEuropeanRoman,
  XFA_ATTRIBUTEENUM_MouseUp,
  XFA_ATTRIBUTEENUM_ExportOnly,
  XFA_ATTRIBUTEENUM_Clear,
  XFA_ATTRIBUTEENUM_Click,
  XFA_ATTRIBUTEENUM_Base64,
  XFA_ATTRIBUTEENUM_Close,
  XFA_ATTRIBUTEENUM_Host,
  XFA_ATTRIBUTEENUM_Global,
  XFA_ATTRIBUTEENUM_Blank,
  XFA_ATTRIBUTEENUM_Table,
  XFA_ATTRIBUTEENUM_Import,
  XFA_ATTRIBUTEENUM_Custom,
  XFA_ATTRIBUTEENUM_MiddleLeft,
  XFA_ATTRIBUTEENUM_PostExecute,
  XFA_ATTRIBUTEENUM_Radix,
  XFA_ATTRIBUTEENUM_PostOpen,
  XFA_ATTRIBUTEENUM_Enter,
  XFA_ATTRIBUTEENUM_Ignore,
  XFA_ATTRIBUTEENUM_Lr_tb,
  XFA_ATTRIBUTEENUM_Fantasy,
  XFA_ATTRIBUTEENUM_Italic,
  XFA_ATTRIBUTEENUM_Author,
  XFA_ATTRIBUTEENUM_ToEdge,
  XFA_ATTRIBUTEENUM_Choice,
  XFA_ATTRIBUTEENUM_Disabled,
  XFA_ATTRIBUTEENUM_CrossHatch,
  XFA_ATTRIBUTEENUM_DataRef,
  XFA_ATTRIBUTEENUM_DashDotDot,
  XFA_ATTRIBUTEENUM_Square,
  XFA_ATTRIBUTEENUM_Dynamic,
  XFA_ATTRIBUTEENUM_Manual,
  XFA_ATTRIBUTEENUM_Etched,
  XFA_ATTRIBUTEENUM_ValidationState,
  XFA_ATTRIBUTEENUM_Cursive,
  XFA_ATTRIBUTEENUM_Last,
  XFA_ATTRIBUTEENUM_Left,
  XFA_ATTRIBUTEENUM_Link,
  XFA_ATTRIBUTEENUM_Long,
  XFA_ATTRIBUTEENUM_InternationalCarrier,
  XFA_ATTRIBUTEENUM_PDF1_3,
  XFA_ATTRIBUTEENUM_PDF1_6,
  XFA_ATTRIBUTEENUM_Serif,
  XFA_ATTRIBUTEENUM_PostSave,
  XFA_ATTRIBUTEENUM_Ready,
  XFA_ATTRIBUTEENUM_PostSign,
  XFA_ATTRIBUTEENUM_Arabic,
  XFA_ATTRIBUTEENUM_Error,
  XFA_ATTRIBUTEENUM_Urlencoded,
  XFA_ATTRIBUTEENUM_Lowered,
};
enum XFA_ATTRIBUTE {
  XFA_ATTRIBUTE_H,
  XFA_ATTRIBUTE_W,
  XFA_ATTRIBUTE_X,
  XFA_ATTRIBUTE_Y,
  XFA_ATTRIBUTE_Id,
  XFA_ATTRIBUTE_To,
  XFA_ATTRIBUTE_LineThrough,
  XFA_ATTRIBUTE_HAlign,
  XFA_ATTRIBUTE_Typeface,
  XFA_ATTRIBUTE_BeforeTarget,
  XFA_ATTRIBUTE_Name,
  XFA_ATTRIBUTE_Next,
  XFA_ATTRIBUTE_DataRowCount,
  XFA_ATTRIBUTE_Break,
  XFA_ATTRIBUTE_VScrollPolicy,
  XFA_ATTRIBUTE_FontHorizontalScale,
  XFA_ATTRIBUTE_TextIndent,
  XFA_ATTRIBUTE_Context,
  XFA_ATTRIBUTE_TrayOut,
  XFA_ATTRIBUTE_Cap,
  XFA_ATTRIBUTE_Max,
  XFA_ATTRIBUTE_Min,
  XFA_ATTRIBUTE_Ref,
  XFA_ATTRIBUTE_Rid,
  XFA_ATTRIBUTE_Url,
  XFA_ATTRIBUTE_Use,
  XFA_ATTRIBUTE_LeftInset,
  XFA_ATTRIBUTE_Widows,
  XFA_ATTRIBUTE_Level,
  XFA_ATTRIBUTE_BottomInset,
  XFA_ATTRIBUTE_OverflowTarget,
  XFA_ATTRIBUTE_AllowMacro,
  XFA_ATTRIBUTE_PagePosition,
  XFA_ATTRIBUTE_ColumnWidths,
  XFA_ATTRIBUTE_OverflowLeader,
  XFA_ATTRIBUTE_Action,
  XFA_ATTRIBUTE_NonRepudiation,
  XFA_ATTRIBUTE_Rate,
  XFA_ATTRIBUTE_AllowRichText,
  XFA_ATTRIBUTE_Role,
  XFA_ATTRIBUTE_OverflowTrailer,
  XFA_ATTRIBUTE_Operation,
  XFA_ATTRIBUTE_Timeout,
  XFA_ATTRIBUTE_TopInset,
  XFA_ATTRIBUTE_Access,
  XFA_ATTRIBUTE_CommandType,
  XFA_ATTRIBUTE_Format,
  XFA_ATTRIBUTE_DataPrep,
  XFA_ATTRIBUTE_WidgetData,
  XFA_ATTRIBUTE_Abbr,
  XFA_ATTRIBUTE_MarginRight,
  XFA_ATTRIBUTE_DataDescription,
  XFA_ATTRIBUTE_EncipherOnly,
  XFA_ATTRIBUTE_KerningMode,
  XFA_ATTRIBUTE_Rotate,
  XFA_ATTRIBUTE_WordCharacterCount,
  XFA_ATTRIBUTE_Type,
  XFA_ATTRIBUTE_Reserve,
  XFA_ATTRIBUTE_TextLocation,
  XFA_ATTRIBUTE_Priority,
  XFA_ATTRIBUTE_Underline,
  XFA_ATTRIBUTE_ModuleWidth,
  XFA_ATTRIBUTE_Hyphenate,
  XFA_ATTRIBUTE_Listen,
  XFA_ATTRIBUTE_Delimiter,
  XFA_ATTRIBUTE_ContentType,
  XFA_ATTRIBUTE_StartNew,
  XFA_ATTRIBUTE_EofAction,
  XFA_ATTRIBUTE_AllowNeutral,
  XFA_ATTRIBUTE_Connection,
  XFA_ATTRIBUTE_BaselineShift,
  XFA_ATTRIBUTE_OverlinePeriod,
  XFA_ATTRIBUTE_FracDigits,
  XFA_ATTRIBUTE_Orientation,
  XFA_ATTRIBUTE_TimeStamp,
  XFA_ATTRIBUTE_PrintCheckDigit,
  XFA_ATTRIBUTE_MarginLeft,
  XFA_ATTRIBUTE_Stroke,
  XFA_ATTRIBUTE_ModuleHeight,
  XFA_ATTRIBUTE_TransferEncoding,
  XFA_ATTRIBUTE_Usage,
  XFA_ATTRIBUTE_Presence,
  XFA_ATTRIBUTE_RadixOffset,
  XFA_ATTRIBUTE_Preserve,
  XFA_ATTRIBUTE_AliasNode,
  XFA_ATTRIBUTE_MultiLine,
  XFA_ATTRIBUTE_Version,
  XFA_ATTRIBUTE_StartChar,
  XFA_ATTRIBUTE_ScriptTest,
  XFA_ATTRIBUTE_StartAngle,
  XFA_ATTRIBUTE_CursorType,
  XFA_ATTRIBUTE_DigitalSignature,
  XFA_ATTRIBUTE_CodeType,
  XFA_ATTRIBUTE_Output,
  XFA_ATTRIBUTE_BookendTrailer,
  XFA_ATTRIBUTE_ImagingBBox,
  XFA_ATTRIBUTE_ExcludeInitialCap,
  XFA_ATTRIBUTE_Force,
  XFA_ATTRIBUTE_CrlSign,
  XFA_ATTRIBUTE_Previous,
  XFA_ATTRIBUTE_PushCharacterCount,
  XFA_ATTRIBUTE_NullTest,
  XFA_ATTRIBUTE_RunAt,
  XFA_ATTRIBUTE_SpaceBelow,
  XFA_ATTRIBUTE_SweepAngle,
  XFA_ATTRIBUTE_NumberOfCells,
  XFA_ATTRIBUTE_LetterSpacing,
  XFA_ATTRIBUTE_LockType,
  XFA_ATTRIBUTE_PasswordChar,
  XFA_ATTRIBUTE_VAlign,
  XFA_ATTRIBUTE_SourceBelow,
  XFA_ATTRIBUTE_Inverted,
  XFA_ATTRIBUTE_Mark,
  XFA_ATTRIBUTE_MaxH,
  XFA_ATTRIBUTE_MaxW,
  XFA_ATTRIBUTE_Truncate,
  XFA_ATTRIBUTE_MinH,
  XFA_ATTRIBUTE_MinW,
  XFA_ATTRIBUTE_Initial,
  XFA_ATTRIBUTE_Mode,
  XFA_ATTRIBUTE_Layout,
  XFA_ATTRIBUTE_Server,
  XFA_ATTRIBUTE_EmbedPDF,
  XFA_ATTRIBUTE_OddOrEven,
  XFA_ATTRIBUTE_TabDefault,
  XFA_ATTRIBUTE_Contains,
  XFA_ATTRIBUTE_RightInset,
  XFA_ATTRIBUTE_MaxChars,
  XFA_ATTRIBUTE_Open,
  XFA_ATTRIBUTE_Relation,
  XFA_ATTRIBUTE_WideNarrowRatio,
  XFA_ATTRIBUTE_Relevant,
  XFA_ATTRIBUTE_SignatureType,
  XFA_ATTRIBUTE_LineThroughPeriod,
  XFA_ATTRIBUTE_Shape,
  XFA_ATTRIBUTE_TabStops,
  XFA_ATTRIBUTE_OutputBelow,
  XFA_ATTRIBUTE_Short,
  XFA_ATTRIBUTE_FontVerticalScale,
  XFA_ATTRIBUTE_Thickness,
  XFA_ATTRIBUTE_CommitOn,
  XFA_ATTRIBUTE_RemainCharacterCount,
  XFA_ATTRIBUTE_KeyAgreement,
  XFA_ATTRIBUTE_ErrorCorrectionLevel,
  XFA_ATTRIBUTE_UpsMode,
  XFA_ATTRIBUTE_MergeMode,
  XFA_ATTRIBUTE_Circular,
  XFA_ATTRIBUTE_PsName,
  XFA_ATTRIBUTE_Trailer,
  XFA_ATTRIBUTE_UnicodeRange,
  XFA_ATTRIBUTE_ExecuteType,
  XFA_ATTRIBUTE_DuplexImposition,
  XFA_ATTRIBUTE_TrayIn,
  XFA_ATTRIBUTE_BindingNode,
  XFA_ATTRIBUTE_BofAction,
  XFA_ATTRIBUTE_Save,
  XFA_ATTRIBUTE_TargetType,
  XFA_ATTRIBUTE_KeyEncipherment,
  XFA_ATTRIBUTE_CredentialServerPolicy,
  XFA_ATTRIBUTE_Size,
  XFA_ATTRIBUTE_InitialNumber,
  XFA_ATTRIBUTE_Slope,
  XFA_ATTRIBUTE_CSpace,
  XFA_ATTRIBUTE_ColSpan,
  XFA_ATTRIBUTE_Binding,
  XFA_ATTRIBUTE_Checksum,
  XFA_ATTRIBUTE_CharEncoding,
  XFA_ATTRIBUTE_Bind,
  XFA_ATTRIBUTE_TextEntry,
  XFA_ATTRIBUTE_Archive,
  XFA_ATTRIBUTE_Uuid,
  XFA_ATTRIBUTE_Posture,
  XFA_ATTRIBUTE_After,
  XFA_ATTRIBUTE_Orphans,
  XFA_ATTRIBUTE_QualifiedName,
  XFA_ATTRIBUTE_Usehref,
  XFA_ATTRIBUTE_Locale,
  XFA_ATTRIBUTE_Weight,
  XFA_ATTRIBUTE_UnderlinePeriod,
  XFA_ATTRIBUTE_Data,
  XFA_ATTRIBUTE_Desc,
  XFA_ATTRIBUTE_Numbered,
  XFA_ATTRIBUTE_DataColumnCount,
  XFA_ATTRIBUTE_Overline,
  XFA_ATTRIBUTE_UrlPolicy,
  XFA_ATTRIBUTE_AnchorType,
  XFA_ATTRIBUTE_LabelRef,
  XFA_ATTRIBUTE_BookendLeader,
  XFA_ATTRIBUTE_MaxLength,
  XFA_ATTRIBUTE_AccessKey,
  XFA_ATTRIBUTE_CursorLocation,
  XFA_ATTRIBUTE_DelayedOpen,
  XFA_ATTRIBUTE_Target,
  XFA_ATTRIBUTE_DataEncipherment,
  XFA_ATTRIBUTE_AfterTarget,
  XFA_ATTRIBUTE_Leader,
  XFA_ATTRIBUTE_Picker,
  XFA_ATTRIBUTE_From,
  XFA_ATTRIBUTE_BaseProfile,
  XFA_ATTRIBUTE_Aspect,
  XFA_ATTRIBUTE_RowColumnRatio,
  XFA_ATTRIBUTE_LineHeight,
  XFA_ATTRIBUTE_Highlight,
  XFA_ATTRIBUTE_ValueRef,
  XFA_ATTRIBUTE_MaxEntries,
  XFA_ATTRIBUTE_DataLength,
  XFA_ATTRIBUTE_Activity,
  XFA_ATTRIBUTE_Input,
  XFA_ATTRIBUTE_Value,
  XFA_ATTRIBUTE_BlankOrNotBlank,
  XFA_ATTRIBUTE_AddRevocationInfo,
  XFA_ATTRIBUTE_GenericFamily,
  XFA_ATTRIBUTE_Hand,
  XFA_ATTRIBUTE_Href,
  XFA_ATTRIBUTE_TextEncoding,
  XFA_ATTRIBUTE_LeadDigits,
  XFA_ATTRIBUTE_Permissions,
  XFA_ATTRIBUTE_SpaceAbove,
  XFA_ATTRIBUTE_CodeBase,
  XFA_ATTRIBUTE_Stock,
  XFA_ATTRIBUTE_IsNull,
  XFA_ATTRIBUTE_RestoreState,
  XFA_ATTRIBUTE_ExcludeAllCaps,
  XFA_ATTRIBUTE_FormatTest,
  XFA_ATTRIBUTE_HScrollPolicy,
  XFA_ATTRIBUTE_Join,
  XFA_ATTRIBUTE_KeyCertSign,
  XFA_ATTRIBUTE_Radius,
  XFA_ATTRIBUTE_SourceAbove,
  XFA_ATTRIBUTE_Override,
  XFA_ATTRIBUTE_ClassId,
  XFA_ATTRIBUTE_Disable,
  XFA_ATTRIBUTE_Scope,
  XFA_ATTRIBUTE_Match,
  XFA_ATTRIBUTE_Placement,
  XFA_ATTRIBUTE_Before,
  XFA_ATTRIBUTE_WritingScript,
  XFA_ATTRIBUTE_EndChar,
  XFA_ATTRIBUTE_Lock,
  XFA_ATTRIBUTE_Long,
  XFA_ATTRIBUTE_Intact,
  XFA_ATTRIBUTE_XdpContent,
  XFA_ATTRIBUTE_DecipherOnly,
};
enum XFA_ELEMENT {
  XFA_ELEMENT_Ps,
  XFA_ELEMENT_To,
  XFA_ELEMENT_Ui,
  XFA_ELEMENT_RecordSet,
  XFA_ELEMENT_SubsetBelow,
  XFA_ELEMENT_SubformSet,
  XFA_ELEMENT_AdobeExtensionLevel,
  XFA_ELEMENT_Typeface,
  XFA_ELEMENT_Break,
  XFA_ELEMENT_FontInfo,
  XFA_ELEMENT_NumberPattern,
  XFA_ELEMENT_DynamicRender,
  XFA_ELEMENT_PrintScaling,
  XFA_ELEMENT_CheckButton,
  XFA_ELEMENT_DatePatterns,
  XFA_ELEMENT_SourceSet,
  XFA_ELEMENT_Amd,
  XFA_ELEMENT_Arc,
  XFA_ELEMENT_Day,
  XFA_ELEMENT_Era,
  XFA_ELEMENT_Jog,
  XFA_ELEMENT_Log,
  XFA_ELEMENT_Map,
  XFA_ELEMENT_Mdp,
  XFA_ELEMENT_BreakBefore,
  XFA_ELEMENT_Oid,
  XFA_ELEMENT_Pcl,
  XFA_ELEMENT_Pdf,
  XFA_ELEMENT_Ref,
  XFA_ELEMENT_Uri,
  XFA_ELEMENT_Xdc,
  XFA_ELEMENT_Xdp,
  XFA_ELEMENT_Xfa,
  XFA_ELEMENT_Xsl,
  XFA_ELEMENT_Zpl,
  XFA_ELEMENT_Cache,
  XFA_ELEMENT_Margin,
  XFA_ELEMENT_KeyUsage,
  XFA_ELEMENT_Exclude,
  XFA_ELEMENT_ChoiceList,
  XFA_ELEMENT_Level,
  XFA_ELEMENT_LabelPrinter,
  XFA_ELEMENT_CalendarSymbols,
  XFA_ELEMENT_Para,
  XFA_ELEMENT_Part,
  XFA_ELEMENT_Pdfa,
  XFA_ELEMENT_Filter,
  XFA_ELEMENT_Present,
  XFA_ELEMENT_Pagination,
  XFA_ELEMENT_Encoding,
  XFA_ELEMENT_Event,
  XFA_ELEMENT_Whitespace,
  XFA_ELEMENT_DefaultUi,
  XFA_ELEMENT_DataModel,
  XFA_ELEMENT_Barcode,
  XFA_ELEMENT_TimePattern,
  XFA_ELEMENT_BatchOutput,
  XFA_ELEMENT_Enforce,
  XFA_ELEMENT_CurrencySymbols,
  XFA_ELEMENT_AddSilentPrint,
  XFA_ELEMENT_Rename,
  XFA_ELEMENT_Operation,
  XFA_ELEMENT_Typefaces,
  XFA_ELEMENT_SubjectDNs,
  XFA_ELEMENT_Issuers,
  XFA_ELEMENT_SignaturePseudoModel,
  XFA_ELEMENT_WsdlConnection,
  XFA_ELEMENT_Debug,
  XFA_ELEMENT_Delta,
  XFA_ELEMENT_EraNames,
  XFA_ELEMENT_ModifyAnnots,
  XFA_ELEMENT_StartNode,
  XFA_ELEMENT_Button,
  XFA_ELEMENT_Format,
  XFA_ELEMENT_Border,
  XFA_ELEMENT_Area,
  XFA_ELEMENT_Hyphenation,
  XFA_ELEMENT_Text,
  XFA_ELEMENT_Time,
  XFA_ELEMENT_Type,
  XFA_ELEMENT_Overprint,
  XFA_ELEMENT_Certificates,
  XFA_ELEMENT_EncryptionMethods,
  XFA_ELEMENT_SetProperty,
  XFA_ELEMENT_PrinterName,
  XFA_ELEMENT_StartPage,
  XFA_ELEMENT_PageOffset,
  XFA_ELEMENT_DateTime,
  XFA_ELEMENT_Comb,
  XFA_ELEMENT_Pattern,
  XFA_ELEMENT_IfEmpty,
  XFA_ELEMENT_SuppressBanner,
  XFA_ELEMENT_OutputBin,
  XFA_ELEMENT_Field,
  XFA_ELEMENT_Agent,
  XFA_ELEMENT_OutputXSL,
  XFA_ELEMENT_AdjustData,
  XFA_ELEMENT_AutoSave,
  XFA_ELEMENT_ContentArea,
  XFA_ELEMENT_EventPseudoModel,
  XFA_ELEMENT_WsdlAddress,
  XFA_ELEMENT_Solid,
  XFA_ELEMENT_DateTimeSymbols,
  XFA_ELEMENT_EncryptionLevel,
  XFA_ELEMENT_Edge,
  XFA_ELEMENT_Stipple,
  XFA_ELEMENT_Attributes,
  XFA_ELEMENT_VersionControl,
  XFA_ELEMENT_Meridiem,
  XFA_ELEMENT_ExclGroup,
  XFA_ELEMENT_ToolTip,
  XFA_ELEMENT_Compress,
  XFA_ELEMENT_Reason,
  XFA_ELEMENT_Execute,
  XFA_ELEMENT_ContentCopy,
  XFA_ELEMENT_DateTimeEdit,
  XFA_ELEMENT_Config,
  XFA_ELEMENT_Image,
  XFA_ELEMENT_SharpxHTML,
  XFA_ELEMENT_NumberOfCopies,
  XFA_ELEMENT_BehaviorOverride,
  XFA_ELEMENT_TimeStamp,
  XFA_ELEMENT_Month,
  XFA_ELEMENT_ViewerPreferences,
  XFA_ELEMENT_ScriptModel,
  XFA_ELEMENT_Decimal,
  XFA_ELEMENT_Subform,
  XFA_ELEMENT_Select,
  XFA_ELEMENT_Window,
  XFA_ELEMENT_LocaleSet,
  XFA_ELEMENT_Handler,
  XFA_ELEMENT_HostPseudoModel,
  XFA_ELEMENT_Presence,
  XFA_ELEMENT_Record,
  XFA_ELEMENT_Embed,
  XFA_ELEMENT_Version,
  XFA_ELEMENT_Command,
  XFA_ELEMENT_Copies,
  XFA_ELEMENT_Staple,
  XFA_ELEMENT_SubmitFormat,
  XFA_ELEMENT_Boolean,
  XFA_ELEMENT_Message,
  XFA_ELEMENT_Output,
  XFA_ELEMENT_PsMap,
  XFA_ELEMENT_ExcludeNS,
  XFA_ELEMENT_Assist,
  XFA_ELEMENT_Picture,
  XFA_ELEMENT_Traversal,
  XFA_ELEMENT_SilentPrint,
  XFA_ELEMENT_WebClient,
  XFA_ELEMENT_LayoutPseudoModel,
  XFA_ELEMENT_Producer,
  XFA_ELEMENT_Corner,
  XFA_ELEMENT_MsgId,
  XFA_ELEMENT_Color,
  XFA_ELEMENT_Keep,
  XFA_ELEMENT_Query,
  XFA_ELEMENT_Insert,
  XFA_ELEMENT_ImageEdit,
  XFA_ELEMENT_Validate,
  XFA_ELEMENT_DigestMethods,
  XFA_ELEMENT_NumberPatterns,
  XFA_ELEMENT_PageSet,
  XFA_ELEMENT_Integer,
  XFA_ELEMENT_SoapAddress,
  XFA_ELEMENT_Equate,
  XFA_ELEMENT_FormFieldFilling,
  XFA_ELEMENT_PageRange,
  XFA_ELEMENT_Update,
  XFA_ELEMENT_ConnectString,
  XFA_ELEMENT_Mode,
  XFA_ELEMENT_Layout,
  XFA_ELEMENT_Sharpxml,
  XFA_ELEMENT_XsdConnection,
  XFA_ELEMENT_Traverse,
  XFA_ELEMENT_Encodings,
  XFA_ELEMENT_Template,
  XFA_ELEMENT_Acrobat,
  XFA_ELEMENT_ValidationMessaging,
  XFA_ELEMENT_Signing,
  XFA_ELEMENT_DataWindow,
  XFA_ELEMENT_Script,
  XFA_ELEMENT_AddViewerPreferences,
  XFA_ELEMENT_AlwaysEmbed,
  XFA_ELEMENT_PasswordEdit,
  XFA_ELEMENT_NumericEdit,
  XFA_ELEMENT_EncryptionMethod,
  XFA_ELEMENT_Change,
  XFA_ELEMENT_PageArea,
  XFA_ELEMENT_SubmitUrl,
  XFA_ELEMENT_Oids,
  XFA_ELEMENT_Signature,
  XFA_ELEMENT_ADBE_JSConsole,
  XFA_ELEMENT_Caption,
  XFA_ELEMENT_Relevant,
  XFA_ELEMENT_FlipLabel,
  XFA_ELEMENT_ExData,
  XFA_ELEMENT_DayNames,
  XFA_ELEMENT_SoapAction,
  XFA_ELEMENT_DefaultTypeface,
  XFA_ELEMENT_Manifest,
  XFA_ELEMENT_Overflow,
  XFA_ELEMENT_Linear,
  XFA_ELEMENT_CurrencySymbol,
  XFA_ELEMENT_Delete,
  XFA_ELEMENT_Deltas,
  XFA_ELEMENT_DigestMethod,
  XFA_ELEMENT_InstanceManager,
  XFA_ELEMENT_EquateRange,
  XFA_ELEMENT_Medium,
  XFA_ELEMENT_TextEdit,
  XFA_ELEMENT_TemplateCache,
  XFA_ELEMENT_CompressObjectStream,
  XFA_ELEMENT_DataValue,
  XFA_ELEMENT_AccessibleContent,
  XFA_ELEMENT_NodeList,
  XFA_ELEMENT_IncludeXDPContent,
  XFA_ELEMENT_XmlConnection,
  XFA_ELEMENT_ValidateApprovalSignatures,
  XFA_ELEMENT_SignData,
  XFA_ELEMENT_Packets,
  XFA_ELEMENT_DatePattern,
  XFA_ELEMENT_DuplexOption,
  XFA_ELEMENT_Base,
  XFA_ELEMENT_Bind,
  XFA_ELEMENT_Compression,
  XFA_ELEMENT_User,
  XFA_ELEMENT_Rectangle,
  XFA_ELEMENT_EffectiveOutputPolicy,
  XFA_ELEMENT_ADBE_JSDebugger,
  XFA_ELEMENT_Acrobat7,
  XFA_ELEMENT_Interactive,
  XFA_ELEMENT_Locale,
  XFA_ELEMENT_CurrentPage,
  XFA_ELEMENT_Data,
  XFA_ELEMENT_Date,
  XFA_ELEMENT_Desc,
  XFA_ELEMENT_Encrypt,
  XFA_ELEMENT_Draw,
  XFA_ELEMENT_Encryption,
  XFA_ELEMENT_MeridiemNames,
  XFA_ELEMENT_Messaging,
  XFA_ELEMENT_Speak,
  XFA_ELEMENT_DataGroup,
  XFA_ELEMENT_Common,
  XFA_ELEMENT_Sharptext,
  XFA_ELEMENT_PaginationOverride,
  XFA_ELEMENT_Reasons,
  XFA_ELEMENT_SignatureProperties,
  XFA_ELEMENT_Threshold,
  XFA_ELEMENT_AppearanceFilter,
  XFA_ELEMENT_Fill,
  XFA_ELEMENT_Font,
  XFA_ELEMENT_Form,
  XFA_ELEMENT_MediumInfo,
  XFA_ELEMENT_Certificate,
  XFA_ELEMENT_Password,
  XFA_ELEMENT_RunScripts,
  XFA_ELEMENT_Trace,
  XFA_ELEMENT_Float,
  XFA_ELEMENT_RenderPolicy,
  XFA_ELEMENT_LogPseudoModel,
  XFA_ELEMENT_Destination,
  XFA_ELEMENT_Value,
  XFA_ELEMENT_Bookend,
  XFA_ELEMENT_ExObject,
  XFA_ELEMENT_OpenAction,
  XFA_ELEMENT_NeverEmbed,
  XFA_ELEMENT_BindItems,
  XFA_ELEMENT_Calculate,
  XFA_ELEMENT_Print,
  XFA_ELEMENT_Extras,
  XFA_ELEMENT_Proto,
  XFA_ELEMENT_DSigData,
  XFA_ELEMENT_Creator,
  XFA_ELEMENT_Connect,
  XFA_ELEMENT_Permissions,
  XFA_ELEMENT_ConnectionSet,
  XFA_ELEMENT_Submit,
  XFA_ELEMENT_Range,
  XFA_ELEMENT_Linearized,
  XFA_ELEMENT_Packet,
  XFA_ELEMENT_RootElement,
  XFA_ELEMENT_PlaintextMetadata,
  XFA_ELEMENT_NumberSymbols,
  XFA_ELEMENT_PrintHighQuality,
  XFA_ELEMENT_Driver,
  XFA_ELEMENT_IncrementalLoad,
  XFA_ELEMENT_SubjectDN,
  XFA_ELEMENT_CompressLogicalStructure,
  XFA_ELEMENT_IncrementalMerge,
  XFA_ELEMENT_Radial,
  XFA_ELEMENT_Variables,
  XFA_ELEMENT_TimePatterns,
  XFA_ELEMENT_EffectiveInputPolicy,
  XFA_ELEMENT_NameAttr,
  XFA_ELEMENT_Conformance,
  XFA_ELEMENT_Transform,
  XFA_ELEMENT_LockDocument,
  XFA_ELEMENT_BreakAfter,
  XFA_ELEMENT_Line,
  XFA_ELEMENT_List,
  XFA_ELEMENT_Source,
  XFA_ELEMENT_Occur,
  XFA_ELEMENT_PickTrayByPDFSize,
  XFA_ELEMENT_MonthNames,
  XFA_ELEMENT_Severity,
  XFA_ELEMENT_GroupParent,
  XFA_ELEMENT_DocumentAssembly,
  XFA_ELEMENT_NumberSymbol,
  XFA_ELEMENT_Tagged,
  XFA_ELEMENT_Items,
};
#define XFA_ELEMENT_UNKNOWN ((XFA_ELEMENT)-1)
typedef struct _XFA_ELEMENTINFO {
  uint32_t uHash;
  const FX_WCHAR* pName;
  XFA_ELEMENT eName;
  FX_DWORD dwPackets;
  FX_DWORD eObjectType;
} XFA_ELEMENTINFO, *XFA_LPELEMENTINFO;
typedef XFA_ELEMENTINFO const* XFA_LPCELEMENTINFO;
int32_t XFA_GetElementCount();
XFA_LPCELEMENTINFO XFA_GetElementByName(const CFX_WideStringC& wsName);
XFA_LPCELEMENTINFO XFA_GetElementByID(XFA_ELEMENT eName);
enum XFA_ATTRIBUTETYPE {
  XFA_ATTRIBUTETYPE_NOTSURE,
  XFA_ATTRIBUTETYPE_Enum,
  XFA_ATTRIBUTETYPE_Cdata,
  XFA_ATTRIBUTETYPE_Boolean,
  XFA_ATTRIBUTETYPE_Integer,
  XFA_ATTRIBUTETYPE_Measure,
};
typedef struct _XFA_ATTRIBUTEINFO {
  uint32_t uHash;
  const FX_WCHAR* pName;
  XFA_ATTRIBUTE eName;
  XFA_ATTRIBUTETYPE eType;
  FX_DWORD dwPackets;
  void* pDefValue;
} XFA_ATTRIBUTEINFO, *XFA_LPATTRIBUTEINFO;
typedef XFA_ATTRIBUTEINFO const* XFA_LPCATTRIBUTEINFO;
int32_t XFA_GetAttributeCount();
XFA_LPCATTRIBUTEINFO XFA_GetAttributeByName(const CFX_WideStringC& wsName);
XFA_LPCATTRIBUTEINFO XFA_GetAttributeByID(XFA_ATTRIBUTE eName);
FX_BOOL XFA_GetAttributeDefaultValue(void*& pValue,
                                     XFA_ELEMENT eElement,
                                     XFA_ATTRIBUTE eAttribute,
                                     XFA_ATTRIBUTETYPE eType,
                                     FX_DWORD dwPacket);
XFA_ATTRIBUTEENUM XFA_GetAttributeDefaultValue_Enum(XFA_ELEMENT eElement,
                                                    XFA_ATTRIBUTE eAttribute,
                                                    FX_DWORD dwPacket);
CFX_WideStringC XFA_GetAttributeDefaultValue_Cdata(XFA_ELEMENT eElement,
                                                   XFA_ATTRIBUTE eAttribute,
                                                   FX_DWORD dwPacket);
FX_BOOL XFA_GetAttributeDefaultValue_Boolean(XFA_ELEMENT eElement,
                                             XFA_ATTRIBUTE eAttribute,
                                             FX_DWORD dwPacket);
int32_t XFA_GetAttributeDefaultValue_Integer(XFA_ELEMENT eElement,
                                             XFA_ATTRIBUTE eAttribute,
                                             FX_DWORD dwPacket);
CXFA_Measurement XFA_GetAttributeDefaultValue_Measure(XFA_ELEMENT eElement,
                                                      XFA_ATTRIBUTE eAttribute,
                                                      FX_DWORD dwPacket);
typedef struct _XFA_ELEMENTHIERARCHY {
  FX_WORD wStart;
  FX_WORD wCount;
  FX_WORD wParentIndex;
} XFA_ELEMENTHIERARCHY, *XFA_LPELEMENTHIERARCHY;
typedef XFA_ELEMENTHIERARCHY const* XFA_LPCELEMENTHIERARCHY;
typedef struct _XFA_SCRIPTHIERARCHY {
  FX_WORD wMethodStart;
  FX_WORD wMethodCount;
  FX_WORD wAttributeStart;
  FX_WORD wAttributeCount;
  int16_t wParentIndex;
} XFA_SCRIPTHIERARCHY, *XFA_LPSCRIPTHIERARCHY;
typedef XFA_SCRIPTHIERARCHY const* XFA_LPCSCRIPTHIERARCHY;
const FX_WORD* XFA_GetElementChildren(XFA_ELEMENT eElement, int32_t& iCount);
const uint8_t* XFA_GetElementAttributes(XFA_ELEMENT eElement, int32_t& iCount);
XFA_LPCELEMENTINFO XFA_GetChildOfElement(XFA_ELEMENT eElement,
                                         XFA_ELEMENT eChild,
                                         FX_DWORD dwPacket);
XFA_LPCATTRIBUTEINFO XFA_GetAttributeOfElement(XFA_ELEMENT eElement,
                                               XFA_ATTRIBUTE eAttribute,
                                               FX_DWORD dwPacket);
#define XFA_PROPERTYFLAG_OneOf 0x01
#define XFA_PROPERTYFLAG_DefaultOneOf 0x02
typedef struct _XFA_PROPERTY {
  FX_WORD eName;
  uint8_t uOccur;
  uint8_t uFlags;
} XFA_PROPERTY, *XFA_LPPROPERTY;
typedef XFA_PROPERTY const* XFA_LPCPROPERTY;
XFA_LPCPROPERTY XFA_GetElementProperties(XFA_ELEMENT eElement, int32_t& iCount);
XFA_LPCPROPERTY XFA_GetPropertyOfElement(XFA_ELEMENT eElement,
                                         XFA_ELEMENT eProperty,
                                         FX_DWORD dwPacket);
typedef struct _XFA_ATTRIBUTEENUMINFO {
  uint32_t uHash;
  const FX_WCHAR* pName;
  XFA_ATTRIBUTEENUM eName;
} XFA_ATTRIBUTEENUMINFO, *XFA_LPATTRIBUTEENUMINFO;
typedef XFA_ATTRIBUTEENUMINFO const* XFA_LPCATTRIBUTEENUMINFO;
XFA_LPCATTRIBUTEENUMINFO XFA_GetAttributeEnumByName(
    const CFX_WideStringC& wsName);
XFA_LPCATTRIBUTEENUMINFO XFA_GetAttributeEnumByID(XFA_ATTRIBUTEENUM eName);
enum XFA_UNIT {
  XFA_UNIT_Unknown,
  XFA_UNIT_Percent,
  XFA_UNIT_Angle,
  XFA_UNIT_Em,
  XFA_UNIT_Pt,
  XFA_UNIT_In,
  XFA_UNIT_Pc,
  XFA_UNIT_Cm,
  XFA_UNIT_Mm,
  XFA_UNIT_Mp,
};
class CXFA_Measurement {
 public:
  void Set(const CFX_WideStringC& wsMeasure);
  void Set(FX_FLOAT fValue, XFA_UNIT eUnit) {
    m_fValue = fValue;
    m_eUnit = eUnit;
  }

  XFA_UNIT GetUnit() const { return m_eUnit; }
  FX_FLOAT GetValue() const { return m_fValue; }
  FX_BOOL ToString(CFX_WideString& wsMeasure) const;
  FX_BOOL ToUnit(XFA_UNIT eUnit, FX_FLOAT& fValue) const;
  FX_FLOAT ToUnit(XFA_UNIT eUnit) const {
    FX_FLOAT f;
    return ToUnit(eUnit, f) ? f : 0;
  }
  CXFA_Measurement() { Set(-1, XFA_UNIT_Unknown); }
  CXFA_Measurement(const CFX_WideStringC& wsMeasure) { Set(wsMeasure); }
  CXFA_Measurement(FX_FLOAT fValue, XFA_UNIT eUnit) { Set(fValue, eUnit); }
  static XFA_UNIT GetUnit(const CFX_WideStringC& wsUnit);

 protected:
  FX_FLOAT m_fValue;
  XFA_UNIT m_eUnit;
};
class CFXJSE_Arguments;
class CXFA_Object;
typedef void (CXFA_Object::*XFA_METHOD_CALLBACK)(CFXJSE_Arguments* pArguments);
typedef struct _XFA_METHODINFO {
  uint32_t uHash;
  const FX_WCHAR* pName;
  XFA_METHOD_CALLBACK lpfnCallback;
} XFA_METHODINFO, *XFA_LPMETHODINFO;
typedef XFA_METHODINFO const* XFA_LPCMETHODINFO;
int32_t XFA_GetMethodCount();
XFA_LPCMETHODINFO XFA_GetMethodByName(XFA_ELEMENT eElement,
                                      const CFX_WideStringC& wsMethodName);
typedef void (CXFA_Object::*XFA_ATTRIBUTE_CALLBACK)(FXJSE_HVALUE hValue,
                                                    FX_BOOL bSetting,
                                                    XFA_ATTRIBUTE eAttribute);
enum XFA_SCRIPT_TYPE {
  XFA_SCRIPT_Basic,
  XFA_SCRIPT_Object,
};
typedef struct _XFA_SCRIPTATTRIBUTEINFO {
  uint32_t uHash;
  const FX_WCHAR* pName;
  XFA_ATTRIBUTE_CALLBACK lpfnCallback;
  int32_t eAttribute;
  FX_WORD eValueType;
} XFA_SCRIPTATTRIBUTEINFO, *XFA_LPSCRIPTATTRIBUTEINFO;
typedef XFA_SCRIPTATTRIBUTEINFO const* XFA_LPCSCRIPTATTRIBUTEINFO;
XFA_LPCSCRIPTATTRIBUTEINFO XFA_GetScriptAttributeByName(
    XFA_ELEMENT eElement,
    const CFX_WideStringC& wsAttributeName);
#endif
