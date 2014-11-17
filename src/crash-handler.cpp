#include "crash-handler.h"
#include <QDir>
#include <QProcess>
#include <QCoreApplication>
#include <QString>

#if defined(Q_WS_X11)
#include "client/linux/handler/exception_handler.h"
#elif defined(Q_WS_WIN)
#include "client/windows/handler/exception_handler.h"
#elif defined(Q_WS_MAC)
#include "client/mac/handler/exception_handler.h"
#endif

namespace Breakpad {
/************************************************************************/
/* CrashHandlerPrivate                                                  */
/************************************************************************/
class CrashHandlerPrivate
{
    public:
    CrashHandlerPrivate()
    {
        handler = NULL;
    }

    ~CrashHandlerPrivate()
    {
        delete handler;
    }

    void InitCrashHandler(const QString& dumpPath);
    static google_breakpad::ExceptionHandler* handler;
    static bool bReportCrashesToSystem;
};

google_breakpad::ExceptionHandler* CrashHandlerPrivate::handler = NULL;
bool CrashHandlerPrivate::bReportCrashesToSystem = false;

/************************************************************************/
/* DumpCallback                                                         */
/************************************************************************/
#if defined(Q_WS_WIN)
bool DumpCallback(const wchar_t* _dump_dir,const wchar_t* _minidump_id,void* context,EXCEPTION_POINTERS* exinfo,MDRawAssertionInfo* assertion,bool success)
#elif defined(Q_WS_X11)
bool DumpCallback(const google_breakpad::MinidumpDescriptor &md,void *context, bool success)
#elif defined(Q_WS_MAC)
bool DumpCallback(const char* _dump_dir,const char* _minidump_id,void *context, bool success)
#endif
{
    Q_UNUSED(context);
#if defined(Q_WS_WIN)
    Q_UNUSED(_dump_dir);
    Q_UNUSED(_minidump_id);
    Q_UNUSED(assertion);
    Q_UNUSED(exinfo);
#endif
    qDebug("Breakpad crash");

    /*
    NO STACK USE, NO HEAP USE THERE !!!
    Creating QString's, using qDebug, etc. - everything is crash-unfriendly.
    */
    return CrashHandlerPrivate::bReportCrashesToSystem ? success : true;
}

    void CrashHandlerPrivate::InitCrashHandler(const QString& dumpPath)
    {
        if (handler != NULL)
            return;

#if defined(Q_WS_WIN)
        std::wstring pathAsStr = (const wchar_t*)dumpPath.utf16();
        handler = new google_breakpad::ExceptionHandler(
            pathAsStr,
            /*FilterCallback*/ 0,
            DumpCallback,
            /*context*/
            0,
            true
            );
#elif defined(Q_WS_X11)
        std::string pathAsStr = dumpPath.toStdString();
        google_breakpad::MinidumpDescriptor md(pathAsStr);
        handler = new google_breakpad::ExceptionHandler(
            md,
            /*FilterCallback*/ 0,
            DumpCallback,
            /*context*/ 0,
            true,
            -1
            );
#elif defined(Q_WS_MAC)
        std::string pathAsStr = dumpPath.toStdString();
        handler = new google_breakpad::ExceptionHandler(
            pathAsStr,
            /*FilterCallback*/ 0,
            DumpCallback,
            /*context*/
            0,
            true,
            NULL
            );
#endif
        qDebug("[breakpad] initialized");
    }

    /************************************************************************/
    /* CrashHandler                                                         */
    /************************************************************************/
    CrashHandler* CrashHandler::instance()
    {
        static CrashHandler globalHandler;
        return &globalHandler;
    }

    CrashHandler::CrashHandler()
    {
        d = new CrashHandlerPrivate();
    }

    CrashHandler::~CrashHandler()
    {
        delete d;
    }

    void CrashHandler::setReportCrashesToSystem(bool report)
    {
        d->bReportCrashesToSystem = report;
    }

    bool CrashHandler::writeMinidump()
    {
        bool res = d->handler->WriteMinidump();
        if (res) {
            qDebug("Breakpad: writeMinidump() success.");
        } else {
            qWarning("Breakpad: writeMinidump() failed.");
        }
        return res;
    }

    void CrashHandler::Init(const QString& reportPath)
    {
        d->InitCrashHandler(reportPath);
    }
}

