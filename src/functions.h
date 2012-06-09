#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtCore>
#include <QDebug>

#include <ctime>

#define JID_RESOURCE_SEPARATOR "/"

QStringList parseJid(QString jid);

void splitJid(const QString &jid, QString *bare, QString *resource = NULL);

Q_DECLARE_METATYPE(const void *)

///////////////////////////////////////////////////////////////////////////////////////////////
// Logging
///////////////////////////////////////////////////////////////////////////////////////////////
extern time_t log_timestamp; 
extern int log_level; 
 
#define LOG_DEBUG 0 
#define LOG_INFO  1 
#define LOG_WARN  2 
#define LOG_ERROR 3 
 
#define LOG(level, format, ...) { \
	    if (level >= log_level) { \
	        time(&log_timestamp); \
	        fprintf(stderr, "%d> %s[%s %s] %s:%s():%d: " format "\n", \
	                level, ctime(&log_timestamp), \
	                __DATE__, __TIME__, __FILE__, __func__, __LINE__, ## __VA_ARGS__); \
	    } \
	}
 
#define LDEBUG(format, ...) LOG(LOG_DEBUG, format, ## __VA_ARGS__) 
#define LINFO(format, ...)  LOG(LOG_INFO,  format, ## __VA_ARGS__) 
#define LWARN(format, ...)  LOG(LOG_WARN,  format, ## __VA_ARGS__) 
#define LERROR(format, ...) LOG(LOG_ERROR, format, ## __VA_ARGS__) 
#define LFATAL(format, ...) { LOG(666, format, ## __VA_ARGS__); exit(1); } 
#define LASSERT(expr, msg_format, ...) { if (!(expr)) { LOG(LOG_ERROR, "Assertion failure for %s\n" msg_format, #expr, ## __VA_ARGS__) } }

#endif // FUNCTIONS_H
