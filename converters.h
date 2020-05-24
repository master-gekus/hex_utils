#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <QByteArray>
#include <QString>

struct converter {
  QString name_;
  QByteArray (*from_)(QString);
  QString (*to_)(QByteArray);
};

QByteArray from_unicode(QString source);
QString to_unicode(QByteArray source);

QByteArray from_utf8(QString source);
QString to_utf8(QByteArray source);

QByteArray from_hex(QString source);
QString to_hex(QByteArray source);

#endif // CONVERTERS_H
