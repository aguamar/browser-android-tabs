// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chromeos/dbus/media_analytics_client.h"

#include <cstdint>
#include <string>
#include <utility>

#include "base/bind.h"
#include "base/logging.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "dbus/bus.h"
#include "dbus/message.h"
#include "dbus/object_path.h"
#include "dbus/object_proxy.h"
#include "third_party/cros_system_api/dbus/service_constants.h"

namespace chromeos {

// The MediaAnalyticsClient implementation used in production.
class MediaAnalyticsClientImpl : public MediaAnalyticsClient {
 public:
  MediaAnalyticsClientImpl() = default;

  ~MediaAnalyticsClientImpl() override = default;

  void AddObserver(Observer* observer) override {
    observer_list_.AddObserver(observer);
  }

  void RemoveObserver(Observer* observer) override {
    observer_list_.RemoveObserver(observer);
  }

  void GetState(DBusMethodCallback<mri::State> callback) override {
    dbus::MethodCall method_call(media_perception::kMediaPerceptionServiceName,
                                 media_perception::kStateFunction);
    dbus_proxy_->CallMethod(
        &method_call, dbus::ObjectProxy::TIMEOUT_USE_DEFAULT,
        base::BindOnce(&MediaAnalyticsClientImpl::OnState,
                       weak_ptr_factory_.GetWeakPtr(), std::move(callback)));
  }

  void SetState(const mri::State& state,
                DBusMethodCallback<mri::State> callback) override {
    DCHECK(state.has_status()) << "Attempting to SetState without status set.";

    dbus::MethodCall method_call(media_perception::kMediaPerceptionServiceName,
                                 media_perception::kStateFunction);

    dbus::MessageWriter writer(&method_call);
    writer.AppendProtoAsArrayOfBytes(state);

    dbus_proxy_->CallMethod(
        &method_call, dbus::ObjectProxy::TIMEOUT_USE_DEFAULT,
        base::BindOnce(&MediaAnalyticsClientImpl::OnState,
                       weak_ptr_factory_.GetWeakPtr(), std::move(callback)));
  }

  void GetDiagnostics(DBusMethodCallback<mri::Diagnostics> callback) override {
    dbus::MethodCall method_call(media_perception::kMediaPerceptionServiceName,
                                 media_perception::kGetDiagnosticsFunction);
    // TODO(lasoren): Verify that this timeout setting is sufficient.
    dbus_proxy_->CallMethod(
        &method_call, dbus::ObjectProxy::TIMEOUT_USE_DEFAULT,
        base::BindOnce(&MediaAnalyticsClientImpl::OnGetDiagnostics,
                       weak_ptr_factory_.GetWeakPtr(), std::move(callback)));
  }

 protected:
  void Init(dbus::Bus* bus) override {
    dbus_proxy_ = bus->GetObjectProxy(
        media_perception::kMediaPerceptionServiceName,
        dbus::ObjectPath(media_perception::kMediaPerceptionServicePath));
    // Connect to the MediaPerception signal.
    dbus_proxy_->ConnectToSignal(
        media_perception::kMediaPerceptionInterface,
        media_perception::kDetectionSignal,
        base::Bind(&MediaAnalyticsClientImpl::OnDetectionSignalReceived,
                   weak_ptr_factory_.GetWeakPtr()),
        base::Bind(&MediaAnalyticsClientImpl::OnSignalConnected,
                   weak_ptr_factory_.GetWeakPtr()));
  }

 private:
  void OnSignalConnected(const std::string& interface,
                         const std::string& signal,
                         bool succeeded) {
    LOG_IF(ERROR, !succeeded)
        << "Connect to " << interface << " " << signal << " failed.";
  }

  // Handler that is triggered when a MediaPerception proto is received from
  // the media analytics process.
  void OnDetectionSignalReceived(dbus::Signal* signal) {
    mri::MediaPerception media_perception;
    dbus::MessageReader reader(signal);
    if (!reader.PopArrayOfBytesAsProto(&media_perception)) {
      LOG(ERROR) << "Invalid detection signal: " << signal->ToString();
      return;
    }

    for (auto& observer : observer_list_)
      observer.OnDetectionSignal(media_perception);
  }

  void OnState(DBusMethodCallback<mri::State> callback,
               dbus::Response* response) {
    if (!response) {
      LOG(ERROR) << "Call to State failed to get response.";
      std::move(callback).Run(base::nullopt);
      return;
    }

    mri::State state;
    dbus::MessageReader reader(response);
    if (!reader.PopArrayOfBytesAsProto(&state)) {
      LOG(ERROR) << "Invalid D-Bus response: " << response->ToString();
      std::move(callback).Run(base::nullopt);
      return;
    }

    std::move(callback).Run(std::move(state));
  }

  void OnGetDiagnostics(DBusMethodCallback<mri::Diagnostics> callback,
                        dbus::Response* response) {
    if (!response) {
      LOG(ERROR) << "Call to GetDiagnostics failed to get response.";
      std::move(callback).Run(base::nullopt);
      return;
    }

    mri::Diagnostics diagnostics;
    dbus::MessageReader reader(response);
    if (!reader.PopArrayOfBytesAsProto(&diagnostics)) {
      LOG(ERROR) << "Invalid GetDiagnostics response: " << response->ToString();
      std::move(callback).Run(base::nullopt);
      return;
    }

    std::move(callback).Run(std::move(diagnostics));
  }

  dbus::ObjectProxy* dbus_proxy_ = nullptr;
  base::ObserverList<Observer> observer_list_;
  base::WeakPtrFactory<MediaAnalyticsClientImpl> weak_ptr_factory_{this};

  DISALLOW_COPY_AND_ASSIGN(MediaAnalyticsClientImpl);
};

MediaAnalyticsClient::~MediaAnalyticsClient() = default;

MediaAnalyticsClient* MediaAnalyticsClient::Create() {
  return new MediaAnalyticsClientImpl;
}

MediaAnalyticsClient::MediaAnalyticsClient() = default;

}  // namespace chromeos
