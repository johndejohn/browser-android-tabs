// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_PACKAGE_SIGNED_EXCHANGE_CONSTS_H_
#define CONTENT_BROWSER_WEB_PACKAGE_SIGNED_EXCHANGE_CONSTS_H_

namespace content {

constexpr char kAcceptHeaderSignedExchangeSuffix[] =
    ",application/signed-exchange;v=b1";

enum class SignedExchangeVersion { kB1 };

// Field names defined in the application/signed-exchange content type:
// https://wicg.github.io/webpackage/draft-yasskin-httpbis-origin-signed-exchanges-impl.html#application-signed-exchange

constexpr char kCertSha256Key[] = "cert-sha256";
constexpr char kCertUrl[] = "cert-url";
constexpr char kDateKey[] = "date";
constexpr char kExpiresKey[] = "expires";
constexpr char kHeadersKey[] = "headers";
constexpr char kIntegrity[] = "integrity";
constexpr char kMethodKey[] = ":method";
constexpr char kSig[] = "sig";
constexpr char kStatusKey[] = ":status";
constexpr char kUrlKey[] = ":url";
constexpr char kValidityUrlKey[] = "validity-url";
constexpr char kCertChainCborMagic[] = u8"\U0001F4DC\u26D3";  // "📜⛓"
constexpr char kCertKey[] = "cert";
constexpr char kOcspKey[] = "ocsp";
constexpr char kSctKey[] = "sct";

}  // namespace content

#endif  // CONTENT_BROWSER_WEB_PACKAGE_SIGNED_EXCHANGE_CONSTS_H_
