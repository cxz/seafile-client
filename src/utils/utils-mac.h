#ifndef SEAFILE_CLIENT_UTILS_MAC_H_
#include <QtGlobal>
#ifdef Q_OS_MAC
#include <QString>

void __mac_setDockIconStyle(bool);

QString __mac_get_path_from_fileId_url(const QString &url);

#endif /* Q_OS_MAC */
#endif /* SEAFILE_CLIENT_UTILS_MAC_H_ */
