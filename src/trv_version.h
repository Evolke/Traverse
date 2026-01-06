#ifndef TRV_VERSION_H
#define TRV_VERSION_H
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)
#define TRV_APP_VERSION STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_BUILD)
#define TRV_APP_COMPANY_NAME "Evolke"
#define TRV_APP_NAME "Traverse"
#endif // TRV_VERSION_H
