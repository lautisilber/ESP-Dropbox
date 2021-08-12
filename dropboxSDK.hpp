#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


#define DBX_RESPONSE_MAX_SIZE 2048
#define DBX_URL_MAX_SIZE 256
#define DBX_RELATIVE_URL_MAX_SIZE DBX_URL_MAX_SIZE - 32
#define DBX_BASE_URL "https://content.dropboxapi.com/2"
#define DBX_ERROR_DEFAULT_RESPONSE "{\"error\": true}"
#define DBX_TOKEN_SIZE 64 + 8
#define DBX_PATH_MAX_SIZE 128
#define DBX_MAX_HEADERS 8

#define HTTPS_HEADER_KEY_MAX_SIZE 32
#define HTTPS_HEADER_VALUE_MAX_SIZE 128


class Header {
private:
    char _key[HTTPS_HEADER_KEY_MAX_SIZE];
    char _value[HTTPS_HEADER_VALUE_MAX_SIZE];
    bool _inUse = false;
public:
    Header(/* args */) {
        memset(_key, HTTPS_HEADER_KEY_MAX_SIZE, '\0');
        memset(_value, HTTPS_HEADER_VALUE_MAX_SIZE, '\0');
    }

    void setKey(const char *key) {
        memset(_key, HTTPS_HEADER_KEY_MAX_SIZE, '\0');
        strncpy(_key, key, HTTPS_HEADER_KEY_MAX_SIZE);
    }
    void setValue(const char *value) {
        memset(_value, HTTPS_HEADER_VALUE_MAX_SIZE, '\0');
        strncpy(_value, value, HTTPS_HEADER_VALUE_MAX_SIZE);
    }
    bool use(bool use) {
        _inUse = use;
        return _inUse;
    }
    bool use() {
        return _inUse;
    }
    void setHeader(const char *key, const char *value, bool use=true) {
        setKey(key);
        setValue(value);
        _inUse = use;
    }
    const char* getKey() { return _key; }
    const char* getValue() { return _value; }
};


class Dropbox {
private:
    char _token[64];
    char _url[DBX_URL_MAX_SIZE] = "";
    int _statusCode = 0;
    char _response[DBX_RESPONSE_MAX_SIZE] = "";
    char _path[DBX_PATH_MAX_SIZE] = "";
    Header _headers[DBX_MAX_HEADERS];

public:
    Dropbox();
    ~Dropbox();

    void begin(const char *token) { strncpy(_token, "Bearer ", 7); strncat(_token, token, DBX_TOKEN_SIZE-8); }

