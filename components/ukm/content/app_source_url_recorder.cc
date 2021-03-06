// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/ukm/content/app_source_url_recorder.h"

#include "base/atomic_sequence_num.h"
#include "services/metrics/public/cpp/delegating_ukm_recorder.h"
#include "services/metrics/public/cpp/ukm_recorder.h"
#include "services/metrics/public/cpp/ukm_source_id.h"
#include "url/gurl.h"

namespace ukm {

SourceId AssignNewAppId() {
  static base::AtomicSequenceNumber seq;
  return ConvertToSourceId(seq.GetNext() + 1, SourceIdType::APP_ID);
}

SourceId AppSourceUrlRecorder::GetSourceIdForApp(AppType type,
                                                 const std::string& id) {
  GURL url;
  if (type == AppType::kArc)
    url = GURL("app://play/" + id);
  else if (type == AppType::kChromeExtension)
    url = GURL("chrome-extension://" + id);
  else
    return kInvalidSourceId;

  return GetSourceIdForUrl(url);
}

SourceId AppSourceUrlRecorder::GetSourceIdForPWA(const GURL& url) {
  return GetSourceIdForUrl(url);
}

SourceId AppSourceUrlRecorder::GetSourceIdForUrl(const GURL& url) {
  ukm::DelegatingUkmRecorder* const recorder =
      ukm::DelegatingUkmRecorder::Get();
  if (!recorder)
    return kInvalidSourceId;

  const SourceId source_id = AssignNewAppId();
  if (base::FeatureList::IsEnabled(kUkmAppLogging)) {
    recorder->UpdateAppURL(source_id, url);
  }
  return source_id;
}

}  // namespace ukm
