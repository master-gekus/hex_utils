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
  if ((L'0' <= c) && (L'9' >= c)) {
    return static_cast<quint8>(c.unicode() - '0');
  }
  else if ((L'A' <= c) && (L'F' >= c)) {
    return static_cast<quint8>(c.unicode() - L'A' + 10);
  }
  else if ((L'a' <= c) && (L'f' >= c)) {
    return static_cast<quint8>(c.unicode() - L'a' + 10);
  }
  else {
    return static_cast<quint8>(-1);
  }
}

QChar to_hex_digit(char c)
{
  if ((0 <= c) && (9 >= c)) {
    return c + L'0';
  }
  else if ((10 <= c) && (15 >= c)) {
    return c - 10 + L'A';
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
