#ifndef SEAFILE_CLIENT_CRASH_HANDLER
#define SEAFILE_CLIENT_CRASH_HANDLER
#include <QString>

namespace Breakpad {

class CrashHandlerPrivate;
class CrashHandler
{
public:
    static CrashHandler* instance();
    void Init(const QString& reportPath);

    void setReportCrashesToSystem(bool report);
    bool writeMinidump();

private:
    Q_DISABLE_COPY(CrashHandler)

    CrashHandler();
    ~CrashHandler();
    CrashHandlerPrivate* d;
};
}
#endif // SEAFILE_CLIENT_CRASH_HANDLER

