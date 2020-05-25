#include "converters.h"

QByteArray from_unicode(QString source)
{
  return QByteArray{reinterpret_cast<const char*>(source.unicode()), source.size() * 2};
}

QString to_unicode(QByteArray source)
{
  return QString{reinterpret_cast<const QChar*>(source.constData()), source.size() / 2};
}

QByteArray from_utf8(QString source)
{
  return source.toUtf8();
}

QString to_utf8(QByteArray source)
{
  return QString::fromUtf8(source);
}

namespace {

quint8 from_hex_digit(QChar c)
{
  if ((u'0' <= c) && (u'9' >= c)) {
    return static_cast<quint8>(c.unicode() - u'0');
  }
  else if ((u'A' <= c) && (u'F' >= c)) {
    return static_cast<quint8>(c.unicode() - u'A' + 10);
  }
  else if ((u'a' <= c) && (u'f' >= c)) {
    return static_cast<quint8>(c.unicode() - u'a' + 10);
  }
  else {
    return static_cast<quint8>(-1);
  }
}

QChar to_hex_digit(char c)
{
  if ((0 <= c) && (9 >= c)) {
    return c + u'0';
  }
  else if ((10 <= c) && (15 >= c)) {
    return c - 10 + u'A';
  }
  else {
    Q_ASSERT(false);
    return 0;
  }
}

} // namespace {

QByteArray from_hex(QString source)
{
  QByteArray result;
  result.reserve(source.size() / 2);
  bool odd{true};
  quint8 h{0};

  for (QChar c : source) {
    quint8 v{from_hex_digit(c)};
    if (static_cast<quint8>(-1) == v) {
      continue;
    }
    if (odd) {
      h = v;
    }
    else {
      result += static_cast<char>((h << 4) | v);
    }
    odd = !odd;
  }

  if (!odd) {
    result += static_cast<char>(h);
  }

  return result;
}

QString to_hex(QByteArray source)
{
  QString result;
  result.reserve(source.size() * 2);

  for (char c : source) {
    result += to_hex_digit((c >> 4) & 0x0F);
    result += to_hex_digit(c & 0x0F);
  }

  return result;
}

namespace {

const constexpr quint8 S_INV = static_cast<quint8>(-1);
const constexpr quint8 S_PADDING = static_cast<quint8>(-2);
const constexpr quint8 S_SPACE = static_cast<quint8>(-3);
const constexpr quint8 b64_decode_[] = {
  /* 0x00: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0x08: */ S_INV, S_SPACE, S_SPACE, S_SPACE, S_SPACE, S_SPACE, S_INV, S_INV,
  /* 0x10: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0x18: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0x20: */ S_SPACE, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0x28: */ S_INV, S_INV, S_INV, 62, S_INV, S_INV, S_INV, 63,
  /* 0x30: */ 52, 53, 54, 55, 56, 57, 58, 59,
  /* 0x38: */ 60, 61, S_INV, S_INV, S_INV, S_PADDING, S_INV, S_INV,
  /* 0x40: */ S_INV, 0, 1, 2, 3, 4, 5, 6,
  /* 0x48: */ 7, 8, 9, 10, 11, 12, 13, 14,
  /* 0x50: */ 15, 16, 17, 18, 19, 20, 21, 22,
  /* 0x58: */ 23, 24, 25, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0x60: */ S_INV, 26, 27, 28, 29, 30, 31, 32,
  /* 0x68: */ 33, 34, 35, 36, 37, 38, 39, 40,
  /* 0x70: */ 41, 42, 43, 44, 45, 46, 47, 48,
  /* 0x78: */ 49, 50, 51, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0x80: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0x88: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0x90: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0x98: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0xA0: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0xA8: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0xB0: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0xB8: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0xC0: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0xC8: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0xD0: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0xD8: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0xE0: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0xE8: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0xF0: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
  /* 0xF8: */ S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
};

const char16_t base64_encode_[] =
    u"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    u"abcdefghijklmnopqrstuvwxyz"
    u"0123456789+/";

} // namespace {

QByteArray from_base64(QString source)
{
  QByteArray result;
  result.reserve(((source.size() + 3) / 4) * 3);
  int stage{0};
  quint8 rest{0};

  for (QChar c : source) {
    quint8 code{(0 != c.row()) ? S_INV : b64_decode_[c.unicode()]};
    if ((S_INV == code) || (S_SPACE == code)) {
      continue;
    }
    if (S_PADDING == code) {
      break;
    }
    switch (stage) {
    case 0:
      rest = code << 2;
      break;

    case 1:
      result += static_cast<char>(rest | (code >> 4));
      rest = code  << 4;
      break;

    case 2:
      result += static_cast<char>(rest | (code >> 2));
      rest = code << 6;
      break;

    case 3:
      result += static_cast<char>(rest | code);
      rest = 0;
      break;

    default:
      Q_ASSERT(false);
      break;
    }

    stage = (stage + 1) % 4;
  }

  return result;
}

QString to_base64(QByteArray source)
{
  QString result;
  result.reserve(((source.size() + 2) / 3) * 4);
  int stage{0};
  quint8 rest{0};

  for (quint8 c : source) {
    switch (stage) {
    case 0:
      result += base64_encode_[c >> 2];
      rest = (c & 0x03) << 4;
      break;

    case 1:
      result += base64_encode_[rest | (c >> 4)];
      rest = (c & 0x0F) << 2;
      break;

    case 2:
      result += base64_encode_[rest | (c >> 6)];
      result += base64_encode_[c & 0x3F];
      rest = 0;
      break;

    default:
      Q_ASSERT(false);
      break;
    }

    stage = (stage + 1) % 3;
  }

  // Padding
  switch (stage) {
  case 0:
    break;

  case 1:
    result += base64_encode_[rest];
    result += u'=';
    result += u'=';
    break;

  case 2:
    result += base64_encode_[rest];
    result += u'=';
    break;

  default:
    Q_ASSERT(false);
    break;
  }

  return result;
}