    void setPath(const char *path);
    bool test();
    bool uploadString(char *content, size_t size, const char *path=nullptr);

private:
    bool get();
    bool post(uint8_t *payload, size_t size);
    void setURL(const char *relativeURL);ç
    void setHeader(const char *key, const char *value, size_t index);
    void deactivateHeader(int index=-1);

private:
    static constexpr char *root_ca PROGMEM = R"rawliteral(-----BEGIN CERTIFICATE-----
MIIGXzCCBUegAwIBAgIQAkuvMgi2l3dyPfgpr40frTANBgkqhkiG9w0BAQsFADBw
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMS8wLQYDVQQDEyZEaWdpQ2VydCBTSEEyIEhpZ2ggQXNz
dXJhbmNlIFNlcnZlciBDQTAeFw0yMDEwMjcwMDAwMDBaFw0yMTExMjEyMzU5NTla
MGkxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T
YW4gRnJhbmNpc2NvMRUwEwYDVQQKEwxEcm9wYm94LCBJbmMxFjAUBgNVBAMMDSou
ZHJvcGJveC5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDUcoP7
52AYL/lIYXViV6lSlngsKrXQ9lYUJjP5ERI50fXu3r+bJZX0L6BAnYrjz13+1iDL
j2Kpwo8DCAtnhY3QK0Dj1FJyFFpBLTLjuJ1Ard5MBgszyhtIOy8dUVB5PRP0eAOS
t37Mm2BkqCms19VzPooOEW7oYfJ7Oj64RDJ3NebVdz/B+L8isnmeQOZIk3PZapG8
lwAq5zZoXVe9H9+yLOaoJDYtOO0622hr8C4kPgKeBH6tK5cMeFSDILN0ljCcCAR0
92fVQ0626ZWQwOR28SEBkQpHbrXrGPuxZs7yEDMLqQ0Smy6qJhJaVypUtK+iB2Ck
tkJ27TcRe5Vv4sEBAgMBAAGjggL6MIIC9jAfBgNVHSMEGDAWgBRRaP+QrwIHdTzM
2WVkYqISuFlyOzAdBgNVHQ4EFgQUPWw+5yqJb4AXcqq1gInhPz0YkMQwJQYDVR0R
BB4wHIINKi5kcm9wYm94LmNvbYILZHJvcGJveC5jb20wDgYDVR0PAQH/BAQDAgWg
MB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjB1BgNVHR8EbjBsMDSgMqAw
hi5odHRwOi8vY3JsMy5kaWdpY2VydC5jb20vc2hhMi1oYS1zZXJ2ZXItZzYuY3Js
MDSgMqAwhi5odHRwOi8vY3JsNC5kaWdpY2VydC5jb20vc2hhMi1oYS1zZXJ2ZXIt
ZzYuY3JsMEwGA1UdIARFMEMwNwYJYIZIAYb9bAEBMCowKAYIKwYBBQUHAgEWHGh0
dHBzOi8vd3d3LmRpZ2ljZXJ0LmNvbS9DUFMwCAYGZ4EMAQICMIGDBggrBgEFBQcB
AQR3MHUwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBNBggr
BgEFBQcwAoZBaHR0cDovL2NhY2VydHMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0U0hB
MkhpZ2hBc3N1cmFuY2VTZXJ2ZXJDQS5jcnQwDAYDVR0TAQH/BAIwADCCAQMGCisG
AQQB1nkCBAIEgfQEgfEA7wB1APZclC/RdzAiFFQYCDCUVo7jTRMZM7/fDC8gC8xO
8WTjAAABdWe4tDEAAAQDAEYwRAIgE7Qw9/ZLlEOWQfSdlhUkKKL/AjVRMpj6dVMs
pDRhzZkCIAMu8EIuY45nqd04JkCEx8zQ4qng9zafzwR4fRpmQ5DNAHYAXNxDkv7m
q0VEsV6a1FbmEDf71fpH3KFzlLJe5vbHDsoAAAF1Z7i0jAAABAMARzBFAiADgRX5
hg86ria/0QOoD+Eg+3NhFsFSh65WpBtkxatoaAIhANCpYx8XvF9BRqPTOih0xmrs
7HmN0p1OSFlgILSyPN+NMA0GCSqGSIb3DQEBCwUAA4IBAQBWBB3yrjBdn1KkGCM1
ZtFb0EIwbdPd08sJxlpBzHRonOa2RWF+MR4tHklk95xiPvZWRK2nQoKW6GNsIM1w
WFGYsSZXa0JB6rHbQslnN8LDFxIEu1qvNa2R9mkwrTsq91169SBn4FXPgvxPwUxO
FpScPooaAB7VFuCOQDh688ZIUVtF6USAOXEiG++B+SiR8hCPkBiNYwAYbAVoV9Rk
fu/UwtH/QXy9It2RYslVbBfK/nBoE2z0n8mmnSQ7oC7YcnUjIbuyzsK7yL3wp2c4
OzRpGxTJLtoRYg9EDRvti5x+eoejZClNqTfMc274PItFWgMBVUBbLW6JAfUVLPzH
IrWY
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIEsTCCA5mgAwIBAgIQBOHnpNxc8vNtwCtCuF0VnzANBgkqhkiG9w0BAQsFADBs
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j
ZSBFViBSb290IENBMB4XDTEzMTAyMjEyMDAwMFoXDTI4MTAyMjEyMDAwMFowcDEL
MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3
LmRpZ2ljZXJ0LmNvbTEvMC0GA1UEAxMmRGlnaUNlcnQgU0hBMiBIaWdoIEFzc3Vy
YW5jZSBTZXJ2ZXIgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC2
4C/CJAbIbQRf1+8KZAayfSImZRauQkCbztyfn3YHPsMwVYcZuU+UDlqUH1VWtMIC
Kq/QmO4LQNfE0DtyyBSe75CxEamu0si4QzrZCwvV1ZX1QK/IHe1NnF9Xt4ZQaJn1
itrSxwUfqJfJ3KSxgoQtxq2lnMcZgqaFD15EWCo3j/018QsIJzJa9buLnqS9UdAn
4t07QjOjBSjEuyjMmqwrIw14xnvmXnG3Sj4I+4G3FhahnSMSTeXXkgisdaScus0X
sh5ENWV/UyU50RwKmmMbGZJ0aAo3wsJSSMs5WqK24V3B3aAguCGikyZvFEohQcft
bZvySC/zA/WiaJJTL17jAgMBAAGjggFJMIIBRTASBgNVHRMBAf8ECDAGAQH/AgEA
MA4GA1UdDwEB/wQEAwIBhjAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIw
NAYIKwYBBQUHAQEEKDAmMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2Vy
dC5jb20wSwYDVR0fBEQwQjBAoD6gPIY6aHR0cDovL2NybDQuZGlnaWNlcnQuY29t
L0RpZ2lDZXJ0SGlnaEFzc3VyYW5jZUVWUm9vdENBLmNybDA9BgNVHSAENjA0MDIG
BFUdIAAwKjAoBggrBgEFBQcCARYcaHR0cHM6Ly93d3cuZGlnaWNlcnQuY29tL0NQ
UzAdBgNVHQ4EFgQUUWj/kK8CB3U8zNllZGKiErhZcjswHwYDVR0jBBgwFoAUsT7D
aQP4v0cB1JgmGggC72NkK8MwDQYJKoZIhvcNAQELBQADggEBABiKlYkD5m3fXPwd
aOpKj4PWUS+Na0QWnqxj9dJubISZi6qBcYRb7TROsLd5kinMLYBq8I4g4Xmk/gNH
E+r1hspZcX30BJZr01lYPf7TMSVcGDiEo+afgv2MW5gxTs14nhr9hctJqvIni5ly
/D6q1UEL2tU2ob8cbkdJf17ZSHwD2f2LSaCYJkJA69aSEaRkCldUxPUd1gJea6zu
xICaEnL6VpPX/78whQYwvwt/Tv9XBZ0k7YXDK/umdaisLRbvfXknsuvCnQsH6qqF
0wGjIChBWUMo0oHjqvbsezt3tkBigAVBRQHvFwY+3sAzm2fTYS5yh+Rp/BIAV0Ae
cPUeybQ=
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j
ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL
MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3
LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug
RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm
+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW
PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM
xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB
Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3
hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg
EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF
MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA
FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec
nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z
eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF
hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2
Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe
vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep
+OkuE6N36B9K
-----END CERTIFICATE-----)rawliteral";
};

