/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "cloud_process_callback_stub.h"
#include "dfs_error.h"
#include "utils_log.h"

namespace OHOS::FileManagement::CloudSync {
using namespace std;

CloudProcessCallbackStub::CloudProcessCallbackStub()
{
    opToInterfaceMap_[SERVICE_CMD_ON_PROCESS] = &CloudProcessCallbackStub::HandleOnProcess;
}

int32_t CloudProcessCallbackStub::OnRemoteRequest(uint32_t code,
                                                  MessageParcel &data,
                                                  MessageParcel &reply,
                                                  MessageOption &option)
{
    if (data.ReadInterfaceToken() != GetDescriptor()) {
        return E_SERVICE_DESCRIPTOR_IS_EMPTY;
    }
    auto interfaceIndex = opToInterfaceMap_.find(code);
    if (interfaceIndex == opToInterfaceMap_.end() || !interfaceIndex->second) {
        LOGE("Cannot response request %d: unknown tranction", code);
        return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
    }
    return (this->*(interfaceIndex->second))(data, reply);
}

int32_t CloudProcessCallbackStub::HandleOnProcess(MessageParcel &data, MessageParcel &reply)
{
    int64_t downloadedSize = data.ReadInt64();
    int64_t totalSize = data.ReadInt64();
    OnDownloadProcess(downloadedSize, totalSize);
    return E_OK;
}
} // namespace OHOS::FileManagement::CloudSync