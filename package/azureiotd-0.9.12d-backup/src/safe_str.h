#ifndef SAFE_STR_H
#define SAFE_STR_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

// Copy src into dst safely (always NUL-terminated).
size_t strlcpy_compat(char *dst, const char *src, size_t dstsz);

// Extract only the hostname part from an input that may be a URL or host:port.
// Operates on dst only; src remains untouched.
// Examples:
//  "https://example.com:443/path?x" -> "example.com"
//  "mqtts://iothub.azure-devices.net" -> "iothub.azure-devices.net"
//  "example.com:8883" -> "example.com"
void safe_hostname_only(const char *src, char *dst, size_t dstsz);

// Copy src to dst and, if there is a final '.' after the last '/', strip the extension.
// e.g. "/opt/bin/app.exe" -> "app"
void safe_remove_file_ext(const char *src, char *dst, size_t dstsz);

#ifdef __cplusplus
}
#endif
#endif