Dropbox::Dropbox() {
    memset(_url, DBX_URL_MAX_SIZE, '\0');
    memset(_response, DBX_RESPONSE_MAX_SIZE, '\0');
    memset(_token, DBX_TOKEN_SIZE, '\0');
}

Dropbox::~Dropbox() {
}

void Dropbox::setHeader(const char *key, const char *value, size_t index) {
    if (index >= DBX_MAX_HEADERS) return;
    _headers[index].setHeader(key, value, true);
}

void Dropbox::deactivateHeader(int index=-1) {
    if (index < 0) {
        for (size_t i = 0; i < DBX_MAX_HEADERS; i++) {
            _headers[i].use(false);
        }
    } else if (index < DBX_MAX_HEADERS) {
        _headers[index].use(false);
    } else return;
}

bool Dropbox::get() {
    if ((WiFi.status() == WL_CONNECTED)) {
        memset(_response, DBX_RESPONSE_MAX_SIZE, '\0');

        HTTPClient http;
        http.begin(_url, Dropbox::root_ca);
        for (size_t i = 0; i < DBX_MAX_HEADERS; i++) {
            if (_headers[i].use()) {
                http.addHeader(_headers[i].getKey(), _headers[i].getValue());
            }
        }
        _statusCode = http.GET();

        if (_statusCode > 0) {
            http.getString().toCharArray(_response, DBX_RESPONSE_MAX_SIZE);
        } else {
            strncpy(_response, DBX_ERROR_DEFAULT_RESPONSE, DBX_RESPONSE_MAX_SIZE);
        }
        http.end();
    }
    return _statusCode >= 200 && _statusCode < 300;
}

bool Dropbox::post(uint8_t *payload, size_t size) {
    if ((WiFi.status() == WL_CONNECTED)) {
        memset(_response, DBX_RESPONSE_MAX_SIZE, '\0');

        HTTPClient http;
        http.begin(_url, Dropbox::root_ca);
        //http.addHeader("Content-Type", "application/json");
        for (size_t i = 0; i < DBX_MAX_HEADERS; i++) {
            if (_headers[i].use()) {
                http.addHeader(_headers[i].getKey(), _headers[i].getValue());
            }
        }
        _statusCode = http.POST(payload, size);

        if (_statusCode > 0) {
            http.getString().toCharArray(_response, DBX_RESPONSE_MAX_SIZE);
        } else {
            strncpy(_response, DBX_ERROR_DEFAULT_RESPONSE, DBX_RESPONSE_MAX_SIZE);
        }
        http.end();
    }
    return _statusCode >= 200 && _statusCode < 300;
}

void Dropbox::setPath(const char *path) {
    memset(_path, DBX_PATH_MAX_SIZE, '\0');
    strcpy(_path, path, DBX_PATH_MAX_SIZE);
}

void Dropbox::setURL(const char *relativeURL) {
    memset(_url, DBX_URL_MAX_SIZE, '\0');
    strncpy(_url, DBX_BASE_URL, DBX_URL_MAX_SIZE);
    if (relativeURL[0] != '/') {
        strcat(_url, "/");
    }
    strncat(_url, relativeURL, DBX_RELATIVE_URL_MAX_SIZE);
}

bool Dropbox::test() {
    setURL("/");
    return = get();
}

bool Dropbox::uploadString(char *content, size_t size, const char *path) {
    StaticJsonDocument json<192>;
    json["path"] = _path;
    json["mode"] = "overwrite";
    json["autorename"] = true;
    json["mute"] = false;
    json["strict_conflict"] = false;
    char dbxArguments[256];
    serializeJson(json, dbxArguments, 256);

    setURL("/files/upload");
    deactivateHeader();
    _headers[0].setHeader("Authorization", _token);
    _headers[1].setHeader("Dropbox-API-Arg", dbxArguments);
    _headers[2].setHeader("Content-Type", "application/octet-stream");

    bool success = post(content, size);
    return success;
}