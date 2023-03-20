/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef CLOUD_DAEMON_STUB_H
#define CLOUD_DAEMON_STUB_H

#include <map>

#include "i_cloud_daemon.h"
#include "iremote_stub.h"
#include "message_option.h"
#include "message_parcel.h"
#include "refbase.h"

namespace OHOS {
namespace Storage {
namespace CloudFile {
class CloudDaemonStub : public IRemoteStub<ICloudDaemon> {
public:
    CloudDaemonStub();
    virtual ~CloudDaemonStub() = default;
    int32_t OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) override;

private:
    using CloudDaemonInterface = int32_t (CloudDaemonStub::*)(MessageParcel &data, MessageParcel &reply);
    std::map<uint32_t, CloudDaemonInterface> opToInterfaceMap_;

    int32_t EchoServerDemoInner(MessageParcel &data, MessageParcel &reply);
};
} // namespace CloudFile
} // namespace Storage
} // namespace OHOS
#endif // CLOUD_DAEMON_STUB_H
